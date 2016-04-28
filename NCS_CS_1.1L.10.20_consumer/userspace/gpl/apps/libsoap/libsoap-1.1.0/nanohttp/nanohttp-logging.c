/******************************************************************
*  $Id: nanohttp-logging.c,v 1.1 2006/07/09 16:22:52 snowdrop Exp $
*
* CSOAP Project:  A http client/server library in C
* Copyright (C) 2003  Ferhat Ayaz
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.
*
* You should have received a copy of the GNU Library General Public
* License along with this library; if not, write to the
* Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA  02111-1307, USA.
*
* Email: ayaz@jprogrammer.net
******************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#endif

#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif

#ifdef AEI_CISCO_HNAP
#include <malloc.h>
#endif

#include "nanohttp-logging.h"
#ifdef AEI_WECB_CUSTOMER_COMCAST
#include "libtr69_client.h"
#include <sys/stat.h>
#include "OID.h"
#include "ctl.h"
#include <sys/stat.h>
#endif

#ifdef AEI_SYSLOG
#include <sys/syslog.h>
#endif

#ifdef WIN32
#ifndef __MINGW32__

/* not thread safe!*/
char *
VisualC_funcname(const char *file, int line)
{
  static char buffer[256]={0};
  int i = strlen(file) - 1;
  while (i > 0 && file[i] != '\\')
    i--;
  sprintf(buffer, "%s:%d", (file[i] != '\\') ? file : (file + i + 1), line);
  return buffer;
}

#endif
#endif

static log_level_t loglevel = HLOG_DEBUG;
static char logfile[75] = { '\0' };
static int log_background = 0;

#ifdef AEI_WECB_CUSTOMER_COMCAST
 int max_size=0;
void Is_set_full_logfile(int b_full)
{
    tr69_set_unfresh_leaf_data(TR69_OID_HNAP_SERVER".LogFile_Full",
    (void *)&b_full, TR69_NODE_LEAF_TYPE_BOOL);
}
#endif

log_level_t
hlog_set_level(log_level_t level)
{
  log_level_t old = loglevel;
  loglevel = level;
  return old;
}


log_level_t
hlog_get_level(void)
{
  return loglevel;
}


void
hlog_set_file(const char *filename)
{
  if (filename)
#ifdef AEI_COVERITY_FIX
    /*CID 31873: Buffer not null terminated(BUFFER_SIZE_WARNING)*/
    strncpy(logfile, filename, sizeof(logfile) - 1);
#else
    strncpy(logfile, filename, 75);
#endif
  else
    logfile[0] = '\0';
}

void
hlog_set_background(int state)
{
  log_background = state;
}

char *
hlog_get_file()
{
  if (logfile[0] == '\0')
    return NULL;
  return logfile;
}

static void
_log_write(log_level_t level, const char *prefix,
        const char *func, int line, const char *format, va_list ap)
{
#ifdef AEI_CISCO_HNAP
    char buffer[1024]={0};
    char buffer2[20480]={0};    // next check use static define?
#else
    char buffer[1054]={0};
    char buffer2[1054]={0};
#endif
    FILE *f=NULL;
#ifdef  AEI_WECB_CUSTOMER_COMCAST
    int rv=-1;
    int type;
    struct stat info;
    long file_size=0;
#endif
#ifdef AEI_CISCO_HNAP
    char timestr[256]={0};
    time_t t;
    struct tm *tmp;

    t = time(NULL);
    tmp = localtime(&t);
    strftime(timestr, sizeof(timestr), "%T", tmp);
#endif

    if (level < loglevel)
        return;

    if (!log_background || hlog_get_file())
    {
#ifdef AEI_CISCO_HNAP

        snprintf(buffer, sizeof(buffer), "%s *%s*:\t[%s:%d] %s\n",timestr, prefix, func, line, format);

#else
#ifdef WIN32
        sprintf(buffer, "*%s*: [%s] %s\n", prefix, func, format);
#else
        sprintf(buffer, "*%s*:(%ld) [%s] %s\n",
                prefix, pthread_self(), func, format);
#endif
#endif

#ifdef AEI_CISCO_HNAP
        vsnprintf(buffer2, sizeof(buffer2), buffer, ap);
#else
        vsprintf(buffer2, buffer, ap);
#endif

        if (!log_background)
        {
            printf("%s", buffer2);
            fflush(stdout);
        }

        if (hlog_get_file())
        {
#ifdef AEI_WECB_CUSTOMER_COMCAST
         if (stat(hlog_get_file(), &info) >= 0)
           {
            file_size=info.st_size;

            if(max_size){
                if(file_size>=max_size){
                    Is_set_full_logfile(1);
                    printf("warning ::log file is full ,please clear the file\n");
                    return ;
                    }
                }
            }
#endif
            f = fopen(hlog_get_file(), "a");
            if (!f)
                f = fopen(hlog_get_file(), "w");
            if (f)
            {
                fprintf(f, buffer2);
                fflush(f);
                fclose(f);
            }
        }
    }
}

void
hlog_verbose(const char *FUNC, int line, const char *format, ...)
{
  va_list ap;

  va_start(ap, format);

#ifdef AEI_SYSLOG
//    vsyslog(LOG_DEBUG | LOG_USER, format, ap);
#endif
  _log_write(HLOG_VERBOSE, "VERBOSE", FUNC, line,format, ap);
  va_end(ap);
}

void
hlog_debug(const char *FUNC, int line, const char *format, ...)
{
  va_list ap;

  va_start(ap, format);

#ifdef AEI_SYSLOG
    vsyslog(LOG_DEBUG | LOG_USER, format, ap);
#endif
  _log_write(HLOG_DEBUG, "DEBUG", FUNC, line,format, ap);
  va_end(ap);
}

void
hlog_info(const char *FUNC, int line, const char *format, ...)
{
  va_list ap;

  va_start(ap, format);

#ifdef AEI_SYSLOG
//    vsyslog(LOG_INFO | LOG_USER, format, ap);
#endif
  _log_write(HLOG_INFO, "INFO", FUNC, line, format, ap);
  va_end(ap);
}

void
hlog_warn(const char *FUNC, int line, const char *format, ...)
{
  va_list ap;

  va_start(ap, format);

#ifdef AEI_SYSLOG
    vsyslog(LOG_WARNING | LOG_USER, format, ap);
#endif
  _log_write(HLOG_WARN, "WARNING", FUNC, line, format, ap);
  va_end(ap);
}

void
hlog_error(const char *FUNC, int line, const char *format, ...)
{
  va_list ap;

  va_start(ap, format);

#ifdef AEI_SYSLOG
    vsyslog(LOG_ERR | LOG_USER, format, ap);
#endif
  _log_write(HLOG_ERROR, "ERROR", FUNC, line,format, ap);
  va_end(ap);
}

#ifdef AEI_CISCO_HNAP
void hnap_get_memstatus(const char *FUNC, int line, const char *infostr)
{
    struct mallinfo info = mallinfo();
    static int last_uordblks=0;

    printf("%s [%s:%d]  uordblks = %d, diff=%d\n", infostr? infostr:"", FUNC, line, info.uordblks, info.uordblks-last_uordblks);
    last_uordblks = info.uordblks;
}

#endif
