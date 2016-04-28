/*
 * <:copyright-BRCM:2013:DUAL/GPL:standard
 * 
 *    Copyright (c) 2013 Broadcom Corporation
 *    All Rights Reserved
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as published by
 * the Free Software Foundation (the "GPL").
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * 
 * A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
 * writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * 
 * :>
 */

// FORMAT: notabs uncrustify:bcm_minimal_i4.cfg

#include "boardparms.h"

#ifdef _CFE_
#include "lib_types.h"
#include "lib_printf.h"
#include "lib_string.h"
#include "bcm_map.h"
#define printk  printf
#else // Linux
#include <linux/kernel.h>
#include <linux/module.h>
#include <bcm_map_part.h>
#include <linux/string.h>
#endif

/* map SWITCH/CROSSBAR port numbers to the order of xMII pas control
   registers in the device
 */

static int port2xmii[] = { -1, -1, -1, -1, -1, 1, -1, 2, -1, -1, -1, 3, 0 };
static void bcm_misc_hw_xmii_pads_init(void)
{
    ETHERNET_MAC_INFO Enet[BP_MAX_ENET_MACS];
    int i,j;
    int u;
    int n;
    // Check for network ports requiring MAC interfaces to be active
    if (BP_SUCCESS == BpGetEthernetMacInfo( Enet, BP_MAX_ENET_MACS ))
    {
        for (i = 0 ; i < BP_MAX_ENET_MACS ; i++) {
            for (j = 0; j < BP_MAX_SWITCH_PORTS ; j++) {
                if ((Enet[i].sw.port_map >> j) & 0x01)  {
                    u = j;
                    n = port2xmii[u];
                    if (n >= 0) {
                        switch (Enet[i].sw.phy_id[u] & MAC_IFACE)
                        {
                        case MAC_IF_RGMII_1P8V :
                            MISC->miscxMIIPadCtrl[n] = MISC->miscxMIIPadCtrl[n] & ~MISC_XMII_PAD_MODEHV;
                            break;
                        case MAC_IF_RGMII_2P5V :
                            MISC->miscxMIIPadCtrl[n] = (MISC->miscxMIIPadCtrl[n] | MISC_XMII_PAD_MODEHV) & ~MISC_XMII_PAD_SEL_GMII;
                            break;
                        case MAC_IF_RGMII_3P3V :
                            MISC->miscxMIIPadCtrl[n] = MISC->miscxMIIPadCtrl[n] | MISC_XMII_PAD_MODEHV | MISC_XMII_PAD_SEL_GMII;
                            break;
                        }
                    }
                }
            }
            for (j = 0; j < BP_MAX_CROSSBAR_PORTS ; j++) {
                u = BP_CROSSBAR_PORT_TO_PHY_PORT(j);
                if (Enet[i].sw.crossbar[j].switch_port != BP_CROSSBAR_NOT_DEFINED)
                {
                    n = port2xmii[u];
                    if (n >= 0) {
                        switch (Enet[i].sw.crossbar[j].phy_id & MAC_IFACE)
                        {
                        case MAC_IF_RGMII_1P8V :
                            MISC->miscxMIIPadCtrl[n] = MISC->miscxMIIPadCtrl[n] & ~MISC_XMII_PAD_MODEHV;
                            break;
                        case MAC_IF_RGMII_2P5V :
                            MISC->miscxMIIPadCtrl[n] = (MISC->miscxMIIPadCtrl[n] | MISC_XMII_PAD_MODEHV) & ~MISC_XMII_PAD_SEL_GMII;
                            break;
                        case MAC_IF_RGMII_3P3V :
                            MISC->miscxMIIPadCtrl[n] = MISC->miscxMIIPadCtrl[n] | MISC_XMII_PAD_MODEHV | MISC_XMII_PAD_SEL_GMII;
                            break;
                        }
                    }
                }
            }
        }
    }
}

/* this function performs any customization to the voltage regulator setting */
static void bcm_misc_hw_vr_init(void)
{
#if defined(_BCM963148_) || defined(CONFIG_BCM963148)
    /* set 1.0V digital voltage switching regulator's gain setting to 16 */
    PROCMON->SSBMaster.wr_data = 0x840; 
    PROCMON->SSBMaster.control = 0x3400;
    PROCMON->SSBMaster.control = 0xb400;
    while(PROCMON->SSBMaster.control&0x8000);

    PROCMON->SSBMaster.wr_data = 0x842;
    PROCMON->SSBMaster.control = 0x3400;
    PROCMON->SSBMaster.control = 0xb400;
    while(PROCMON->SSBMaster.control&0x8000);

    PROCMON->SSBMaster.wr_data = 0x840;
    PROCMON->SSBMaster.control = 0x3400;
    PROCMON->SSBMaster.control = 0xb400;
    while(PROCMON->SSBMaster.control&0x8000);
#endif

    return;
}

void bcm_misc_hw_init(void)
{
    bcm_misc_hw_vr_init();
    bcm_misc_hw_xmii_pads_init();
}



#ifndef _CFE_
arch_initcall(bcm_misc_hw_init);
#endif

