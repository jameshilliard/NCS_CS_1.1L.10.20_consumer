/*
 * All cgi of Diagnostics 
 *
 */
#ifndef _CGI_DIAGNOSTICS_H
#define _CGI_DIAGNOSTICS_H
int cgiPostResets();
//Logs
void cgiGetLogs(int argc, char **argv, char *varValue);
int cgiClearLogs();
int cgiSaveLog(struct client *cl, struct http_request *req);
//NAT Table
void cgiGetNatTable(int argc, char **argv, char *varValue);
//NAT ALG
void cgiPostNatALGSettings();
void cgiGetNatALGSettings(int argc, char **argv, char *varValue);
//Update firmware
int cgiPostUpdateFirmware(char *uploadFileName);
void cgiGetUpdateProgress(int argc, char **argv, char *varValue);
void cgiGetCurFirmVersion(int argc, char **argv, char *varValue);
//Diagnostics Troubleshoot
int cgiPostTroubleShoot();
void cgiGetPingResult(int argc, char **argv, char *varValue);
void cgiGetTracerouteResult(int argc, char **argv, char *varValue);
void cgiGetNSLookupResult(int argc, char **argv, char *varValue);

//Diagnostics Troubleshoot
void cgiPostSyslogSettings();
void cgiGetSyslogSettings(int argc, char **argv, char *varValue);

//Connection to AT&T
void cgiPostRestartWAN();

//Advanced Management
void cgiPostAdvManagement();
void cgiPostAdvReboot();
void cgiGetAdvManagement(int argc, char **argv, char *varValue);
void cgiGetCurIPInfo(int argc, char **argv, char *varValue);
void cgiGetDevSerialNum(int argc, char **argv, char *varValue);
void cgiPostAdvPassword();
void cgiPostForgotPassword();
void cgiPostAdvPwdErrClear();
void cgiGetMainProductInfo(int argc, char **argv, char *varValue);
void cgiGetMainNetStation(int argc, char **argv, char *varValue);

void cgiGetRedirectIP(int argc, char **argv, char *varValue);
void actZeroConfAutoSync(int argc, char **argv, char *varValue);
void actZeroConfUpgrade(int argc, char **argv, char *varValue);
void cgiPostFactoryDefault();
void cgiPostAdvLanguage();

void cgiGetAdvTR69();
void cgiPostAdvTR69();

//local ssh
void cgiGetAdvLocalssh();
void cgiPostAdvLocalssh();


#define DEVICE_DHCPV4_CLIENT "Device.DHCPv4.Client"
#define DEVICE_DHCPV4_CLIENT_1 "Device.DHCPv4.Client.1"
#define DEVICE_IP_INTER1_IPADDR_1 "Device.IP.Interface.1.IPv4Address.1"
#define BUF_LEN20 20
#define BUF_LEN64 64
#endif
