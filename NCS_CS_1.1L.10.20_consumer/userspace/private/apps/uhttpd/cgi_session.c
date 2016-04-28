/*
 * session control
 */

#include "uhttpd.h"
#include "uhttpd-utils.h"
#include "cgi_main.h"
#include "cgi_session.h"
#include "cgi_act_tr69.h"
#include "md5.h"
#ifdef AEI_UHTTPD_REALM_AUTH
#include "dal_users.h"
#endif

int login_max_try;
int login_left_try;
int login_error_waiting_seconds = 30;
int login_error_last_time = 0;
static int login_error_count = 0;
#ifdef AEI_UHTTPD_REALM_AUTH
int check_user_realm(char *user, char *passwd, char *realm, int *level);
#endif
/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiLogin
 *
 *  [DESCRIPTION]:
 *      user login
 *
 *  [return]:
 *      new session_id
 *      1:username/password error       
 *      2:session full,can not login
  *     4:over maximum unsuccessful login attempts 
 **************************************************************************/
#ifndef AEI_UHTTPD_REALM_AUTH
unsigned int cgiLogin()
#else
unsigned int cgiLogin(char *realm, int *level)
#endif
{
    gui_debug("login_username=%s, login_password=%s, need_check_passwd=%d", gWebParams.login_username, gWebParams.login_password, gWebParams.need_check_passwd);
    
    if (!session_has_empty())   return 2;

#ifndef AEI_UHTTPD_REALM_AUTH
    int isValid = isValidUser(gWebParams.login_username, gWebParams.login_password, gWebParams.need_check_passwd);
    if(isValid == 4)
        return 4;
    else if(isValid == 0)
        return 1;
#else
    int user_level = 0;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned int cur_time = (unsigned int)tv.tv_sec;
    if ((cur_time - login_error_last_time) < login_error_waiting_seconds)
        return 4;
    else if (login_left_try <= 0)
        login_left_try = login_max_try;

    if(NULL == realm)
        return 1;
    int isValid = check_user_realm(gWebParams.login_username, gWebParams.login_password, realm, &user_level);
    if(isValid != 0)
        return 1;
    *level = user_level;
#endif

    unsigned int session_id = session_gen_session_id();
    gui_debug("cgiLogin session_id=%u",session_id);
    session_has_logined(session_id);
    return session_id;
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      isValidUser
 *
 *  [DESCRIPTION]:
 *  [return]:
 *      1:valid user
 *      4:over maximum unsuccessful login attempts 
 **************************************************************************/
int isValidUser(char *username, char *userpass, int need_check_passwd)
{
    int result = 0;
    int num = 0;

	char tr69_username[64] = {0};
	char tr69_password[64] = {0};
	char tr69_auth_level[4] = {0};
	char tr69_user_enable[4] = {0};
	int auth_level = 0;
	char buf[4] = {0};
	char mdmoid_user_object[256] = {0};
	int user_entries = 0;
	int i = 0;
	char *strVal = NULL; 
    int passwd_error = 0;

    if(tr69GetUnfreshLeafData(buf, "Device.Users", "UserNumberOfEntries"))
	{
	    gui_debug( "Get Device.Users.UserNumberOfEntries error!");
	}
    num = atoi(buf);
	      

    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned int cur_time = (unsigned int)tv.tv_sec;
    //gui_debug("current => %d, lasttime => %d, waiting_seconds => %d", cur_time, login_error_last_time, login_error_waiting_seconds);
    
    if ((cur_time - login_error_last_time) < login_error_waiting_seconds)
    {
        result = 4;
    }
    else if (login_left_try <= 0)
        login_left_try = login_max_try;
    gui_debug("login_left_try => %d, login_max_try => %d", login_left_try, login_max_try);

    
    for (int i=1; i<=num; i++)
    {
	    sprintf(mdmoid_user_object, "%s.%d", "Device.Users.User", i);

		if(tr69GetUnfreshLeafData(tr69_username, mdmoid_user_object, "Username"))	
	    {
            gui_debug( "Get %s.Username error!", mdmoid_user_object);
        }
	        
		if(tr69GetUnfreshLeafData(tr69_password, mdmoid_user_object, "Password"))
	    {
	        gui_debug( "Get %s.Password error!", mdmoid_user_object);
	    }

	    if(tr69GetUnfreshLeafData(tr69_user_enable, mdmoid_user_object, "Enable"))
	    {
	        gui_debug( "Get %s.Enable error!", mdmoid_user_object);
	    }
	        
	    if(tr69GetUnfreshLeafData(tr69_auth_level, mdmoid_user_object, "X_ACTIONTEC_COM_AuthLevel"))
	    {
	        gui_debug( "Get %s.X_ACTIONTEC_COM_AuthLevel error!", mdmoid_user_object);
	    }

        gui_debug("USER_NAME=%s==%s, USER_PASSWORD=%s==%s", gWebParams.login_username,tr69_username,gWebParams.login_password,tr69_password);
            
        if (safe_strcmp(tr69_username, gWebParams.login_username) == 0)
        {
            gui_debug("username => %s, enable = %d", tr69_username, atoi(tr69_user_enable));
            if (atoi(tr69_user_enable) == 0)    return 0;

            if (need_check_passwd)
            {
                struct MD5Context md5c;
                unsigned char signature[16] = {0};
                unsigned char csig[16] = {0};
                char *clabel = NULL, *hexfmt = "%02X";
                unsigned int bp;
                int j = 0;     

                MD5Init(&md5c);
                MD5Update(&md5c, (unsigned char *)gWebParams.login_password, strlen(gWebParams.login_password));
                MD5Final(signature, &md5c);

                clabel = tr69_password;     
                for (j = 0; j < 16; j++) {
                    if (isxdigit((int) clabel[0]) && isxdigit((int) clabel[1]) &&
                                    sscanf((tr69_password + (j * 2)), hexfmt, &bp) == 1) {
                        csig[j] = (unsigned char) bp;
                    } else {                
                        break;
                    }
                    clabel += 2;
                }        

                //gui_debug("@@@@password signature=%s,csig=%s",signature,csig);
				for (j = 0; j < sizeof signature; j++) 
				{
					if (signature[j] != csig[j]) {							
				   	 	passwd_error = 1;
					}
				}
                
                if (passwd_error == 0)
                {
                    gui_debug("passwd_error == 0");
                    login_error_count = 0;
                    login_left_try = login_max_try;
                    result = 1;
                    return result;
                }
                else
                {
                    login_left_try--;
                    login_error_count ++;
                    result = 0;
                    if (login_left_try == 0)
                    {
                        login_error_last_time = cur_time;
                        result = 4;
                    }
                    gui_debug("passwd_error != 0 result = %d", result);
                    return result;
                }
            }
            else
            {
                login_error_count = 0;
                login_left_try = login_max_try;
                result = 0;
                return result;
            }
        }
    }

    return result;
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiLogout
 *
 *  [DESCRIPTION]:
 *      user logout
 *
 *  [return]:
 **************************************************************************/
void cgiLogout(unsigned int session_id)
{
    gui_debug("cgiLogout session_id=%u", session_id);
    session_logout(session_id);
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetSessionID
 *
 *  [DESCRIPTION]:
 *      get this request session_id from browser send cookie field.
 *
 *  [return]:
 **************************************************************************/
void cgiGetSessionID(int argc, char **argv, char *varValue)
{
    struct http_request *req;
    
    if (argc < 4) return;
    
    req = (struct http_request *)argv[3];

    unsigned int session_id = getSessionIDFromReq(req);
    sprintf(varValue, "%u", session_id);
    
#if 0   
    foreach_header(i, req->headers)
    {
        //gui_debug("headers name=%s, headers value=%s", req->headers[i], req->headers[i+1]);
        if (safe_strcmp(req->headers[i], "Cookie") == 0)
        {
            gui_debug("%s = %s", req->headers[i], req->headers[i+1]);
            
            char *dest[128];
            int num = 0;
            int j;
            
            gui_split(req->headers[i+1], ";", dest, &num);
            //gui_debug("num=%d", num);
            for (j=0; j<num; ++j)
            {
                //gui_debug("UH_SESSION_COOKIE=%s, dest[j]=%s", UH_SESSION_COOKIE, dest[j]);
                if (strstr(dest[j], UH_SESSION_COOKIE) != NULL)
                {
                    char *dest2[32];
                    int num2 = 0;
                    
                    gui_split(dest[j], "=", dest2, &num2);
                    //gui_debug("num2=%d", num2);
                    if (num2 != 2)  break;
                    
                    //gui_debug("dest2[0]=%s, dest2[1]=%s", dest2[0], dest2[1]);
                    session_id = dest2[1];
                    break;
                }
            }

            break;
        }
    }
    
    sprintf(varValue, "%s|", getValueFromDal(session_id));
    gui_debug("varValue=%s", varValue);
#endif  
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetISLogined
 *
 *  [DESCRIPTION]:
 *      check this session_id is logined?
 *
 *  [return]:
 **************************************************************************/
void cgiGetISLogined(int argc, char **argv, char *varValue)
{
    struct http_request *req;
    
    if (argc < 4) return;
    
    req = (struct http_request *)argv[3];

    unsigned int session_id = getSessionIDFromReq(req);
    int is_logined = session_is_logined(session_id);
    
    
    sprintf(varValue, "%d", is_logined);
}



/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetUserLevel
 *
 *  [DESCRIPTION]:
 *      get user level
 *
 *  [return]:
 *      4: root;    2:admin;    1:normal user
 **************************************************************************/
void cgiGetUserLevel(int argc, char **argv, char *varValue)
{
    int result = 1;
    int num = 0;

    char user_name[64] = {0};
    char user_level[4] = {0};
    char buf[4] = {0};
    char mdmoid_user_object[256] = {0};

    if(tr69GetUnfreshLeafData(buf, "Device.Users", "UserNumberOfEntries"))
    {
        gui_debug( "Get Device.Users.UserNumberOfEntries error!");
    }
    num = atoi(buf);
          

    for (int i=1; i<=num; i++)
    {
        sprintf(mdmoid_user_object, "%s.%d", "Device.Users.User", i);

        if(tr69GetUnfreshLeafData(user_name, mdmoid_user_object, "Username"))   
        {
            gui_debug( "Get %s.Username error!", mdmoid_user_object);
        }

        if(tr69GetUnfreshLeafData(user_level, mdmoid_user_object, "X_ACTIONTEC_COM_AuthLevel"))
        {
            gui_debug( "Get %s.X_ACTIONTEC_COM_AuthLevel error!", mdmoid_user_object);
        }

        
        if (safe_strcmp(user_name, gWebParams.login_username) == 0)
        {
            switch (atoi(user_level))
            {
                case USERLEVEL_ROOT:
                    result = 4;
                    break;
                case USERLEVEL_ADMIN:
                    result = 2;
                    break;
                case USERLEVEL_USER:
                    result = 1;
                    break;
            }
            break;
        }
    }

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%d", result);
    strcat(varValue, buf);
}

#ifdef AEI_UHTTPD_REALM_AUTH
int check_user_realm(char *user, char *passwd, char *realm, int *level)
{
    DAL_USR_RC isValid;
    DAL_Passwd login_passwd;
    DAL_UAS_Desc uas_desc;
    int user_level;

    if(NULL == realm)
        return 1;
    //password
    login_passwd.passwd = passwd;
    login_passwd.type = DAL_USR_PSWD_TEXT;
    // which user access source to check
    uas_desc.id = DAL_UAS_ID_WEB; //access from web
    uas_desc.argv = realm;
    isValid = dal_userGetAuthLevel(user, &login_passwd, &uas_desc, &user_level);
    if(isValid == DAL_USR_OK)
    {
        *level = user_level;
        return 0;
    }
    else
        return 1;
}
#endif
