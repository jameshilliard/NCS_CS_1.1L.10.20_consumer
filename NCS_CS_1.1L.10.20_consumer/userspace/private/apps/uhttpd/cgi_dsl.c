#include "uhttpd.h"
#include "uhttpd-utils.h"
#include "cgi_main.h"
#include "cgi_dsl.h"

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostBroadbandSettings
 *
 *  [DESCRIPTION]:
 *      Broadband Configure Settings: WAN Type, Line Mode, Transport Mode, VLAN ID, MTU and so on.
 *
 *  [return]:
 **************************************************************************/
int cgiPostBroadbandSettings()
{
    int result = 0;
    int ret_size = sizeof(dal_ret_t);

    dal_ret_t ret_igdConfig;
    memset(&ret_igdConfig, 0, ret_size);
    dal_ret_t ret_dslAtmLink;
    memset(&ret_dslAtmLink, 0, ret_size);
    dal_ret_t ret_dslPtmLink;
    memset(&ret_dslPtmLink, 0, ret_size);
    dal_ret_t ret_ethLink;
    memset(&ret_ethLink, 0, ret_size);
    dal_ret_t ret_optical;
    memset(&ret_optical, 0, ret_size);
    if(!safe_strcmp(gWebParams.bb_ispType, "auto"))
    {
        char atm_line_mode[BUFLEN_32]={0};
        char ptm_line_mode[BUFLEN_32]={0};
        tsl_strncpy(atm_line_mode,"ADSL_Modulation_All",sizeof(atm_line_mode));
        tsl_strncpy(ptm_line_mode,"VDSL_Modulation_All",sizeof(ptm_line_mode));
        ret_igdConfig.param[DSL_IGD_WAN_MODE] = &gWebParams.bb_ispType[0];
        ret_igdConfig.param[DSL_IGD_VDSL_TRSPT_MODE] = &gWebParams.bb_transMode[0];
        ret_igdConfig.param[DSL_IGD_ADSL_TRSPT_MODE] = &gWebParams.bb_transMode[0];
        ret_igdConfig.param[DSL_IGD_ETH_TRSPT_MODE] = &gWebParams.bb_transMode[0];
        ret_igdConfig.param[DSL_IGD_OPT_TRSPT_MODE] = &gWebParams.bb_transMode[0];

        ret_dslAtmLink.param[DSL_ATM_CONF_MTU] = &gWebParams.bb_mtu[0];
        ret_dslAtmLink.param[DSL_ATM_CONF_VLAN] = &gWebParams.bb_vlanId[0];
        ret_dslAtmLink.param[DSL_ATM_CONF_MODE] = &atm_line_mode[0];

        ret_dslPtmLink.param[DSL_PTM_CONF_MTU] = &gWebParams.bb_mtu[0];
        ret_dslPtmLink.param[DSL_PTM_CONF_VLAN] = &gWebParams.bb_vlanId[0];
        ret_dslPtmLink.param[DSL_PTM_CONF_MODE] = &ptm_line_mode[0];

        ret_ethLink.param[ETH_CONF_MTU] = &gWebParams.bb_mtu[0];
        ret_ethLink.param[ETH_CONF_VLAN] = &gWebParams.bb_vlanId[0];

        ret_optical.param[ETH_CONF_MTU] = &gWebParams.bb_mtu[0];
        ret_optical.param[ETH_CONF_VLAN] = &gWebParams.bb_vlanId[0];

        gui_debug("\nYUKI=====AUTO======\nDSL_IGD_WAN_MODE=%s\nDSL_IGD_VDSL_TRSPT_MODE=%s\nDSL_IGD_ADSL_TRSPT_MODE=%s\nDSL_ATM_CONF_MTU=%s\nDSL_ATM_CONF_VLAN=%s\nDSL_PTM_CONF_MTU=%s\nDSL_PTM_CONF_VLAN=%s\n",ret_igdConfig.param[DSL_IGD_WAN_MODE],ret_igdConfig.param[DSL_IGD_VDSL_TRSPT_MODE],ret_igdConfig.param[DSL_IGD_ADSL_TRSPT_MODE],ret_dslAtmLink.param[DSL_ATM_CONF_MTU],ret_dslAtmLink.param[DSL_ATM_CONF_VLAN],ret_dslPtmLink.param[DSL_PTM_CONF_MTU],ret_dslPtmLink.param[DSL_PTM_CONF_VLAN]);
        result = dal_dsl_set_IGDCongfig(&ret_igdConfig);
        result = dal_dsl_set_AtmLinkCongfig(&ret_dslAtmLink);
        result = dal_dsl_set_PtmLinkCongfig(&ret_dslPtmLink);
        result = dal_dsl_set_EthLinkCongfig(&ret_ethLink);
        result = dal_dsl_set_OptLinkCongfig(&ret_optical);
        result = dal_SaveConfig();
    }
    else if(!safe_strcmp(gWebParams.bb_ispType, "manual_PTM_ADSL") || !safe_strcmp(gWebParams.bb_ispType, "manual_PTM_ADSL_bonding"))
    {
        ret_igdConfig.param[DSL_IGD_WAN_MODE] = &gWebParams.bb_ispType[0];
        ret_igdConfig.param[DSL_IGD_ADSL_TRSPT_MODE] = &gWebParams.bb_transMode[0];

        ret_dslAtmLink.param[DSL_ATM_CONF_MTU] = &gWebParams.bb_mtu[0];
        ret_dslAtmLink.param[DSL_ATM_CONF_VLAN] = &gWebParams.bb_vlanId[0];
        ret_dslAtmLink.param[DSL_ATM_CONF_MODE] = &gWebParams.bb_lineMode[0];

        gui_debug("\nYUKI=====ATM======\nDSL_IGD_WAN_MODE=%s\nDSL_IGD_ADSL_TRSPT_MODE=%s\nDSL_ATM_CONF_MTU=%s\nDSL_ATM_CONF_VLAN=%s\nDSL_ATM_CONF_MODE=%s\n",ret_igdConfig.param[DSL_IGD_WAN_MODE],ret_igdConfig.param[DSL_IGD_ADSL_TRSPT_MODE],ret_dslAtmLink.param[DSL_ATM_CONF_MTU],ret_dslAtmLink.param[DSL_ATM_CONF_VLAN],ret_dslAtmLink.param[DSL_ATM_CONF_MODE]);
        result = dal_dsl_set_IGDCongfig(&ret_igdConfig);
        result = dal_dsl_set_AtmLinkCongfig(&ret_dslAtmLink);
        result = dal_SaveConfig();
    }
    else if(!safe_strcmp(gWebParams.bb_ispType, "manual_PTM_VDSL") || !safe_strcmp(gWebParams.bb_ispType, "manual_PTM_VDSL_bonding")){
        ret_igdConfig.param[DSL_IGD_WAN_MODE] = &gWebParams.bb_ispType[0];
        ret_igdConfig.param[DSL_IGD_VDSL_TRSPT_MODE] = &gWebParams.bb_transMode[0];

        ret_dslPtmLink.param[DSL_PTM_CONF_MTU] = &gWebParams.bb_mtu[0];
        ret_dslPtmLink.param[DSL_PTM_CONF_VLAN] = &gWebParams.bb_vlanId[0];
        ret_dslPtmLink.param[DSL_PTM_CONF_MODE] = &gWebParams.bb_lineMode[0];

        gui_debug("\nYUKI=====PTM======\nDSL_IGD_WAN_MODE=%s\nDSL_IGD_VDSL_TRSPT_MODE=%s\nDSL_PTM_CONF_MTU=%s\nDSL_PTM_CONF_VLAN=%s\nDSL_PTM_CONF_MODE=%s\n",ret_igdConfig.param[DSL_IGD_WAN_MODE],ret_igdConfig.param[DSL_IGD_VDSL_TRSPT_MODE],ret_dslPtmLink.param[DSL_PTM_CONF_MTU],ret_dslPtmLink.param[DSL_PTM_CONF_VLAN],ret_dslPtmLink.param[DSL_PTM_CONF_MODE]);
        result = dal_dsl_set_IGDCongfig(&ret_igdConfig);
        result = dal_dsl_set_PtmLinkCongfig(&ret_dslPtmLink);
        result = dal_SaveConfig();
    }
    else if(!safe_strcmp(gWebParams.bb_ispType, "manual_ETHERNET"))
    {
        ret_igdConfig.param[DSL_IGD_WAN_MODE] = &gWebParams.bb_ispType[0];
        ret_igdConfig.param[DSL_IGD_ETH_TRSPT_MODE] = &gWebParams.bb_transMode[0];

        ret_ethLink.param[ETH_CONF_MTU] = &gWebParams.bb_mtu[0];
        ret_ethLink.param[ETH_CONF_VLAN] = &gWebParams.bb_vlanId[0];

        gui_debug("\nYUKI=====ETH======\nDSL_IGD_WAN_MODE=%s\nETH_TRSPT_MODE=%s\nETH_CONF_MTU=%s\nETH_CONF_VLAN=%s\n",ret_igdConfig.param[DSL_IGD_WAN_MODE],ret_igdConfig.param[DSL_IGD_VDSL_TRSPT_MODE],ret_ethLink.param[ETH_CONF_MTU],ret_ethLink.param[ETH_CONF_VLAN]);
        result = dal_dsl_set_IGDCongfig(&ret_igdConfig);
        result = dal_dsl_set_EthLinkCongfig(&ret_ethLink);
        result = dal_SaveConfig();
    }
    else if(!safe_strcmp(gWebParams.bb_ispType, "manual_OPTICAL"))
    {
        ret_igdConfig.param[DSL_IGD_WAN_MODE] = &gWebParams.bb_ispType[0];
        ret_igdConfig.param[DSL_IGD_OPT_TRSPT_MODE] = &gWebParams.bb_transMode[0];

        ret_optical.param[ETH_CONF_MTU] = &gWebParams.bb_mtu[0];
        ret_optical.param[ETH_CONF_VLAN] = &gWebParams.bb_vlanId[0];

        gui_debug("\nYUKI=====OPTICAL======\nDSL_IGD_WAN_MODE=%s\nDSL_IGD_OPT_TRSPT_MODE=%s\nETH_CONF_MTU=%s\nETH_CONF_VLAN=%s\n",ret_igdConfig.param[DSL_IGD_WAN_MODE],ret_igdConfig.param[DSL_IGD_OPT_TRSPT_MODE],ret_optical.param[ETH_CONF_MTU],ret_optical.param[ETH_CONF_VLAN]);
        result = dal_dsl_set_IGDCongfig(&ret_igdConfig);
        result = dal_dsl_set_OptLinkCongfig(&ret_optical);
        result = dal_SaveConfig();
    }
    
    //for 6rd
    if(dal_ipv6_get_wan_enable && !dal_checkCurrentWanMode())
    {
        dal_setIPv6rdMTU(gWebParams.bb_mtu);
    }
    return result;

}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetBroadbandSettings
 *
 *  [DESCRIPTION]:
 *      Broadband Configure Settings: WAN Type, Line Mode, Transport Mode, VLAN ID, MTU and so on.
 *
 *  [return]:
 **************************************************************************/
