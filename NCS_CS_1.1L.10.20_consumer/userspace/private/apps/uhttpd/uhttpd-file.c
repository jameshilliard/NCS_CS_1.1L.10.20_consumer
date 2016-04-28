/*
 * uhttpd - Tiny single-threaded httpd - Static file handler
 *
 *   Copyright (C) 2010 Jo-Philipp Wich <xm@subsignal.org>
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#define _XOPEN_SOURCE 500   /* strptime() */
#define _BSD_SOURCE         /* scandir(), timegm() */

#include "uhttpd.h"
#include "uhttpd-utils.h"
#include "uhttpd-file.h"

#include "uhttpd-mimetypes.h"

#ifdef AEI_REVJ
extern int parse_tag(struct client *cl, struct http_request *req, FILE *fp);
#endif
#ifdef AEI_REVJ
#include "cgi_main.h"
#endif

static const char * uh_file_mime_lookup(const char *path)
{
    struct mimetype *m = &uh_mime_types[0];
    const char *e;

    while( m->extn )
    {
        e = &path[strlen(path)-1];

        while( e >= path )
        {
            if( (*e == '.') && !strcasecmp(&e[1], m->extn) )
                return m->mime;

            e--;
        }

        m++;
    }

    return "application/octet-stream";
}

static const char * uh_file_mktag(struct stat *s)
{
    static char tag[128];

    snprintf(tag, sizeof(tag), "\"%x-%x-%x\"",
        (unsigned int) s->st_ino,
        (unsigned int) s->st_size,
        (unsigned int) s->st_mtime
    );

    return tag;
}

static time_t uh_file_date2unix(const char *date)
{
    struct tm t;

    memset(&t, 0, sizeof(t));

    if( strptime(date, "%a, %d %b %Y %H:%M:%S %Z", &t) != NULL )
        return timegm(&t);

    return 0;
}

static char * uh_file_unix2date(time_t ts)
{
    static char str[128];
    struct tm *t = gmtime(&ts);

    strftime(str, sizeof(str), "%a, %d %b %Y %H:%M:%S GMT", t);

    return str;
}

static char * uh_file_header_lookup(struct http_request *req, const char *name)
{
    int i;

    foreach_header(i, req->headers)
    {
        if( ! strcasecmp(req->headers[i], name) )
            return req->headers[i+1];
    }

    return NULL;
}


static int uh_file_response_ok_hdrs(struct client *cl, struct http_request *req, struct stat *s)
{
    ensure_ret(uh_http_sendf(cl, NULL, "Connection: close\r\n"));

    if( s )
    {
        ensure_ret(uh_http_sendf(cl, NULL, "ETag: %s\r\n", uh_file_mktag(s)));
        ensure_ret(uh_http_sendf(cl, NULL, "Last-Modified: %s\r\n", uh_file_unix2date(s->st_mtime)));
    }

    return uh_http_sendf(cl, NULL, "Date: %s\r\n", uh_file_unix2date(time(NULL)));
}

static int uh_file_response_200(struct client *cl, struct http_request *req, struct stat *s)
{
    ensure_ret(uh_http_sendf(cl, NULL, "HTTP/%.1f 200 OK\r\n", req->version));
    return uh_file_response_ok_hdrs(cl, req, s);
}

static int uh_file_response_304(struct client *cl, struct http_request *req, struct stat *s)
{
    ensure_ret(uh_http_sendf(cl, NULL, "HTTP/%.1f 304 Not Modified\r\n", req->version));
    return uh_file_response_ok_hdrs(cl, req, s);
}

static int uh_file_response_412(struct client *cl, struct http_request *req)
{
    return uh_http_sendf(cl, NULL,
        "HTTP/%.1f 412 Precondition Failed\r\n"
        "Connection: close\r\n", req->version);
}

