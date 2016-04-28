#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cgi_main.h"
#include "cgi_ipv6.h"
/****************************IPV6 WAN Settings Begin***********************/
/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetIPv6WANStatus
 *
 *  [DESCRIPTION]:
 *      Get IPV6 WAN Status
 *
 *  [return]:Status|Service Type|Service Availability|Global Unicast address|link-local address|delegated prefix address|default gateway|6rd Prefix|Relay IPv4 Address|MTU|Transmit Packets|Transmit Error|Transmit Discards
 **************************************************************************/
void cgiGetIPv6WANStatus(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_64] = {0};
    CtlIPv6WANStatus ipv6WANStatus = {0};
    dal_getIPv6WANStatus(&ipv6WANStatus);

    sprintf(buf, "%s|", getValueFromDal(ipv6WANStatus.ipv6WANStatus));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ipv6WANStatus.ipv6ServiceType));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ipv6WANStatus.ipv6ServiceAvailability));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ipv6WANStatus.ipv6WANGUA));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ipv6WANStatus.ipv6WANLLA));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ipv6WANStatus.ipv6WANPD));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ipv6WANStatus.ipv6WANGW));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ipv6WANStatus.ipv6rdPD));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ipv6WANStatus.ipv6rdBRAddress));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%d|", ipv6WANStatus.ipv6MTU);
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%lld|", ipv6WANStatus.ipv6PktTransmit);
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%lld|", ipv6WANStatus.ipv6PktError);
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%lld", ipv6WANStatus.ipv6PktDiscards);
    strcat(varValue, buf);

    gui_debug("\nYUKI===Get IPV6 WAN Status===varValue=%s\n",varValue);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGet6rd
 *
 *  [DESCRIPTION]:
 *      Get ipv6 wan 6rd settings.
 *
 *  [return]:6rd enabled|DHCPv4 6rd Option|6rd Prefix|6rd Prefix Length|IPv4 CE Mask Length|IPv4 Border Router Address|6rd MTU Size|IPv4 WAN Address
 **************************************************************************/
void cgiGet6rd(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_64] = {0};
    dal_ret_t *ret_ipv4 = NULL;
    CtlIPv6rdParameter iPv6rdParameter = {0};
    dal_ipv6_get_6rd(&iPv6rdParameter);
    if(iPv6rdParameter.ipv6rd_enable == 1)
    {
        strcat(varValue, "1|");
    }
    else
        strcat(varValue, "0|");

    if(iPv6rdParameter.allowDHCPConfiguration == 1)
        strcat(varValue, "1|");
    else
        strcat(varValue, "0|");

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(iPv6rdParameter.ipv6rdprefix));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%ld|", iPv6rdParameter.ipv6rdprefixlen);
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%ld|", iPv6rdParameter.ipv6rdipv4masklen);
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(iPv6rdParameter.ipv6rdbripv4addr));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%ld|", iPv6rdParameter.ipv6rdMTU);
    strcat(varValue, buf);

    //Get IPV4 WAN Address
    ret_ipv4 = dal_wan_if_get_ips();
    if(ret_ipv4 == NULL)
        strcat(varValue, "|");
    else
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(ret_ipv4->param[WAN_IP_ADDRESS]));
        strcat(varValue, buf);
    }
    gui_debug("\nYUKI===Get 6rd Settings===varValue=%s\n",varValue);
}

