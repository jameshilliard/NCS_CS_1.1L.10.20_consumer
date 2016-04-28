#include "uhttpd.h"
#include "uhttpd-utils.h"
#include "cgi_main.h"
#include "cgi_wan.h"

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiSetWANIPSettings
 *
 *  [DESCRIPTION]:
 *      Call WAN IP DAL interface.
 *
 *  [return]:
 **************************************************************************/
static int cgiSetWANIPSettings(int flag){
    int result = 0;
    int ret_size = sizeof(dal_ret_t);
    dal_ret_t ret_ip;
    memset(&ret_ip, 0, ret_size);
    char wanEnb[BUFLEN_16] = {0};
    
    //set IP
    if(flag)
        strcpy(wanEnb, "Enable");
    else
        strcpy(wanEnb, "Disable");
    ret_ip.param[WAN_IP_ENABLED] = &wanEnb[0];

    if(!safe_strcmp("Static", gWebParams.wan_ipType)){
        ret_ip.param[WAN_IP_ADDRESS] = &gWebParams.wan_ipAddress[0];
        ret_ip.param[WAN_IP_NET_MASK] = &gWebParams.wan_ipMask[0];
        ret_ip.param[WAN_IP_DEF_GW] = &gWebParams.wan_ipDefGtw[0];
    }
    ret_ip.param[WAN_IP_ADDRESS_TYPE] = &gWebParams.wan_ipType[0];

    gui_debug("\nYUKI======WAN_IP_ADDRESS_TYPE=%s\nWAN_IP_ADDRESS=%s\nWAN_IP_NET_MASK=%s\nWAN_IP_DEF_GW=%s\nWAN_IP_ENABLED=%s\n",ret_ip.param[WAN_IP_ADDRESS_TYPE],ret_ip.param[WAN_IP_ADDRESS],ret_ip.param[WAN_IP_NET_MASK],ret_ip.param[WAN_IP_DEF_GW],ret_ip.param[WAN_IP_ENABLED]);
    result = dal_wan_if_set_ips(&ret_ip);
    return result;
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiSetDNSSettings
 *
 *  [DESCRIPTION]:
 *      Call DNS DAL interface.
 *
 *  [return]:
 **************************************************************************/
static int cgiSetDNSSettings(){
    int result = 0;
    int ret_size = sizeof(dal_ret_t);
    dal_ret_t ret_dhcpserver;
    memset(&ret_dhcpserver, 0, ret_size);

    //set dns
    if(!safe_strcmp("STATIC", gWebParams.wan_dnsType)){
        ret_dhcpserver.param[WAN_DNS_PRIMARY] = &gWebParams.wan_dns1[0];
        ret_dhcpserver.param[WAN_DNS_SECONDARY] = &gWebParams.wan_dns2[0];
    }
    ret_dhcpserver.param[WAN_DNS_TYPE] = &gWebParams.wan_dnsType[0];
    
    gui_debug("\nYUKI======WAN_DNS_TYPE=%s\nWAN_DNS_PRIMARY=%s\nWAN_DNS_SECONDARY=%s\n",ret_dhcpserver.param[WAN_DNS_TYPE],ret_dhcpserver.param[WAN_DNS_PRIMARY],ret_dhcpserver.param[WAN_DNS_SECONDARY]);
    result = dal_wan_set_dns(&ret_dhcpserver);

    return result;
}
/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostWanSettings
 *
 *  [DESCRIPTION]:
 *      IPOE, STATIC IP, DNS
 *
 *  [return]:
 **************************************************************************/
int cgiPostWANSettings()
{
    int result = 0;
    char ispType[BUFLEN_16] = {0};
    char dnsType[BUFLEN_16] = {0};

    dal_ret_t *ret_ip = NULL;
    ret_ip = dal_wan_if_get_ips();
    if(ret_ip == NULL)
        return;
    dal_ret_t *ret_dns = NULL;
    ret_dns = dal_wan_get_dns();
    if(ret_dns == NULL)
        return;

    sprintf(ispType, "%s", getValueFromDal(ret_ip->param[WAN_IP_ADDRESS_TYPE]));
    sprintf(dnsType, "%s", getValueFromDal(ret_dns->param[WAN_DNS_TYPE]));

    //if(!safe_strcmp("DHCP", gWebParams.wan_ipType) && !safe_strcmp("DHCP", ispType))
    {
        result = cgiSetWANIPSettings(0);
    }
    result = cgiSetWANIPSettings(1);
    result = cgiSetDNSSettings(0);
    result = dal_SaveConfig();

    return result;
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetIPSettings
 *
 *  [DESCRIPTION]:
 *      Get IP Settings of WAN Settings
 *
 *  [return]:
 **************************************************************************/
void cgiGetIPSettings(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    dal_ret_t *ret_info = NULL;
    ret_info = dal_wan_if_get_ips();
    if(ret_info == NULL)
        return;

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[WAN_IP_ENABLED]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[WAN_IP_ADDRESS_TYPE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[WAN_IP_ADDRESS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[WAN_IP_NET_MASK]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[WAN_IP_DEF_GW]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[WAN_IP_EXT_ADDRESS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[WAN_IP_CONN_TYPE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[WAN_IP_CONN_STATUS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[WAN_IP_PHY_CONN_TYPE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[WAN_IP_MAC_ADDRESS]));
    strcat(varValue, buf);

    gui_debug("\nYUKI===IP Settings===varValue=%s\n",varValue);
    free_dal_ret(&ret_info);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetDNSSettings
 *
 *  [DESCRIPTION]:
 *      Get DNS Settings of WAN Settings
 *
 *  [return]:
 **************************************************************************/
void cgiGetDNSSettings(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    dal_ret_t *ret_info = NULL;
    ret_info = dal_wan_get_dns();
    if(ret_info == NULL)
        return;

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[WAN_DNS_TYPE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[WAN_DNS_PRIMARY]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[WAN_DNS_SECONDARY]));
    strcat(varValue, buf);

    gui_debug("\nYUKI===DNS Settings===varValue=%s\n",varValue);
    free_dal_ret(&ret_info);
}