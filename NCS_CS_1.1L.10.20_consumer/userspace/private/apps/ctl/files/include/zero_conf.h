/*
 *      Include file of zero_conf.c
 *
 *      Authors: Jerome Zhao 	<jzhao@actiontec.com>
 *
 *      This file is for zero config
 *
 */


#ifndef __ZERO_CONF_H__
#define __ZERO_CONF_H__

/*Interval seconds between retry*/
#define INTERVAL_TELUS    30
#define MAX_TRY_COUNTER 3

#define INTERVAL_VZ    300

/*Define for Auto Sync Wireless settings*/
#define XML_FILE_TELUS    "/tmp/get_parameters.telus.xml"
#define XML_FILE_VZ    "/tmp/get_parameters.vz.xml"


#define XML_INPUT_FILE "wirelesssettings.xml"
#define SUCCESS_FILE "wirelesssync_successful.xml"
#define FAIL_FILE "wirelesssync_Failure.xml"

/* Define for upgrade*/
#define UPGRADE_INPUT_XML "firmwarelinks.xml"
#define UPGRADE_LOCAL_XML "/tmp/upgrade_url.xml"
#define UPGRADE_DOWNLOAD "/tmp/upgrade.bin"

/* Product type*/
#define PRODUCT_TYPE "WXB6000"
/*Vendor ID, WP used this to identify that is a actiontec peripheral*/
#define VENDOR_ID "ACTIONTEC_WP"
/*Protocal version*/
#define PROTOCOL_VERSION "1.0"


//////////////////////
// Moved from zero_conf.c

#define WIRELESS_STATUS_OFF_TO_ON(index) ((0 == g_old_wireless_status[index]) && (1 == g_new_wireless_status[index]))

typedef enum {
    WGET_NO_PROBLEM = 0,
    WGET_GENERIC_ERROR,
    WGET_PARSE_ERROR,
    WGET_FILE_IO_ERROR,
    WGET_NETWORK_FAILURE,
    WGET_SSL_VERIFY_FAILURE,
    WGET_USER_PWD_AUTH_FAILURE,
    WGET_PROTOCOL_ERROR,
    WGET_SERVER_ISSUE_ERROR_RESPONSE,
} WGET_RV;

#define SERVER_ERROR_RESPONSE -2
#define FAILED  -1
#define SUCCESS   0

#define ON  1
#define OFF 0

#if 0 
#define DEBUGP(format, args...) fprintf(stderr, format, ## args)
#else
#define DEBUGP(format, args...)
#endif

#ifdef AEI_TELUS_ZERO_CONF
struct upgrade_conf {
    char name[32];
    char value[1024];
};
#endif

//mapping for struct wifi_conf wecb_wifi[]
#define WIRELESS_STATUS     0 
#define SECURITY_TYPE       1
#define AUTH_TYPE           2
#define WEP_KEY_LEN         3
#define SECURITY_KEY        4
#define AEI_SSID                5 
#define BROADCAST_SSID      6
#define CHANNEL             7
#define WIRELESS_MODE       8
#define ACL_ENABLE          9
#define ACL_POLICY          10
#define ACL_NUM             11
#define ACL_MAC_LIST        12
#define SECURITY_ENCRYPTION_MODE      13
#define ERROR_NO            14
#define ERROR_DESC          15

#define ENABLE_BROADCAST_SSID "Yes"

#define CHANNEL_AUTO 0
#define CHANNEL_1 1
#define CHANNEL_11 11
#define CHANNEL_36 36
#define CHANNEL_149 149
#define OID_RADIO_1_AUTO_CHANNEL_ENABLE "Device.WiFi.Radio.1.AutoChannelEnable"
#define OID_RADIO_2_AUTO_CHANNEL_ENABLE "Device.WiFi.Radio.1.AutoChannelEnable"

#if defined(AEI_WECB_CUSTOMER_TELUS)
#define WIFI_SSID_DIX_2G  "-2.4G"
#define WIFI_SSID_DIX_5G  "-5G"
#elif defined(AEI_WECB_CUSTOMER_NCS_CS)
#define WIFI_SSID_DIX_2G  "-EXT"
#define WIFI_SSID_DIX_5G  "-5G-EXT"
#else
#define WIFI_SSID_DIX_2G  ""
#define WIFI_SSID_DIX_5G  "-5G"
#endif

#define SIZE_32             32

//#if 0
// According to commit 4e11d92a0c1a07fa826cefa19d14e84c8cd7b716.
// Delete redefinition of wifi_conf and upgrade_conf
typedef struct wifi_conf_tag {
    char name[32];
    char oid[128];
    char value[8][18*32]; // ssid 1-8 , Wireless MAC Authentication support for each SSID with a maximum of 32 entries in the filtering list ,modified by libby
    int changed[8]; //0: no config 1: config change: 2 config no change
} wifi_conf;

struct upgrade_conf {
    char name[32];
    char value[1024];
};
//#endif

#define SECURITY_WPA    "WPA"
#define SECURITY_WPA2   "WPA2"
#define SECURITY_BOTH   "WPA-WPA2"
#define SECURITY_WPA_OID    "Device.WiFi.AccessPoint.%d.Security.X_ACTIONTEC_COM_WPAEncryptionMode"
#define SECURITY_WPA2_OID   "Device.WiFi.AccessPoint.%d.Security.X_ACTIONTEC_COM_WPA2EncryptionMode"

#define SECURITY_WPA    "WPA"
#define SECURITY_WPA2   "WPA2"
#define SECURITY_BOTH   "WPA-WPA2"
#define SECURITY_WPA_OID    "Device.WiFi.AccessPoint.%d.Security.X_ACTIONTEC_COM_WPAEncryptionMode"
#define SECURITY_WPA2_OID   "Device.WiFi.AccessPoint.%d.Security.X_ACTIONTEC_COM_WPA2EncryptionMode"

///////
#define WECB_WIFI_PARAS_NUM ((sizeof(wecb_wifi)/sizeof(wifi_conf)))

struct wireless_mode_map{
    char router_mode[128];
    char wecb_mode[32];
};

/*
Compatibility:    b,g,n
Balanced:          g,n
Performance:    n
Legacy/Mixed:  b,g
802.11g:           g
802.11b:           b
*/

///////
#define WIRELESS_MODE_MAPS_SIZE (sizeof(wireless_mode_maps)/sizeof(struct wireless_mode_map))

///////
#define ROUTER_TO_SNIFF "ToSniff"
#define ROUTER_TELUS    "telus"
#define ROUTER_VZ "vz"

#define IS_SNIFF_ROUTER_TYPE()   (!tsl_strcmp(connected_router_type, ROUTER_TO_SNIFF))
#define IS_CONNECTED_TO_TELUS() (!tsl_strcmp(connected_router_type, ROUTER_TELUS))
#define IS_CONNECTED_TO_VZ()    (!tsl_strcmp(connected_router_type, ROUTER_VZ))
#define ROUTER_TYPE_TR_PATH     "Device.X_ACTIONTEC_COM_ZeroConf.Extender.X_ACTIONTEC_COM_RouterType"


/////////////////////////////
// Declaration
extern wifi_conf wecb_wifi[];

#endif