//Set ipv6 wan 6rd settings.
static void cgiSet6rd()
{
    CtlIPv6rdParameter iPv6rdParameter = {0};
    iPv6rdParameter.ipv6rd_enable = gWebParams.ipv6_wan_enable;
    if(gWebParams.ipv6_wan_enable)
    {
        if(gWebParams.ipv6_wan_6rd_dhcp == 1)
            iPv6rdParameter.allowDHCPConfiguration = 1;
        else
        {
            iPv6rdParameter.allowDHCPConfiguration = 0;
            iPv6rdParameter.ipv6rdipv4masklen = gWebParams.ipv6_wan_6rd_ipv4masklen;
            iPv6rdParameter.ipv6rdprefixlen = gWebParams.ipv6_wan_6rd_prefixLen;
            tsl_strncpy(iPv6rdParameter.ipv6rdbripv4addr, gWebParams.ipv6_wan_6rd_ipv4RouterAddr, BUFLEN_32);
            tsl_strncpy(iPv6rdParameter.ipv6rdprefix, gWebParams.ipv6_wan_6rd_prefix, BUFLEN_64);
        }

        iPv6rdParameter.ipv6rdMTU = gWebParams.ipv6_wan_6rd_mtu;
    }
    gui_debug("YUKI===Set: ipv6rd_enable = %d; allowDHCPConfiguration=%d;ipv6rdipv4masklen=%ld;ipv6rdprefixlen=%ld; mtu=%ld; ipv6rdbripv4addr=%s;ipv6rdprefix=%s\n",iPv6rdParameter.ipv6rd_enable, iPv6rdParameter.allowDHCPConfiguration, iPv6rdParameter.ipv6rdipv4masklen,iPv6rdParameter.ipv6rdprefixlen,iPv6rdParameter.ipv6rdMTU,iPv6rdParameter.ipv6rdbripv4addr,iPv6rdParameter.ipv6rdprefix);

    dal_ipv6_set_6rd(&iPv6rdParameter);
    dal_SaveConfig();
}

//Set ipv6 wan dhcpv6 settings.
static void cgiSetDhcpv6()
{
    dal_ipv6_set_dhcpv6(gWebParams.ipv6_wan_enable);
    dal_SaveConfig();
    gui_debug("YUKI===Set: DHCPV6 end.");
}

//Get Setting of wan static settings.
void cgiGetIPv6WANStatic(int argc, char **argv, char *varValue)
{

}

//Set ipv6 wan static settings.
static void cgiSetIPv6WANStatic()
{
    
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostIPv6WANSettings
 *
 *  [DESCRIPTION]:
 *
 *      IPv6 WAN
 *
 *  [return]:
 **************************************************************************/
void cgiPostIPv6WANSettings()
{
    if(!safe_strcmp(gWebParams.action, "test"))
    {
        gui_debug("=====begin trigger6rdNUD===\n");
        dal_trigger6rdNUD();
    }else{
        if(gWebParams.ipv6_wan_pro == 0)
        {
            //6rd
            cgiSet6rd();
        }
        else if(gWebParams.ipv6_wan_pro == 2)
        {
            //DHCPV6
            cgiSetDhcpv6();
        }else{
            //Static
            //cgiSetIPv6WANStatic();
        }
    }
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetIPv6WANCurMode
 *
 *  [DESCRIPTION]:
 *      Get current  IPV6 WAN protocol.
 *
 *  [return]:service Type|6rd settings or dhcpv6 settings
 **************************************************************************/
void cgiGetIPv6WANCurMode(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_64] = {0};
    int ipv6_serviceType = 0;
    //Get current serviceType
    ipv6_serviceType = dal_checkCurrentWanMode();

    if(ipv6_serviceType == -1)
    {   
        //default get 6rd
        ipv6_serviceType = 0;
    }

    sprintf(buf, "%d", ipv6_serviceType);
    strcat(varValue, buf);
    gui_debug("\nYUKI===Get current  IPV6 WAN protocol===varValue=%s\n",varValue);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetIPv6Enabled
 *
 *  [DESCRIPTION]:
 *      If IPV6 WAN is enabled.
 *
 *  [return]:0 or 1
 **************************************************************************/
void cgiGetIPv6WANEnabled(int argc, char **argv, char *varValue)
{
    tsl_bool_t ipv6WANEabled = 0;
    if(dal_ipv6_get_wan_enable() == 1)
        strcat(varValue, "1");
    else
        strcat(varValue, "0");
    gui_debug("\nYUKI===IPV6 WAN Eabled===varValue=%s\n",varValue);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGet6rdNUDResult
 *
 *  [DESCRIPTION]:
 *      Get 6rd Continuity result.
 *
 *  [return]:
 **************************************************************************/
void cgiGet6rdNUDResult(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_64] = {0};
    dal_get6rdNUDResult(buf, BUFLEN_64);
    strcat(varValue, buf);
    gui_debug("\nYUKI===Get 6rd Continuity result=%s\n",varValue);
}
/****************************IPV6 WAN Settings End*************************/

/****************************IPV6 Firewall Settings Begin******************/
/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetIPv6FirewallAdvSettings
 *
 *  [DESCRIPTION]:
 *      Get IPV6 Firewall Advanced Settings.
 *
 *  [return]:IPV6_FW_AllowESP|IPV6_FW_AllowAH|IPV6_FW_AllowHIP|IPV6_FW_AllowIKEP
 **************************************************************************/
void cgiGetIPv6FirewallAdvSettings(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_64] = {0};
    dal_ret_t *ret_fw_ipv6 = NULL;
    dal_ret_t *ret_advfw_ipv6 = NULL;
    ret_fw_ipv6 = dal_ipv6_firewall_get_ipsec_rules();
    ret_advfw_ipv6 = dal_ipv6_get_firewall_advanced();
    if(ret_fw_ipv6 == NULL )
    {
        gui_debug("== Get dal_ipv6_firewall_get_ipsec_rules Failed==\n");
        return;
    }

    sprintf(buf, "%s|", ret_fw_ipv6->param[IPV6_FW_AllowESP]);
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", ret_fw_ipv6->param[IPV6_FW_AllowAH]);
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", ret_fw_ipv6->param[IPV6_FW_AllowHIP]);
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s", ret_fw_ipv6->param[IPV6_FW_AllowIKEP]);
    strcat(varValue, buf);

    gui_debug("\nYUKI===Get IPV6 Firewall Advanced Settings===varValue=%s\n",varValue);
    free_dal_ret(&ret_fw_ipv6);

    if(ret_advfw_ipv6 == NULL )
    {
        gui_debug("== Get dal_ipv6_get_firewall_advanced Failed==\n");
        return;
    }

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "|%s", ret_advfw_ipv6->param[IPV6_FW_ADV_DROPICMPREQU]);
    strcat(varValue, buf);

    gui_debug("\nYUKI===Get IPV6 Firewall Advanced Settings===varValue=%s\n",varValue);
    free_dal_ret(&ret_advfw_ipv6);


}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostIPv6FirewallAdvSettings
 *
 *  [DESCRIPTION]:
 *      Set IPV6 Firewall Advanced Settings.
 *
 *  [return]:
 **************************************************************************/
