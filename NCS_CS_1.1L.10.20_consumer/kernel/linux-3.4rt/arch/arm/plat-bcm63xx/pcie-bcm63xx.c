#if defined(CONFIG_BCM_KF_ARM_BCM963XX)
/*
<:copyright-BRCM:2013:DUAL/GPL:standard

   Copyright (c) 2013 Broadcom Corporation
   All Rights Reserved

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation (the "GPL").

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

:>
*/
#include <linux/pci.h>
#include <linux/delay.h>
#include <bcm_map_part.h>
#include <bcm_intr.h>
#include <board.h>
#include <pmc_pcie.h>

#if 0
#define DPRINT(x...)        printk(x)
#define TRACE() 						DPRINT("%s\n",__FUNCTION__)
#define TRACE_READ(x...)		printk(x)
#define TRACE_WRITE(x...)		printk(x)
#else
#undef DPRINT
#define DPRINT(x...)
#define TRACE()
#define TRACE_READ(x...)
#define TRACE_WRITE(x...)
#endif

/*PCI-E */
#define BCM_BUS_PCIE_ROOT							0
#if defined(PCIEH) && defined(PCIEH_1)
#define NUM_CORE	2
#else
#define NUM_CORE	1
#endif

extern unsigned long getMemorySize(void);
static int __init bcm63xx_pcie_get_baraddrsize_index(void);
static struct pci_bus __init * bcm63xx_pcie_scan_bus(int nr, struct pci_sys_data *sys);
static int __init bcm63xx_pcie_map_irq(const struct pci_dev *dev, u8 slot, u8 pin);
static int __init bcm63xx_pcie_setup(int nr, struct pci_sys_data *sys);

/* calculate size dynamically according to the RAM
 * 0x01 ... 64KB
 * 0x02 ... 128KB
 * 0x03 ... 256KB ...
 * 0x0d ... 256MB ...
 * 0x14 ... 32GB 
 */
static int __init bcm63xx_pcie_get_baraddrsize_index(void)
{
	unsigned long memsize; /*in K units*/
	int i = 0;
	
	memsize = ((getMemorySize()) >> 10);
	printk("getMemorySize() = %lu\n", getMemorySize());

	for ( i = 0; i < PCIE_MISC_RC_BAR_CONFIG_LO_SIZE_MAX; i++) {
		if ((64 * (1 << i)) >= memsize) {
			break;
		}
	}
	
	printk("PCIE_MISC.RC_BAR1_CONFIG_LO.size = 0x%x\n", i + 1);
	return (i + 1);
}

static void __init bcm63xx_pcie_pcie_reset(int index, bool PowerOn)
{
#if defined(PCIE3_CORE) /* CONFIG_BCM963138 */
  u32 val = __raw_readl(MISC_BASE+offsetof(Misc,miscPCIECtrl));

	TRACE();
	if(PowerOn) {
	  val &= ~(1<<index);
	  __raw_writel(val, MISC_BASE+offsetof(Misc,miscPCIECtrl));
	  mdelay(10);
	  val |= (1<<index);
	  __raw_writel(val, MISC_BASE+offsetof(Misc,miscPCIECtrl));
	  mdelay(10);
	} else {
		val &= ~(1<<index);
		__raw_writel(val, MISC_BASE+offsetof(Misc,miscPCIECtrl));
	}
	/* this is a critical delay */
	mdelay(500);
#endif
	
}

/*
 * PCIe host controller registers
 * one entry per port
 */

static struct resource bcm63xx_pcie_owin[NUM_CORE] = {
	{
	.name = "bcm63xx pcie0",
	.start = PCIEH_0_MEM_BASE,
	.end   = PCIEH_0_MEM_BASE+PCIEH_0_MEM_SIZE-1,
	.flags = IORESOURCE_MEM,
	},
#if defined(PCIEH_1)	
	{
	.name = "bcm63xx pcie1",
	.start = PCIEH_1_MEM_BASE,
	.end   = PCIEH_1_MEM_BASE+PCIEH_1_MEM_SIZE-1,
	.flags = IORESOURCE_MEM,
	},
#endif	
};

/*
 * Per port control structure
 */
static struct bcm63xx_pcie_port {
	unsigned char * __iomem regs;
	struct resource *owin_res;
	unsigned int irq;
	struct hw_pci hw_pci;

