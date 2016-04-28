/*
 * Broadband DSL 
 *
 */
#ifndef _CGI_DSL_H
#define _CGI_DSL_H
int cgiPostBroadbandSettings();
void cgiGetBroadbandSettings(int argc, char **argv, char *varValue);
void cgiGetBroadbandStatus(int argc, char **argv, char *varValue);
void cgiGetAutoMode(int argc, char **argv, char *varValue);
void cgiGetBBPhysicalLinkStatus(int argc, char **argv, char *varValue);
void cgiGetBBIPConnectionStatus(int argc, char **argv, char *varValue);
void cgiGetBBTotalBytesReceived(int argc, char **argv, char *varValue);
#endif