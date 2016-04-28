#ifndef _DEBUG_H
#define _DEBUG_H

#include "libbb_udhcp.h"

#include <stdio.h>

#ifdef AEI_BRCM_CORE
#include "cms.h"
#include "cms_eid.h"
#include "cms_util.h"
#include "ctl_log.h"
#else
#ifdef SYSLOG
#include <syslog.h>
#endif
#endif /* AEI_BRCM_CORE */

#ifdef AEI_BRCM_CORE
#define OPEN_LOG(name) do {;} while(0)
#define CLOSE_LOG() do {;} while(0)
#define LOG(level, str, args...) do{if(level==LOG_ERR) ctllog_error(str, ## args); else ctllog_debug(str, ## args);} while (0)

#else

#ifdef SYSLOG
# define LOG(level, str, args...) do { printf(str, ## args); \
				printf("\n"); \
				syslog(level, str, ## args); } while(0)
# define OPEN_LOG(name) openlog(name, 0, 0)
#define CLOSE_LOG() closelog()
#else
# define LOG_EMERG	"EMERGENCY!"
# define LOG_ALERT	"ALERT!"
# define LOG_CRIT	"critical!"
# define LOG_WARNING	"warning"
# define LOG_ERR	"error"
# define LOG_INFO	"info"
# define LOG_DEBUG	"debug"
# define LOG(level, str, args...) do { printf("%s, ", level); \
				printf(str, ## args); \
				printf("\n"); } while(0)
# define OPEN_LOG(name) do {;} while(0)
#define CLOSE_LOG() do {;} while(0)
#endif
#endif /* AEI_BRCM_CORE */

#ifdef VERBOSE
# define DEBUG(level, str, args...) LOG(level, str, ## args)
#else
# define DEBUG(level, str, args...) do {;} while(0)
#endif

#endif
