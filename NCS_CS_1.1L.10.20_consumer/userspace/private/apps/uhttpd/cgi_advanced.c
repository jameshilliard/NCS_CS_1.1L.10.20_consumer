#include "uhttpd.h"
#include "uhttpd-utils.h"
#include "cgi_main.h"
#include "cgi_advanced.h"
#include "cgi_act_tr69.h"

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetModelNumber
 *
 *  [DESCRIPTION]:
 *
 *   cli -g Device.DeviceInfo.ModelName
 *
 **************************************************************************/
void cgiGetModelNumber(int argc, char **argv, char *varValue)
{
    gui_debug("cgiGetModelNumber start");

    char modelname[BUFLEN_64+1] = {0};
    
    if(tr69GetUnfreshLeafData(modelname, "Device.DeviceInfo", "ModelName")) {
        gui_debug("get Status error");
        strcat(varValue, " ");
    }else{
        strcat(varValue, modelname);
    }


    gui_debug(">>>>>>%s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetSwapImageVersion
 *
 *  [DESCRIPTION]:
 *
 *
 **************************************************************************/
void cgiGetSwapImageVersion(int argc, char **argv, char *varValue)
{
    char buf_ver[BUFLEN_128] = {'\0'};
    gui_debug("cgiGetSwapImageVersion start");


    dal_get_image_info(buf_ver, BUFLEN_128);
    //return snprintf(varValue, var_len, "%d|%s|%s", activeImage, partition_one_image, partition_two_image)

    strcat(varValue, buf_ver);
    gui_debug(">>>>>>%s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}



void cgiPostAdvSwap()
{
    char cmd[BUFLEN_128];
    
    int currentimage = gWebParams.swap_currentversion;
    int activeimage = (currentimage == 1) ? 2 : 1;
    gui_debug("activeimage = %d", activeimage);

    dal_change_image(activeimage);
    //snprintf(cmd, sizeof(cmd),  "echo %d > /proc/nvram/BootImage ", current);
    //gui_debug(cmd);
    //system(cmd);
    
    return;
}   



//return string: //enable|ip|
void cgiGetAdvSystemLog(int argc, char **argv, char *varValue){
    gui_debug("cgiGetAdvSystemLog start");

    char systemlog_node[BUFLEN_64] = {0};

    char adv_systemlog_enable[BUFLEN_8] = {0};
    char adv_systemlog_ip[BUFLEN_64] = {0};


    sprintf(systemlog_node, "%s", "Device.X_ACTIONTEC_COM_SYSLOG");

    if(tr69GetUnfreshLeafData(adv_systemlog_enable, systemlog_node, "Enable")) {
        gui_debug("get Enable error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, adv_systemlog_enable);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(adv_systemlog_ip, systemlog_node, "ServerIP")) {
        gui_debug("get ServerIP error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, adv_systemlog_ip);
        strcat(varValue, "|");
    }

    gui_debug(">>>>>>%s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}

void cgiPostAdvSystemLog()
{
    gui_debug("adv_systemlog_enable => %d", gWebParams.adv_systemlog_enable);
    gui_debug("adv_systemlog_ip => %s", gWebParams.adv_systemlog_ip);

    tr69CommitSetLeafDataInt("Device.X_ACTIONTEC_COM_SYSLOG", 0, "Enable", gWebParams.adv_systemlog_enable);

    if (gWebParams.adv_systemlog_enable == 1){
        tr69CommitSetLeafDataString("Device.X_ACTIONTEC_COM_SYSLOG", 0, "ServerIP", gWebParams.adv_systemlog_ip);
    }
    
    tr69CommitSetLeafDataEnd("Device.X_ACTIONTEC_COM_SYSLOG", 0);
    tr69SaveNow();

    return;
}   


//return string: //enable
void cgiGetHnapLogInfo(int argc, char **argv, char *varValue){
    gui_debug("cgiGetHnapLogInfo start");

    char systemlog_node[BUFLEN_64] = {0};

    char hnaplog_enable[BUFLEN_8] = {0};
 
    sprintf(systemlog_node, "%s", "Device.X_ACTIONTEC_COM_HNAP.Server");

    if(tr69GetUnfreshLeafData(hnaplog_enable, systemlog_node, "LogFile_Enable")) {
        gui_debug("get Enable error");
        strcat(varValue, " ");
        //strcat(varValue, " |");
    }else{
        strcat(varValue, hnaplog_enable);
        //strcat(varValue, "|");
    }

    gui_debug(">>>>>>%s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}


//return string: xxxx||||xxxxx|||||
void cgiGetHnapLogContent(int argc, char **argv, char *varValue){
    gui_debug("cgiGetHnapLogInfo start");

    FILE *fp;
    char line[500] = {'\0'};
    char special_buf[1024] = {'\0'};   
    int totalrows = 0;
    int idx = 0;

    gui_debug("cgiGetHnapLogContent start \n");

    fp = fopen("/tmp/hnap_log.txt", "r");
    if(NULL == fp)  goto openfile_error;

    while(!feof(fp)) {
        fgets(line, 500, fp);
        totalrows++;
    }
    fclose(fp);
    gui_debug("totalrows = %d\n", totalrows);

    fp = fopen("/tmp/hnap_log.txt", "r");
    if(NULL == fp)  goto openfile_error;

    memset(line, '\0', sizeof(line));
    while (fgets(line, 500, fp) != NULL) {
        idx++;
        if (idx < totalrows - 100)   continue;
        
        //gui_debug("%s\n", line);
        if(strlen(line) > 0)
        {
            strncpy(special_buf, line, safe_strlen(line));
            GUI_SpeciCharEncode(special_buf, sizeof(special_buf));

            strcat(varValue, special_buf);
            strcat(varValue, "||||");
        }
        memset(line, '\0', sizeof(line));
        memset(special_buf, '\0', sizeof(special_buf));
    }


    fclose(fp);

openfile_error:
    gui_debug("cgiGetHnapLogContent end\n");
    return;
}


void cgiPostHnapLog()
{
    gui_debug("hnaplog_action => %s", gWebParams.hnaplog_action);

    if(safe_strcmp(gWebParams.hnaplog_action, "enable") == 0) {
        tr69SetLeafDataInt("Device.X_ACTIONTEC_COM_HNAP.Server", 0, "LogFile_Enable", 1);
    } else if(safe_strcmp(gWebParams.hnaplog_action, "disable") == 0) {
        tr69SetLeafDataInt("Device.X_ACTIONTEC_COM_HNAP.Server", 0, "LogFile_Enable", 0);
    } else if(safe_strcmp(gWebParams.hnaplog_action, "clear") == 0) {
        tr69SetLeafDataInt("Device.X_ACTIONTEC_COM_HNAP.Server", 0, "LogFile_Clean", 1);
    }

    tr69CommitSetLeafDataEnd("Device.X_ACTIONTEC_COM_HNAP.Server", 0);
    tr69SaveNow();

    return;
}   



//return string: //enable
void cgiGetOption60List(int argc, char **argv, char *varValue){
    gui_debug("cgiGetOption60List start");

    char buf[1024] = {0};
    int num = 0;

    char option60_enable[16] = {0};
    char option60_alias[65] = {0};
    char option60_value[256] = {0};

    char mdmoid_option60_object[256] = {0};

    if(tr69GetUnfreshLeafData(buf, "Device.DHCPv4.Client.1", "SentOptionNumberOfEntries")) {
        gui_debug( "Get Device.Users.SentOptionNumberOfEntries error!");
    }
    num = atoi(buf);
    gui_debug("SentOptionNumberOfEntries = %d", num);

    for (int i=1; i<=num; i++){
        sprintf(mdmoid_option60_object, "%s.%d", "Device.DHCPv4.Client.1.SentOption", i);

        if(tr69GetUnfreshLeafData(option60_enable, mdmoid_option60_object, "enable")) {
            gui_debug( "Get %s.enable error!", mdmoid_option60_object);
        }

        if(tr69GetUnfreshLeafData(option60_alias, mdmoid_option60_object, "Alias")) {
            gui_debug( "Get %s.Alias error!", mdmoid_option60_object);
        }

        if(tr69GetUnfreshLeafData(option60_value, mdmoid_option60_object, "Value")) {
            gui_debug( "Get %s.Value error!", mdmoid_option60_object);
        }
        
        sprintf(buf, "%s|%s|%s",option60_enable, option60_alias, option60_value); 
        strcat(varValue, buf);
        if (i != num)   strcat(varValue, "||");
    }

    gui_debug("option60 list => %s\n\n", varValue);
}


void cgiPostOption60Add()
{
    gui_debug("cgiPostOption60Add");
    gui_debug("adv_option60_enable => %d", gWebParams.adv_option60_enable);
    gui_debug("adv_option60_alias => %s", gWebParams.adv_option60_alias);
    gui_debug("adv_option60_value => %s", gWebParams.adv_option60_value);

    int instanceId = 0;
    int ret = tr69_add_instance("Device.DHCPv4.Client.1.SentOption", &instanceId);
    gui_debug("instanceId => %d", instanceId);

    tr69CommitSetLeafDataInt("Device.DHCPv4.Client.1.SentOption", instanceId, "Enable", gWebParams.adv_option60_enable);
    tr69CommitSetLeafDataString("Device.DHCPv4.Client.1.SentOption", instanceId, "Alias", gWebParams.adv_option60_alias);
    tr69CommitSetLeafDataString("Device.DHCPv4.Client.1.SentOption", instanceId, "Value", gWebParams.adv_option60_value);


    tr69CommitSetLeafDataEnd("Device.DHCPv4.Client.1.SentOption", instanceId);
    tr69SaveNow();

    return;
}


void cgiPostOption60Modify()
{
    gui_debug("cgiPostOption60Modify");
    gui_debug("adv_option60_idx => %d", gWebParams.adv_option60_idx);
    gui_debug("adv_option60_enable => %d", gWebParams.adv_option60_enable);
    gui_debug("adv_option60_alias => %s", gWebParams.adv_option60_alias);
    gui_debug("adv_option60_value => %s", gWebParams.adv_option60_value);

    int instanceId = gWebParams.adv_option60_idx;
    gui_debug("instanceId => %d", instanceId);

    tr69CommitSetLeafDataInt("Device.DHCPv4.Client.1.SentOption", instanceId, "Enable", gWebParams.adv_option60_enable);
    tr69CommitSetLeafDataString("Device.DHCPv4.Client.1.SentOption", instanceId, "Alias", gWebParams.adv_option60_alias);
    tr69CommitSetLeafDataString("Device.DHCPv4.Client.1.SentOption", instanceId, "Value", gWebParams.adv_option60_value);


    tr69CommitSetLeafDataEnd("Device.DHCPv4.Client.1.SentOption", instanceId);
    tr69SaveNow();

    return;
}


void cgiPostOption60Delete()
{
    gui_debug("cgiPostOption60Delete");
    gui_debug("adv_option60_idx => %d", gWebParams.adv_option60_idx);
    gui_debug("adv_option60_enable => %d", gWebParams.adv_option60_enable);
    gui_debug("adv_option60_alias => %s", gWebParams.adv_option60_alias);
    gui_debug("adv_option60_value => %s", gWebParams.adv_option60_value);

    tr69_del_instance("Device.DHCPv4.Client.1.SentOption", gWebParams.adv_option60_idx);

    return;
}


void daemon_telnetd_start(){
    gui_debug("daemon_telnetd_start start");
    system("/bin/start_telnetd.sh");
}


void cgiGetAdvAutoUpgrade(int argc, char **argv, char *varValue) {
    gui_debug("cgiGetAutoUpgrade start");

    char autoupgrade_node[BUFLEN_64] = {0};

    char autoupgrade_enable[BUFLEN_8] = {0};
    char autoupgrade_url[BUFLEN_256+1] = {0};
    char autoupgrade_ver[BUFLEN_256+1] = {0};


    sprintf(autoupgrade_node, "%s", "Device.X_ACTIONTEC_Upgrade.WanUpgradeConfig");

    if(tr69GetUnfreshLeafData(autoupgrade_enable, autoupgrade_node, "Enable")) {
        gui_debug("get Enable error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, autoupgrade_enable);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(autoupgrade_url, autoupgrade_node, "UrlAddress")) {
        gui_debug("get UrlAddress error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, autoupgrade_url);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(autoupgrade_ver, autoupgrade_node, "VerUrlAddress")) {
        gui_debug("get VerUrlAddress error");
        strcat(varValue, " ");
    }else{
        strcat(varValue, autoupgrade_ver);
    }


    gui_debug(">>>>>>%s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}


void cgiPostAdvAutoUpgrade()
{
    gui_debug("auto_upgrade_enable => %d", gWebParams.auto_upgrade_enable);
    gui_debug("auto_upgrade_url => %s", gWebParams.auto_upgrade_url);
    gui_debug("auto_upgrade_ver => %s", gWebParams.auto_upgrade_ver);

    tr69CommitSetLeafDataInt("Device.X_ACTIONTEC_Upgrade.WanUpgradeConfig", 0, "Enable", gWebParams.auto_upgrade_enable);

    if (gWebParams.auto_upgrade_enable == 1){
        tr69CommitSetLeafDataString("Device.X_ACTIONTEC_Upgrade.WanUpgradeConfig", 0, "UrlAddress", gWebParams.auto_upgrade_url);
        tr69CommitSetLeafDataString("Device.X_ACTIONTEC_Upgrade.WanUpgradeConfig", 0, "VerUrlAddress", gWebParams.auto_upgrade_ver);
    }
    
    tr69CommitSetLeafDataEnd("Device.X_ACTIONTEC_Upgrade.WanUpgradeConfig", 0);
    tr69SaveNow();

    return;
}


//return string: //enable|
void cgiGetAdvSerialPort(int argc, char **argv, char *varValue){
    gui_debug("cgiGetAdvSerialPort start");

    char serialport_node[BUFLEN_64] = {0};
    char serialport_enable[BUFLEN_8] = {0};

    snprintf(serialport_node, sizeof(serialport_node), "%s", TR69_OID_CONSOLE_ACESS);

    if(tr69GetUnfreshLeafData(serialport_enable, serialport_node, "Enable")) {
        gui_debug("get Enable error");
        strcat(varValue, "0|");
    }else{
        strcat(varValue, serialport_enable);
        strcat(varValue, "|");
    }


    gui_debug(">>>>>>%s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}

void cgiPostAdvSerialPort()
{
    gui_debug("advanced_serialport_enable => %d", gWebParams.advanced_serialport_enable);

    tr69CommitSetLeafDataInt(TR69_OID_CONSOLE_ACESS, 0, "Enable", gWebParams.advanced_serialport_enable);
    tr69CommitSetLeafDataEnd(TR69_OID_CONSOLE_ACESS, 0);
    tr69SaveNow();

    return;
}   
