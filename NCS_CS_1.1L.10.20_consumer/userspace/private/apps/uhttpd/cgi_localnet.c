/*****CGI for Local Network****/
#include "cgi_common.h"
#include "cgi_localnet.h"
#include "uhttpd-file.h"


/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *          Get local network status 
 *           
 * 
 **************************************************************************/
void cgiGetLocalNetStatus(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_512] = {0};
    // snprintf(buf,sizeof(buf),"192.168.1.116|255.255.255.0|192.168.1.6|192.168.1.254|N/A|N/A|Private||");
    // strcat(varValue, buf);
   
    dal_ret_t *ret_info = NULL;
    
    ret_info = dal_dhcp_get_cfg();
    if (ret_info == NULL) 
    {
        gui_debug("cgiGetLocalNetStatus ret_info is null");
        return;
    }
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[DHCP_DEVICE_PRI_IP]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[DHCP_DEVICE_PRI_SUBNET_MASK]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[DHCP_PRI_START_IP_ADDR]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[DHCP_PRI_END_IP_ADDR]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[DHCP_PUB_POOL_ENABLE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s||", getValueFromDal(ret_info->param[DHCP_PRIMARY_POOL]));
    strcat(varValue, buf);
    
    free_dal_ret(&ret_info); 
    gui_debug("Local net stat varvalue = %s",varValue);

}

/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *          Get local network IPv4 statictics 
 *           
 * 
 **************************************************************************/
void cgiGetLANIPv4Statistics(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    snprintf(buf,sizeof(buf),"2566|0|0||");
    strcat(varValue, buf);
    gui_debug("Local net IPv4 statistics varvalue = %s",varValue);
}

/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *          Get local network ethernet statistics 
 *           
 * 
 **************************************************************************/
void cgiGetLANEthStatistics(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_512] = {0};
    int dal_num = 0;
    int i = 0;
   
    dal_ret_t *dal_get = NULL;
    dal_get = dalLan_getEthPortStatistics(&dal_num);
    if (dal_get == NULL||dal_num == 0) 
    {
        gui_debug("dalLan_getEthPortStatistics is null");
        return;
    }

    for(i = 0; i < dal_num; i++)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[LAN_ETH_PORT_STATS_Num]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[LAN_ETH_PORT_STATS_Status]));
        strcat(varValue, buf);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[LAN_ETH_PORT_STATS_MaxBitRate]));
        strcat(varValue, buf);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[LAN_ETH_PORT_STATS_TxPkts]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[LAN_ETH_PORT_STATS_TxBytes]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[LAN_ETH_PORT_STATS_TxDropped]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[LAN_ETH_PORT_STATS_TxErrors]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[LAN_ETH_PORT_STATS_RxPkts]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[LAN_ETH_PORT_STATS_RxBytes]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[LAN_ETH_PORT_STATS_RxUni]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[LAN_ETH_PORT_STATS_RxMulti]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[LAN_ETH_PORT_STATS_RxDropped]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s||", getValueFromDal(dal_get[i].param[LAN_ETH_PORT_STATS_RxErrors]));
        strcat(varValue, buf);
    }

    free_dal_ret(dal_get);
    gui_debug("LAN Ethernet Ports Statistics varvalue = %s",varValue);
}

/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *          Get local network Subnet&DHCP status 
 *           
 * 
 **************************************************************************/
