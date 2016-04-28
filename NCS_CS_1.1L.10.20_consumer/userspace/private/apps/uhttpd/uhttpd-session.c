/*
 * deal with session
 */

#include "uhttpd.h"
#include "uhttpd-utils.h"

//#include "uhttpd-session.h"
#include "cgi_main.h"


static char *session_need_check_file_ext[] = {
    "html",
    "htm",
    "cgi",
    "act",
    "cmd"
};

static char *session_free_visit[] = {
    //"main",
    "home",
    "wirelesschanging_notify",
    "firmwareupgrade_notify",
    "forgot_password",   
    "thankyou",    
    "attention",
    "firsttime_login",
    "test",
    "pwd_reset",
    "telnetd_start"
};


HTTP_SESSION gSession[99];

CGI_REJECTITEM CgiRejectPageMap[] = {

#if defined(AEI_WECB_CUSTOMER_TELUS)

    {"wl_basic.html", 6, 3},
    {"wl_adv.html", 4, 3},
    {"adv_password.html", 2, 3},
    {"adv_upgrade.html", 4, 3},
    {"adv_swap.html", 4, 3},

    {"adv_tr69.html", 4, 3},
    {"adv_localssh.html", 4, 3},
    {"forgot_password.html", 2, 3},
    {"adv_option60.html", 0, 3},
    {"wl_security.html", 6, 3},

    {"moca_setup.html", 4, 2},
    {"moca_status.html", 4, 2},
#endif
    
    { NULL, 7, 3}
};

static int get_max_session()
{
    int userNum = 0;
    //fake login
    userNum = UH_SESSION_MAX;
#if 0 //sharko should modify here.
    tsl_rv_t ret = TSL_RV_FAIL;

    ret = dal_sys_get_max_current_user(&userNum);
    if (ret != TSL_RV_SUC || userNum == 0)
    {
        userNum = UH_SESSION_MAX;
    }
#endif
    return userNum;
}


/*
 *  init session array.
*/
void session_init()
{
    gui_debug("session init");
    int i = 0;
    for (i=0; i<UH_SESSION_MAX; i++)
    {
        gSession[i].session_id = 0;
        
        memset(gSession[i].username, 0, sizeof(gSession[i].username));
        memset(gSession[i].userpass, 0, sizeof(gSession[i].userpass));
#ifdef AEI_UHTTPD_REALM_AUTH
        gSession[i].client_realm = NULL;
#endif
        
        gSession[i].user_level = 0;
        gSession[i].session_start = 0;
        gSession[i].is_login = 0;
    }
}


/*
 *  check request url whether need check session
 *  1:need check session
*/
int session_is_needcheck(char *url)
{
    char url_tmp[128] = {0};
    char *fullname[64];
    char *filename[64];
    char *ext[64];
    int num = 0;
    
    memset(url_tmp, 0, sizeof(url_tmp));
    strncpy(url_tmp, url, (strlen(url)>=128)?127:strlen(url));
    
    gui_split(url_tmp, "/", fullname, &num);
    if (num < 1)    return 0;
        
    gui_split(fullname[num-1], ".", filename, &num);
    if (num < 1)    return 0;
        
    gui_split(filename[1], "?", ext, &num);
    if (num < 0)    return 0;
    
    //gui_debug("filename[0]=%s, filename[1]=%s, ext[0]=%s", filename[0], filename[1], ext[0]);         
    for(int i=0; i<array_size(session_need_check_file_ext); i++ )
    {
        if (safe_strcmp(ext[0], session_need_check_file_ext[i]) == 0)
        {
            for(int j=0; j<array_size(session_free_visit); j++)
            {
                //gui_debug("url=%s, session_free_visit[%d]=%s, filename[0]=%s", url, j, session_free_visit[j], filename[0]);
                if (safe_strcmp(filename[0], session_free_visit[j]) == 0)
                    return 0;
            }
            return 1;
        }
    }
    
    return 0;
}

/*
 *  generate a new session id.
*/
unsigned int session_gen_session_id()
{
    unsigned int session_id;
    
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned int sec = tv.tv_sec;
    unsigned int usec = tv.tv_usec;

    sec = 10000000 + sec % 10000000;
    usec = usec / 10000;
    session_id = sec*100 + usec;
    //gui_debug("sec=%d, usec=%d, session_id=%d", sec, usec, session_id);
    
    return session_id;
}