void cgiGetBroadbandSettings(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    dal_ret_t *ret_igdConfig = NULL;
    dal_ret_t *ret_dslAtmLink = NULL;
    dal_ret_t *ret_dslPtmLink = NULL;
    dal_ret_t *ret_ethLink = NULL;
    dal_ret_t *ret_optical = NULL;

    ret_igdConfig = dal_dsl_get_IGDCongfig();
    if(ret_igdConfig == NULL)
        return;
    ret_dslAtmLink = dal_dsl_get_AtmLinkCongfig();
    if(ret_dslAtmLink == NULL)
        return;
    ret_dslPtmLink = dal_dsl_get_PtmLinkCongfig();
    if(ret_dslPtmLink == NULL)
        return;
    ret_ethLink = dal_dsl_get_EthLinkCongfig();
    if(ret_ethLink == NULL)
        return;

    ret_optical = dal_dsl_get_OptLinkCongfig();
    if(ret_optical == NULL)
        return;

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_igdConfig->param[DSL_IGD_WAN_MODE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_igdConfig->param[DSL_IGD_VDSL_TRSPT_MODE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_igdConfig->param[DSL_IGD_ADSL_TRSPT_MODE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_igdConfig->param[DSL_IGD_ETH_TRSPT_MODE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_dslAtmLink->param[DSL_ATM_CONF_MTU]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_dslAtmLink->param[DSL_ATM_CONF_VLAN]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_dslAtmLink->param[DSL_ATM_CONF_MODE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_dslAtmLink->param[DSL_ATM_CONF_PRIORITY]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_dslPtmLink->param[DSL_PTM_CONF_MTU]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_dslPtmLink->param[DSL_PTM_CONF_VLAN]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_dslPtmLink->param[DSL_PTM_CONF_MODE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_dslPtmLink->param[DSL_PTM_CONF_PRIORITY]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_ethLink->param[ETH_CONF_MTU]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_ethLink->param[ETH_CONF_VLAN]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_ethLink->param[ETH_CONF_PRIORITY]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_igdConfig->param[DSL_IGD_OPT_TRSPT_MODE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_optical->param[ETH_CONF_MTU]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_optical->param[ETH_CONF_VLAN]));
    strcat(varValue, buf);

    gui_debug("\nYUKI===Broadband Configure Settings===varValue=%s\n",varValue);
    free_dal_ret(&ret_igdConfig);
    free_dal_ret(&ret_dslAtmLink);
    free_dal_ret(&ret_dslPtmLink);
    free_dal_ret(&ret_ethLink);
    free_dal_ret(&ret_optical);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetBBPhysicalLinkStatus
 *
 *  [DESCRIPTION]:
 *      Get Broadband Connection Status
 *
 *  [return]:
 **************************************************************************/
void cgiGetBBPhysicalLinkStatus(int argc, char **argv, char *varValue)
{
    dal_ret_t *ret_bbConStatus = NULL;
    ret_bbConStatus = dal_wan_if_get_broadband_connection_status();

    if(ret_bbConStatus == NULL)
        return;
    memset(gWebParams.bb_phy_link_stat , 0, sizeof(gWebParams.bb_phy_link_stat));
    sprintf(gWebParams.bb_phy_link_stat, "%s", getValueFromDal(ret_bbConStatus->param[WAN_BB_PHYSICAL_LINK_STATUS]));
    strcat(varValue, gWebParams.bb_phy_link_stat);  

    memset(gWebParams.bb_ip_con_stat , 0, sizeof(gWebParams.bb_ip_con_stat));
    sprintf(gWebParams.bb_ip_con_stat, "%s", getValueFromDal(ret_bbConStatus->param[WAN_BB_IP_CONNECTION_STATUS]));

    memset(gWebParams.bb_toltal_bytes_rev , 0, sizeof(gWebParams.bb_toltal_bytes_rev));
    sprintf(gWebParams.bb_toltal_bytes_rev, "%s", getValueFromDal(ret_bbConStatus->param[WAN_BB_TOTAL_BYTES_RECEIVED]));
    
    gui_debug("\nBroadband Connectivity PhysicalLinkStatus ===varValue=%s,link_stat=%s,con_stat=%s,bytes_rev=%s\n", varValue,gWebParams.bb_phy_link_stat,gWebParams.bb_ip_con_stat,gWebParams.bb_toltal_bytes_rev);
    free_dal_ret(&ret_bbConStatus);    
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetBBIPConnectionStatus
 *
 *  [DESCRIPTION]:
 *      Get Broadband IP Connection Status
 *
 *  [return]:
 **************************************************************************/
void cgiGetBBIPConnectionStatus(int argc, char **argv, char *varValue)
{
    strcat(varValue, gWebParams.bb_ip_con_stat); 
    gui_debug("\nBroadband Connectivity bb_ip_con_stat ===varValue=%s\n", varValue);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetBBTotalBytesReceived
 *
 *  [DESCRIPTION]:
 *      Get Broadband Total Bytes Received
 *
 *  [return]:
 **************************************************************************/
void cgiGetBBTotalBytesReceived(int argc, char **argv, char *varValue)
{
    strcat(varValue, gWebParams.bb_toltal_bytes_rev); 
    gui_debug("\nBroadband Connectivity bb_toltal_bytes_rev ===varValue=%s\n", varValue);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetLine1Status
 *
 *  [DESCRIPTION]:
 *      Get Broadband Status Line1
 *
 *  [return]:
 **************************************************************************/
static void cgiGetLine1Status(char *varValue){
    char buf[BUFLEN_256] = {0};
    dal_ret_t *ret_bbStatus = NULL;
    ret_bbStatus = dal_get_broadband_status();
    if(ret_bbStatus == NULL)
        return;

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_LINE_STATE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_DOWNSTREAM_SYNC_RATE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_UPSTREAM_SYNC_RATE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_MODULATION]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_DATA_PATH]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_BONDING]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_DOWN_STREAM_SN_MARGIN]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_UP_STREAM_SN_MARGIN]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_DOWN_STREAM_LINE_ATTEMUATION]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_UP_STREAM_LINE_ATTEMUATION]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_DOWN_STREAM_OUTPUT_POWER]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_UP_STREAM_OUTPUT_POWER]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_DOWN_STREAM_ERRORED_SECONDS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_UP_STREAM_ERRORED_SECONDS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_DOWN_STREAM_LOSS_SIGNAL]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_UP_STREAM_LOSS_SIGNAL]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_DOWN_STREAM_LOSS_FRAME]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_UP_STREAM_LOSS_FRAME]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_DOWN_STREAM_FEC_ERRLRS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_UP_STREAM_FEC_ERRORS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_DOWN_STREAM_CRC_ERRORS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_UP_STREAM_CRC_ERRORS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_IPV4_TRANSMIT_PACKETS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_IPV4_TRANSMIT_ERROR]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_IPV4_TRANSMIT_DISCARDS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_MTU]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_CONNECTION]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_BROADBAND_IPV4_ADDRESS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_GATEWAY_IPV4_ADDRESS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_MAC_ADDRESS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_PRIMARY_DNS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_SECONDARY_DNS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_PRIMARY_DNS_NAME]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbStatus->param[BROADBAND_STATUS_SECONDARY_DNS_NAME]));
    strcat(varValue, buf);

    gui_debug("\nYUKI===Broadband Status Line 1===varValue=%s\n", varValue);
    free_dal_ret(&ret_bbStatus);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetLine2Status
 *
 *  [DESCRIPTION]:
 *      Get Broadband Status Line2
 *
 *  [return]:
 **************************************************************************/