void cgiGetLANSubnetDhcp(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_512] = {0};
    //snprintf(buf,sizeof(buf),"192.168.1.254|255.255.255.0|192.168.1.1|192.168.1.254|0|220.248.17.168|255.255.255.0|N/A|N/A|0|1|192.168.1.122|10.0.0.2|255.0.0.0|1|16|0|0||");
   // strcat(varValue, buf);
   
    dal_ret_t *ret_info = NULL;
    
    ret_info = dal_dhcp_get_cfg();
    if (ret_info == NULL) 
    {
        gui_debug("cgiGetLANSubnetDhcp ret_info is null");
        return;
    }
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[DHCP_DEVICE_PRI_IP]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[DHCP_DEVICE_PRI_SUBNET_MASK]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[DHCP_PRI_START_IP_ADDR]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[DHCP_PRI_END_IP_ADDR]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[DHCP_PUB_POOL_ENABLE]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", strcmp(getValueFromDal(ret_info->param[DHCP_DEVICE_PUB_IP]),"NULL")==0?"":getValueFromDal(ret_info->param[DHCP_DEVICE_PUB_IP]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", strcmp(getValueFromDal(ret_info->param[DHCP_DEVICE_PUB_SUBNET_MASK]),"NULL")==0?"":getValueFromDal(ret_info->param[DHCP_DEVICE_PUB_SUBNET_MASK]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", strcmp(getValueFromDal(ret_info->param[DHCP_PUB_START_IP_ADDR]),"NULL")==0?"":getValueFromDal(ret_info->param[DHCP_PUB_START_IP_ADDR]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", strcmp(getValueFromDal(ret_info->param[DHCP_PUB_END_IP_ADDR]),"NULL")==0?"":getValueFromDal(ret_info->param[DHCP_PUB_END_IP_ADDR]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", strcmp(getValueFromDal(ret_info->param[CASCADED_ROUTER_ENABLE]),"NULL")==0?"":getValueFromDal(ret_info->param[CASCADED_ROUTER_ENABLE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", strcmp(getValueFromDal(ret_info->param[CASCADED_ROUTER_GW]),"NULL")==0?"":getValueFromDal(ret_info->param[CASCADED_ROUTER_GW]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", strcmp(getValueFromDal(ret_info->param[CASCADED_ROUTER_NETWORK]),"NULL")==0?"":getValueFromDal(ret_info->param[CASCADED_ROUTER_NETWORK]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", strcmp(getValueFromDal(ret_info->param[CASCADED_ROUTER_NETMASK]),"NULL")==0?"":getValueFromDal(ret_info->param[CASCADED_ROUTER_NETMASK]));
    strcat(varValue, buf);    
    
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[DHCP_LEASE_DAY]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[DHCP_LEASE_HOUR]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[DHCP_LEASE_MINUTE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_info->param[DHCP_LEASE_SECOND]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s||", getValueFromDal(ret_info->param[DHCP_PRIMARY_POOL]));
    strcat(varValue, buf);
    
    free_dal_ret(&ret_info);   
    gui_debug("Local net subnet DHCP varvalue = %s",varValue);
}



/**************************************************************************
 *
 *  [DESCRIPTION]:
 *      Get lan dns host mappings lists.
 *
 **************************************************************************/
void cgiGetLANDNSHostMapping(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_512] = {0};
    int dal_num = 0;
    int i = 0;
    
    dal_ret_t *dal_get = NULL;
    
    dal_get = dal_dns_get_mapping_list(&dal_num);
    if (dal_get == NULL||dal_num == 0) 
    {
        gui_debug(" dal_get local_dns_mapping_list is null");
        return;
    }

    for(i = 0; i < dal_num; i++)
    {
    if(strlen(varValue)+sizeof(buf)>=WEB_BUF_SIZE_MAX)
        break;
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[DNS_HOSTMAPPING_HOST_NAME]));
        strcat(varValue, buf);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[DNS_HOSTMAPPING_HOST_IP]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s||", getValueFromDal(dal_get[i].param[DNS_HOSTMAPPING_HOST_MAC]));
        strcat(varValue, buf);        
    }

    free_dal_ret(dal_get); 
    
    gui_debug("Local net dev dns varvalue = %s",varValue);
}


/**************************************************************************
 *
 *  [DESCRIPTION]:
 *      Get Local Network device lists.
 *
 **************************************************************************/