	bool enabled;
	bool link;
} bcm63xx_pcie_ports[NUM_CORE] = {
	{
	.regs = (unsigned char * __iomem)PCIE_0_BASE, /* this is mapped address */
	.owin_res = & bcm63xx_pcie_owin[0],
	.irq = INTERRUPT_ID_PCIE0,
	.hw_pci = {
		.domain 	= 0,
		.swizzle 	= pci_std_swizzle,
		.nr_controllers = 1,
		.setup 		= bcm63xx_pcie_setup,
		.scan 		= bcm63xx_pcie_scan_bus,
		.map_irq 	= bcm63xx_pcie_map_irq,
		},
	.enabled = 0,
	.link = 0,
	},
#if defined(PCIEH_1)	
	{
	.regs = (unsigned char * __iomem)PCIE_1_BASE,
	.owin_res = & bcm63xx_pcie_owin[1],
	.irq = INTERRUPT_ID_PCIE1,
	.hw_pci = {
		.domain 	= 1,
		.swizzle 	= pci_std_swizzle,
		.nr_controllers = 1,
		.setup 		= bcm63xx_pcie_setup,
		.scan 		= bcm63xx_pcie_scan_bus,
		.map_irq 	= bcm63xx_pcie_map_irq,
		},
	.enabled = 0,
	.link = 0,	
	},
#endif	
};

static struct pcie_port *bcm63xx_pcie_bus2port(struct pci_bus *bus)
{

	struct pci_sys_data *sys = bus->sysdata;
	return sys->private_data;

}

static void bcm63xx_pcie_config_select(unsigned char * __iomem regs, u32 bus_no, u32 dev_no, u32 func_no)
{
	/* set device bus/func/func */
#if defined(UBUS2_PCIE)
  __raw_writel((bus_no<<PCIE_EXT_CFG_BUS_NUM_SHIFT)|(dev_no <<PCIE_EXT_CFG_DEV_NUM_SHIFT)|(func_no<<PCIE_EXT_CFG_FUNC_NUM_SHIFT),
  			regs+PCIEH_PCIE_EXT_CFG_REGS+offsetof(PcieExtCfgRegs,index));

#endif    
}
    
static u32 __iomem bcm63xx_pcie_config_offset_aligned(int bus_no, int where)
{
   if(bus_no == BCM_BUS_PCIE_ROOT ) {
        /* access offset 0 */
        return where&~3;
    } else {
        /* access offset */
        return (PCIEH_DEV_OFFSET+where)&~3;
    }	
}


static int bcm63xx_pcie_valid_config(int bus_no, int dev_no)
{
	/* to tune more ?*/
    if (bus_no == BCM_BUS_PCIE_ROOT ) {
        /* bridge */
        return (dev_no == 0); /*otherwise will loop for the rest of the device*/
    } else {
       	return (dev_no == 0); /*otherwise will loop for the rest of the device*/
    }
    return 0;	
}	

/*
 * PCIe config cycles are done by programming the PCIE_CONF_ADDR register
 * and then reading the PCIE_CONF_DATA register. Need to make sure these
 * transactions are atomic.
 */

static int bcm63xx_pciebios_read(struct pci_bus *bus, u32 devfn, int where,
			int size, u32 *val)
{
  struct bcm63xx_pcie_port *port = (struct bcm63xx_pcie_port*)bcm63xx_pcie_bus2port(bus);	
	unsigned char * __iomem regs = port->regs;
	u32 __iomem offset;
	int busno = bus->number;
	int slot = PCI_SLOT(devfn);
	int fn = PCI_FUNC(devfn);
  u32 data;

	TRACE();

  TRACE_READ("R device (bus)%d/(slot)%d/func(%d) at %d size %d, val=0x%x\n", busno, slot, fn, where, size, *val);		
	if (bcm63xx_pcie_valid_config(busno, PCI_SLOT(devfn)) == 0){
		*val = -1;
		TRACE_READ("not valid config\n");
		return PCIBIOS_DEVICE_NOT_FOUND;
	}
		
	offset = (u32)regs + bcm63xx_pcie_config_offset_aligned(busno,where);
	
	if (((size == 2) && (where & 1)) ||((size == 4) && (where & 3))) {
		 BUG_ON(1);
		 return PCIBIOS_BAD_REGISTER_NUMBER;
	}

	bcm63xx_pcie_config_select(regs, busno,slot,fn);

  data = __raw_readl(offset);

  TRACE_READ("reading 0x%x @ 0x%x\n", data, offset);
    
  if (data == 0xdeaddead) {
		*val = -1;
		return PCIBIOS_DEVICE_NOT_FOUND;
  }
  if (size == 1)
     *val = (data >> ((where & 3) << 3)) & 0xff;
  else if (size == 2)
     *val = (data >> ((where & 3) << 3)) & 0xffff;
  else
     *val = data;

  TRACE_READ("val= 0x%x\n", *val);        	
  return PCIBIOS_SUCCESSFUL;
}
	

