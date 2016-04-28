#include "uhttpd.h"
#include "uhttpd-utils.h"
#include "cgi_main.h"
#include "cgi_dev.h"
#include "AEI_BMU_lib.h"

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostRestart
 *
 *  [DESCRIPTION]:
 *      Restart Device.
 *
 *  [return]:
 **************************************************************************/
int cgiPostRestart()
{
    int result = 0;
    result = dal_sys_reboot();
    gui_debug("\nYUKI===Device Restart===result=%d\n",result);
    return result;
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetSystemInfo
 *
 *  [DESCRIPTION]:
 *      Get System Information.
 *
 *  [return]:
 **************************************************************************/
void cgiGetSystemInfo(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    dal_ret_t *ret_dev = NULL;
    ret_dev = dal_get_device_info();

    if(ret_dev == NULL)
        return;

    sprintf(buf, "%s|", getValueFromDal(ret_dev->param[DAL_DEVINFO_MANUFACTURER]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_dev->param[DAL_DEVINFO_MOD_NUM]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_dev->param[DAL_DEVINFO_SER_NUM]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_dev->param[DAL_DEVINFO_SW_VER]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_dev->param[DAL_DEVINFO_MAC]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_dev->param[DAL_DEVINFO_FIRST_USE_DATE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_dev->param[DAL_DEVINFO_UT]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_dev->param[DAL_DEVINFO_DATA_PUMP_VER]));
    strcat(varValue, buf);

    gui_debug("\nYUKI===Get System Information ===varValue=%s\n",varValue);
    free_dal_ret(&ret_dev);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      isValidUserPassword
 *
 *  [DESCRIPTION]:
 *      There may be multi-user, once the user is enabled and the password is right, it will login successfully.
 *
 *  [return]:
 **************************************************************************/
int isValidUserPassword(char *pwd)
{
    int result = 0;
    dal_ret_t *ret_user = NULL;
    int num = 0;
    ret_user = dal_sys_get_user_list(&num);
#ifdef AEI_REMOTE_GUI
    char RGui_PW[64] = {0};
    tsl_rv_t ret = TSL_RV_ERR;
    if(gWebParams.cli_addr)
        gui_debug("Now coming ip is :%s", gWebParams.cli_addr);
    tsl_bool_t onLanSide = tr69_util_getNetworkAccessMode(gWebParams.cli_addr);
    if(onLanSide)
    {
#endif
    if (ret_user == NULL)
    {
        gui_debug("Get user info failed.\n");
        return result;
    }

    for (int i=0; i<num; i++)
    {
        if(!safe_strcmp(pwd, ret_user[i].param[USER_PASSWORD]) && !safe_strcmp("1", ret_user[i].param[USER_ENABLE]))
        {
            result = 1;
            break;
        }
    }
    
    free_dal_ret(&ret_user);
#ifdef AEI_REMOTE_GUI
    }
    else
    {
       ret = tr69_util_get_RemoteGui_user(RGui_PW); 
       if(ret == TSL_RV_SUC)
       {
           if(!safe_strcmp(RGui_PW, pwd))
               result = 1;
       }
    }
#endif
    gui_debug("If the access code is invalid: %d", result);
    return result;
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      isValidAdminUser
 *
 *  [DESCRIPTION]:
 *      If Admin user is valid.
 *
 *  [return]:
 **************************************************************************/
#define DEF_LOGIN_PASSWORD  "abcd123456"
#define DEF_LOGIN_NAME  "admin"
#define DEF_LOGIN_ALIAS  "admin"

static int isValidAdminUser(char *pwd)
{
    int result = 0;
    dal_ret_t *ret_admin = NULL;
    int num = 0;
    ret_admin = dal_sys_get_user_list(&num);
    if (ret_admin == NULL || num == 0)
    {
        gui_debug("Get user info failed.\n");
        return result;
    }

    if(!safe_strcmp("1", ret_admin[0].param[USER_ENABLE]) && !safe_strcmp(pwd, ret_admin[0].param[USER_PASSWORD]) && !safe_strcmp(DEF_LOGIN_NAME, ret_admin[0].param[USER_NAME]) && !safe_strcmp(DEF_LOGIN_ALIAS, ret_admin[0].param[USER_ALIAS]))
    {
        result = 1;
    }

    free_dal_ret(&ret_admin);
    gui_debug("isValidAdminUser result: %d", result);
    return result;
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostChangeUserInfo
 *
 *  [DESCRIPTION]:
 *      GUI can only modify the Admin user password. The Admin user is the default first user.
 *
 *  [return]:
 **************************************************************************/

int cgiPostChangeUserInfo(struct client *cl, struct http_request *req)
{
    int result = 0;
    int ret_size = sizeof(dal_ret_t);
    dal_ret_t ret_user;
    memset(&ret_user, 0, ret_size);

    char defAdminAlias[BUFLEN_64] = {0};
    char defAdminName[BUFLEN_64] = {0};
    char defAdminPwd[BUFLEN_64] = {0};
    
    strcpy(defAdminName, DEF_LOGIN_NAME);
    strcpy(defAdminAlias, DEF_LOGIN_ALIAS);
    strcpy(defAdminPwd, DEF_LOGIN_PASSWORD);

    if(!safe_strcmp(gWebParams.action, "new"))
    {
        if(isValidAdminUser(gWebParams.old_pwd))
        {
        
            ret_user.param[USER_ALIAS] = &defAdminAlias[0];
            ret_user.param[USER_NAME] = &defAdminName[0];
            ret_user.param[USER_PASSWORD] = &gWebParams.new_pwd[0];
            gui_debug("Change admin access code: %s",ret_user.param[USER_PASSWORD]);
            result = dal_sys_set_user(&ret_user);
            result = dal_SaveConfig();
            strcpy(gWebParams.pwd_reset_result, "1");
            int session_id = getSessionIDFromReq(req);
            session_logout(session_id);

        }else{
           //The enter old password is wrong.
            gui_debug("The old admin access code entered is incorrect\n.");
            strcpy(gWebParams.pwd_reset_result, "0");
            result = 1;
        }

    }else if(!safe_strcmp(gWebParams.action, "restore")){
        //Now, the default admin password is fixed, because the dal hasn't implemented the DAL of default password.
        ret_user.param[USER_ALIAS] = &defAdminAlias[0];
        ret_user.param[USER_NAME] = &defAdminName[0];
        ret_user.param[USER_PASSWORD] = &defAdminPwd[0];

        gui_debug("Restore default access code: %s",ret_user.param[USER_PASSWORD]);
        result = dal_sys_set_user(&ret_user);
        result = dal_SaveConfig();
        int session_id = getSessionIDFromReq(req);
        session_logout(session_id);
    }

    return result;
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetIGMPProxyInfo
 *
 *  [DESCRIPTION]:
 *      Get igmp proxy information (Multicast in GUI).
 *
 *  [return]:
 **************************************************************************/
void cgiGetIGMPProxyInfo(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_64] = {0};
    dal_ret_t *ret_proxy = NULL;
    ret_proxy = dal_get_igmp_proxy_info();
    if(ret_proxy == NULL)
        return;

    sprintf(buf, "%s|", getValueFromDal(ret_proxy->param[IGMP_PROXY_VERSION]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_proxy->param[IGMP_PROXY_ROBUSTNESS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_proxy->param[IGMP_PROXY_QI]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_proxy->param[IGMP_PROXY_QRV]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_proxy->param[IGMP_PROXY_UNSOLICIT_INTERVAL]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_proxy->param[IGMP_PROXY_FASTLEAVE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_proxy->param[IGMP_PROXY_LMQI]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_proxy->param[IGMP_PROXY_LMQC]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_proxy->param[IGMP_PROXY_MAX_GROUP_MEMBERS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_proxy->param[IGMP_PROXY_DEFAULT_FORWARD]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_proxy->param[IGMP_PROXY_SNOOPING_TIME]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_proxy->param[IGMP_PROXY_SNOOPING_MODE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_proxy->param[IGMP_PROXY_QOS_TOS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_proxy->param[IGMP_PROXY_QOS_PBIT]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_proxy->param[IGMP_PROXY_QOS_MARKER]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s", getValueFromDal(ret_proxy->param[IGMP_PROXY_STATUS]));
    strcat(varValue, buf);

    gui_debug("\nYUKI===Get Multicast Information ===varValue=%s\n",varValue);
    free_dal_ret(&ret_proxy);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetIGMPSnoopingStats
 *
 *  [DESCRIPTION]:
 *      Get IGMP group stats (IGMP Snooping Statistics in GUI).
 *
 *  [return]:port|address|filter mode|sources|timeout||
 **************************************************************************/
void cgiGetIGMPSnoopingStats(int argc, char **argv, char *varValue)
{
    char line[BUFLEN_256] = {0};
    char line_tmp[BUFLEN_256] = {0};
    int count = 0;
    char bridge[BUFLEN_64] = {0};
    char group[BUFLEN_64] = {0};
    char interface[BUFLEN_64] = {0};
    char mode[BUFLEN_64] = {0};
    char source[BUFLEN_64] = {0};
    char host[BUFLEN_64] = {0};
    char timeout[BUFLEN_64] = {0};

    FILE* igmpSnpSts = fopen("/proc/net/igmp_snooping", "r");
    if(igmpSnpSts != NULL)
    {
        while ( fgets(line, sizeof(line), igmpSnpSts) )
        {
            char *tmp = NULL;
            char *blank = " ";
            int i = 0;
            // read pass header line
            if ( count++ < 1 ) continue;
            if (!strncmp(line, "bridge", 6)) continue;

            //Whether there is the same group
            strcpy(line_tmp, line);
            tmp = strtok(line_tmp, blank);
            while(tmp) {
                i++;
                tmp= strtok(NULL, blank);
            }

            if(i == 8)
                sscanf(line, "%s %s %s %s %s %s %s", bridge,group, interface, mode, source, host,timeout);
            else if(i == 3)
                sscanf(line, "%s %s", host,timeout);

            strcat(varValue, bridge);
            strcat(varValue, "|");
            strcat(varValue, group);
            strcat(varValue, "|");
            strcat(varValue, interface);
            strcat(varValue, "|");
            strcat(varValue, mode);
            strcat(varValue, "|");
            strcat(varValue, source);
            strcat(varValue, "|");
            strcat(varValue, host);
            strcat(varValue, "|");
            strcat(varValue, timeout);
            strcat(varValue, "||");

            memset(line, 0, sizeof(line));
        }
        fclose(igmpSnpSts);
    }
    gui_debug("\nYUKI===Get IGMP Snooping Statistics ===varValue=%s\n",varValue);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetIGMPSummary
 *
 *  [DESCRIPTION]:
 *      Get Summary.
 *
 *  [return]:receive query|send query|receive reportv1| send reportv1|receive reportv2|send reportv2|receive leavev2|send leavev2|receive reportv3|send reportv3|receive other|send other
 **************************************************************************/
void cgiGetIGMPSummary(int argc, char **argv, char *varValue)
{
    FILE *fp = NULL;
    char buf[4096];
    int len = 0;

    fp = fopen("/tmp/.igmp.info", "r");
    if(fp == NULL)
        return;

    while (fgets(buf, 4096, fp))
    {
        char *p = NULL;
        int i = 1;

        if(buf[strlen(buf)-1] == '\n')
            buf[strlen(buf)-1] = 0;
        p = strtok(buf, "|");
        while(p && i<=12){
            strcat(varValue, p);
            strcat(varValue, "|");
            i++;
            p = strtok(NULL, "|");
        }
        memset(buf, 0 , 4096);
    }
    fclose(fp);

    len = strlen(varValue);
        varValue[len-1] = '\0';   // remove the last '|' character

    gui_debug("\nYUKI===Get Summary ===varValue=%s\n",varValue);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetIGMPGroupHosts
 *
 *  [DESCRIPTION]:
 *      Get IGMP host stats (Multicast Group Hosts in GUI).
 *
 *  [return]: group|address|interface||
 **************************************************************************/
void cgiGetIGMPGroupHosts(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_64] = {0};
    dal_ret_t *ret_ghost = NULL;
    int num =0;
    ret_ghost = dal_get_igmp_host_stats_list(&num);
    if(ret_ghost == NULL)
        return;
    for(int i=0; i<num; i++)
    {
        sprintf(buf, "%s|", getValueFromDal(ret_ghost[i].param[IGMP_HOST_STATS_GROUP]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(ret_ghost[i].param[IGMP_HOST_STATS_ADDRESS]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s||", getValueFromDal(ret_ghost[i].param[IGMP_HOST_STATS_INTERFACE]));
        strcat(varValue, buf);
    }

    gui_debug("\nYUKI===Get Multicast Group Hosts ===varValue=%s\n",varValue);
    free_dal_ret(&ret_ghost);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostRemoteGUIGSettings
 *
 *  [DESCRIPTION]:
 *      Set Remote GUI
 *
 *  [return]:
 **************************************************************************/
void cgiPostRemoteGUIGSettings()
{
    int ret_size = sizeof(dal_ret_t);
    dal_ret_t remotegUI;
    memset(&remotegUI, 0, ret_size);

    if(!safe_strcmp(gWebParams.remote_state, "true"))
    {
        remotegUI.param[REMOTE_GUI_USER_NAME] = &gWebParams.remote_name[0];
        remotegUI.param[REMOTE_GUI_USER_PW] = &gWebParams.remote_pwd[0];
        remotegUI.param[REMOTE_GUI_PORT] = &gWebParams.remote_port[0];
        remotegUI.param[REMOTE_GUI_TIMEOUT] = &gWebParams.remote_timeout[0];
    }
    remotegUI.param[REMOTE_GUI_ENABLED] = &gWebParams.remote_state[0];

    gui_debug("REMOTE_GUI_ENABLED=%s\n",remotegUI.param[REMOTE_GUI_ENABLED]);
    gui_debug("REMOTE_GUI_USER_NAME=%s\n",remotegUI.param[REMOTE_GUI_USER_NAME]);
    gui_debug("REMOTE_GUI_USER_PW=%s\n",remotegUI.param[REMOTE_GUI_USER_PW]);
    gui_debug("REMOTE_GUI_PORT=%s\n",remotegUI.param[REMOTE_GUI_PORT]);
    gui_debug("REMOTE_GUI_TIMEOUT=%s\n",remotegUI.param[REMOTE_GUI_TIMEOUT]);

    if(dal_set_remote_gui(&remotegUI) != TSL_RV_SUC)
    {
        gui_debug("dal_set_remote_gui set failed.");
    }
    
    dal_SaveConfig();
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetRemoteGUIGSettings
 *
 *  [DESCRIPTION]:
 *      Get Remote GUI Settings
 *
 *  [return]:REMOTE_GUI_ENABLED|REMOTE_GUI_USER_NAME|REMOTE_GUI_USER_PW|REMOTE_GUI_PORT|REMOTE_GUI_TIMEOUT
 **************************************************************************/
void cgiGetRemoteGUIGSettings(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    dal_ret_t *remotegUI = NULL;
    remotegUI = dal_get_remote_gui();
    if(remotegUI == NULL)
        return;

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(remotegUI->param[REMOTE_GUI_ENABLED]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(remotegUI->param[REMOTE_GUI_USER_NAME]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(remotegUI->param[REMOTE_GUI_USER_PW]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(remotegUI->param[REMOTE_GUI_PORT]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s", getValueFromDal(remotegUI->param[REMOTE_GUI_TIMEOUT]));
    strcat(varValue, buf);

    gui_debug("\nYUKI===Get Remote GUI Settings===varValue=%s\n",varValue);
    free_dal_ret(&remotegUI);
}

#ifdef AEI_SYSTEM_BATTERY
void cgiGetBatteryStatus(int argc, char **argv, char *varValue)
{
    TBatteryStatus Status;
    int warning = 0;
    char buf[BUFLEN_64] = {0};

    if(Battery_Info_Update(&Status) == BMU_FAILURE)
    {
        return;
    }

    if(!Status.exist)
    {
        warning = 1;
    }
    else if(Status.CycleCount > CYCLE_LIFE_ALERT)
    {
        /*Battery life is critically low, less than 80% of full life*/
        warning = 1;
    }
    else if(Status.status == POWER_SUPPLY_STATUS_DISCHARGING 
            && Status.AcOn == BMU_POWER_AC_OFF 
            && Status.remainCapacity < BAT_CRITICAL_LOW_CAPACITY)
    {
        warning = 1;
    }

    sprintf(buf,"%d|%s",warning,Battery_get_status_string(&Status));
    strcat(varValue, buf);
}
/*
 * Get Battery Information to display on GUI
 * format
 * ACON|Capacity(%)|EstMinToEmpty(min)|ChargeToFull(min)|Status|temperature|CycleCount|FRU
 */
#define BAT_FRU_ID "92TT2001H"
void cgiGetBatteryInfo(int argc, char **argv, char *varValue)
{
    TBatteryStatus Status;
    int warning = 0;
    char buf[BUFLEN_64] = {0};

    if(Battery_Info_Update(&Status) == BMU_FAILURE)
    {
        return;
    }
    /*AC ON flag*/
    sprintf(buf,"%d|",Status.AcOn);
    strcat(varValue, buf);

    /*current Capacity at % */
    sprintf(buf,"%d|",Status.remainCapacity);
    strcat(varValue, buf);

    /*Estimated time to empty at minutes when discharging */
    sprintf(buf,"%d|",Status.EstMinToEmpty);
    strcat(varValue, buf);

    /*estimated time to full at minutes when charging */
    sprintf(buf,"%d|",Status.AtRateToFull);
    strcat(varValue, buf);

    /*Status*/
    sprintf(buf,"%s|",Battery_get_status_string(&Status));
    strcat(varValue, buf);

    /*Temperature*/
    sprintf(buf,"%d|",Status.temperature/10);
    strcat(varValue, buf);

    /*Cycle count*/
    sprintf(buf,"%d|",Status.CycleCount);
    strcat(varValue, buf);

    /*FRU*/
    sprintf(buf,"%s|",BAT_FRU_ID);
    strcat(varValue, buf);
}
void cgiGetTemperatureInfo(int argc, char **argv, char *varValue)
{
    int ret;
    int retValue;
    int warning = 0;
    char buf[BUFLEN_256] = {0};

    ret = Battery_get_LIS3DH_temperature(&retValue);
    if(ret == BMU_SUCCESS){
        if(retValue > EMERGENCY_TEMPERATURE)
        {
            warning  = 1;
        }
        sprintf(buf,"%d|%d",warning, retValue);
    }
    else{
        sprintf(buf,"1|Error");
    }
    strcat(varValue, buf);
}
#endif
