
#ifndef CGI_FIREWALL_H
#define CGI_FIREWALL_H


int cgiPostFirewallRules();
//void cgiGetFirewallSchedulers(int argc, char **argv, char *varValue);
//void cgiGetFirewallNetwork(int argc, char **argv, char *varValue);
//void cgiGetFirewallWanConnectionType(int argc, char **argv, char *varValue);
void cgiGetFirewallStatus(int argc, char **argv, char *varValue);
void cgiGetFirewallRuleEnable(int argc, char **argv, char *varValue);
void cgiGetFirewallRules(int argc, char **argv, char *varValue);
int cgiPostPinholesService();
void getAllPinholesServices(int argc, char **argv, char *varValue);
void getAllPinholesRules(int argc, char **argv, char *varValue);
int cgiPostPinholesRule();
int cgiPostFirewallAdvancedSettings();
void cgiGetFirewallAdvancedSettings(int argc, char **argv, char *varValue);
int cgiPostFirewallIPPassthru();
void cgiGetFirewallIPPassthru(int argc, char **argv, char *varValue);
#endif