static int bcm63xx_pciebios_write(struct pci_bus *bus, u32 devfn,
			int where, int size, u32 val)
{
  struct bcm63xx_pcie_port *port = (struct bcm63xx_pcie_port*)bcm63xx_pcie_bus2port(bus);	
	unsigned char * __iomem regs = port->regs;
	u32 __iomem offset;
	int busno = bus->number;
	int slot = PCI_SLOT(devfn);
	int fn = PCI_FUNC(devfn);
  u32 data;
	
	TRACE();

  TRACE_WRITE("W device (bus)%d/(slot)%d/func(%d) at %d size %d, val=0x%x\n", busno, slot, fn, where, size, val);
	if (bcm63xx_pcie_valid_config(busno, PCI_SLOT(devfn)) == 0)
	{
		TRACE_WRITE("not valid config\n");
		return PCIBIOS_DEVICE_NOT_FOUND;
	}
		
	bcm63xx_pcie_config_select(regs, busno,slot,fn);
	offset = (u32)regs + bcm63xx_pcie_config_offset_aligned(busno,where);
	
	if (((size == 2) && (where & 1)) ||((size == 4) && (where & 3))) {
		 BUG_ON(1);
		 return PCIBIOS_BAD_REGISTER_NUMBER;
	}

  data = __raw_readl(offset);
  DPRINT("reading 0x%x @ 0x%x\n", data, offset);
  if (size == 1)
		data = (data & ~(0xff << ((where & 3) << 3))) | (val << ((where & 3) << 3));
	else if (size == 2)
		data = (data & ~(0xffff << ((where & 3) << 3))) | (val << ((where & 3) << 3));
  else
  	data = val;

  TRACE_WRITE("writing 0x%x @ 0x%x\n", data, offset);
	__raw_writel(data, offset);

  return PCIBIOS_SUCCESSFUL;
}


struct pci_ops bcm63xx_pcie_ops = {
	.read   = bcm63xx_pciebios_read,
	.write  = bcm63xx_pciebios_write
};

static struct bcm63xx_pcie_port *bcm63xx_pcie_sysdata2port(struct pci_sys_data *sysdata)
{
	unsigned port;
	TRACE();
	port = sysdata->domain;
	BUG_ON(port >= ARRAY_SIZE(bcm63xx_pcie_ports));
	return & bcm63xx_pcie_ports[port];
}