static int uh_file_if_match(struct client *cl, struct http_request *req, struct stat *s, int *ok)
{
    const char *tag = uh_file_mktag(s);
    char *hdr = uh_file_header_lookup(req, "If-Match");
    char *p;
    int i;

    if( hdr )
    {
        p = &hdr[0];

        for( i = 0; i < strlen(hdr); i++ )
        {
            if( (hdr[i] == ' ') || (hdr[i] == ',') )
            {
                hdr[i++] = 0;
                p = &hdr[i];
            }
            else if( !strcmp(p, "*") || !strcmp(p, tag) )
            {
                *ok = 1;
                return *ok;
            }
        }

        *ok = 0;
        ensure_ret(uh_file_response_412(cl, req));
        return *ok;
    }

    *ok = 1;
    return *ok;
}

static int uh_file_if_modified_since(struct client *cl, struct http_request *req, struct stat *s, int *ok)
{
    char *hdr = uh_file_header_lookup(req, "If-Modified-Since");
    *ok = 1;

    if( hdr )
    {
        if( uh_file_date2unix(hdr) >= s->st_mtime )
        {
            *ok = 0;
            ensure_ret(uh_file_response_304(cl, req, s));
        }
    }

    return *ok;
}

static int uh_file_if_none_match(struct client *cl, struct http_request *req, struct stat *s, int *ok)
{
    const char *tag = uh_file_mktag(s);
    char *hdr = uh_file_header_lookup(req, "If-None-Match");
    char *p;
    int i;
    *ok = 1;

    if( hdr )
    {
        p = &hdr[0];

        for( i = 0; i < strlen(hdr); i++ )
        {
            if( (hdr[i] == ' ') || (hdr[i] == ',') )
            {
                hdr[i++] = 0;
                p = &hdr[i];
            }
            else if( !strcmp(p, "*") || !strcmp(p, tag) )
            {
                *ok = 0;

                if( (req->method == UH_HTTP_MSG_GET) ||
                    (req->method == UH_HTTP_MSG_HEAD) )
                    ensure_ret(uh_file_response_304(cl, req, s));
                else
                    ensure_ret(uh_file_response_412(cl, req));

                break;
            }
        }
    }

    return *ok;
}

static int uh_file_if_range(struct client *cl, struct http_request *req, struct stat *s, int *ok)
{
    char *hdr = uh_file_header_lookup(req, "If-Range");
    *ok = 1;

    if( hdr )
    {
        *ok = 0;
        ensure_ret(uh_file_response_412(cl, req));
    }

    return *ok;
}

static int uh_file_if_unmodified_since(struct client *cl, struct http_request *req, struct stat *s, int *ok)
{
    char *hdr = uh_file_header_lookup(req, "If-Unmodified-Since");
    *ok = 1;

    if( hdr )
    {
        if( uh_file_date2unix(hdr) <= s->st_mtime )
        {
            *ok = 0;
            ensure_ret(uh_file_response_412(cl, req));
        }
    }

    return *ok;
}


static int uh_file_scandir_filter_dir(const struct dirent *e)
{
    return strcmp(e->d_name, ".") ? 1 : 0;
}

static void uh_file_dirlist(struct client *cl, struct http_request *req, struct path_info *pi)
{
    int i;
    int count = 0;
    char filename[PATH_MAX];
    char *pathptr;
    struct dirent **files = NULL;
    struct stat s;

    ensure_out(uh_http_sendf(cl, req,
        "<html><head><title>Index of %s</title></head>"
        "<body><h1>Index of %s</h1><hr /><ol>",
            pi->name, pi->name
    ));

    if( (count = scandir(pi->phys, &files, uh_file_scandir_filter_dir, alphasort)) > 0 )
    {
        memset(filename, 0, sizeof(filename));
        memcpy(filename, pi->phys, sizeof(filename));
        pathptr = &filename[strlen(filename)];

        /* list subdirs */
        for( i = 0; i < count; i++ )
        {
            strncat(filename, files[i]->d_name,
                sizeof(filename) - strlen(files[i]->d_name));

            if( !stat(filename, &s) &&
                (s.st_mode & S_IFDIR) && (s.st_mode & S_IXOTH)
            )
                ensure_out(uh_http_sendf(cl, req,
                    "<li><strong><a href='%s%s'>%s</a>/</strong><br />"
                    "<small>modified: %s<br />directory - %.02f kbyte"
                    "<br /><br /></small></li>",
                        pi->name, files[i]->d_name, files[i]->d_name,
                        uh_file_unix2date(s.st_mtime), s.st_size / 1024.0
                ));

            *pathptr = 0;
        }

        /* list files */
        for( i = 0; i < count; i++ )
        {
            strncat(filename, files[i]->d_name,
                sizeof(filename) - strlen(files[i]->d_name));

            if( !stat(filename, &s) &&
                !(s.st_mode & S_IFDIR) && (s.st_mode & S_IROTH)
            )
                ensure_out(uh_http_sendf(cl, req,
                    "<li><strong><a href='%s%s'>%s</a></strong><br />"
                    "<small>modified: %s<br />%s - %.02f kbyte<br />"
                    "<br /></small></li>",
                        pi->name, files[i]->d_name, files[i]->d_name,
                        uh_file_unix2date(s.st_mtime),
                        uh_file_mime_lookup(filename), s.st_size / 1024.0
                ));

            *pathptr = 0;
        }
    }

    ensure_out(uh_http_sendf(cl, req, "</ol><hr /></body></html>"));
    ensure_out(uh_http_sendf(cl, req, ""));

out:
    if( files )
    {
        for( i = 0; i < count; i++ )
            free(files[i]);

        free(files);
    }
}

