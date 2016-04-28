

#ifndef __CTL_PARAMS_H__
#define __CTL_PARAMS_H__


/** Config file version.
 *
 */
#define CMS_CONFIG_FILE_VERSION "3.0"


/** Number of spaces to indent each line in the config file.
 *
 */
#define CMS_CONFIG_FILE_INDENT 2




/** This is the Unix Domain Socket address for communications with smd used
 *  by the messaging library.
 *
 * Note two different addresses are defined, one for modem and one for DESKTOP_LINUX testing.
 *  It is highly unlikely that this needs to be changed.
 */
#ifdef DESKTOP_LINUX
#define SMD_MESSAGE_ADDR  "/var/tmp/smd_messaging_server_addr"
#else
#define SMD_MESSAGE_ADDR  "/var/smd_messaging_server_addr"
#endif


/** This is the number of fully connected connections that can be queued
 *  up at the SMD message server socket.
 *
 *  It is highly unlikely that this needs to be changed.
 */
#define SMD_MESSAGE_BACKLOG  3


/** Special hack for the smd dynamic launch service, when it launches a server app, the
 *  server app will find its server fd at this number.
 *
 * It is highly unlikely that this needs to be changed.
 */
#define CMS_DYNAMIC_LAUNCH_SERVER_FD  3



/** This is the port ftpd listens on.
 * 
 * Note two different ports are defined, one for modem and one for DESKTOP_LINUX testing.
 * It is highly unlikely that this needs to be changed.
 */
#ifdef DESKTOP_LINUX
#define FTPD_PORT       44421
#else
#define FTPD_PORT       21
#endif


/** This is the port tftpd listens on.
 * 
 * Note two different ports are defined, one for modem and one for DESKTOP_LINUX testing.
 * It is highly unlikely that this needs to be changed.
 */
#ifdef DESKTOP_LINUX
#define TFTPD_PORT      44469
#else
#define TFTPD_PORT      69
#endif


/** This is the port sshd listens on.
 * 
 * Note two different ports are defined, one for modem and one for DESKTOP_LINUX testing.
 * It is highly unlikely that this needs to be changed.
 */
#ifdef DESKTOP_LINUX
#define SSHD_PORT       44422
#else
#define SSHD_PORT       22
#endif


/** The amount of idle time, in seconds, before sshd exits.
 *
 * Make this relatively long because the user might be configuring something,
 * then gets confused and have to look up some manual.
 * If 0, then no timeout.
 */
#define SSHD_EXIT_ON_IDLE_TIMEOUT  600


/** This is the port telnetd listens on.
 * 
 * Note two different ports are defined, one for modem and one for DESKTOP_LINUX testing.
 * It is highly unlikely that this needs to be changed.
 */
#ifdef DESKTOP_LINUX
#define TELNETD_PORT    44423
#else
#define TELNETD_PORT    23
#define TELNETSSLD_PORT    992
#endif


/** The amount of idle time, in seconds, before telnetd exits.
 *
 * Make this relatively long because the user might be configuring something,
 * then gets confused and have to look up some manual.
 * If 0, then no timeout.
 */
#define TELNETD_EXIT_ON_IDLE_TIMEOUT  600


/** This is the port httpd listens on.
 * 
 * Note two different ports are defined, one for modem and one for DESKTOP_LINUX testing.
 * It is highly unlikely that this needs to be changed.
 */
#ifdef DESKTOP_LINUX
#define HTTPD_PORT      44480
#else
#ifdef SUPPORT_HTTPD_SSL
#define HTTPD_PORT      443
#else
#define HTTPD_PORT      80
#endif
#endif

#define STUNNEL_PORT      443

/** The amount of idle time, in seconds, before httpd exits.
 *
 * Make this relatively long because the user might be configuring something,
 * then gets confused and have to look up some manual.
 */
#define CTL_HTTPD_EXIT_ON_IDLE_TIMEOUT  600


/** The amount of idle time, in seconds, before consoled exits.
 *
 * Make this relatively long because the user might be configuring something,
 * then gets confused and have to look up some manual.
 * If 0, then no timeout.
 */
#define CTL_CONSOLED_EXIT_ON_IDLE_TIMEOUT  600


/** This is the port snmpd listens on.
 * 
 * Note two different ports are defined, one for modem and one for DESKTOP_LINUX testing.
 * It is highly unlikely that this needs to be changed.
 */
#ifdef DESKTOP_LINUX
#define SNMPD_PORT      44161
#else
#define SNMPD_PORT      161
#endif

/** This is the port tr64c listens on.
* LGD_TODO: Due to the time limit, it still have one DESKTOP_LINUX version TR64C, 
* in the future will add it.
*/
#define TR64C_HTTP_CONN_PORT     49431


/** This is the port tr69c listens on for connection requests from the ACS.
 * 
 */
#ifdef ACTION_TEC_VERIZON
#define TR69C_CONN_REQ_PORT      4567
#else
#define TR69C_CONN_REQ_PORT      30005
#endif


/** This is the path part of the URL for tr69c connection requests from the ACS.
 * 
 */
#define TR69C_CONN_REQ_PATH      "/"


/** The amount of idle time, in seconds, before tr69c exits.
 *
 * This value does not need to be very large because the ACS is usually running
 * a script so it will do all the actions it needs back-to-back and then be
 * completely done.  So if we see no more requests from the ACS for 30 seconds,
 * that probably means the ACS is completely done.  However, if response time
 * is very important, and you do not want the tr69c client to exit, then you
 * can set this to a very large value (e.g. 2160356, which is one year).
 */
#define TR69C_EXIT_ON_IDLE_TIMEOUT       30 


/** Maximum number of Layer 2 bridges supported.
 * 
 * If this value is changed, be sure to also modify the default value in
 * the data model.
 */
#define MAX_LAYER2_BRIDGES                16


/** Maximum depth of objects in the Data Model that we can support.
 *  If the data model has a greater actual depth than what is defined here,
 *  cmsMdm_init() will fail.
 */
#define MAX_INSTANCE_DEPTH    6


/** Maximum length of a parameter name in the Data Model that we can support.
 *  If the data model has a greater actual param name length than what is defined here,
 *  cmsMdm_init() will fail.
 */
#define MAX_PARAM_NAME_LENGTH   55

/** DNS Probing parameters for both dnsprobe and dproxy. They probe every
 * 30 seconds. Timeout is 3 seconds and only retry 2 more times. */
#define CTL_DNS_PROBE_INTERVAL 30
//#define CTL_DNS_PROBE_TIMEOUT 3 
#define CTL_DNS_PROBE_TIMEOUT 2 
#define CTL_DNS_PROBE_MAX_TRY 3



#define TIMEOUT_PRIMARY	2
#define TIMEOUT_SECONDARY 2
#define MAX_NUM_OF_PRIMARY_REQ 3
#define MAX_NUM_OF_SEC_REQ 3
#define FAIL_BACK_INTERVAL 120
#define NORMAL_STATE 1
#define FAIL_BACK_STATE 2

/* ACTION_TEC define rule type for DSCPMatchingObj */
#define RULE_TYPE_WMM 0
#define RULE_TYPE_DSCPMATCHING 1


#endif  /* __CTL_PARAMS_H__ */
