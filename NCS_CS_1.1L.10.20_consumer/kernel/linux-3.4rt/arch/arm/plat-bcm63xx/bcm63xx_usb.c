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
/*
 ****************************************************************************
 * File Name  : bcm63xx_usb.c
 *
 * Description: This file contains the initilzation and registration routines
 * to enable USB controllers on bcm63xxx boards. 
 *
 *
 ***************************************************************************/

#if defined(CONFIG_USB) || defined(CONFIG_USB_MODULE)

#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/clkdev.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/bug.h>
#include <linux/usb/ehci_pdriver.h>
#include <linux/usb/ohci_pdriver.h>

#include <bcm_map_part.h>
#include <bcm_intr.h>
#include <pmc_usb.h>

extern void bcm_set_pinmux(unsigned int pin_num, unsigned int mux_num);


#define CAP_TYPE_EHCI       0x00
#define CAP_TYPE_OHCI       0x01
#define CAP_TYPE_XHCI       0x02

/*TODO double check the values for these 2 structures */
static struct usb_ehci_pdata bcm_ehci_pdata = {
    .caps_offset         = 0,
    .has_tt              = 0,
    .has_synopsys_hc_bug = 0,
    .port_power_off      = 0,
};

static struct usb_ohci_pdata bcm_ohci_pdata = {};

static struct platform_device *xhci_dev;
static struct platform_device *ehci_dev;
static struct platform_device *ohci_dev;


static __init struct platform_device *bcm_add_usb_host(int type, int id,
                        uint32_t mem_base, uint32_t mem_size, int irq,
                        const char *devname, void *private_data)
{
    struct resource res[2];
    struct platform_device *pdev;
    //static const u64 usb_dmamask = ~(u32)0;
    static const u64 usb_dmamask = 0xffffffff;

    memset(&res, 0, sizeof(res));
    res[0].start = mem_base;
    res[0].end   = mem_base + (mem_size -1);
    res[0].flags = IORESOURCE_MEM;

    res[1].flags = IORESOURCE_IRQ;
    res[1].start = res[1].end = irq;

    pdev = platform_device_alloc(devname, id);
    if(!pdev)
    {
        printk(KERN_ERR "Error Failed to allocate platform device for devname=%s id=%d\n",
                devname, id);
        return 0;
    }

    platform_device_add_resources(pdev, res, 2);

    pdev->dev.dma_mask = (u64 *)&usb_dmamask;
    pdev->dev.coherent_dma_mask = 0xffffffff;

    if(private_data)
    {
        pdev->dev.platform_data = private_data;
    }

    if(platform_device_add(pdev))
    {
        printk(KERN_ERR "Error Failed to add platform device for devname=%s id=%d\n",
                devname, id);
        return 0;
    }

    return pdev;
}

static __init int bcm_add_usb_hosts(void)
{
    printk("++++ Powering up USB blocks\n");
    if(pmc_usb_power_up(PMC_USB_HOST_ALL))
    {
        printk(KERN_ERR "+++ Failed to Power Up USB Host\n");
        return -1;
    }
    mdelay(1);

    /*initialize XHCI settings*/
    USBH_CTRL->usb30_ctl1 |= USB3_IOC;
    USBH_CTRL->usb30_ctl1 |= XHC_SOFT_RESETB;
    USBH_CTRL->usb30_ctl1 |= PHY3_PLL_SEQ_START;

#if defined(CONFIG_BCM963138)
    /*TODO verify these on BO and remove these workarounds */

    /* work around to avoid USB3.0 issue of contoller being reset when UBUS is loaded */
    USBH_CTRL->bridge_ctl = (USBH_CTRL->bridge_ctl & 0xFFFF00FF) | (0x1000);

    /*  default settings in chip are not correct for pwron/pwrflt, adjust these settings
     *  for IPP to work properly
     */
    bcm_set_pinmux(132, 1);/* usb0_pwrflt */
    bcm_set_pinmux(133, 1);/* usb0_pwron  */
    bcm_set_pinmux(134, 1);/* usb1_pwrflt */
    bcm_set_pinmux(135, 1);/* usb1_pwron  */

    /*adjust the default AFE settings for better eye diagrams */
    USBH_CTRL->mdio =0x00ac6a0;
    USBH_CTRL->mdio =0x02ac6a0;
    mdelay(1);
    USBH_CTRL->mdio =0x00ac6a0;
#endif

    /*initialize EHCI & OHCI settings*/
    USBH_CTRL->bridge_ctl &= ~(EHCI_ENDIAN_SWAP | OHCI_ENDIAN_SWAP);
    USBH_CTRL->setup |= (USBH_IOC);
    USBH_CTRL->setup |= (USBH_IPP);

    xhci_dev = bcm_add_usb_host(CAP_TYPE_XHCI, 0, USB_XHCI_PHYS_BASE,
        0x1000, INTERRUPT_ID_USB_XHCI, "xhci-hcd", NULL);
    ehci_dev = bcm_add_usb_host(CAP_TYPE_EHCI, 0, USB_EHCI_PHYS_BASE,
        0x100, INTERRUPT_ID_USB_EHCI, "ehci-platform", &bcm_ehci_pdata);
    ohci_dev = bcm_add_usb_host(CAP_TYPE_OHCI, 0, USB_OHCI_PHYS_BASE,
        0x100, INTERRUPT_ID_USB_OHCI, "ohci-platform", &bcm_ohci_pdata);

    return 0;
}

#if defined CONFIG_USB_MODULE || defined CONFIG_USB_XHCI_HCD_MODULE
static void bcm_mod_cleanup(void)
{
    // we want to just disable usb interrupts and power down usb
    // we'll probably be restart later, re-add resources ok then?
    platform_device_del(xhci_dev);
    platform_device_del(ehci_dev);
    platform_device_del(ohci_dev);
    pmc_usb_power_down(PMC_USB_HOST_ALL);
    mdelay(1);
}

module_init(bcm_add_usb_hosts);
module_exit(bcm_mod_cleanup);

MODULE_LICENSE("GPL");
#else
arch_initcall(bcm_add_usb_hosts);
#endif

#endif /* defined(CONFIG_USB) || defined(CONFIG_USB_MODULE) */
