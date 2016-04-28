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
#ifndef _CFE_
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#else
#include "lib_printf.h"
#endif

#ifndef _CFE_
#define PRINTK	printk
#else
#define PRINTK	xprintf
#endif

#include "pmc_drv.h"
#include "pmc_switch.h"
#include "BPCM.h"

int pmc_switch_power_up(void)
{
	int ret;
#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
	BPCM_GLOBAL_CNTL global_cntl;
#if defined(CONFIG_BCM963148)
	BPCM_SGPHY_CNTL sgphy_cntl;
#endif

#if defined(CONFIG_BCM963148)
	sgphy_cntl.Reg32 = 0x33;
	ret = WriteBPCMRegister(PMB_ADDR_SWITCH,
			BPCMRegOffset(sgphy_cntl), sgphy_cntl.Reg32);
#endif

	ret = ReadBPCMRegister(PMB_ADDR_SWITCH,
			BPCMRegOffset(global_control), &global_cntl.Reg32);
	global_cntl.Bits.z1_ck250_clk_en = 0;
	global_cntl.Bits.z2_ck250_clk_en = 0;
	ret = WriteBPCMRegister(PMB_ADDR_SWITCH,
			BPCMRegOffset(global_control), global_cntl.Reg32);
	ret = ReadBPCMRegister(PMB_ADDR_SWITCH,
			BPCMRegOffset(global_control), &global_cntl.Reg32);
	global_cntl.Bits.z1_ck250_clk_en = 1;
	global_cntl.Bits.z2_ck250_clk_en = 1;
	ret = WriteBPCMRegister(PMB_ADDR_SWITCH,
		BPCMRegOffset(global_control), global_cntl.Reg32);

	PRINTK("%s: Rgmii Tx clock is gated out. \n", __FUNCTION__);

#endif
	ret = PowerOnDevice(PMB_ADDR_SWITCH);

	return ret;
}

int pmc_switch_power_down(void)
{
	return PowerOffDevice(PMB_ADDR_SWITCH, 0);
}

#ifndef _CFE_
EXPORT_SYMBOL(pmc_switch_power_up);
EXPORT_SYMBOL(pmc_switch_power_down);
int pmc_switch_init(void)
{
	int ret;

	ret = pmc_switch_power_up();
	if (ret != 0)
		PRINTK("%s:%d:initialization fails! ret = %d\n", __func__, __LINE__, ret);

	return ret;
}
//early_initcall(pmc_switch_init);
#endif

