#ifndef CGI_LOCALNETWORK_H
#define CGI_LOCALNETWORK_H
int cgiPostLANSubnetDhcp();
int cgiPostLANIPAllocation();
int cgiPostLANDeviceDNS();

void cgiGetLocalNetStatus(int argc, char **argv, char *varValue);
void cgiGetLANIPv4Statistics(int argc, char **argv, char *varValue);
void cgiGetLANWiFiStatus(int argc, char **argv, char *varValue);
void cgiGetLANSubnetDhcp(int argc, char **argv, char *varValue);
void cgiGetLANDeviceList(int argc, char **argv, char *varValue);
void cgiGetLANIPAllocationList(int argc, char **argv, char *varValue);
int cgiPostPortConfigure();
void cgiGetPortConfigure(int argc, char **argv, char *varValue);
void cgiGetLANDNSHostMapping(int argc, char **argv, char *varValue);
#endif
