#ifndef CGI_MOCA_H
#define CGI_MOCA_H

//get function
void cgiGetMocaInfo(int argc, char **argv, char *varValue);
void cgiGetMocaStatus(int argc, char **argv, char *varValue);

//post function
int cgiPostMocaSetup();
#endif