#if 0 // AEI_OPENWRT sharko modify here.
//get hidden page data from dal function.
HIDDENPAGE_VAR GetHiddenPageData[] = {
    {HIDDENPAGE_HTML, dal_get_hiddenpage_htmldata},
    {HIDDENPAGE_XML, dal_sys_get_hidden_page_info},
    {NULL, NULL}
};

int uh_hiddenpage_lookup(const char *url)
{
    gui_debug("req->url=%s", url);
    for (int i=0; i<ARRAY_SIZE(GetHiddenPageData)-1; i++)
    {
        gui_debug("GetHiddenPageData[i].hiddenpagename=%s", GetHiddenPageData[i].hiddenpagename);
        if (tsl_strstr(url, GetHiddenPageData[i].hiddenpagename) != NULL)
            return 1;
    }

    return 0;
}

void uh_file_hidden_request(struct client *cl, struct http_request *req)
{
    char buf[UH_LIMIT_MSGHEAD];
    char *ret = NULL;

    /* get hidden page data */
    for (int i=0; i<ARRAY_SIZE(GetHiddenPageData)-1; i++)
    {
        if (strstr(req->url, GetHiddenPageData[i].hiddenpagename) != NULL)
        {
            ret = (*GetHiddenPageData[i].hiddenpagefunc)();
            if (ret == NULL) return;

            break;
        }
    }

    /* send header */
    ensure_out(uh_file_response_200(cl, req, NULL));
    gui_debug("uh_file_mime_lookup(req->url)=%s", uh_file_mime_lookup(req->url));
    ensure_out(uh_http_sendf(cl, NULL, "Content-Type: %s\r\n", uh_file_mime_lookup(req->url)));
    ensure_out(uh_http_sendf(cl, NULL, "Connection: keep-alive\r\n"));

    if (strstr(req->url, ".html") || strstr(req->url, ".htm"))
    {
        //ensure_out(uh_http_sendf(cl, NULL, "Cache-Control: no-cache\r\n")) ;
        ensure_out(uh_http_sendf(cl, NULL, "Cache-Control: private, no-cache, must-revalidate\r\n")) ;
        ensure_out(uh_http_sendf(cl, NULL, "Expires: 0\r\n")) ;
    }
    else
    {
        ensure_out(uh_http_sendf(cl, NULL, "Cache-Control: max-age=19184000000\r\n")) ;
    }

    /* if request was HTTP 1.1 we'll respond chunked */
    if( (req->version > 1.0) && (req->method != UH_HTTP_MSG_HEAD) )
        ensure_out(uh_http_send(cl, NULL, "Transfer-Encoding: chunked\r\n", -1));
    
    /* close header */
    ensure_out(uh_http_send(cl, NULL, "\r\n", -1));

    /* send body */
    if (req->method != UH_HTTP_MSG_HEAD)
    {
        for (int i=0; i<safe_strlen(ret)/UH_LIMIT_MSGHEAD+1; i++)
        {
            /* pump file data */
            memset(buf, 0, sizeof(buf));
            memcpy(buf, ret+i*UH_LIMIT_MSGHEAD, sizeof(buf));
            ensure_out(uh_http_send(cl, req, buf, strlen(buf)));
        }

        /* send trailer in chunked mode */
        ensure_out(uh_http_send(cl, req, "", 0));
    }

out:
    if(ret)
        free(ret);
}
#endif

