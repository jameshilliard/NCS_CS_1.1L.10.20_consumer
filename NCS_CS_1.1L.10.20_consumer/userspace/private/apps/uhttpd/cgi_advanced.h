#ifndef CGI_ADVANCED_H
#define CGI_ADVANCED_H

//get function
void cgiGetModelNumber(int argc, char **argv, char *varValue);
void cgiGetSwapImageVersion(int argc, char **argv, char *varValue);
void cgiGetAdvSystemLog(int argc, char **argv, char *varValue);
void cgiGetHnapLogInfo(int argc, char **argv, char *varValue);
void cgiGetHnapLogContent(int argc, char **argv, char *varValue);
void cgiGetOption60List(int argc, char **argv, char *varValue);
void cgiGetAdvAutoUpgrade(int argc, char **argv, char *varValue);

void cgiPostAdvSwap();
void cgiPostAdvSystemLog();
void cgiPostHnapLog();
void cgiPostAdvAutoUpgrade();

void cgiPostOption60Add();
void cgiPostOption60Modify();
void cgiPostOption60Delete();


void daemon_telnetd_start();

void cgiGetAdvSerialPort(int argc, char **argv, char *varValue);
void cgiPostAdvSerialPort();

#endif