/*
 *  add this session_id into array
*/
void session_has_logined(unsigned int session_id)
{
    for (int i=0; i<UH_SESSION_MAX; i++)
    {
        if (gSession[i].session_id == 0)
        {
            struct timeval tv;
            gettimeofday(&tv, NULL);
            gSession[i].session_start = (unsigned int)tv.tv_sec;
            
            gSession[i].session_id = session_id;            
            gSession[i].is_login = 1;
            
            break;
        }
    }
}

/*
 *  check session array,whether can add new session.
 *  1:can add new user.
*/
int session_has_empty()
{
    int userNum = get_max_session();
    int curSessionNum = 0;

    for (int i=0; i<UH_SESSION_MAX; i++)
    {
        //gui_debug("gSession[%d].session_id=%u", i, gSession[i].session_id);

        if (gSession[i].session_id != 0)
            curSessionNum++;
    }

    if (curSessionNum >= userNum)
        return 0;
    else
        return 1;
}


/*
 *  check online user 
 *  1:the session has logined.
*/
int session_is_logined(unsigned int session_id)
{
    for (int i=0; i<UH_SESSION_MAX; i++)
    {
        gui_debug("gSession[%d].session_id=%u,session_id=%u",i,gSession[i].session_id,session_id);
        if (gSession[i].session_id == session_id)
            return 1;
    }
    
    return 0;
}


/*
 *  check session lifetime status
*/
void session_timeout_check()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned int sec = (unsigned int)tv.tv_sec;
    
    for (int i=0; i<UH_SESSION_MAX; i++)
    {
        //gui_debug("sec=%u, gSession[%d].session_start=%u, session_id=%u", sec, i, gSession[i].session_start, gSession[i].session_id);
        if (gSession[i].session_id >0 && (sec - gSession[i].session_start)>g_sys_setting.session_lifetime)
        {
            gWebParams.wcb_main_login_success=0; 
            session_clear_single(gSession[i].session_id);
        }
    }
}


/*
 *  clear this session
*/
void session_clear_single(unsigned int session_id)
{
    //gui_debug("session_id=%u", session_id);
    for (int i=0; i<UH_SESSION_MAX; i++)
    {
        if (gSession[i].session_id == session_id)
        {
            gSession[i].session_start = 0;
            gSession[i].session_id = 0;
            gSession[i].is_login = 0;
#ifdef AEI_UHTTPD_REALM_AUTH
            gSession[i].client_realm = NULL;
#endif
            break;
        }
    }
}


/*
 *  update session start time
*/
void session_update_starttime(unsigned int session_id)
{
    //gui_debug("session_id=%u", session_id);
    for (int i=0; i<UH_SESSION_MAX; i++)
    {
        if (gSession[i].session_id == session_id)
        {
            struct timeval tv;
            gettimeofday(&tv, NULL);
            gSession[i].session_start = (unsigned int)tv.tv_sec;
            
            break;
        }
    }
}