void uh_file_request(struct client *cl, struct http_request *req, struct path_info *pi)
{
    int rlen;
    int ok = 1;
#ifdef AEI_REVJ
    FILE *fp = NULL;
#else
    int fd = -1;
#endif  
    char buf[UH_LIMIT_MSGHEAD];

    /* we have a file */
#ifdef AEI_REVJ
    if( (pi->stat.st_mode & S_IFREG) && ((fp = fopen(pi->phys, "r")) != NULL) )
#else
    if( (pi->stat.st_mode & S_IFREG) && ((fd = open(pi->phys, O_RDONLY)) > 0) )
#endif              
    {
        /* test preconditions */
#ifdef AEI_REVJ
        if (!(strstr(pi->phys, ".html") || strstr(pi->phys, ".htm") || strstr(pi->phys, ".js") || strstr(pi->phys, ".css")|| strstr(pi->phys, ".xml")))
        {
            if(ok) ensure_out(uh_file_if_modified_since(cl, req, &pi->stat, &ok));
        }
#else
        if(ok) ensure_out(uh_file_if_modified_since(cl, req, &pi->stat, &ok));
#endif
        if(ok) ensure_out(uh_file_if_match(cl, req, &pi->stat, &ok));
        if(ok) ensure_out(uh_file_if_range(cl, req, &pi->stat, &ok));
        if(ok) ensure_out(uh_file_if_unmodified_since(cl, req, &pi->stat, &ok));

#ifdef AEI_REVJ
        if (!(strstr(pi->phys, ".html") || strstr(pi->phys, ".htm") || strstr(pi->phys, ".js") || strstr(pi->phys, ".css")|| strstr(pi->phys, ".xml")))
        {
            if(ok) ensure_out(uh_file_if_none_match(cl, req, &pi->stat, &ok));
        }
#else
        if(ok) ensure_out(uh_file_if_none_match(cl, req, &pi->stat, &ok));
#endif

        if( ok > 0 )
        {
            /* write status */
            ensure_out(uh_file_response_200(cl, req, &pi->stat));

            ensure_out(uh_http_sendf(cl, NULL, "Content-Type: %s\r\n", uh_file_mime_lookup(pi->name)));
            //ensure_out(uh_http_sendf(cl, NULL, "Content-Length: %i\r\n", pi->stat.st_size));
            
            ensure_out(uh_http_sendf(cl, NULL, "Connection: keep-alive\r\n"));
            //ensure_out(uh_http_sendf(cl, NULL, "Content-Length: %i\r\n", 0));

#ifdef AEI_REVJ
#ifdef HAVE_SESSION
            if (req->session_id > 0)
            {
                ensure_out(uh_http_sendf(cl, NULL, "Set-Cookie: %s=%u;\r\n", UH_SESSION_COOKIE, req->session_id));
                session_update_starttime(req->session_id);
            }
#endif          
            if ((strstr(pi->phys, ".html") || strstr(pi->phys, ".htm") || strstr(pi->phys, ".js") || strstr(pi->phys, ".css")|| strstr(pi->phys, ".xml")))
            {
                //ensure_out(uh_http_sendf(cl, NULL, "Cache-Control: no-cache\r\n")) ;
                ensure_out(uh_http_sendf(cl, NULL, "Cache-Control: private, no-cache, must-revalidate\r\n")) ;
                ensure_out(uh_http_sendf(cl, NULL, "Expires: 0\r\n")) ;
            }
            else
            {
                ensure_out(uh_http_sendf(cl, NULL, "Cache-Control: max-age=19184000000\r\n")) ;
            }
#endif

            /* if request was HTTP 1.1 we'll respond chunked */
            if( (req->version > 1.0) && (req->method != UH_HTTP_MSG_HEAD) )
                ensure_out(uh_http_send(cl, NULL, "Transfer-Encoding: chunked\r\n", -1));

            /* close header */
            ensure_out(uh_http_send(cl, NULL, "\r\n", -1));

            /* send body */
            if( req->method != UH_HTTP_MSG_HEAD )
            {
#ifdef AEI_REVJ
                if (strstr(pi->phys, ".html") || strstr(pi->phys, ".htm")|| strstr(pi->phys, ".xml"))
                {
                    memset(gWebParams.query_string, 0, strlen(gWebParams.query_string));
                    if (pi->query)
                        strncpy(gWebParams.query_string, pi->query, 256);
                    else
                        strncpy(gWebParams.query_string, "NULL", 4);
                }
//gui_debug("pi->phys=%s, pi->query=%s, query_string=%s", pi->phys, pi->query, gWebParams.query_string);
                    
                if (strstr(pi->phys, ".html") || strstr(pi->phys, ".htm") || strstr(pi->phys, ".js") || strstr(pi->phys, ".xml"))
                {
                    if (parse_tag(cl, req, fp) == -1)
                    {
                        goto out;
                    }
                }
                else
                {
                    while( (rlen = fread(buf, 1, sizeof(buf), fp)) > 0 )
                        ensure_out(uh_http_send(cl, req, buf, rlen));
                }
#else               
                /* pump file data */
                while( (rlen = read(fd, buf, sizeof(buf))) > 0 )
                    ensure_out(uh_http_send(cl, req, buf, rlen));
#endif                  

                /* send trailer in chunked mode */
                ensure_out(uh_http_send(cl, req, "", 0));
            }
        }

        /* one of the preconditions failed, terminate opened header and exit */
        else
        {
            ensure_out(uh_http_send(cl, NULL, "\r\n", -1));
        }
    }

    /* directory */
    else if( (pi->stat.st_mode & S_IFDIR) && !cl->server->conf->no_dirlists )
    {
        /* write status */
        ensure_out(uh_file_response_200(cl, req, NULL));

        if( req->version > 1.0 )
            ensure_out(uh_http_send(cl, NULL, "Transfer-Encoding: chunked\r\n", -1));

        ensure_out(uh_http_send(cl, NULL, "Content-Type: text/html\r\n\r\n", -1));

        /* content */
        uh_file_dirlist(cl, req, pi);
    }

    /* 403 */
    else
    {
        ensure_out(uh_http_sendhf(cl, 403, "Forbidden",
            "Access to this resource is forbidden"));
    }

out:
#ifdef AEI_REVJ
    if (fp)
        fclose(fp);
#else   
    if( fd > -1 )
        close(fd);
#endif      
}


