/*
 * session control
 *
 */
#ifndef CGI_SESSION_H
#define CGI_SESSION_H


#ifndef AEI_UHTTPD_REALM_AUTH
unsigned int cgiLogin();
#else
unsigned int cgiLogin(char *realm, int *level);
int check_user_realm(char *user, char *passwd, char *realm, int *level);
#endif
int isValidUser(char *username, char *userpass, int need_check_passwd);

void cgiGetSessionID(int argc, char **argv, char *varValue);
void cgiGetISLogined(int argc, char **argv, char *varValue);

void cgiGetUserLevel(int argc, char **argv, char *varValue);

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiLogout
 *
 *  [DESCRIPTION]:
 *      user logout
 *
 *  [return]:
 **************************************************************************/
void cgiLogout(unsigned int session_id);

#endif