static void __init bcm63xx_hw_pcie_setup(struct bcm63xx_pcie_port *port)
{
#if defined(UBUS2_PCIE)
	unsigned char * __iomem regs = port->regs;
	TRACE();
		
	__raw_writel(PCIE_CPU_INTR1_PCIE_INTD_CPU_INTR | PCIE_CPU_INTR1_PCIE_INTC_CPU_INTR |PCIE_CPU_INTR1_PCIE_INTB_CPU_INTR |PCIE_CPU_INTR1_PCIE_INTA_CPU_INTR,
				regs+PCIEH_CPU_INTR1_REGS+offsetof(PcieCpuL1Intr1Regs,maskClear));
				/*&((PcieCpuL1Intr1Regs*)(regs+PCIEH_CPU_INTR1_REGS))->maskClear);*/
            
  /* setup outgoing mem resource window */
	__raw_writel((port->owin_res->end & PCIE_MISC_CPU_2_PCI_MEM_WIN_LO_BASE_LIMIT_LIMIT_MASK)
    	 	|((port->owin_res->start >> PCIE_MISC_CPU_2_PCI_MEM_WIN_LO_BASE_LIMIT_LIMIT_SHIFT) << PCIE_MISC_CPU_2_PCI_MEM_WIN_LO_BASE_LIMIT_BASE_SHIFT),
				regs+PCIEH_MISC_REGS+offsetof(PcieMiscRegs,cpu_2_pcie_mem_win0_base_limit));
 				//&((PcieMiscRegs*)(regs+PCIEH_MISC_REGS))->cpu_2_pcie_mem_win0_base_limit);
    	 																								
  __raw_writel((port->owin_res->start & PCIE_MISC_CPU_2_PCI_MEM_WIN_LO_BASE_ADDR_MASK),
  			regs+PCIEH_MISC_REGS+offsetof(PcieMiscRegs,cpu_2_pcie_mem_win0_lo));
				//&((PcieMiscRegs*)(regs+PCIEH_MISC_REGS))->cpu_2_pcie_mem_win0_lo);
   	 
  /* setup incoming DDR memory BAR(1) */
#if 0 //defined(CONFIG_CPU_LITTLE_ENDIAN)
	__raw_writel(PCIE_RC_CFG_VENDOR_SPECIFIC_REG1_ENDIAN_MODE_BYTE_ALIGN,
				regs+PCIEH_RC_CFG_VENDOR_REGS+offsetof(PcieRcCfgVendorRegs,specificReg1));
				//&((PcieRcCfgVendorRegs*)(regs + PCIEH_RC_CFG_VENDOR_REGS))->specificReg1);
#endif
	__raw_writel((DDR_UBUS_ADDRESS_BASE & PCIE_MISC_RC_BAR_CONFIG_LO_MATCH_ADDRESS_MASK)| bcm63xx_pcie_get_baraddrsize_index(),
				regs+PCIEH_MISC_REGS+offsetof(PcieMiscRegs,rc_bar1_config_lo));
				//&((PcieMiscRegs*)(regs+PCIEH_MISC_REGS))->rc_bar1_config_lo);


	__raw_writel(PCIE_MISC_UBUS_BAR_CONFIG_ACCESS_EN,
				regs+PCIEH_MISC_REGS+offsetof(PcieMiscRegs,ubus_bar1_config_remap));
				//&((PcieMiscRegs*)(regs+PCIEH_MISC_REGS))->ubus_bar1_config_remap);

  /* set device bus/func/func -no need*/
  /* setup class code, as bridge */       
  __raw_writel((__raw_readl(regs+PCIEH_BLK_428_REGS+offsetof(PcieBlk428Regs,idVal3))& PCIE_IP_BLK428_ID_VAL3_REVISION_ID_MASK) | (PCI_CLASS_BRIDGE_PCI << 8),
  			regs+PCIEH_BLK_428_REGS+offsetof(PcieBlk428Regs,idVal3));
 
  /* disable bar0 size -no need*/

	/* disable data bus error for enumeration */
	__raw_writel(__raw_readl(regs+PCIEH_MISC_REGS+offsetof(PcieMiscRegs,misc_ctrl))|PCIE_MISC_CTRL_CFG_READ_UR_MODE,
				regs+PCIEH_MISC_REGS+offsetof(PcieMiscRegs,misc_ctrl));  
#endif

#if defined(PCIE3_CORE)
	/* Misc performance addition */
	__raw_writel(__raw_readl(regs+PCIEH_MISC_REGS+offsetof(PcieMiscRegs,misc_ctrl))
								|PCIE_MISC_CTRL_MAX_BURST_SIZE_128B
								|PCIE_MISC_CTRL_BURST_ALIGN
								|PCIE_MISC_CTRL_PCIE_IN_WR_COMBINE
								|PCIE_MISC_CTRL_PCIE_RCB_MPS_MODE
								|PCIE_MISC_CTRL_PCIE_RCB_64B_MODE,
							  regs+PCIEH_MISC_REGS+offsetof(PcieMiscRegs,misc_ctrl));
#endif
}


static int __init bcm63xx_pcie_setup(int nr, struct pci_sys_data *sys)
{

	struct bcm63xx_pcie_port *port = bcm63xx_pcie_sysdata2port(sys);

	TRACE();
	BUG_ON(request_resource(&iomem_resource, port->owin_res));

 	pci_add_resource_offset(&sys->resources, port->owin_res, sys->mem_offset); 
	/* pcibios_init_hw will add resource offset */
	sys->private_data = port;
	bcm63xx_hw_pcie_setup(port);

	return 1;
}

static struct pci_bus __init *
bcm63xx_pcie_scan_bus(int nr, struct pci_sys_data *sys)
{
	TRACE();	
	return pci_scan_root_bus(NULL, sys->busnr, &bcm63xx_pcie_ops, sys, &sys->resources);
}


