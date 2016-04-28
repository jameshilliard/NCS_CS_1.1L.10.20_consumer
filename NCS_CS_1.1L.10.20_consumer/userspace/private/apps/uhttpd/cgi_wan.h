/*
 * WAN 
 *
 */
#ifndef _CGI_WAN_H
#define _CGI_WAN_H
int cgiPostWANSettings();
void cgiGetWANSettings(int argc, char **argv, char *varValue);
void cgiGetIPSettings(int argc, char **argv, char *varValue);
void cgiGetDNSSettings(int argc, char **argv, char *varValue);
#endif