#ifdef AEI_REVJ
void uh_download_file(struct client *cl, struct http_request *req)
{
    uh_http_sendf(cl, NULL, "HTTP/%.1f 200 OK\r\n", req->version);
    
    if( (req->version > 1.0) && (req->method != UH_HTTP_MSG_HEAD) )
    {
        gui_debug("start send chunker; req->version=%f, req->method=%d", req->version, req->method);
        uh_http_sendf(cl, NULL, "Transfer-Encoding: chunked\r\n", -1);
    }

    

    do_download(cl, req);
    //uh_http_send(cl, req, buf, rlen);
    //uh_http_send(cl, req, "configuration file content", strlen("configuration file content"));
    
    uh_http_send(cl, req, "", 0);

}

void send_download_file(struct client *cl, struct http_request *req, char *filename, char *buf, int buflen)
{
    gui_debug("Content-Type=%s, filename=%s, buflen=%d", uh_file_mime_lookup(filename), filename, buflen);

    //uh_http_sendf(cl, NULL, "Content-Type: application/octet-stream\r\n");
    uh_http_sendf(cl, NULL, "Content-Type: %s\r\n", uh_file_mime_lookup(filename));
    uh_http_sendf(cl, NULL, "Content-Disposition: attachment;filename=%s\r\n", filename);
    
    //uh_http_sendf(cl, NULL, "Content-Length: %i\r\n", strlen("configuration file content"));
    uh_http_send(cl, NULL, "\r\n", -1);
    

    uh_http_send(cl, req, buf, buflen);
}

#endif
