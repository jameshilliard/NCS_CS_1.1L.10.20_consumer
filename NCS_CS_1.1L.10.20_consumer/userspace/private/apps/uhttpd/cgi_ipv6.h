#ifndef _CGI_IPV6_H
#define _CGI_IPV6_H
void cgiGetIPv6WANStatus(int argc, char **argv, char *varValue);
void cgiPostIPv6WANSettings();
void cgiGetIPv6WANCurMode(int argc, char **argv, char *varValue);
void cgiGetIPv6WANEnabled(int argc, char **argv, char *varValue);
void cgiGet6rdNUDResult(int argc, char **argv, char *varValue);
void cgiGetIPv6FirewallAdvSettings(int argc, char **argv, char *varValue);
int cgiPostIPv6FirewallAdvSettings();
void cgiGetLocalNetIPv6Status(int argc, char **argv, char *varValue);
void cgiGetLANIPv6Statistics(int argc, char **argv, char *varValue);
void cgiPostIPv6LANSettings();
void cgiGetIPv6LANSettings(int argc, char **argv, char *varValue);
void cgiGet6rd(int argc, char **argv, char *varValue);
void cgiGetIPv6WANStatic(int argc, char **argv, char *varValue);
#endif