int cgiPostIPv6FirewallAdvSettings()
{
    int result = 0;
    char fwESP_6rd[BUFLEN_4] = {0};
    char fwAH_6rd[BUFLEN_4] = {0};
    char fwHIP_6rd[BUFLEN_4] = {0};
    char fwIKEP_6rd[BUFLEN_4] = {0};
    char fwICMP_ip6[BUFLEN_4] = {0};
    int ret_size = sizeof(dal_ret_t);
    dal_ret_t ret_fw_ipv6;
    dal_ret_t ret_advfw_ipv6;
    memset(&ret_fw_ipv6, 0, ret_size);

    sprintf(fwESP_6rd, "%d", gWebParams.ipv6_6rd_fwESP);
    sprintf(fwAH_6rd, "%d", gWebParams.ipv6_6rd_fwAH);
    sprintf(fwHIP_6rd, "%d", gWebParams.ipv6_6rd_fwHIP);
    sprintf(fwIKEP_6rd, "%d", gWebParams.ipv6_6rd_fwIKEP);
    sprintf(fwICMP_ip6, "%d", gWebParams.ipv6_adv_fwICMP);

    ret_fw_ipv6.param[IPV6_FW_AllowAH] = &fwAH_6rd[0];
    ret_fw_ipv6.param[IPV6_FW_AllowESP] = &fwESP_6rd[0];
    ret_fw_ipv6.param[IPV6_FW_AllowHIP] = &fwHIP_6rd[0];
    ret_fw_ipv6.param[IPV6_FW_AllowIKEP] = &fwIKEP_6rd[0];
    ret_advfw_ipv6.param[IPV6_FW_ADV_DROPICMPREQU] = &fwICMP_ip6[0];
    
    gui_debug("YUKI=====Set IPV6 Firewall Advanced Settings:IPV6_FW_AllowAH=%s; IPV6_FW_AllowESP=%s; IPV6_FW_AllowHIP=%s; IPV6_FW_AllowIKEP=%s; IPV6_FW_ADV_DROPICMPREQU=%s\n",ret_fw_ipv6.param[IPV6_FW_AllowAH],ret_fw_ipv6.param[IPV6_FW_AllowESP],ret_fw_ipv6.param[IPV6_FW_AllowHIP],ret_fw_ipv6.param[IPV6_FW_AllowIKEP],ret_advfw_ipv6.param[IPV6_FW_ADV_DROPICMPREQU]);
    result = dal_ipv6_firewall_set_ipsec_rules(&ret_fw_ipv6);
    result = dal_ipv6_set_firewall_advanced(&ret_advfw_ipv6);
    result = dal_SaveConfig();
    return result;
}
/****************************IPV6 Firewall Settings End*******************/


