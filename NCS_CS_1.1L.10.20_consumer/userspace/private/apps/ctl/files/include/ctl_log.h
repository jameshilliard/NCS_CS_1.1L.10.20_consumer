#ifndef _CTL_LOG_H
#define _CTL_LOG_H

#include <sys/time.h>
#include <time.h>

#ifdef AEI_SYSLOG
#include <sys/syslog.h>
#endif

#if 0
#define SHOWDATE()	do { \
	time_t the_time; \
	tm *tm_ptr;  \
	(void) time(&the_time); \
	tm_ptr = localtime(&the_time); \
	fprintf( stdout, "[%02d:%02d:%02d] ", \
			tm_ptr->tm_hour,tm_ptr->tm_min,tm_ptr->tm_sec ); \
} while(0)
#else
#define SHOWDATE()	
#endif

enum CmsLogLevel{
    CTL_LOG_LEVEL_NULL,
    CTL_LOG_LEVEL_ERR,
    CTL_LOG_LEVEL_WARN,
    CTL_LOG_LEVEL_NOTICE,
    CTL_LOG_LEVEL_DEBUG
};

extern int CTL_LOG_LEVEL;

#ifdef AEI_SYSLOG
#define ctllog_info(format, args...)	do { \
                    syslog(LOG_INFO | LOG_DAEMON, format, ##args); \
				} while(0)

#define ctllog_debug(format, args...)	do { \
                    if(CTL_LOG_LEVEL < CTL_LOG_LEVEL_DEBUG)\
                        break;\
                    syslog(LOG_DEBUG | LOG_DAEMON, format, ##args); \
                    SHOWDATE(); \
                    fprintf(stdout,"DEBUG %s:%d ",__FILE__,__LINE__);\
                    fprintf(stdout,format, ##args);\
                    fprintf(stdout,"\n" );\
				} while(0)

#define ctllog_notice(format, args...)	do { \
                    if(CTL_LOG_LEVEL < CTL_LOG_LEVEL_NOTICE)\
                        break;\
                    syslog(LOG_NOTICE | LOG_DAEMON, format, ##args); \
                    SHOWDATE(); \
                    fprintf(stdout,"NOTICE %s:%d ",__FILE__,__LINE__);\
                    fprintf(stdout,format, ##args);\
                    fprintf(stdout,"\n" );\
				} while(0)

#define ctllog_warn(format, args...)	do { \
                    if(CTL_LOG_LEVEL < CTL_LOG_LEVEL_WARN)\
                        break;\
                    syslog(LOG_WARNING | LOG_DAEMON, format, ##args); \
                    SHOWDATE(); \
                    fprintf(stdout,"WARNING %s:%d ",__FILE__,__LINE__);\
                    fprintf(stdout,format, ##args);\
                    fprintf(stdout,"\n" );\
				} while(0)

#define ctllog_error(format, args...)	do { \
                    if(CTL_LOG_LEVEL < CTL_LOG_LEVEL_ERR)\
                        break;\
                    syslog(LOG_ERR | LOG_DAEMON, format, ##args); \
                    SHOWDATE(); \
                    fprintf(stdout,"WARNING %s:%d ",__FILE__,__LINE__);\
                    fprintf(stdout,format, ##args);\
                    fprintf(stdout,"\n" );\
				} while(0)
#else
#define ctllog_debug(arg...)	do { \
                    if(CTL_LOG_LEVEL < CTL_LOG_LEVEL_DEBUG)\
                        break;\
					SHOWDATE(); \
					fprintf(stdout,"DEBUG %s:%d ",__FILE__,__LINE__);\
					fprintf(stdout,##arg);\
					fprintf(stdout,"\n" );\
				} while(0)

#define ctllog_notice(arg...)	do { \
                    if(CTL_LOG_LEVEL < CTL_LOG_LEVEL_NOTICE)\
                        break;\
					SHOWDATE(); \
					fprintf(stdout,"NOTICE %s:%d ",__FILE__,__LINE__);\
					fprintf(stdout,##arg);\
					fprintf(stdout,"\n" );\
				} while(0)

#define ctllog_warn(arg...)	do { \
                    if(CTL_LOG_LEVEL < CTL_LOG_LEVEL_WARN)\
                        break;\
					SHOWDATE(); \
					fprintf(stdout,"WARNING %s:%d ",__FILE__,__LINE__);\
					fprintf(stdout,##arg);\
					fprintf(stdout,"\n" );\
				} while(0)

#define ctllog_error(arg...)	do { \
                    if(CTL_LOG_LEVEL < CTL_LOG_LEVEL_ERR)\
                        break;\
					SHOWDATE(); \
					fprintf(stdout,"ERROR %s:%d ",__FILE__,__LINE__);\
					fprintf(stdout,##arg);\
					fprintf(stdout,"\n" );\
				} while(0)
#endif

				
#define V6_DEBUG	

#define CTL_MODULE_NCS_CFG_SYNC 0X00000001
#define CTL_MODULE_MOCA 0X00000002
#define CTL_MODULE_WAN_UPGRADE 0X00000003
#define ALL_CTL_MODULE (CTL_MODULE_MOCA)
#define ALL_CTL_MODULE_UPGRDE (CTL_MODULE_WAN_UPGRADE)
#define ctldbg_module(module_mask ,arg...)        do { \
        if((!(module_mask & ALL_CTL_MODULE))||(!(module_mask & ALL_CTL_MODULE_UPGRDE)))\
            break;\
        SHOWDATE(); \
        fprintf(stdout,"##%s:%d :%s() ",__FILE__,__LINE__,__func__);\
        fprintf(stdout,##arg);\
        fprintf(stdout,"\n" );\
    } while(0)

#endif