void cgiGetLANDeviceList(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_512] = {0};
    int dal_num = 0;
    int i = 0;
    //snprintf(buf,sizeof(buf),"00:0d:93:4a:41:16|192.168.1.65|Thu Oct 22 00:05:43 1972|Off|DHCP|Ethernet||00:14:d1:f0:f6:be|192.168.1.66|Thu Oct 22 00:05:43 1989|Off|DHCP|Wireless||00:14:d1:f0:f6:b7|192.168.1.68|Thu Oct 22 00:05:43 1992|Off|DHCP|Wireless||");
    //strcat(varValue, buf);

    
    dal_ret_t *dal_get = NULL;
    
    dal_get = dal_dhcp_get_conn_list(&dal_num);
    if (dal_get == NULL||dal_num == 0) 
    {
        gui_debug("cgiGetLANSubnetDhcp dal_get is null");
        return;
    }

    for(i = 0; i < dal_num; i++)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[DHCP_CONN_PHY_ADDR]));
        strcat(varValue, buf);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[DHCP_CONN_IP_ADDR]));
        strcat(varValue, buf);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[DHCP_CONN_LAST_ACTIVITY_TIME]));
        strcat(varValue, buf);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[DHCP_CONN_STATUS]));
        strcat(varValue, buf);
    
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[DHCP_CONN_ADDRESS_SRC]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[DHCP_CONN_INTERFACE_TYPE]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s||", getValueFromDal(dal_get[i].param[DHCP_CONN_HOST_NAME]));
        strcat(varValue, buf);        
    }

    free_dal_ret(dal_get); 
    
    gui_debug("Local net subnet DHCP varvalue = %s",varValue);
}


/**************************************************************************
 *
 *  [DESCRIPTION]:
 *      Get Local Network IP Address Allocation lists.
 *
 **************************************************************************/
void cgiGetLANIPAllocationList(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_512] = {0};
    int dal_num = 0;
    int i = 0;
   
    dal_ret_t *dal_get = NULL;
    dal_get = dal_dhcp_get_static_conn_list(&dal_num);
    if (dal_get == NULL||dal_num == 0) 
    {
        gui_debug("cgiGetLANIPAllocationList dal_get is null");
        return;
    }

    for(i = 0; i < dal_num; i++)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[DHCP_STATIC_CONN_HOST_NAME]));

        if (strcmp(buf, "NULL|") == 0)
            sprintf(buf, "%s", "N/A|");
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[DHCP_STATIC_CONN_PHY_ADDR]));
        strcat(varValue, buf);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[DHCP_STATIC_CONN_IP_ADDR]));
        strcat(varValue, buf);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s||", getValueFromDal(dal_get[i].param[DHCP_STATIC_CONN_PID]));
        strcat(varValue, buf);
    }

    free_dal_ret(dal_get);
    gui_debug("Local net subnet DHCP varvalue = %s",varValue);
}

/**************************************************************************
 *
 *  [DESCRIPTION]:
 *      Local Network IP Allocation settings.
 *
 **************************************************************************/
int cgiPostLANIPAllocation()
{
    tsl_rv_t result = 0;
    dal_ret_t addEntry;
    int ret_size = sizeof(dal_ret_t);
    memset(&addEntry, 0, ret_size);
    char enableStaticConn[BUFLEN_16] = {0};

    if(!safe_strcmp("add", gWebParams.local_allo_button)){
        //add
        strcpy(enableStaticConn, "true");
        addEntry.param[DHCP_STATIC_CONN_ENABLE] = &enableStaticConn[0];
        addEntry.param[DHCP_STATIC_CONN_IP_ADDR] = &gWebParams.man_add_ip_value[0];
        addEntry.param[DHCP_STATIC_CONN_PHY_ADDR] = &gWebParams.man_add_mac_value[0];
        gui_debug("Add infos:\nDHCP_STATIC_CONN_ENABLE=%s\nDHCP_STATIC_CONN_IP_ADDR=%s\nDHCP_STATIC_CONN_PHY_ADDR=%s\n",addEntry.param[DHCP_STATIC_CONN_ENABLE],addEntry.param[DHCP_STATIC_CONN_IP_ADDR],addEntry.param[DHCP_STATIC_CONN_PHY_ADDR]);
        result = dal_dhcp_add_static_conn(&addEntry);
    }
    else if(!safe_strcmp("remove", gWebParams.local_allo_button)){
        gui_debug("Delete OID=%s\n",gWebParams.local_allo_rm_id);
        result = dal_dhcp_delete_static_conn(gWebParams.local_allo_rm_id);
    }
    result = dal_SaveConfig();
}


/**************************************************************************
 *
 *  [DESCRIPTION]:
 *      Local dev dns host mapping settings.
 *
 **************************************************************************/
