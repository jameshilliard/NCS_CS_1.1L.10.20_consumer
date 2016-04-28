#ifndef _WIFI_WPS_H_
#define _WIFI_WPS_H_

#define WPS_DAEMON_ADDR         "127.0.0.1"
#define WPS_DAEMON_PORT         12345

#define LEN_MAC_ADDR            17
#define UDP_EVENT_TYPE_WPS      1

#define WPS_FLAG_2G				"WPS_FLAG_2G"
#define WPS_FLAG_5G				"WPS_FLAG_5G"
#define WPS_FLAG_POLL			"POLL"
#define WPS_FLAG_ACTV			"ACTV"
#define WPS_FLAG_2G_PASSIVE		"WPS_FLAG_2G: POLL" //length must be 17
#define WPS_FLAG_5G_PASSIVE		"WPS_FLAG_5G: POLL" //length must be 17
#define WPS_FLAG_2G_ACTIVE		"WPS_FLAG_2G: ACTV" //length must be 17
#define WPS_FLAG_5G_ACTIVE		"WPS_FLAG_5G: ACTV" //length must be 17

#define WPS_FLAG_PBC_CLIENT		"WPS_PBC_CLIENT"
#define WPS_PBC_CLIENT_2G		"WPS_PBC_CLIENT_2G" //length must be 17
#define WPS_PBC_ACCESS_2G		"WPS_PBC_ACCESS_2G" //length must be 17
#define WPS_PBC_CLIENT_5G		"WPS_PBC_CLIENT_5G" //length must be 17
#define WPS_PBC_ACCESS_5G		"WPS_PBC_ACCESS_5G" //length must be 17
#define WPS_FLAG_IEEE1905		"WPS_FLAG_IEEE1905" //length must be 17


/*  UDP_EVENT_TYPE_WPS (for user <-> user)
    * for tlv_hdr->type==UDP_EVENT_TYPE_WPS,
    * tlv_hdr->subtype is wps event ID,
    * data[0:17]-BSSID, data[18:35]-station address.
    * [xx:xx:xx:xx:xx:xx\0]
    */
typedef enum _wps_event {
    WPS_STATE_DISABLED	= 0,
    WPS_STATE_READY		= 1,//enabled
	WPS_STATE_SUCCESS	= 2,
	WPS_STATE_ERROR		= 3,
	WPS_STATE_TIMEOUT	= 4,
	WPS_STATE_LOCKED	= 5,
	WPS_STATE_OVERLAPPED= 6,
    WPS_STATE_IN_PROGRESS
} wps_event;

int send_wps_event(wps_event event, char* bssid, char* addr_sta);

#endif


