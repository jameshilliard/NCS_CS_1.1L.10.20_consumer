#ifndef CGI_WIRELESS_H
#define CGI_WIRELESS_H
void cgiGetSSIDList(int argc, char **argv, char *varValue);
void cgiGetChannelList(int argc, char **argv, char *varValue);
void cgiGetWirelessBasic(int argc, char **argv, char *varValue);
void cgiGetWiFiNetList(int argc, char **argv, char *varValue);
void cgiGetWirelessSecurity(int argc, char **argv, char *varValue);
void cgiGetWirelessAdvanced(int argc, char **argv, char *varValue);
void cgiGetWirelessWPS(int argc, char **argv, char *varValue);
void cgiGetWirelessMacFilter(int argc, char **argv, char *varValue);
void cgiGetWirelessAdvancedWps(int argc, char **argv, char *varValue);
void cgiGetWirelessStatus(int argc, char **argv, char *varValue);
void cgiGetLANWiFiStatistics(int argc, char **argv, char *varValue);
void cgiGetLANEthStatistics(int argc, char **argv, char *varValue);
void cgiGetMacFilter(int argc, char **argv, char *varValue);
void cgiGetQuantennaCard(int argc, char **argv, char *varValue);

int cgiPostWirelessBasic();
int cgiPostWirelessSecurity();
int cgiPostWirelessAdv();
int cgiPostWirelessWPS();
int cgiPostWirelessRadioReset();
int cgiPostWirelessAdvancedWps();
int cgiPostMacFilter();
int cgiPostWirelessMacFilter();
#endif