/*
 *  logout this session. session_id to be set 0
*/
void session_logout(unsigned int session_id)
{
    for (int i=0; i<UH_SESSION_MAX; i++)
    {
        if (session_id == gSession[i].session_id)
        {
            gWebParams.wcb_main_login_success=0;
            session_clear_single(gSession[i].session_id);
            break;
        }
    }
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      isAllowAccess
 *
 *  [ARGUMENT]:
 *      char *filename:visit filename
 *      char *username:username
 *  [DESCRITION]:
 *      
 *  [return]:
 *      1:access visit, 0:reject
 **************************************************************************/
#ifndef AEI_UHTTPD_REALM_AUTH
int isAllowAccess(char *url, char *username)
{
    int result = 1;
    int num = 0;

    char user_name[64] = {0};
    char user_level[4] = {0};
    int i_userlevel = 0;
    
    char buf[4] = {0};
    char mdmoid_user_object[256] = {0};
    int user_entries = 0;
    int i = 0;

    char modelname[BUFLEN_64] = {0};
    int modeltype = 1;
    if(tr69GetUnfreshLeafData(modelname, "Device.DeviceInfo", "ModelName")) {
        gui_debug("get Status error");
    }
    if (gui_strstr(modelname, "WCB") != NULL)   modeltype = 2;


    if(tr69GetUnfreshLeafData(buf, "Device.Users", "UserNumberOfEntries")) {
        gui_debug( "Get Device.Users.UserNumberOfEntries error!");
    }
    num = atoi(buf);

    for (int i=1; i<=num; i++) {
        sprintf(mdmoid_user_object, "%s.%d", "Device.Users.User", i);

        if(tr69GetUnfreshLeafData(user_name, mdmoid_user_object, "Username")) {
            gui_debug( "Get %s.Username error!", mdmoid_user_object);
        }
            
        if(tr69GetUnfreshLeafData(user_level, mdmoid_user_object, "X_ACTIONTEC_COM_AuthLevel")) {
            gui_debug( "Get %s.X_ACTIONTEC_COM_AuthLevel error!", mdmoid_user_object);
        }

        if (safe_strcmp(user_name, gWebParams.login_username) == 0) {
            switch (atoi(user_level))
            {
                case USERLEVEL_ROOT:
                    i_userlevel = 4;
                    break;
                case USERLEVEL_ADMIN:
                    i_userlevel = 2;
                    break;
                case USERLEVEL_USER:
                    i_userlevel = 1;
                    break;
            }

            gui_debug("url => %s", url);
            int i = 0;
            for ( ; CgiRejectPageMap[i].filename != NULL; i++ )
            {
                if(strstr(url, CgiRejectPageMap[i].filename) != NULL) {
                    gui_debug("CgiRejectPageMap filename => %s, attr = %d", CgiRejectPageMap[i].filename, CgiRejectPageMap[i].attr);

                    if ((CgiRejectPageMap[i].attr & i_userlevel) != 0 && (CgiRejectPageMap[i].modeltype & modeltype) != 0)
                        result = 1;
                    else
                        result = 0;
                    //result = (CgiRejectPageMap[i].attr & i_userlevel) & (CgiRejectPageMap[i].modeltype & modeltype);
                    gui_debug("CgiRejectPageMap[i].attr => %d, i_userlevel = %d, after & = %d", CgiRejectPageMap[i].attr, i_userlevel, (CgiRejectPageMap[i].attr & i_userlevel));
                    gui_debug("CgiRejectPageMap[i].modeltype => %d, modeltype = %d, after & = %d", CgiRejectPageMap[i].modeltype, modeltype, (CgiRejectPageMap[i].modeltype & modeltype));
                    gui_debug("result => %d", result);
                    break;
                }
            }
        } // end safe_strcmp
    }

    return result;
}
#endif
#ifdef AEI_UHTTPD_REALM_AUTH
int isAllowAccess(char *url, int level)
{
    int i, i_userlevel = 0, modeltype = 1;
    char modelname[BUFLEN_64] = {0};

    if(tr69GetUnfreshLeafData(modelname, "Device.DeviceInfo", "ModelName"))
    {
        gui_debug("get Status error");
    }
    if (gui_strstr(modelname, "WCB") != NULL)
        modeltype = 2;

    switch (level)
    {
        case USERLEVEL_ROOT:
            i_userlevel = 4;
            break;
        case USERLEVEL_ADMIN:
            i_userlevel = 2;
            break;
        case USERLEVEL_USER:
            i_userlevel = 1;
            break;
    }

    for (i = 0; CgiRejectPageMap[i].filename != NULL; i++ )
    {
        if(strstr(url, CgiRejectPageMap[i].filename) != NULL)
        {
            if ((CgiRejectPageMap[i].attr & i_userlevel) != 0 && (CgiRejectPageMap[i].modeltype & modeltype) != 0)
                return 0;
            else
                return 1;
        }
    }
    return 0;
}

/* set realm of session_id to array.
 * 0 - success
 * 1 - failure
*/
int set_session_realm2id(unsigned int session_id, char *realm, int level)
{
    if(session_is_logined(session_id) == 0)
        return 1;

    if(realm == NULL)
        return 1;

    for (int i=0; i<UH_SESSION_MAX; i++)
    {
        if (gSession[i].session_id == session_id)
        {
            gSession[i].client_realm = realm;
            gSession[i].user_level = level;
            return 0;
        }
    }
    return 0;
}
/*
 * get the realm correspond to the session_id
*/
int get_session_realm2id(unsigned int session_id, char *realm, int *level)
{
    if(session_is_logined(session_id) == 0)
        return 1;

    if(realm == NULL)
        return 1;

    for (int i=0; i<UH_SESSION_MAX; i++)
    {
        if (gSession[i].session_id == session_id)
        {
                if((gSession[i].client_realm != NULL) && (!strcmp(gSession[i].client_realm, realm)))
                {
                    *level = gSession[i].user_level;
                    return 0;
                }
                else
                    return 1;
        }
    }
    return 1;
}
#endif
