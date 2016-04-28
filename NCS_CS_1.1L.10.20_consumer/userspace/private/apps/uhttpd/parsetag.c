/*
 * parse tag from html
 */

#include <fcntl.h>
#include <time.h>
#include <strings.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/limits.h>

#include "uhttpd.h"
#include "uhttpd-utils.h"

#include "cgi_main.h"
#include "parsetag.h"

extern void uh_file_request(struct client *cl, struct http_request *req, struct path_info *pi);

struct tag_set tag_sets[] = {
    {"echo", echoParameter},
    {"echoreq", echoParameter},
    {"include", include},
    { NULL, NULL }
};

static char *get_arg(char *args, char **next)
{
    char *arg, *end;

    /* Parse out arg, ... */
    if (!(end = strchr(args, ','))) {
        end = args + strlen(args);
        *next = NULL;
    } else
        *next = end + 1;

    /* Skip whitespace and quotation marks on either end of arg */
    for (arg = args; isspace(*arg) || *arg == '"'; arg++);
    for (*end-- = '\0'; isspace(*end) || *end == '"'; end--)
        *end = '\0';

    return arg;
}


int parse_tag(struct client *cl, struct http_request *req, FILE *fp)
{   
    char buf[UH_LIMIT_MSGHEAD];
    struct timeval tv1;
    struct timeval tv2;
    char value[UH_LIMIT_MSGHEAD*3];
    int rlen = 0;
    
    memset(buf, 0, sizeof(buf));
    gettimeofday(&tv1, NULL);   
    memset(value, 0, sizeof(value));
    
    while( (rlen = fread(value, 1, sizeof(value), fp)) > 0 )
    {
        gettimeofday(&tv2, NULL);
        gui_debug("rlen=%d", rlen);
        
        char *p;    
        char head[UH_LIMIT_MSGHEAD] = {0};
        int idx = 0;
        
        p = strstr(value, "<?");
        while (p)
        {
            memset(head, 0, sizeof(head));
            strncpy(head, &value[idx], (int)(p-&value[idx]));
            idx += (int)(p-&value[idx]) + 2;
            //gui_debug("idx = %d, head 1 = %s\n", idx, head);
            ensure_out(uh_http_send(cl, req, head, strlen(head)));
            
            char *q;
            q = strstr(p, "?>");
            if (q)  
            {           
                memset(head, 0, sizeof(head));
                strncpy(head, &value[idx], (int)(q-&value[idx]));
                idx += (int)(q-&value[idx]) + 2;
                
                //gui_debug("idx = %d, tag = %s\n", idx, head);
                memset(buf, 0, sizeof(buf));
                gui_trim(head);
                parseFunc(head, buf, cl, req);
                
                if (buf[0] != '\0')
                    ensure_out(uh_http_send(cl, req, buf, strlen(buf)));
                memset(buf, 0, sizeof(buf));
                
                p = strstr(q, "<?");
            }
            else
            {
                idx -= 2;
                break;
            }
        }
        
        ensure_out(uh_http_send(cl, req, &value[idx], strlen(&value[idx])));

    }   
    
    gettimeofday(&tv2, NULL);
    gui_debug("parse html time=%f", gui_distime(tv1, tv2));
    ensure_out(uh_http_send(cl, req, buf, strlen(buf)));

    return 0;

out:
    return -1;
}


void parseFunc(char *tagname, char *buf, struct client *cl, struct http_request *req)
{
    //gui_debug("\n\n###$$$enter parseFunc $$$###\n\n");

    char *args, *end, *next;
    int argc;
    char * argv[20];    //void??
    struct tag_set *set;
        
    // Parse out ( args )
    if (!(args = strchr(tagname, '(')))
        return;
    if (!(end = strchr(tagname, ')')))
        return;
    *args++ = *end = '\0';

    // Set up argv list
    argv[0] = tagname;
    for (argc = 1; argc < 20 && args; argc++, args = next) {
        if (!(argv[argc] = get_arg(args, &next)))
            break;
    }

    if (safe_strcmp("include", tagname) == 0)
    {
        argv[argc++] = (char *)cl;
        argv[argc++] = (char *)req;
    }
    if (safe_strcmp("echoreq", tagname) == 0)
    {
        argv[argc++] = (char *)cl;
        argv[argc++] = (char *)req;
    }

    for (set = &tag_sets[0]; set->tagname; set++)
    {
        if (strncmp(set->tagname, tagname, strlen(set->tagname)) == 0)
        {       
            set->output(argc, argv, buf);
            break;
        }
    }
}