static int __init bcm63xx_pcie_map_irq(const struct pci_dev *dev, u8 slot,
	u8 pin)
{
	struct bcm63xx_pcie_port *port = (struct bcm63xx_pcie_port*)bcm63xx_pcie_bus2port(dev->bus);
	TRACE();  
	return port->irq;
}

#define BCM4360_D11AC_SROMLESS_ID	0x4360
#define BCM4360_D11AC_ID	0x43a0
#define BCM4360_D11AC2G_ID	0x43a1
#define BCM4360_D11AC5G_ID	0x43a2
#define BCM4352_D11AC_ID	0x43b1
#define BCM4352_D11AC2G_ID	0x43b2
#define BCM4352_D11AC5G_ID	0x43b3
#define BCM43602_CHIP_ID	0xaa52
#define BCM43602_D11AC_ID	0x43ba
#define BCM43602_D11AC2G_ID	0x43bb
#define BCM43602_D11AC5G_ID	0x43bc

#define IS_DEV_AC3X3(d) (((d) == BCM4360_D11AC_ID) || \
	                 ((d) == BCM4360_D11AC2G_ID) || \
	                 ((d) == BCM4360_D11AC5G_ID) || \
	                 ((d) == BCM4360_D11AC_SROMLESS_ID) || \
	                 ((d) == BCM43602_D11AC_ID) || \
	                 ((d) == BCM43602_D11AC2G_ID) || \
	                 ((d) == BCM43602_D11AC5G_ID) || \
	                 ((d) == BCM43602_CHIP_ID))

#define IS_DEV_AC2X2(d) (((d) == BCM4352_D11AC_ID) ||	\
	                 ((d) == BCM4352_D11AC2G_ID) || \
	                 ((d) == BCM4352_D11AC5G_ID))

static void __init bcm63xx_pcie_fixup_mps(struct pci_dev *dev)
{
#if defined(PCIE3_CORE)	
	if (dev->vendor == 0x14e4) {
		if (IS_DEV_AC3X3(dev->device) || IS_DEV_AC2X2(dev->device)) {
			/* set 4360 specific tunables
			 * wlan driver will set mps but cannot populate to RC, 
			 * fake/hijack it so linux sw can sync it up
			 */
			dev->pcie_mpss = 2;
		}
	}
#endif	
}

static void __init bcm63xx_pcie_fixup_final(struct pci_dev *dev)
{
#if defined(PCIE3_CORE)
	pcie_bus_config = PCIE_BUS_SAFE;

	bcm63xx_pcie_fixup_mps(dev);

	/* sync-up mps */
	if (dev->bus && dev->bus->self) {
		pcie_bus_configure_settings(dev->bus, dev->bus->self->pcie_mpss);
	}
#endif	
}
DECLARE_PCI_FIXUP_FINAL(PCI_ANY_ID, PCI_ANY_ID, bcm63xx_pcie_fixup_final);


static int bcm63xx_pcie_link_up(int index)
{		
	struct bcm63xx_pcie_port* port=&bcm63xx_pcie_ports[index];
	TRACE();

	port->link = __raw_readl(port->regs+PCIEH_BLK_1000_REGS+offsetof(PcieBlk1000Regs,dlStatus)) & PCIE_IP_BLK1000_DL_STATUS_PHYLINKUP_MASK;
	if(port->link) {
		printk("PCIE port %d link-up \n", index);
	}
		
	return port->link;
}

static int __init bcm63xx_pcie_init(void)
{
	int i;
	bool shutdown;
	TRACE();
  
	/* pcie ports, domain 1/2 */
	for (i = 0; i < NUM_CORE; i++) {
		shutdown = TRUE;
		if (kerSysGetPciePortEnable(i)) {
			pmc_pcie_power_up(i);
			bcm63xx_pcie_pcie_reset(i, TRUE);
			bcm63xx_pcie_ports[i].enabled =1;			
			if(bcm63xx_pcie_link_up(i)) {
				pci_common_init(&(bcm63xx_pcie_ports[i].hw_pci));
				shutdown = FALSE;
	  	}
	  }
		if(shutdown) {
			/* power off ports without link */
			printk("PCIE port %d power-down \n", i);
			bcm63xx_pcie_pcie_reset(i, FALSE);
			pmc_pcie_power_down(i);
		}
	}
	return 0;
}
arch_initcall(bcm63xx_pcie_init);
#endif
