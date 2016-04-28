#ifndef _NL_KERNEL_H_
#define _NL_KERNEL_H_

//#define NETLINK_USER 28 //defined in linux/netlink.h

typedef struct _tlv_hdr {
    unsigned char type;
    unsigned char subtype;
    unsigned short len;
    unsigned char data[0];
} tlv_hdr;

typedef enum _nl_type {
    NL_TYPE_GROUPS,
    NL_TYPE_BUTTON,
    NL_TYPE_LED,
    NL_TYPE_WIFI_EVENT
} nl_type;
/*  NL_TYPE_GROUPS (only for user -> kernel)
    * for tlv_hdr->type==NL_TYPE_GROUPS,
    * tlv_hdr->subtype is the groups id,
    * len is 0 and data would be ignored.
    */

/*  NL_TYPE_BUTTON (only for user <- kernel)
    * for tlv_hdr->type==NL_TYPE_BUTTON,
    * tlv_hdr->subtype is button ID,
    * len is 1 and data[0] would be button state.
    */

typedef enum _nl_button_id {
    NL_BUTTON_ID_RESET,
    NL_BUTTON_ID_WPS,
    NL_BUTTON_COUNT
} nl_button_id;

typedef enum _nl_button_state {
    NL_BUTTON_KEYUP,
    NL_BUTTON_KEYDOWN
} nl_button_state;

/*  NL_TYPE_LED (only for user -> kernel)
    * for tlv_hdr->type==NL_TYPE_LED,
    * tlv_hdr->subtype is the led ID,
    * data[0] is the led state,
    * for NL_LED_OFF and NL_LED_ON
    * len is 1 and more data would be ignored.
    * for NL_LED_BLINK, data[1:4]-led on interval in ms, data[5:8]-led off interval in ms
    * data[9:12]-led blink cycles, for NL_LED_BLINK_STOP, data[13:16]-led stop(off) interval in ms
    */

typedef enum _nl_led_id {
    NL_LED_ID_WPS_GREEN,
    NL_LED_ID_WPS_RED,
    NL_LED_ID_2G,
    NL_LED_ID_5G,
    NL_LED_COUNT
} nl_led_id;

typedef enum _nl_led_state {
    NL_LED_OFF,
    NL_LED_ON,
    NL_LED_BLINK,       //on-off-on-off-...
    NL_LED_BLINK_STOP   //blink-stop, blink-stop, ...
} nl_led_state;

/*  NL_TYPE_WIFI_EVENT (only for user <- kernel)
    * for tlv_hdr->type==NL_TYPE_WIFI_EVENT,
    * tlv_hdr->subtype is wifi event ID,
    * data[0:5]-BSSID, data[6:11]-station address.
    */
typedef enum _nl_wifi_event {
	NL_WIFI_RESERVED0,
	NL_WIFI_MGMT_AUTH_REQ,
	NL_WIFI_MGMT_AUTH_RESP,
	NL_WIFI_MGMT_ASSOC_REQ,
	NL_WIFI_MGMT_ASSOC_RESP,//CONNECTED
	NL_WIFI_MGMT_DISASSOC, 	//DISCONNECTED
	NL_WIFI_MGMT_DEAUTH,
	NL_WIFI_DATA_XXXX,
	NL_WIFI_CTRL_XXXX,
	NL_WIFI_RESERVED
} nl_wifi_event;

typedef struct _wifi_event_info {
    unsigned char 	bssid[6];
    unsigned char 	staAddr[6];
    unsigned short 	event;
    unsigned short	status;
} wifi_event_info;

typedef enum _led_state {
    LED_STATE_OFF,
    LED_STATE_ON
} led_state;

typedef struct _led_op_cb {
    void (*set_led_gpio)(unsigned int led_id, unsigned int on);
} led_op_cb;

typedef struct _button_op_cb {
    nl_button_state (*get_button_gpio)(unsigned int button_id);
} button_op_cb;

void act_set_led_callback(led_op_cb* op_cb);
void act_set_button_callback(button_op_cb* op_cb);
void act_button_notify(nl_button_id button_id, nl_button_state button_state);
void act_wifi_notify(wifi_event_info* event_info);
void wlan_set_2g_led(unsigned int mode, unsigned int on_ms, unsigned int off_ms, unsigned int cycles);
void wlan_set_5g_led(unsigned int mode, unsigned int on_ms, unsigned int off_ms, unsigned int cycles);

#endif //_NL_KERNEL_H_
