#include <sys/sysinfo.h>
#include "uhttpd.h"
#include "uhttpd-utils.h"
#include "cgi_main.h"
#include "cgi_diagnostics.h"
#include "dal_comm.h"
#include "cgi_act_tr69.h"
#include "md5.h"

#ifdef AEI_BCM_FIRMWARE_UPLOAD
#include "cgi_upload.h"
#endif


#define SYSLOGS_ALL_PATH  "/tmp/log/syslog.all"

#define NEW_IMAGE 1

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostResets
 *
 *  [DESCRIPTION]:
 *      Diagnostics - Resets
 *
 *  [return]:
 **************************************************************************/
int cgiPostResets()
{
    int result = 0;
#if 0
    if(!safe_strcmp(gWebParams.btn_action, "resetIp"))
    {
        result = dal_sys_reset_ip();
    }
    else if(!safe_strcmp(gWebParams.btn_action, "resetDevice"))
    {
        result = dal_sys_restore_default();
    }
    else if(!safe_strcmp(gWebParams.btn_action, "resetConn"))
    {
        result = dal_sys_reset_connections();
    }

    gui_debug("\nYUKI===Resets btn_action:%s===result=%d\n",gWebParams.btn_action, result);
#endif
    return result;
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetLogs
 *
 *  [DESCRIPTION]:
 *      Get Diagnostics Logs.
 *
 *  [return]:
 **************************************************************************/
void cgiGetLogs(int argc, char **argv, char *varValue)
{
    FILE *fd=NULL;
    char  line[BUFLEN_512] = {0};   

    if((fd = fopen(SYSLOGS_ALL_PATH, "r")) != NULL)
    {
        while(fgets(line, sizeof(line), fd) )
        {
            gui_debug("Diagnostics Logs line: %s\n", line);
            if(line[strlen(line)-1]=='\n')//remove the \n
                line[strlen(line)-1]='\0';
            strcat(line,"<br>");
            strcat(varValue, line);
        }
        fclose(fd);
    }
   gui_debug("\nYUKI===Get Diagnostics Logs ===varValue=%s\n",varValue);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiClearLogs
 *
 *  [DESCRIPTION]:
 *      Clear Diagnostics logs.
 *
 *  [return]:
 **************************************************************************/
int cgiClearLogs()
{
    unlink(SYSLOGS_ALL_PATH);
    gui_debug("\nYUKI===Clear Diagnostics Logs successfully.\n");
    return 0;
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiSaveLog
 *
 *  [DESCRIPTION]:
 *      Save Diagnostics logs of file named log.txt in the PC.
 *
 *  [return]:
 **************************************************************************/
int cgiSaveLog(struct client *cl, struct http_request *req)
{
#if 0
    FILE *fd=NULL;
    char line[BUFLEN_512] = {0}; 
    char logs_buf[WEB_BUF_SIZE_MAX] = {0};
    int buflen = 0;
    if((fd = fopen(SYSLOGS_ALL_PATH, "r")) != NULL)
    {
        while(fgets(line, sizeof(line), fd) )
        {
            gui_debug("Diagnostics Logs line: %s\n", line);
            strcat(logs_buf, line);
        }
        fclose(fd);
    }
    buflen = strlen(logs_buf);
    send_download_file(cl, req, "logs.txt", logs_buf, buflen);
    gui_debug("\nYUKI===Save Diagnostics Logs successfully.\n");
#endif

    return 0;
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetNatTable
 *
 *  [DESCRIPTION]:
 *      Get Diagnostics NAT Table.
 *
 *  [return]:sessions available|sessions in use|nat table
 **************************************************************************/
void cgiGetNatTable(int argc, char **argv, char *varValue)
{
#if 0
    char buf[BUFLEN_512] = {0};
    dal_ret_t *ret_nat = NULL;
    int num = 0;
    int session_use = 0;
    int session_avail = 0;
    ret_nat = dal_get_nattable_data(&num, &session_avail, &session_use);
    if(ret_nat == NULL || num == 0)
    {
        gui_debug("\nYUKI===Get Diagnostics NAT Table Failed or no NAT Results\n");
        return;
    }

    sprintf(buf, "%d||", session_avail);
    strcat(varValue, buf);
    sprintf(buf, "%d||", session_use);
    strcat(varValue, buf);
    for (int i = 0; i < num; ++i)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(ret_nat[i].param[FW_NATTABLE_IPFamily]));
        strcat(varValue, buf);
    }
    
    gui_debug("\nYUKI===Get Diagnostics NAT Table ===varValue=%s\n",varValue);
#endif
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostUpdateFirmware
 *
 *  [DESCRIPTION]:
 *      Update firmware
 *
 *  [return]:
 **************************************************************************/
int cgiPostUpdateFirmware(char *uploadFileName)
{
    int ret=-1;
    char cmd[BUFLEN_256] = {'\0'};

#if 0 //for  qca openwrt
    int ret_size = sizeof(dal_ret_t);
    dal_ret_t ret;
    memset(&ret, 0, ret_size);
    ret.param[NEW_IMAGE] = &uploadFileName[0];
    gui_debug("==update firmware file path=%s\n",ret.param[NEW_IMAGE]);
    dal_sys_update(&ret);
    //dal_SaveConfig();
#endif

#ifdef AEI_BCM_FIRMWARE_UPLOAD
    gui_debug("==^_^==update firmware file path=%s\n",uploadFileName);
    //flash_image_from_file(uploadFileName);
    int filesize = get_file_size(uploadFileName);
    gui_debug("Upgrade firmware %s 's filesize = %d", uploadFileName, filesize);
    ret=flash_image_from_file(uploadFileName);
    gui_debug("flash_image_from_file result = %d", ret);
    gWebParams.upgrade_status = ret;
    gui_debug("gWebParams.upgrade_status = %d", gWebParams.upgrade_status);

    //delete upload file
    memset (cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd)-1, "rm -rf %s", uploadFileName);
    gui_debug("cmd = %s", cmd);
    system(cmd);

    if (ret==0){
        system( "reboot" );
    }
#endif
    return ret;
}
/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetNatTable
 *
 *  [DESCRIPTION]:
 *      Get Update firmware progress.
 *
 *  [return]:-1, -2 or update percent.
 **************************************************************************/
void cgiGetUpdateProgress(int argc, char **argv, char *varValue)
{
    int result = 0;
    result = dal_sys_get_update_progress();
    sprintf(varValue, "%d", result);
    gui_debug("===update firmware progress:%s\n",varValue);

}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetCurFirmVersion
 *
 *  [DESCRIPTION]:
 *      Get Current firmware version.
 *
 *  [return]:
 **************************************************************************/
void cgiGetCurFirmVersion(int argc, char **argv, char *varValue)
{
#if 0
    char buf[BUFLEN_256] = {0};
    dal_ret_t *ret_dev = NULL;
    ret_dev = dal_get_device_info();
    if(ret_dev == NULL)
        return;
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s", getValueFromDal(ret_dev->param[DAL_DEVINFO_SW_VER]));
    strcat(varValue, buf);

    gui_debug("\nYUKI===Get Current firmware version ===varValue=%s\n",varValue);
    free_dal_ret(&ret_dev);
#endif
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostTroubleShoot
 *
 *  [DESCRIPTION]:
 *      Ping/Traceroute/NSLookup
 *
 *  [return]:
 **************************************************************************/
int cgiPostTroubleShoot()
{
    int result = 0;
#if 0
    int ret_size = sizeof(dal_ret_t);
    char request[BUFLEN_64] = {0};
    dal_ret_t ret;
    memset(&ret, 0, ret_size);
    strcpy(request, DIAGNOSTICS_START);
    gui_debug("===troubleshoot action = %s; protocol = %s; request=%s; address=%s\n",gWebParams.action, gWebParams.troubleshoot_protocol, request, gWebParams.troubleshoot_address);
    if(!safe_strcmp(gWebParams.troubleshoot_protocol, "ipv4"))
    {
        //IPV4
        if(!safe_strcmp(gWebParams.action, "ping"))
        {
            //Ping
            ret.param[DIAGNOSTICS_REQ] = &request[0];
            ret.param[DIAGNOSTICS_HOST_NAME] = &gWebParams.troubleshoot_address[0];
            gui_debug("Ping===troubleshoot DIAGNOSTICS_REQ = %s; DIAGNOSTICS_HOST_NAME=%s\n",ret.param[DIAGNOSTICS_REQ], ret.param[DIAGNOSTICS_HOST_NAME]);
            unlink(PING_FIN_FILE);
            unlink(PING_RESULT_FILE);
            unlink(PING_STATUS_FILE);
            result = dal_sys_set_ping(&ret);
        }
        else if(!safe_strcmp(gWebParams.action, "traceroute"))
        {
            //Traceroute
            ret.param[DIAGNOSTICS_REQ] = &request[0];
            ret.param[DIAGNOSTICS_HOST_NAME] = &gWebParams.troubleshoot_address[0];
            gui_debug("Traceroute===troubleshoot DIAGNOSTICS_REQ = %s; DIAGNOSTICS_HOST_NAME=%s\n",ret.param[DIAGNOSTICS_REQ], ret.param[DIAGNOSTICS_HOST_NAME]);
            result = dal_sys_set_trace_route(&ret);
        }
        else if(!safe_strcmp(gWebParams.action, "NSLookup"))
        {
            //NSLookup
            ret.param[DIAGNOSTICS_REQ] = &request[0];
            ret.param[DIAGNOSTICS_HOST_NAME] = &gWebParams.troubleshoot_address[0];
            gui_debug("NSLookup===troubleshoot DIAGNOSTICS_REQ = %s; DIAGNOSTICS_HOST_NAME=%s\n",ret.param[DIAGNOSTICS_REQ], ret.param[DIAGNOSTICS_HOST_NAME]);
            result = dal_sys_set_nsl_lookup(&ret);
        }

    }else{
        //IPV6
    }
    result = dal_SaveConfig();
#endif

    return result;

}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetPingResult
 *
 *  [DESCRIPTION]:
 *      Get Ping Results.
 *
 *  [return]:
 **************************************************************************/
void cgiGetPingResult(int argc, char **argv, char *varValue)
{
#if 0
    FILE *fd_result=NULL;
    FILE *fd_status=NULL;
    char line[BUFLEN_512] = {0};
    char ping_result[WEB_BUF_SIZE_MAX] = {0};
    char ping_status[WEB_BUF_SIZE_MAX] = {0};

    //ping_result.log
    if((fd_status = fopen(PING_STATUS_FILE, "r")) != NULL)
    {
        while(fgets(line, sizeof(line), fd_status) )
        {
            //gui_debug("Ping Results line: %s\n", line);
            if(line[strlen(line)-1]=='\n')//remove the \n
               line[strlen(line)-1]='\0';
            strcat(line,"&#13;&#10;");
            strcat(ping_status, line);
        }
        fclose(fd_status);
    }
    strcat(ping_status, "||");
    strcat(varValue, ping_status);

    //ping.log
    if((fd_result = fopen(PING_RESULT_FILE, "r")) != NULL)
    {
        while(fgets(line, sizeof(line), fd_result) )
        {
            //gui_debug("Ping Results line: %s\n", line);
            if(line[strlen(line)-1]=='\n')//remove the \n
               line[strlen(line)-1]='\0';
            strcat(line,"&#13;&#10;");
            strcat(ping_result, line);
        }
        fclose(fd_result);
    }
    strcat(varValue, ping_result);

   gui_debug("\nYUKI===Get Ping Results ===varValue=%s\n",varValue);
#endif
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetTracerouteResult
 *
 *  [DESCRIPTION]:
 *      Get Traceroute Results.
 *
 *  [return]:
 **************************************************************************/
void cgiGetTracerouteResult(int argc, char **argv, char *varValue)
{
#if 0
    FILE *fd=NULL;
    char  line[BUFLEN_512] = {0};   

    if((fd = fopen(TRACE_ROUTE_RESULT_FILE, "r")) != NULL)
    {
        while(fgets(line, sizeof(line), fd) )
        {
            //gui_debug("Traceroute Results line: %s\n", line);
            if(line[strlen(line)-1]=='\n')//remove the \n
               line[strlen(line)-1]='\0';
            strcat(line,"&#13;&#10;");
            strcat(varValue, line);
        }
        fclose(fd);
    }
   gui_debug("\nYUKI===Get Traceroute Results ===varValue=%s\n",varValue);
#endif
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetNSLookupResult
 *
 *  [DESCRIPTION]:
 *      Get NSLookup Results.
 *
 *  [return]:
 **************************************************************************/
void cgiGetNSLookupResult(int argc, char **argv, char *varValue)
{
#if 0
    FILE *fd=NULL;
    char  line[BUFLEN_512] = {0};   

    if((fd = fopen(NSL_LOOKUP_RESULT_FILE, "r")) != NULL)
    {
        while(fgets(line, sizeof(line), fd) )
        {
            //gui_debug("NSLookup Results line: %s\n", line);
            if(line[strlen(line)-1]=='\n')//remove the \n
               line[strlen(line)-1]='\0';
            strcat(line,"&#13;&#10;");
            strcat(varValue, line);
        }
        fclose(fd);
    }
   gui_debug("\nYUKI===Get NSLookup Results ===varValue=%s\n",varValue);
#endif
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostNatALGSettings
 *
 *  [DESCRIPTION]:
 *      Set NAT ALG Settings: VPN pass-through, VoIP SIP, VoIP H323.
 *
 *  [return]:
 **************************************************************************/
void cgiPostNatALGSettings()
{
#if 0
    int ret_size = sizeof(dal_ret_t);
    dal_ret_t voip_alg;
    dal_ret_t vpn_alg;
    memset(&voip_alg, 0, ret_size);
    memset(&vpn_alg, 0, ret_size);

    voip_alg.param[FW_ALG_VOIP_SET_SIP] = &gWebParams.alg_voipSip[0];
    voip_alg.param[FW_ALG_VOIP_SET_H323] = &gWebParams.alg_voipH323[0];

    vpn_alg.param[FW_ALG_VPN_SET_PPTP] = &gWebParams.alg_vpn[0];

    gui_debug("FW_ALG_VOIP_SET_SIP=%s;FW_ALG_VOIP_SET_H323=%s;FW_ALG_VPN_SET_PPTP=%s\n",voip_alg.param[FW_ALG_VOIP_SET_SIP],voip_alg.param[FW_ALG_VOIP_SET_H323],vpn_alg.param[FW_ALG_VPN_SET_PPTP]);

    if(dal_set_voip_alg(&voip_alg) != TSL_RV_SUC)
    {
        gui_debug("dal_set_voip_alg set failed.");
    }

    if(dal_set_vpn_alg(&vpn_alg) != TSL_RV_SUC)
    {
        gui_debug("dal_set_vpn_alg set failed.");
    }
    
    dal_SaveConfig();
#endif
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetNatALGSettings
 *
 *  [DESCRIPTION]:
 *      Get NAT ALG Settings: VPN pass-through, VoIP SIP, VoIP H323.
 *
 *  [return]:1|1|1
 **************************************************************************/
void cgiGetNatALGSettings(int argc, char **argv, char *varValue)
{
#if 0
    char buf[BUFLEN_256] = {0};
    dal_ret_t *ret_voip_alg = NULL;
    dal_ret_t *ret_vpn_alg = NULL;
    ret_voip_alg = dal_get_voip_alg();
    ret_vpn_alg = dal_get_vpn_alg();
    if(ret_voip_alg == NULL || ret_vpn_alg == NULL)
        return;

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_vpn_alg->param[FW_ALG_VPN_SET_PPTP]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_voip_alg->param[FW_ALG_VOIP_SET_SIP]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s", getValueFromDal(ret_voip_alg->param[FW_ALG_VOIP_SET_H323]));
    strcat(varValue, buf);

    gui_debug("\nYUKI===Get NAT ALG Settings===varValue=%s\n",varValue);
    free_dal_ret(&ret_vpn_alg);
    free_dal_ret(&ret_voip_alg);
#endif
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostSyslogSettings
 *
 *  [DESCRIPTION]:
 *      Set Syslog Settings.
 *
 *  [return]:
 **************************************************************************/
void cgiPostSyslogSettings()
{
#if 0
    int ret_size = sizeof(dal_ret_t);
    dal_ret_t syslog;
    memset(&syslog, 0, ret_size);

    if(!safe_strcmp(gWebParams.syslog_stats, ENABLE_LOGGING_TO_SERVER))
    {
        syslog.param[SYSLOG_SERVER] = &gWebParams.syslog_server[0];
    }
    syslog.param[ENABLE_LOGHOST] = &gWebParams.syslog_stats[0];

    gui_debug("ENABLE_LOGHOST=%s\n",syslog.param[ENABLE_LOGHOST]);
    gui_debug("SYSLOG_SERVER=%s\n",syslog.param[SYSLOG_SERVER]);

    if(dal_sys_set_syslog_server(&syslog) != TSL_RV_SUC)
    {
        gui_debug("dal_sys_set_syslog_server set failed.");
    }
    
    dal_SaveConfig();
#endif
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetSyslogSettings
 *
 *  [DESCRIPTION]:
 *      Get Syslog Settings
 *
 *  [return]:ENABLE_LOGHOST|SYSLOG_SERVER
 **************************************************************************/
void cgiGetSyslogSettings(int argc, char **argv, char *varValue)
{
#if 0
    char buf[BUFLEN_256] = {0};
    dal_ret_t *syslog = NULL;
    syslog = dal_sys_get_syslog_server();
    if(syslog == NULL)
        return;

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(syslog->param[ENABLE_LOGHOST]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(syslog->param[SYSLOG_SERVER]));
    strcat(varValue, buf);

    gui_debug("\nYUKI===Get Syslog Settings===varValue=%s\n",varValue);
    free_dal_ret(&syslog);
#endif
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostRestartWAN
 *
 *  [DESCRIPTION]:
 *      Restart WAN connection
 *
 *  [return]:
 **************************************************************************/
void cgiPostRestartWAN()
{
    char cmd[BUFLEN_256];

    memset (cmd, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd)-1, "kill -s USR1 `pidof udhcpc` > /dev/null");
    system(cmd);
}

void cgiPostAdvManagement()
{
    gui_debug("%s==%s===%s==%d",gWebParams.adv_ip,gWebParams.adv_subnet,gWebParams.adv_gateway,gWebParams.adv_autoconfig);
    if(tr69SetUnfreshLeafDataString(DEVICE_DHCPV4_CLIENT, 1, "X_ACTIONTEC_COM_DefaultIPAddress", gWebParams.adv_ip))
    {
        gui_debug("Set Device.DHCPv4.Client.1.X_ACTIONTEC_COM_DefaultIPAddress error!");
    }


    if(tr69SetUnfreshLeafDataString(DEVICE_DHCPV4_CLIENT, 1, "X_ACTIONTEC_COM_DefaultSubnetMask", gWebParams.adv_subnet))
    {
        gui_debug("Set Device.DHCPv4.Client.1.X_ACTIONTEC_COM_DefaultSubnetMask error!");
    }

    if(tr69SetUnfreshLeafDataString(DEVICE_DHCPV4_CLIENT, 1, "X_ACTIONTEC_COM_DefaultIPRouters", gWebParams.adv_gateway))
    {
        gui_debug("Set Device.DHCPv4.Client.1.X_ACTIONTEC_COM_DefaultIPRouters error!");
    }
    
    if(tr69SetLeafDataInt("Device.X_ACTIONTEC_COM_ZeroConf.Extender", 0, "Enable",gWebParams.adv_autoconfig))
    {
        gui_debug("Set Device.X_ACTIONTEC_COM_ZeroConf.Extender Enable error!");
    }
        
    if(tr69SaveNow())
    {
        gui_debug("tr69SaveNow error!");
    }    
    return;
}   


void cgiPostAdvReboot()
{
    system( "reboot" );
}

void cgiGetAdvManagement(int argc, char **argv, char *varValue)
{	
    char default_gateway[BUF_LEN20]={0};
    char default_ip[BUF_LEN20]={0};
    char default_subnet[BUF_LEN20]={0};
    char autoconfig_str[BUF_LEN20]={0};
    char buf[BUF_LEN20]={0};
 
    if(tr69GetUnfreshLeafData(default_ip, DEVICE_DHCPV4_CLIENT_1, "X_ACTIONTEC_COM_DefaultIPAddress"))
    {
        gui_debug("Get Device.DHCPv4.Client.1.X_ACTIONTEC_COM_DefaultIPAddress error!");
    } 
    sprintf(buf, "%s|", default_ip);    
    strcat(varValue, buf);		

    if(tr69GetUnfreshLeafData(default_subnet, DEVICE_DHCPV4_CLIENT_1, "X_ACTIONTEC_COM_DefaultSubnetMask"))
    {
        gui_debug("Get Device.DHCPv4.Client.1.X_ACTIONTEC_COM_DefaultSubnetMask error!");
    } 
    sprintf(buf, "%s|", default_subnet);    
    strcat(varValue, buf);		

    if(tr69GetUnfreshLeafData(default_gateway, DEVICE_DHCPV4_CLIENT_1, "X_ACTIONTEC_COM_DefaultIPRouters"))
    {
        gui_debug("Get Device.DHCPv4.Client.1.X_ACTIONTEC_COM_DefaultIPRouters error!");
    } 
    sprintf(buf, "%s|", default_gateway);    
    strcat(varValue, buf);	
		
    if(tr69GetFreshLeafData(autoconfig_str, "Device.X_ACTIONTEC_COM_ZeroConf.Extender", "Enable"))
    {
        gui_debug("Get Device.X_ACTIONTEC_COM_ZeroConf.Extender Enable error!");
    } 
    sprintf(buf, "%s|", autoconfig_str);    
    strcat(varValue, buf);	

}    


void cgiGetDevSerialNum(int argc, char **argv, char *varValue)
{

    char serial_numb[BUF_LEN64] = {0};


    if(tr69GetUnfreshLeafData(serial_numb, "Device.DeviceInfo", "SerialNumber"))
    {
        gui_debug( "Get Device.DeviceInfo.SerialNumber error!");
    }
    strcat(varValue, serial_numb);		
    gui_debug("serial_numb = %s",varValue);
}    


void cgiGetCurIPInfo(int argc, char **argv, char *varValue)
{

    char cur_ip_str[BUF_LEN20] = {0};
    char cur_subnet_str[BUF_LEN20] = {0};
    char cur_gateway_str[BUF_LEN20] = {0};
	char gateway_str[BUF_LEN20]={0};
    char eth_mac[BUF_LEN64] = {0};
    char buf[BUF_LEN64]={0};


    if(tr69GetUnfreshLeafData(cur_ip_str, DEVICE_IP_INTER1_IPADDR_1, "IPAddress"))
    {
        gui_debug( "Get Device.IP.Interface.1.IPv4Address.1.IPAddress error!");
    }
    sprintf(buf, "%s|", cur_ip_str);    
    strcat(varValue, buf);		

    if(tr69GetUnfreshLeafData(cur_subnet_str, DEVICE_IP_INTER1_IPADDR_1, "SubnetMask"))
    {
        gui_debug( "Get Device.IP.Interface.1.IPv4Address.1.SubnetMask error!");
    }
    sprintf(buf, "%s|", cur_subnet_str);    
    strcat(varValue, buf);	

    if(tr69GetUnfreshLeafData(cur_gateway_str, DEVICE_DHCPV4_CLIENT_1, "IPRouters"))
    {
        gui_debug( "Get Device.DHCPv4.Client.1.IPRouters error!");
    }    
    if(!safe_strcmp(cur_gateway_str, "NULL") || !safe_strcmp(cur_gateway_str, "0.0.0.0"))
    {            
        if(tr69GetUnfreshLeafData(gateway_str, DEVICE_DHCPV4_CLIENT_1, "X_ACTIONTEC_COM_DefaultIPRouters"))
        {
            gui_debug( "Get Device.DHCPv4.Client.1.X_ACTIONTEC_COM_DefaultIPRouters error!");
        } 
        strcpy(cur_gateway_str,gateway_str);
    }
    sprintf(buf, "%s|", cur_gateway_str);    
    strcat(varValue, buf);

    if(tr69GetUnfreshLeafData(eth_mac, "Device.Ethernet.Interface.1", "MACAddress"))
    {
        gui_debug( "Get Device.Ethernet.Interface.1.MACAddress error!");
    }
    sprintf(buf, "%s|", eth_mac);    
    strcat(varValue, buf);

    gui_debug("IP Info = %s",varValue);
}    

void cgiPostAdvPwdErrClear()
{
    gWebParams.change_pwd_old_error = 0;
    gui_debug("old err = %d",gWebParams.change_pwd_old_error);
}

void cgiPostAdvPassword()
{
    int result = 0;
    int num = 0;

    char tr69_username[64] = {0};
    char tr69_password[64] = {0};
    char buf[4] = {0};
    char mdmoid_user_object[256] = {0};

    if(tr69GetUnfreshLeafData(buf, "Device.Users", "UserNumberOfEntries"))
    {
        gui_debug( "Get Device.Users.UserNumberOfEntries error!");
    }
    num = atoi(buf);
          

    for (int i=1; i<=num; i++)
    {
        sprintf(mdmoid_user_object, "%s.%d", "Device.Users.User", i);

        if(tr69GetUnfreshLeafData(tr69_username, mdmoid_user_object, "Username"))   
        {
            gui_debug( "Get %s.Username error!", mdmoid_user_object);
        }
            
        if(tr69GetUnfreshLeafData(tr69_password, mdmoid_user_object, "Password"))
        {
            gui_debug( "Get %s.Password error!", mdmoid_user_object);
        }

        gui_debug("Change password USER_NAME=%s==%s, adv_currentpass=%s==%s", gWebParams.login_username,tr69_username,gWebParams.adv_currentpass,tr69_password);
            
        if (safe_strcmp(tr69_username, gWebParams.login_username) == 0)
        {
            struct MD5Context md5c;
            unsigned char signature[16] = {0};
            unsigned char csig[16] = {0};
            char *clabel = NULL, *hexfmt = "%02X";
            unsigned int bp;
            int j = 0; 
            char new_password_md5[64] = {0};
            char pass_buf[4] = {0};

            MD5Init(&md5c);
            MD5Update(&md5c, (unsigned char *)gWebParams.adv_currentpass, strlen(gWebParams.adv_currentpass));
            MD5Final(signature, &md5c);

            clabel = tr69_password;     
            for (j = 0; j < 16; j++) {
                if (isxdigit((int) clabel[0]) && isxdigit((int) clabel[1]) &&
                                sscanf((tr69_password + (j * 2)), hexfmt, &bp) == 1) {
                    csig[j] = (unsigned char) bp;
                } else {                
                    break;
                }
                
                clabel += 2;
            }        

            gui_debug("@@@@Change    password signature=%s,csig=%s",signature,csig);
            gWebParams.change_pwd_old_error = 0;
            for (j = 0; j < sizeof signature; j++) 
            {
                if (signature[j] != csig[j]) {
                {
                    gWebParams.change_pwd_old_error = 1;
                    return;
                }
              }
            }



            MD5Init(&md5c);
            MD5Update(&md5c, (unsigned char *)gWebParams.adv_newpass, strlen(gWebParams.adv_newpass));
            MD5Final(signature, &md5c);
            
            for (j = 0; j < sizeof signature; j++) {
                sprintf(pass_buf, hexfmt, signature[j]);
                strcat(new_password_md5, pass_buf);
            }


            gui_debug("new pass md5=%s",new_password_md5);
            if(tr69SetUnfreshLeafDataString("Device.Users.User", i, "Password", new_password_md5))
            {                   
                gui_debug( "Set Device.Users.User.%d.Password error!", i);
            }  
                
            break;
        }
    }

    if(tr69SaveNow())
    {
         gui_debug("tr69SaveNow error!");
    }     
    return;
}


void cgiPostForgotPassword()
{
    int num = 0;

    char tr69_username[64] = {0};
    char tr69_password[64] = {0};
    char buf[4] = {0};
    char mdmoid_user_object[256] = {0};

    struct MD5Context md5c;
    unsigned char signature[16] = {0};
    char *hexfmt = "%02X";
    int j = 0; 
    char new_password_md5[64] = {0};
    char pass_buf[4] = {0};
    

    if(tr69GetUnfreshLeafData(buf, "Device.Users", "UserNumberOfEntries"))
    {
        gui_debug( "Get Device.Users.UserNumberOfEntries error!");
    }
    num = atoi(buf);
          

    for (int i=1; i<=num; i++)
    {
        memset(&md5c,0,sizeof(md5c));
        memset (signature, 0, sizeof(signature));
        memset (new_password_md5, 0, sizeof(new_password_md5));
        memset (pass_buf, 0, sizeof(pass_buf));
        j = 0; 

    
        sprintf(mdmoid_user_object, "%s.%d", "Device.Users.User", i);

        if(tr69GetUnfreshLeafData(tr69_username, mdmoid_user_object, "Username"))   
        {
            gui_debug( "Get %s.Username error!", mdmoid_user_object);
        }
            
        if(tr69GetUnfreshLeafData(tr69_password, mdmoid_user_object, "Password"))
        {
            gui_debug( "Get %s.Password error!", mdmoid_user_object);
        }

        gui_debug("Forgot password USER_NAME==%s, re_enter_password=%s==%s",tr69_username,gWebParams.re_enter_password,tr69_password);

#if defined(AEI_WECB_CUSTOMER_TELUS) || defined(AEI_WECB_CUSTOMER_VERIZON) || defined(AEI_WECB_CUSTOMER_NCS)
        if (safe_strcmp(tr69_username, "admin") == 0)
#else
        if (0)            
#endif
        {
            MD5Init(&md5c);
            MD5Update(&md5c, (unsigned char *)gWebParams.re_enter_password, strlen(gWebParams.re_enter_password));
            MD5Final(signature, &md5c);
            
            for (j = 0; j < sizeof signature; j++) {
                sprintf(pass_buf, hexfmt, signature[j]);
                strcat(new_password_md5, pass_buf);
            }


            gui_debug("new pass md5=%s",new_password_md5);
            if(tr69SetUnfreshLeafDataString("Device.Users.User", i, "Password", new_password_md5))
            {                   
                gui_debug( "Set Device.Users.User.%d.Password error!", i);
            }  
                
        }
    }

    if(tr69SaveNow())
    {
         gui_debug("tr69SaveNow error!");
    }     
    return;
}


void cgiGetRedirectIP(int argc, char **argv, char *varValue)
{
    char default_ip[BUF_LEN20]={0};
 
    if(tr69GetUnfreshLeafData(default_ip, DEVICE_DHCPV4_CLIENT_1, "X_ACTIONTEC_COM_DefaultIPAddress"))
    {
        gui_debug("Get Device.DHCPv4.Client.1.X_ACTIONTEC_COM_DefaultIPAddress error!");
    } 
    strcat(varValue, default_ip);
}

void cgiGetMainProductInfo(int argc, char **argv, char *varValue)
{	
    char buf[BUF_LEN64]={0};
	unsigned long sec, mn, hr, day;
    

    if(tr69GetUnfreshLeafData(buf, "Device.DeviceInfo", "SoftwareVersion"))
    {
        gui_debug("Get Device.DeviceInfo.SoftwareVersion error!");
    } 
    sprintf(buf, "%s|", buf);    
    strcat(varValue, buf);	

    struct sysinfo info ;

	sysinfo(&info);
	sec = (unsigned long) info.uptime ;
	day = sec / 86400;

	sec %= 86400;
	hr = sec / 3600;
	sec %= 3600;
	mn = sec / 60;
	sec %= 60;
	sprintf(buf, "%dday:%dh:%dm:%ds|",day, hr, mn, sec); 
    strcat(varValue, buf);	

    memset(buf, 0, sizeof(buf));
    if(tr69GetUnfreshLeafData(buf, "Device.DeviceInfo", "X_ACTIONTEC_COM_FCC_ID"))
    {
        gui_debug("Get Device.DeviceInfo.SoftwareVersion error!");
        strcat(varValue, "|");
    }
    else {
        sprintf(buf, "%s|", buf);    
        strcat(varValue, buf);    
    }
}

void cgiGetMainNetStation(int argc, char **argv, char *varValue)
{	
	char buf[256] = {0};
	char full_name[256] = {0};
	int host_entries = 0;
	int i = 0;
	int nBytesSent = 0;	
	char ip_address[64] = {0};	
	char mac_address[64] = {0};	
	char interface_type[64] = {0};
	int active = 0;
	
	
	if(tr69GetUnfreshLeafData(buf, "Device.Hosts", "HostNumberOfEntries"))
    {
        gui_debug("Get Device.Hosts.HostNumberOfEntries error");
    }
	host_entries = atoi(buf);

	gui_debug("host_entries: %d", host_entries);

	for(i = 1; i <= host_entries; i++)
	{
		sprintf(full_name, "Device.Hosts.Host.%d", i);

		if(tr69GetUnfreshLeafData(ip_address, full_name, "IPAddress"))
        {
            gui_debug( "Get %s.IPAddress error", full_name);
        }   
        
		if(tr69GetUnfreshLeafData(mac_address, full_name, "PhysAddress"))
        {
            gui_debug( "Get %s.PhysAddress error", full_name);
        }  
        
		if(tr69GetUnfreshLeafData(interface_type, full_name, "X_ACTIONTEC_COM_InterfaceType"))
        {
            gui_debug( "Get %s.X_ACTIONTEC_COM_InterfaceType error", full_name);
        }  
        
		if(tr69GetUnfreshLeafData(buf, full_name, "Active"))
        {
            gui_debug("Get %s.Active error", full_name);
        }  
		active = atoi(buf);

		if(active)
		{
	        sprintf(buf, "%s|%s|%s|%d||",interface_type, mac_address, ip_address, active); 
            strcat(varValue, buf);
		}
	}
}


void cgiPostFactoryDefault()
{	
    gui_debug("@@will restore default@@");
	//system( "killall data_center" );
	//system( "rm -f /mnt/rt_conf/pre_cfg.xml.gz" );
	//system( "sync" );
    system("cli -r default");
    system( "reboot" );
} 


void cgiPostAdvLanguage()
{	

} 

//call by wirelesschanging_notify.html
void actZeroConfAutoSync(int argc, char **argv, char *varValue)
{
    char autoconfig_str[BUF_LEN20]={0};

    if(tr69GetFreshLeafData(autoconfig_str, "Device.X_ACTIONTEC_COM_ZeroConf.Extender", "Enable"))
    {
        gui_debug("Get Device.X_ACTIONTEC_COM_ZeroConf.Extender Enable error!");
    } 

    if (atoi(autoconfig_str) == 1){
        system("zero_conf_telus AutoSync &");
        gui_debug("AutoSync_Done");        
    }
    strcat(varValue, "AutoSync_Done");
}

//call by firmwareupgrade_notify.html
void actZeroConfUpgrade(int argc, char **argv, char *varValue)
{
    char autoconfig_str[BUF_LEN20]={0};

    if(tr69GetFreshLeafData(autoconfig_str, "Device.X_ACTIONTEC_COM_ZeroConf.Extender", "Enable"))
    {
        gui_debug("Get Device.X_ACTIONTEC_COM_ZeroConf.Extender Enable error!");
    } 

    if (atoi(autoconfig_str) == 1){
        system("zero_conf_telus Upgrade &");
        gui_debug("Upgrade_Done");
    }
    strcat(varValue, "Upgrade_Done");    
}



void cgiGetAdvTR69(int argc, char **argv, char *varValue){
    gui_debug("cgiGetAdvTR69 start");

    char tr69_node[BUFLEN_64] = {0};

    char tr69_enable[BUFLEN_8] = {0};
    char tr69_url[BUFLEN_256+1] = {0};
    char tr69_username[BUFLEN_256+1] = {0};
    char tr69_password[BUFLEN_256+1] = {0};
    char tr69_periodicinformenable[BUFLEN_4] = {0};
    char tr69_periodicnnforminterval[BUFLEN_4] = {0};
    char tr69_periodicinformtime[BUFLEN_64] = {0};
    char tr69_connectionrequesturl[BUFLEN_256+1] = {0};
    char tr69_connectionrequestusername[BUFLEN_256+1] = {0};
    char tr69_connectionrequestpassword[BUFLEN_256+1] = {0};


    sprintf(tr69_node, "%s", "Device.ManagementServer");

    if(tr69GetUnfreshLeafData(tr69_url, tr69_node, "URL")) {
        gui_debug("get URL error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, tr69_url);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(tr69_username, tr69_node, "Username")) {
        gui_debug("get Username error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, tr69_username);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(tr69_password, tr69_node, "Password")) {
        gui_debug("get Password error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, tr69_password);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(tr69_periodicinformenable, tr69_node, "PeriodicInformEnable")) {
        gui_debug("get PeriodicInformEnable error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, tr69_periodicinformenable);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(tr69_periodicnnforminterval, tr69_node, "PeriodicInformInterval")) {
        gui_debug("get PeriodicInformInterval error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, tr69_periodicnnforminterval);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(tr69_periodicinformtime, tr69_node, "PeriodicInformTime")) {
        gui_debug("get PeriodicInformTime error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, tr69_periodicinformtime);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(tr69_connectionrequesturl, tr69_node, "ConnectionRequestURL")) {
        gui_debug("get ConnectionRequestURL error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, tr69_connectionrequesturl);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(tr69_connectionrequestusername, tr69_node, "ConnectionRequestUsername")) {
        gui_debug("get ConnectionRequestUsername error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, tr69_connectionrequestusername);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(tr69_connectionrequestpassword, tr69_node, "ConnectionRequestPassword")) {
        gui_debug("get ConnectionRequestPassword error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, tr69_connectionrequestpassword);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(tr69_enable, tr69_node, "EnableCWMP")) {
        gui_debug("get EnableCWMP error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, tr69_enable);
        strcat(varValue, "|");
    }


    gui_debug(">>>>>>%s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}

void cgiPostAdvTR69()
{
    gui_debug("advanced_tr69_reboot => %d", gWebParams.advanced_tr69_reboot);
    gui_debug("advanced_tr69_enable => %d", gWebParams.advanced_tr69_enable);
    gui_debug("advanced_tr69_url => %s", gWebParams.advanced_tr69_url);
    gui_debug("advanced_tr69_username => %s", gWebParams.advanced_tr69_username);
    gui_debug("advanced_tr69_password => %s", gWebParams.advanced_tr69_password);
    gui_debug("advanced_tr69_informenable => %s", gWebParams.advanced_tr69_informenable);
    gui_debug("advanced_tr69_informinterval => %s", gWebParams.advanced_tr69_informinterval);
    gui_debug("advanced_tr69_informtime => %s", gWebParams.advanced_tr69_informtime);
    
    //gui_debug("advanced_tr69_backoff_interval => %s", gWebParams.advanced_tr69_backoff_interval);
    //gui_debug("advanced_tr69_connection_url => %s", gWebParams.advanced_tr69_connection_url);
    gui_debug("advanced_tr69_connection_username => %s", gWebParams.advanced_tr69_connection_username);
    gui_debug("advanced_tr69_connection_password => %s", gWebParams.advanced_tr69_connection_password);


    tr69CommitSetLeafDataInt("Device.ManagementServer", 0, "EnableCWMP", gWebParams.advanced_tr69_enable);
    if (gWebParams.advanced_tr69_enable == 1){
        if(gWebParams.advanced_tr69_reboot==1)
        {
            system("killall watchdog_tr69");
            system("killall tr69");
            tr69CommitSetLeafDataString("Device.ManagementServer", 0, "PeriodicInformTime", "0000-00-00T00:00:00+0000");
        }
        tr69CommitSetLeafDataString("Device.ManagementServer", 0, "URL", gWebParams.advanced_tr69_url);
        tr69CommitSetLeafDataString("Device.ManagementServer", 0, "Username", gWebParams.advanced_tr69_username);
        tr69CommitSetLeafDataString("Device.ManagementServer", 0, "Password", gWebParams.advanced_tr69_password);
        tr69CommitSetLeafDataInt("Device.ManagementServer", 0, "PeriodicInformEnable", atoi(gWebParams.advanced_tr69_informenable));
        tr69CommitSetLeafDataInt("Device.ManagementServer", 0, "PeriodicInformInterval", atoi(gWebParams.advanced_tr69_informinterval));
        

        //advanced_tr69_backoff_interval
        //tr69CommitSetLeafDataString("Device.ManagementServer", 0, "ConnectionRequestURL", gWebParams.advanced_tr69_connection_url);    
        tr69CommitSetLeafDataString("Device.ManagementServer", 0, "ConnectionRequestUsername", gWebParams.advanced_tr69_connection_username);
        tr69CommitSetLeafDataString("Device.ManagementServer", 0, "ConnectionRequestPassword", gWebParams.advanced_tr69_connection_password);
    }
    
    tr69CommitSetLeafDataEnd("Device.ManagementServer", 0);
    tr69SaveNow();

    if (gWebParams.advanced_tr69_reboot == 1){
        sleep(5);   //after tr69_save_now call reboot, do not save data to flash.so need sleep 5 seconds.
        system( "reboot" );
    }

    return;
}   


//return string: //enable|timeout|username|password|port|
void cgiGetAdvLocalssh(int argc, char **argv, char *varValue){
    gui_debug("cgiGetAdvLocalssh start");

    char ssh_node[BUFLEN_64] = {0};

    char ssh_enable[BUFLEN_8] = {0};
    char ssh_username[BUFLEN_64] = {0};
    //char ssh_password[BUFLEN_128] = {0};
    char ssh_timeout[BUFLEN_16] = {0};
    char ssh_port[BUFLEN_8] = {0};


    sprintf(ssh_node, "%s", "Device.X_ACTIONTEC_COM_RemoteLogin");

    if(tr69GetUnfreshLeafData(ssh_enable, ssh_node, "Enable")) {
        gui_debug("get Enable error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, ssh_enable);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(ssh_timeout, ssh_node, "IdleTimeout")) {
        gui_debug("get ssh_timeout error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, ssh_timeout);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(ssh_username, ssh_node, "Username")) {
        gui_debug("get Username error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, ssh_username);
        strcat(varValue, "|");
    }

    strcat(varValue, " |");
    //if(tr69GetUnfreshLeafData(ssh_password, ssh_node, "Password")) {
        //gui_debug("get Password error");
        //strcat(varValue, " |");
    //}else{
        //strcat(varValue, ssh_password);
        //strcat(varValue, "|");
    //}

    if(tr69GetUnfreshLeafData(ssh_port, ssh_node, "Port")) {
        gui_debug("get Port error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, ssh_port);
        strcat(varValue, "|");
    }

    gui_debug(">>>>>>%s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}

void cgiPostAdvLocalssh()
{
    gui_debug("advanced_ssh_enable => %d", gWebParams.advanced_ssh_enable);
    gui_debug("advanced_ssh_timeout => %d", gWebParams.advanced_ssh_timeout);
    gui_debug("advanced_ssh_username => %s", gWebParams.advanced_ssh_username);
    gui_debug("advanced_ssh_password => %s", gWebParams.advanced_ssh_password);
    gui_debug("advanced_ssh_port => %d", gWebParams.advanced_ssh_port);
    

    tr69CommitSetLeafDataInt("Device.X_ACTIONTEC_COM_RemoteLogin", 0, "Enable", gWebParams.advanced_ssh_enable);

    if (gWebParams.advanced_ssh_enable == 1){
        tr69CommitSetLeafDataInt("Device.X_ACTIONTEC_COM_RemoteLogin", 0, "IdleTimeout", gWebParams.advanced_ssh_timeout);
        tr69CommitSetLeafDataString("Device.X_ACTIONTEC_COM_RemoteLogin", 0, "Username", gWebParams.advanced_ssh_username);
        tr69CommitSetLeafDataInt("Device.X_ACTIONTEC_COM_RemoteLogin", 0, "Port", gWebParams.advanced_ssh_port);

        if (safe_strcmp(gWebParams.advanced_ssh_password, "") != 0)
            tr69CommitSetLeafDataString("Device.X_ACTIONTEC_COM_RemoteLogin", 0, "Password", gWebParams.advanced_ssh_password);    
    }
    
    tr69CommitSetLeafDataEnd("Device.X_ACTIONTEC_COM_RemoteLogin", 0);
    tr69SaveNow();

    return;
}   
