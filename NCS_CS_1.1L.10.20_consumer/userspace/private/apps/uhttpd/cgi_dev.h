/*
 * All cgi of Device 
 *
 */
#ifndef _CGI_DEV_H
#define _CGI_DEV_H
void cgiGetSystemInfo(int argc, char **argv, char *varValue);
int cgiPostRestart();

//Access Code
int isValidUserPassword(char *pwd);
int cgiPostChangeUserInfo(struct client *cl, struct http_request *req);

//IGMP
void cgiGetIGMPProxyInfo(int argc, char **argv, char *varValue);
void cgiGetIGMPSnoopingStats(int argc, char **argv, char *varValue);
void cgiGetIGMPSummary(int argc, char **argv, char *varValue);
void cgiGetIGMPGroupHosts(int argc, char **argv, char *varValue);
//Remote GUI
void cgiPostRemoteGUIGSettings();
void cgiGetRemoteGUIGSettings(int argc, char **argv, char *varValue);
#ifdef AEI_SYSTEM_BATTERY
void cgiGetBatteryInfo(int argc, char **argv, char *varValue);
void cgiGetBatteryStatus(int argc, char **argv, char *varValue);
void cgiGetTemperatureInfo(int argc, char **argv, char *varValue);
#endif

#endif