/****************************IPV6 LAN Settings Begin**********************/
/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetLocalNetIPv6Status
 *
 *  [DESCRIPTION]:
 *      Get LAN IPv6 informations in Local Network Status.
 *
 *  [return]:
 **************************************************************************/
void cgiGetLocalNetIPv6Status(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    CtlIPv6LANStatus ipv6LANStatus = {0};
    dal_getIPv6LANStatus(&ipv6LANStatus);

    sprintf(buf, "%s|", getValueFromDal(ipv6LANStatus.ipv6LANStatus));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ipv6LANStatus.ipv6LANGUA));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ipv6LANStatus.ipv6LANLLA));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ipv6LANStatus.ipv6LANSubnet));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ipv6LANStatus.ipv6RAPrefix));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ipv6LANStatus.ipv6WANAddress));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s", getValueFromDal(ipv6LANStatus.ipv6LANDelegatedPrefixSubnet));
    strcat(varValue, buf);

    gui_debug("\nYUKI===Get IPV6 WAN Status===varValue=%s\n",varValue);
}

/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *          Get local network IPv6 statictics 
 *           
 * 
 **************************************************************************/
void cgiGetLANIPv6Statistics(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    snprintf(buf,sizeof(buf),"1024|0|0||");
    strcat(varValue, buf);
    gui_debug("Local net IPv6 statistics varvalue = %s",varValue);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostIPV6LANSettings
 *
 *  [DESCRIPTION]:
 *      Set IPV6 LAN Settings.
 *
 *  [return]:
 **************************************************************************/
void cgiPostIPv6LANSettings()
{

    CtlIPv6LANInfo ipv6LANInfo = {0};

    ipv6LANInfo.isEnableLAN = gWebParams.ipv6_lan_enable;
    ipv6LANInfo.isEnableDHCPPD = gWebParams.ipv6_lan_dhcpPd;
    ipv6LANInfo.isEnableDHCPAddress = gWebParams.ipv6_lan_dhcpAddress;

    gui_debug("YUKI===Set: isEnableLAN = %d; isEnableDHCPPD=%d;isEnableDHCPAddress=%ld\n",ipv6LANInfo.isEnableLAN, ipv6LANInfo.isEnableDHCPPD, ipv6LANInfo.isEnableDHCPAddress);
    dal_setIPv6LANInfo(&ipv6LANInfo);
    dal_SaveConfig();
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetIPv6LANSettings
 *
 *  [DESCRIPTION]:
 *      Get IPV6 LAN Settings in Configure page.
 *
 *  [return]:ipv6 enable| DHCPv6 prefix delegation| DHCPv6 address assignment
 **************************************************************************/
void cgiGetIPv6LANSettings(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    CtlIPv6LANInfo ipv6LANInfo = {0};
    dal_getIPv6LANInfo(&ipv6LANInfo);

    sprintf(buf, "%d|", ipv6LANInfo.isEnableLAN);
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%d|", ipv6LANInfo.isEnableDHCPPD);
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%d", ipv6LANInfo.isEnableDHCPAddress);
    strcat(varValue, buf);

    gui_debug("\nYUKI===Get IPV6 LAN Settings===varValue=%s\n",varValue);
}

/****************************IPV6 LAN Settings End******************/
