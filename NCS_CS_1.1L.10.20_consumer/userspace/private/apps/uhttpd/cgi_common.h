
#ifndef CGI_COMMON_H
#define CGI_COMMON_H

#include "uhttpd.h"
#include "uhttpd-utils.h"
#include "cgi_main.h"

#define kprintf(format, arg...)              \
        do{\
            fprintf(stdout,"[%s] [%s] ", __FILE__, __FUNCTION__);\
            fprintf(stdout, format, ##arg);\
        }while(0)        

#define CGI_DAL
#define CGI_MESSAGE_KK

void trim(char *s);

#endif

