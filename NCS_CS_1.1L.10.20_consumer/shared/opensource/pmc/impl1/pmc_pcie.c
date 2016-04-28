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
#include <linux/delay.h>
#endif

#include "pmc_drv.h"
#include "pmc_pcie.h"
#include "BPCM.h"

static void pmc_pcie_power_set(int index, bool PowerOn)
{
	int ret;
	BPCM_PWR_ZONE_N_CONTROL pwr_zone_ctrl;
	u32 pmc_pcie_addr;
#if defined(CONFIG_BCM963381)
        pmc_pcie_addr = PMB_ADDR_PCIE0;
#else	
	pmc_pcie_addr = (index ? PMB_ADDR_PCIE1:PMB_ADDR_PCIE0);
#endif
	/* Power Zone#0 */
	pwr_zone_ctrl.Reg32=0;
	
	if(PowerOn)
		pwr_zone_ctrl.Bits.pwr_up_req = 1;
	else
		pwr_zone_ctrl.Bits.pwr_dn_req = 1;

	ret = WriteZoneRegister(pmc_pcie_addr, 0, 0, pwr_zone_ctrl.Reg32);
	if (ret)
		BUG_ON(1);

	mdelay(10);

#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963381) || defined(CONFIG_BCM963148)
	{
		BPCM_SR_CONTROL sr_ctrl;

		/* IDDQ reset */
		if(PowerOn)
			sr_ctrl.Reg32 = 0;
		else
			sr_ctrl.Reg32 = 4; // Only iddq

		ret = WriteBPCMRegister(pmc_pcie_addr, BPCMRegOffset(sr_control), sr_ctrl.Reg32);
		if (ret)
			BUG_ON(1);
	}
#endif

}


void pmc_pcie_power_up(int unit)
{
	pmc_pcie_power_set(unit, 1);
}

void pmc_pcie_power_down(int unit)
{
	pmc_pcie_power_set(unit, 0);
}

#ifndef _CFE_
EXPORT_SYMBOL(pmc_pcie_power_up);
EXPORT_SYMBOL(pmc_pcie_power_down);
#endif

