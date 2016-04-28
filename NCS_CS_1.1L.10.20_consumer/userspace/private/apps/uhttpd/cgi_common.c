
#include "cgi_common.h"

/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *          Common Functions 
 *           
 * 
 **************************************************************************/
void trim(char *s)
{
    int len = tsl_strlen(s);
    int lws;

    /* trim trailing whitespace */
    while (len && isspace(s[len-1]))
        --len;

    /* trim leading whitespace */
    if (len) {
        lws = strspn(s, " \n\r\t\v");
        if (lws) {
            len -= lws;
            memmove(s, s + lws, len);
        }
    }
    s[len] = '\0';
}