static void cgiGetLine2Status(char *varValue){
    char buf[BUFLEN_256] = {0};
    strcpy(varValue, buf);
    dal_ret_t *ret_bbline2Status = NULL;
    ret_bbline2Status = dal_get_DSL_Line2_info();
    if(ret_bbline2Status == NULL)
        return;
	memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_LINE_STATE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_DOWNSTREAM_SYNC_RATE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_UPSTREAM_SYNC_RATE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_MODULATION]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_DATA_PATH]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_BONDING]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_DOWN_STREAM_SN_MARGIN]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_UP_STREAM_SN_MARGIN]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_DOWN_STREAM_LINE_ATTEMUATION]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_UP_STREAM_LINE_ATTEMUATION]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_DOWN_STREAM_OUTPUT_POWER]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_UP_STREAM_OUTPUT_POWER]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_DOWN_STREAM_ERRORED_SECONDS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_UP_STREAM_ERRORED_SECONDS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_DOWN_STREAM_LOSS_SIGNAL]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_UP_STREAM_LOSS_SIGNAL]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_DOWN_STREAM_LOSS_FRAME]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_UP_STREAM_LOSS_FRAME]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_DOWN_STREAM_FEC_ERRLRS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_UP_STREAM_FEC_ERRORS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_DOWN_STREAM_CRC_ERRORS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_UP_STREAM_CRC_ERRORS]));
    strcat(varValue, buf);
	#if 0
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_LINE2_STATE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_LINE2_DOWNSTREAM_SYNC_RATE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_LINE2_UPSTREAM_SYNC_RATE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_LINE2_MODULATION]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
	sprintf(buf, "unknow|");
    //sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_LINE2_DATA_PATH]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
	sprintf(buf, "0|");
    //sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_LINE2_DOWN_STREAM_SN_MARGIN]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_LINE2_UP_STREAM_SN_MARGIN]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
	sprintf(buf, "0|");
    //sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_LINE2_DOWN_STREAM_LINE_ATTEMUATION]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_LINE2_UP_STREAM_LINE_ATTEMUATION]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "0|");
    //sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_LINE2_DOWN_STREAM_OUTPUT_POWER]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_bbline2Status->param[BROADBAND_STATUS_LINE2_UP_STREAM_OUTPUT_POWER]));
    strcat(varValue, buf);