int cgiPostLANDeviceDNS()
{
    tsl_rv_t result = 0;
    dal_ret_t ret;
    int ret_size = sizeof(dal_ret_t);
    memset(&ret, 0, ret_size);

    ret.param[DNS_HOSTMAPPING_HOST_NAME] = &gWebParams.dev_dns_hostname;
    ret.param[DNS_HOSTMAPPING_HOST_IP] = &gWebParams.dev_dns_ip;
    ret.param[DNS_HOSTMAPPING_HOST_MAC] = &gWebParams.dev_dns_mac;

    result = dal_dns_host_mapping_set_cfg(&ret);

    result = dal_SaveConfig();
    return result;
}


/**************************************************************************
 *
 *  [DESCRIPTION]:
 *      Local Network Subnet & DHCP settings.
 *
 **************************************************************************/
int cgiPostLANSubnetDhcp()
{
    tsl_rv_t result = 0;
    dal_ret_t ret;
    int ret_size = sizeof(dal_ret_t);
    memset(&ret, 0, ret_size);

    //gui_debug("dev_ipv4_ad=%s, submask=%s, start=%s,end=%s", gWebParams.dev_ipv4_addr_value, gWebParams.subnet_mask_value, gWebParams.dhcpv4_start_addr_value, gWebParams.dhcpv4_end_addr_value);
    //gui_debug("pubena_sel=%d, pubv4addr=%s, pubsubmask=%s,pubv4start=%s", gWebParams.pub_sub_enable_sel, gWebParams.pub_ipv4_addr_value, gWebParams.pub_sub_mask_value, gWebParams.pub_dhcpv4_start_addr_value);
    //gui_debug("pubv4_endaddr=%s, poolradio=%d, cas_sel=%d,cas_rtaddr=%s", gWebParams.pub_dhcpv4_end_addr_value, gWebParams.dhcp_pool_radio, gWebParams.cascad_router_enable_sel, gWebParams.cascad_router_addr_value);
    //gui_debug("cas_net_addr=%s,cas_submask=%s", gWebParams.cas_net_addr_value, gWebParams.cascad_sub_mask_value);

    ret.param[DHCP_DEVICE_PRI_IP] = &gWebParams.dev_ipv4_addr_value;
    ret.param[DHCP_DEVICE_PRI_SUBNET_MASK] = &gWebParams.subnet_mask_value;
    ret.param[DHCP_PRI_START_IP_ADDR] = &gWebParams.dhcpv4_start_addr_value;
    ret.param[DHCP_PRI_END_IP_ADDR] = &gWebParams.dhcpv4_end_addr_value;
    ret.param[DHCP_PUB_POOL_ENABLE] = (gWebParams.pub_sub_enable_sel==1?"true":"false");
    ret.param[DHCP_DEVICE_PUB_IP] = &gWebParams.pub_ipv4_addr_value;
    ret.param[DHCP_DEVICE_PUB_SUBNET_MASK] = &gWebParams.pub_sub_mask_value;
    ret.param[DHCP_PUB_START_IP_ADDR] = &gWebParams.pub_dhcpv4_start_addr_value;
    ret.param[DHCP_PUB_END_IP_ADDR] = &gWebParams.pub_dhcpv4_end_addr_value;
    ret.param[DHCP_PRIMARY_POOL] = itoa(gWebParams.dhcp_pool_radio);
   
    ret.param[CASCADED_ROUTER_ENABLE] = (gWebParams.cascad_router_enable_sel==1?"true":"false");
    ret.param[CASCADED_ROUTER_GW] = &gWebParams.cascad_router_addr_value;
    ret.param[CASCADED_ROUTER_NETWORK] = &gWebParams.cas_net_addr_value;
    ret.param[CASCADED_ROUTER_NETMASK] = &gWebParams.cascad_sub_mask_value;
    
    ret.param[DHCP_LEASE_DAY] = &gWebParams.localnet_lease0_value;
    ret.param[DHCP_LEASE_HOUR] = &gWebParams.localnet_lease1_value;
    ret.param[DHCP_LEASE_MINUTE] = &gWebParams.localnet_lease2_value;
    ret.param[DHCP_LEASE_SECOND] = &gWebParams.localnet_lease3_value;

    result = dal_dhcp_set_cfg(&ret);

    gui_debug("result=%d,DHCP_LEASE=%s,%s,%s,%s", result , ret.param[DHCP_LEASE_DAY], ret.param[DHCP_LEASE_HOUR], ret.param[DHCP_LEASE_MINUTE], ret.param[DHCP_LEASE_SECOND]);
    gui_debug("pubenable=%s, pubip=%s, pubsubmask=%s,pubstart=%s,pubend=%s", ret.param[DHCP_PUB_POOL_ENABLE], ret.param[DHCP_DEVICE_PUB_IP], ret.param[DHCP_DEVICE_PUB_SUBNET_MASK], ret.param[DHCP_PUB_START_IP_ADDR],ret.param[DHCP_PUB_END_IP_ADDR]);
    gui_debug("v4ip=%s, v4addr=%s, submask=%s,v4start=%s", ret.param[DHCP_DEVICE_PRI_IP], ret.param[DHCP_DEVICE_PRI_SUBNET_MASK], ret.param[DHCP_PRI_START_IP_ADDR], ret.param[DHCP_PRI_END_IP_ADDR]);
    gui_debug("cas_enalbe=%s, cas_gw=%s, cas_net=%s,cas_mask=%s", ret.param[CASCADED_ROUTER_ENABLE], ret.param[CASCADED_ROUTER_GW], ret.param[CASCADED_ROUTER_NETWORK], ret.param[CASCADED_ROUTER_NETMASK]);

    dal_SaveConfig();
    return result;
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      getGuiValueByMbrAndDuplex
 *
 *  [DESCRIPTION]:
 *      Get GUI shown value by MaxBitRate and DuplexMode.
 *
 *  [return]:
 **************************************************************************/
static void getGuiValueByMbrAndDuplex(char *maxBitRate, char *duplexMode, int *port_value)
{
    gui_debug("===enter maxBitRate=%s; duplexMode=%s\n", maxBitRate, duplexMode);
    if(!safe_strcmp(maxBitRate, "Auto"))
    {
        //Auto
        *port_value = 0;
    }
    else if(!safe_strcmp(maxBitRate, "10") && !safe_strcmp(duplexMode, "HALF"))
    {
        //10M Half Duplex
        *port_value = 1;
    }
    else if(!safe_strcmp(maxBitRate, "10") && !safe_strcmp(duplexMode, "FULL"))
    {
        //10M Full Duplex
        *port_value = 2;
    }
    else if(!safe_strcmp(maxBitRate, "100") && !safe_strcmp(duplexMode, "HALF"))
    {
        //100M Half Duplex
        *port_value = 3;
    }
    else if(!safe_strcmp(maxBitRate, "100") && !safe_strcmp(duplexMode, "FULL"))
    {
        //100M Full Duplex
        *port_value = 4;
    }
    else if(!safe_strcmp(maxBitRate, "1000") && !safe_strcmp(duplexMode, "HALF"))
    {
        //1G Half Duplex
        *port_value = 5;
    }
    else if(!safe_strcmp(maxBitRate, "1000") && !safe_strcmp(duplexMode, "FULL"))
    {
        //1G Full Duplex
        *port_value = 6;
    }else{
        *port_value = 0;
    }
    gui_debug("===port value for GUI is port_value: %d\n", *port_value);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      getMbrAndDuplexByGui
 *
 *  [DESCRIPTION]:
 *      Get MaxbitRate and DuplexMode by the value from gui.
 *
 *  [return]:
 **************************************************************************/
static void getMbrAndDuplexByGui(char *maxBitRate, char *duplexMode, int port_value)
{
    gui_debug("===enter port_value=%d\n",port_value);
    memset(maxBitRate, 0, sizeof(maxBitRate));
    memset(duplexMode, 0, sizeof(duplexMode));

    if(port_value == 0)
    {
        //Auto
        strcpy(maxBitRate, "Auto");
        strcpy(duplexMode, "Auto");
    }
    else if(port_value == 1)
    {
        //10M Half Duplex
        strcpy(maxBitRate, "10");
        strcpy(duplexMode, "HALF");
    }
    else if(port_value == 2)
    {
        //10M Full Duplex
        strcpy(maxBitRate, "10");
        strcpy(duplexMode, "FULL");
    }
    else if(port_value == 3)
    {
        //100M Half Duplex
        strcpy(maxBitRate, "100");
        strcpy(duplexMode, "HALF");
    }
    else if(port_value == 4)
    {
        //100M Full Duplex
        strcpy(maxBitRate, "100");
        strcpy(duplexMode, "FULL");
    }
    else if(port_value == 5)
    {
        //1G Half Duplex
        strcpy(maxBitRate, "1000");
        strcpy(duplexMode, "HALF");
    }
    else if(port_value == 6)
    {
        //1G Full Duplex
        strcpy(maxBitRate, "1000");
        strcpy(duplexMode, "FULL");
    }
    gui_debug("===maxBitRate=%s; duplexMode=%s\n", maxBitRate, duplexMode);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostPortConfigure
 *
 *  [DESCRIPTION]:
 *      Set MaxbitRate and DuplexMode for the port.
 *
 *  [return]:0/1
 **************************************************************************/
int cgiPostPortConfigure()
{
    tsl_rv_t result = 0;
    char maxBitRate[BUFLEN_16] = {0};
    char duplexMode[BUFLEN_16] = {0};
    dal_ret_t ret_set;
    int ret_size = sizeof(dal_ret_t);
    int dal_num = 0;
    dal_ret_t *dal_get = NULL;
    dal_get = dalLan_getEthPortInfo(&dal_num);

    if (dal_get == NULL || dal_num == 0) 
    {
        gui_debug("Get dalLan_getEthPortInfo failed.");
        return result;
    }

    for(int i=0; i<dal_num; i++)
    {
        int port_value = 0;
        if(i == 0)
        {
            //Port 1
            port_value = gWebParams.configure_port1;
        }
        else if(i == 1)
        {
            //Port 2
            port_value = gWebParams.configure_port2;
        }
        else if(i == 2)
        {
            //Port 3
            port_value = gWebParams.configure_port3;
        }
        else if(i == 3)
        {
            //Port 4
            port_value = gWebParams.configure_port4;
        }

        getMbrAndDuplexByGui(maxBitRate, duplexMode, port_value);
        dal_get[i].param[LAN_ETH_PORT_MaxBitRate] = &maxBitRate[0];
        dal_get[i].param[LAN_ETH_PORT_DuplexMode] = &duplexMode[0];
        memset(&ret_set, 0, ret_size);
        ret_set = dal_get[i];
        gui_debug("===Set: port : %d; LAN_ETH_PORT_MaxBitRate=%s; LAN_ETH_PORT_DuplexMode=%s\n", i+1, dal_get[i].param[LAN_ETH_PORT_MaxBitRate], dal_get[i].param[LAN_ETH_PORT_DuplexMode]);
        result = dalLan_setEthPortInfo(&ret_set, i+1);
        result = dal_SaveConfig();
    }
    free_dal_ret(&dal_get);
    return result;
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetPortConfigure
 *
 *  [DESCRIPTION]:
 *      Get MaxbitRate and DuplexMode for the port.
 *
 *  [return]:port 1 MaxbitRate| port 1 DuplexMode||port 2 MaxbitRate| port 2 DuplexMode||port 3 MaxbitRate| port 3 DuplexMode||port 4 MaxbitRate| port 4 DuplexMode||
 **************************************************************************/
void cgiGetPortConfigure(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    char maxBitRate[BUFLEN_16] = {0};
    char duplexMode[BUFLEN_16] = {0};
    int port_value = 0;
    int dal_num = 0;
    dal_ret_t *dal_get = NULL;
    dal_get = dalLan_getEthPortInfo(&dal_num);

    if(dal_get == NULL || dal_num == 0)
    {
        gui_debug("Get dalLan_getEthPortInfo failed.");
        return;
    }
    
    for(int i=0; i<dal_num; i++)
    {

        memset(buf, 0, sizeof(buf));
        memset(maxBitRate, 0, sizeof(maxBitRate));
        memset(duplexMode, 0, sizeof(duplexMode));

        sprintf(maxBitRate, "%s", dal_get[i].param[LAN_ETH_PORT_MaxBitRate]);
        sprintf(duplexMode, "%s", dal_get[i].param[LAN_ETH_PORT_DuplexMode]);
        getGuiValueByMbrAndDuplex(maxBitRate, duplexMode, &port_value);

        sprintf(buf, "%d|", port_value);
        strcat(varValue, buf);
    }
    free_dal_ret(&dal_get);    
    gui_debug("Get MaxbitRate and DuplexMode for the port. varValue: %s\n", varValue);
}