void cgiGetUserPermissions(char *buf)
{
#if 0 //sharko should modify here.
    char bbuf[BUFLEN_64] = {0};
    dal_ret_t *ret_info = NULL;
    int num = 0;
    char cur_usr[64]={0};

    ret_info = dal_sys_get_user_list(&num);
    if (ret_info == NULL) return;

    for (int i=0; i<num; i++)
    {
        bbuf[0] = '\0';
        //tsl_printf("--%s(%d): ret_info[%d].param[USER_PID]=%s\n", __FUNCTION__, __LINE__, i, ret_info[i].param[USER_PID]);
        sprintf(bbuf, "%s|", getValueFromDal(ret_info[i].param[USER_PID]));
        strcat(buf, bbuf);

        bbuf[0] = '\0';
        //tsl_printf("--%s(%d): ret_info[%d].param[USER_FULLNAME]=%s\n", __FUNCTION__, __LINE__, i, ret_info[i].param[USER_FULLNAME]);
        sprintf(bbuf, "%s|", getValueFromDal(ret_info[i].param[USER_FULLNAME]));
        strcat(buf, bbuf);

        bbuf[0] = '\0';
        //tsl_printf("--%s(%d): ret_info[%d].param[USER_NAME]=%s\n", __FUNCTION__, __LINE__, i, ret_info[i].param[USER_NAME]);
        sprintf(bbuf, "%s|", getValueFromDal(ret_info[i].param[USER_NAME]));
        strcat(buf, bbuf);
             if(strcmp(gWebParams.login_username,getValueFromDal(ret_info[i].param[USER_NAME]))==0)
        {
                strcpy(cur_usr,getValueFromDal(ret_info[i].param[USER_NAME]));
        }
           else
         {
           strcpy(cur_usr,"");
              }
        bbuf[0] = '\0';
        //tsl_printf("--%s(%d): ret_info[%d].param[USER_AUTH_LEVEL]=%s\n", __FUNCTION__, __LINE__, i, ret_info[i].param[USER_AUTH_LEVEL]);
        sprintf(bbuf, "%s||", getValueFromDal(ret_info[i].param[USER_AUTH_LEVEL]));
        if(strlen(cur_usr)>0)
        {
              strcpy(buf,"");
         strcpy(buf,getValueFromDal(ret_info[i].param[USER_AUTH_LEVEL]));
         break;
        }
        else
        {
        strcat(buf, bbuf);
        }
    }

    free_dal_ret(&ret_info);
//tsl_printf("\n\n\n--%s:test end buf=%s--\n\n\n", __FUNCTION__, buf);
#endif
    //sprintf(buf, "%s", "pid1|Administrator|admin|0||pid2|Test Accout|test|1||");
}

void include(int argc, char **argv, char *buf)
{

    // working structs
    struct http_request *req;
    struct client *cl;
    FILE *fp;
    char value[WEB_BUF_SIZE_MAX];
    int rlen;
    
    char filename[128];
    char subpath[64];
    char *psubpath;
    char permis[WEB_BUF_SIZE_MAX];
    
    if (argc < 4) return;
    
    cl = (struct client *)argv[2];
    req = (struct http_request *)argv[3];
    
    
    
    psubpath = strrchr(req->url, '/');
    strncpy(subpath, req->url, (int)(psubpath-req->url)+1);
    subpath[(int)(psubpath-req->url)+1] = '\0';
    //cgiGetUserPermissions(permis);
    //gui_debug("\nn\###$$$gWebParams.login_username=%s,permis=%s$$$###\n\n", gWebParams.login_username,permis);
    if(1)
    {
          if(strcmp(argv[1],"act_menu_top.inc")==0)
            ;
    }
    else
    {
       if(strcmp(permis,"0")==0&&strcmp(argv[1],"act_menu_top.inc")==0)
        ;
       if(strcmp(argv[1],"act_menu_top.inc")==0&&strcmp(permis,"1")==0)
            strcpy(argv[1],"act_menu_top2.inc");
       if(strcmp(permis,"0")==0&&strcmp(argv[1],"act_menu_left_firewall.inc")==0)
        ;
       if(strcmp(argv[1],"act_menu_left_firewall.inc")==0&&strcmp(permis,"1")==0)
            strcpy(argv[1],"act_menu_left_firewall2.inc");
    }
    sprintf(filename, "%s%s%s", cl->server->conf->docroot, subpath, argv[1]);
    gui_debug("\nn\###$$$--%s: filename=%s, argv[1]=%s$$$###\n\n", __FUNCTION__, filename, argv[1]);
    if((fp = fopen(filename, "r")) != NULL)
    {
        if (strstr(argv[1], ".html") || strstr(argv[1], ".htm"))
        {
            //printf("--%s: argv[1]=%s\n", __FUNCTION__, argv[1]);
            parse_tag(cl, req, fp);
        }
        else
        {
            gui_debug("include unhtml file .\n");
            while( (rlen = fread(value, 1, sizeof(value), fp)) > 0 )
                ensure_out(uh_http_send(cl, req, value, rlen));
        }
    }
    else
    {
        gui_debug("open include file error.\n");
    }
    
out:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
        gui_debug("include end 000.\n");  
    }
    gui_debug("include end 111.\n");    
    return;
   
}

void echoParameter(int argc, char **argv, char *buf)
{
    char value[WEB_BUF_SIZE_MAX];
    
    if (argc < 2) return;
    
    value[0] = '\0';
    cgiEchoParameter(argc, argv, value);
    
    strcat(buf, value);
}