#endif
    gui_debug("\nYUKI===Broadband Status Line 2===varValue=%s\n", varValue);
    free_dal_ret(&ret_bbline2Status);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetBroadbandStatus
 *
 *  [DESCRIPTION]:
 *      Get Broadband Status
 *
 *  [return]:
 **************************************************************************/
void cgiGetBroadbandStatus(int argc, char **argv, char *varValue)
{

    gui_debug("argc=%d", argc);
    if(argc > 2 && safe_strcmp(argv[2], "2") == 0)
    {
        cgiGetLine2Status(varValue);
    }else{
        cgiGetLine1Status(varValue);
    }
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetAutoMode
 *
 *  [DESCRIPTION]:
 *      Get Auto WAN Mode and Line Mode
 *
 *  [return]:
 **************************************************************************/
void cgiGetAutoMode(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    dal_ret_t *ret = NULL;
    ret = dal_dsl_get_modulation();
    if(ret == NULL)
        return;

    //WAN MODE
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret->param[BROADBAND_GET_DSL_WAN_TYPE]));
    strcat(varValue, buf);

    //LINE MODE
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret->param[BROADBAND_GET_DSL_LINE_TYPE]));
    strcat(varValue, buf);

    //is bond
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret->param[BROADBAND_GET_DSL_ISBOND]));
    strcat(varValue, buf);

    gui_debug("\nYUKI===Broadband Auto Mode===varValue=%s\n", varValue);
    free_dal_ret(&ret);
}
