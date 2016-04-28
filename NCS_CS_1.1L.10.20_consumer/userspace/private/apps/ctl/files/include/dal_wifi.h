#ifndef DAL_WIFI_H
#define DAL_WIFI_H


/**************************************************************************
 *  WiFi Interface
 **************************************************************************/
#define SIZE_4      4
#define SIZE_5      5
#define SIZE_10     10
#define SIZE_16     16
#define SIZE_20     20
#define SIZE_32     32
#define SIZE_64     64
#define SIZE_256	256
#define SIZE_1024	1024

#define STR_BANDWIDTH_20        "20MHz"
#define STR_BANDWIDTH_40        "40MHz"
#define STR_BANDWIDTH_80        "80MHz"
#define STR_BANDWIDTH_160       "160MHz"
#define STR_BANDWIDTH_D80       "80+80MHz"

typedef enum _band_width_t {
    BAND_WIDTH_AUTO,
    BAND_WIDTH_20MHZ    = (0x1 << 0),
    BAND_WIDTH_40MHZ    = (0x1 << 1),
    BAND_WIDTH_80MHZ    = (0x1 << 2),
    BAND_WIDTH_160MHZ   = (0x1 << 3),
    BAND_WIDTH_D80MHZ   = (0x1 << 4)
} band_width_t;

//001
typedef struct _ssid_info_t {
    char ifname[SIZE_16];
    char ssid[SIZE_32+1];
    unsigned int index_vlan;
    unsigned int index_ssid;
    unsigned int is5g   : 1;
} ssid_info_t;

#ifndef CTLMEM_FREE_BUF_AND_NULL_PTR
#define CTLMEM_FREE_BUF_AND_NULL_PTR(p) \
   do { \
         if ((p) != NULL) {free((p)); (p) = NULL;}   \
       } while (0)
#endif
/*  001
    function            : dal_wifi_get_ssid_list - Get the ssid list
    parameter           : 
        plist   : Pointer to a buffer receiving ssid info
        size    : Max count can be receved with plist
    return value        : sccess return count of valid ssids, otherwise return error
*/
int dal_wifi_get_ssid_list(ssid_info_t* plist, unsigned int size);

/*  002 - SSIDEnable
    function    : dal_wifi_get_SSIDEnable
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : 0 - disable, otherwise - enable
*/
unsigned int dal_wifi_get_SSIDEnable(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_SSIDEnable(unsigned int index_vlan, unsigned int index_ssid, unsigned int enable);


/*  003
    function    : dal_wifi_get_ssid
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    ssid        : [out] the buffer used to receive ssid
    size        : [in] size of buffer
    return value : 0 - success, otherwise - fail
*/
int dal_wifi_get_ssid(unsigned int index_vlan, unsigned int index_ssid, char* ssid, const unsigned int size);
int dal_wifi_set_ssid(unsigned int index_vlan, unsigned int index_ssid, char* ssid);

/*  004
    function    : dal_wifi_get_channel
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    primary_channel: [out] pointer to a buffer to receive primary channel number
    bandWidth   : [out] enum band_width_t
    above       : [out] 2nd channel is above primary channel or not when primary channel is between 5 and 9.
    return value : 0 on success, otherwise -1.
*/
int dal_wifi_get_channel(unsigned int index_vlan, unsigned int index_ssid,
    unsigned int *primary_channel, unsigned int * bandWidth, unsigned int *above);
int dal_wifi_set_channel(unsigned int index_vlan, unsigned int index_ssid,
    unsigned int primary_channel, band_width_t bandWidth, unsigned int above);


/*  005
    function    : dal_wifi_get_AutoChannelEnable
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : 0 - manual, 1 - auto
*/
unsigned int dal_wifi_get_AutoChannelEnable(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_AutoChannelEnable(unsigned int index_vlan, unsigned int index_ssid, unsigned int AutoChannelEnable);

/*
    function    : dal_wifi_get_AutoChannelBootOnly
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : 0 - boot up and running config, 1 - boot up only
*/
unsigned int dal_wifi_get_AutoChannelBootOnly(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_AutoChannelBootOnly(unsigned int index_vlan, unsigned int index_ssid, unsigned int AutoChannelBootOnly);

typedef enum _security_mode_t {
    SECURITY_MODE_OFF,
    SECURITY_MODE_WEP64,
    SECURITY_MODE_WEP128,
    SECURITY_MODE_PSK_WPA,
    SECURITY_MODE_PSK_WPA2,
    SECURITY_MODE_PSK_WPAWPA2,
    SECURITY_MODE_EAP_WPA,
    SECURITY_MODE_EAP_WPA2,
    SECURITY_MODE_EAP_WPAWPA2,
} security_mode_t;
/*  006 - security_mode
    function    : dal_wifi_get_security_mode
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : 0 - off, 1 - wpa, 2 - wpa2, 3 - wpa/wpa2, 4 - wep
*/
security_mode_t dal_wifi_get_security_mode(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_security_mode(unsigned int index_vlan, unsigned int index_ssid, security_mode_t security_mode);

/*  007 - RadioEnabled
    function    : dal_wifi_get_RadioEnabled
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : 0 - disable, otherwise - enable
*/
unsigned int dal_wifi_get_RadioEnabled(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_RadioEnabled(unsigned int index_vlan, unsigned int index_ssid, unsigned int enable);

/*  008 - SSIDAdvertisementEnabled
    function    : dal_wifi_get_SSIDAdvertisementEnabled
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : 0 - disable, otherwise - enable
*/
unsigned int dal_wifi_get_SSIDAdvertisementEnabled(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_SSIDAdvertisementEnabled(unsigned int index_vlan, unsigned int index_ssid, unsigned int enable);

typedef enum _acl_policy_t {
    ACL_DISABLE,
    ACL_ALLOW,
    ACL_DENY
} acl_policy_t;
typedef struct _acl_info_t {
    acl_policy_t policy;
    unsigned int count;
    char mac[SIZE_32][SIZE_20];//mac addrs: 00:11:22:33:44:55\0
} acl_info_t;
/*  009 - ACL
    function    : dal_wifi_get_ACL
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    acl         : access control list info
    return value : 0 - success, other - error
*/
int dal_wifi_get_ACL(unsigned int index_vlan, unsigned int index_ssid, acl_info_t* acl);
int dal_wifi_set_ACL(unsigned int index_vlan, unsigned int index_ssid, acl_info_t* acl);

typedef enum _ieee80211_standard_t {
    IEEE80211_AUTO,
    IEEE80211_A     = (0x1 << 0),
    IEEE80211_B     = (0x1 << 1),
    IEEE80211_G     = (0x1 << 2),
    IEEE80211_N     = (0x1 << 3),
    IEEE80211_AC    = (0x1 << 4)
} ieee80211_standard_t;
/*  010 - Standard
    function    : dal_wifi_get_standard
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : 0 - auto, 1 - 802.11a, 2 - 802.11b, 4 - 802.11g, 8 - 802.11n, 16 - 802.11ac or their bitwise-or combination
*/
ieee80211_standard_t dal_wifi_get_standard(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_standard(unsigned int index_vlan, unsigned int index_ssid, ieee80211_standard_t standard);

/*  011 - TransmitPower
    function    : dal_wifi_get_TransmitPower
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : TransmitPower (percent based 100)
*/
unsigned int dal_wifi_get_TransmitPower(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_TransmitPower(unsigned int index_vlan, unsigned int index_ssid, unsigned int TransmitPower);

/*  012 - BeaconInterval
    function    : dal_wifi_get_BeaconInterval
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : BeaconInterval
*/
unsigned int dal_wifi_get_BeaconInterval(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_BeaconInterval(unsigned int index_vlan, unsigned int index_ssid, unsigned int BeaconInterval);

/*  013 - DTIMInterval
    function    : dal_wifi_get_DTIMInterval
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : DTIMInterval
*/
unsigned int dal_wifi_get_DTIMInterval(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_DTIMInterval(unsigned int index_vlan, unsigned int index_ssid, unsigned int DTIMInterval);

/*  014 - FragmentationThreshold
    function    : dal_wifi_get_FragmentationThreshold
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : FragmentationThreshold
*/
unsigned int dal_wifi_get_FragmentationThreshold(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_FragmentationThreshold(unsigned int index_vlan, unsigned int index_ssid, unsigned int FragmentationThreshold);

/*  015 - RTSThreshold
    function    : dal_wifi_get_RTSThreshold
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : RTSThreshold
*/
unsigned int dal_wifi_get_RTSThreshold(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_RTSThreshold(unsigned int index_vlan, unsigned int index_ssid, unsigned int RTSThreshold);

/*  016 - WMMEnable
    function    : dal_wifi_get_WMMEnable
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : 0 - disable, otherwise - enable
*/
unsigned int dal_wifi_get_WMMEnable(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_WMMEnable(unsigned int index_vlan, unsigned int index_ssid, unsigned int enable);

/*  017 - UAPSDEnable
    function    : dal_wifi_get_UAPSDEnable
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : 0 - disable, otherwise - enable
*/
unsigned int dal_wifi_get_UAPSDEnable(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_UAPSDEnable(unsigned int index_vlan, unsigned int index_ssid, unsigned int enable);

/*  018 - commit
    function    : dal_wifi_set_commit
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : 0 - success, otherwise - fail
*/
int dal_wifi_set_commit(unsigned int index_vlan, unsigned int index_ssid);

typedef struct _wifi_stats_t{
    unsigned int tx_total_bytes;
    unsigned int rx_total_bytes;
    unsigned int tx_total_packets;
    unsigned int rx_total_packets;
    unsigned int tx_errors;
    unsigned int rx_errors;
    unsigned int tx_unicast;
    unsigned int rx_unicast;
    unsigned int tx_discard;
    unsigned int rx_discard;
    unsigned int tx_multicast;
    unsigned int rx_multicast;
    unsigned int tx_broadcast;
    unsigned int rx_broadcast;
    unsigned int rx_unknown;
}wifi_stats_t;

/*  019 - Stats
    function    : dal_wifi_get_stats
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    stats       : [out] Pointer to buffer to recv stats info
    return value : 0 on success, fail return -1.
*/
int dal_wifi_get_stats(unsigned int index_vlan, unsigned int index_ssid, wifi_stats_t* stats);

/*  020 - PossibleDataTransmitRates
    function    : dal_wifi_get_PossibleDataTransmitRates
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    buf       : [out] Pointer to buffer to recv rates list
    len         : [in] max length of the buffer
    return value : 0 on success, fail return -1.
*/
int dal_wifi_get_PossibleDataTransmitRates(unsigned int index_vlan, unsigned int index_ssid, char* buf, unsigned int len);

typedef enum _cts_protection_mode_t {
    CTS_PROTECT_MODE_NONE,
    CTS_PROTECT_MODE_CTSONLY,
    CTS_PROTECT_MODE_RTSCTS
} cts_protection_mode_t;
/*  021 - CTSProtectMode
    function    : dal_wifi_get_CTSProtectMode
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : 2 - rtscts, 1 - cts only, other - none
*/
cts_protection_mode_t dal_wifi_get_CTSProtectMode(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_CTSProtectMode(unsigned int index_vlan, unsigned int index_ssid, cts_protection_mode_t CTSProtectMode);

/*  022 - AMPDU
    function    : dal_wifi_get_AMPDU
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : 0 - disable, otherwise - enable
*/
unsigned int dal_wifi_get_AMPDU(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_AMPDU(unsigned int index_vlan, unsigned int index_ssid, unsigned int enable);

/*  023 - AMSDU
    function    : dal_wifi_get_AMSDU
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : 0 - disable, otherwise - enable
*/
unsigned int dal_wifi_get_AMSDU(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_AMSDU(unsigned int index_vlan, unsigned int index_ssid, unsigned int enable);

/*  024 - ShortGuardInterval
    function    : dal_wifi_get_ShortGuardInterval
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : 0 - disable, otherwise - enable
*/
unsigned int dal_wifi_get_ShortGuardInterval(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_ShortGuardInterval(unsigned int index_vlan, unsigned int index_ssid, unsigned int enable);

/*  025 - TxRate
    function    : dal_wifi_get_TxRate
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : TxRate, unit: Mbps, 0 means Auto
*/
unsigned int dal_wifi_get_TxRate(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_TxRate(unsigned int index_vlan, unsigned int index_ssid, unsigned int TxRate);

typedef struct _psk_data_t {
    char psk[SIZE_64+1];
    unsigned int type;//0 - Hex, 1 - ASCII
} psk_data_t;
typedef struct _wpa_encrypt_mode_t {
    unsigned int tkip   :1;
    unsigned int aes    :1;
} wpa_encrypt_mode_t;
typedef struct _wpa_auth_mode_t {
    unsigned int psk   :1;
    unsigned int eap   :1;
} wpa_auth_mode_t;

typedef struct _wpa_data_t {
    unsigned int key_index; //based 0
    unsigned int group_key_update_interval;
    unsigned int eap_port;
    unsigned int wpa : 1;
    unsigned int wpa2 : 1;
    wpa_encrypt_mode_t wpa_encrypt_mode;
    wpa_encrypt_mode_t wpa2_encrypt_mode;
    wpa_auth_mode_t wpa_auth_mode;
    wpa_auth_mode_t wpa2_auth_mode;
    psk_data_t key[SIZE_10];
    char eap_server[SIZE_20];
    char eap_secret[SIZE_64+1];
} wpa_data_t;
/*  026 - wpa
    function    : dal_wifi_get_wpa
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    data        : [out] pointer to wpa data buffer
    return value : 0 - success, non-zero - fail
*/
int dal_wifi_get_wpa(unsigned int index_vlan, unsigned int index_ssid, wpa_data_t* data);
int dal_wifi_set_wpa(unsigned int index_vlan, unsigned int index_ssid, wpa_data_t* data);

typedef struct _wep_auth_mode_t {//eap mode when both open and share are 0
    unsigned int open   :1;
    unsigned int share   :1;
} wep_auth_mode_t;
typedef struct _wep_data_t {
    unsigned int key_index; //based 0
    unsigned int eap_port;
    wep_auth_mode_t wep_auth_mode;
    psk_data_t key[SIZE_4];
    char eap_server[SIZE_20];
    char eap_secret[SIZE_64+1];
} wep_data_t;
/*  027 - wep
    function    : dal_wifi_get_wep
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    data        : [out] pointer to wep data buffer
    return value : 0 - success, non-zero - fail
*/
int dal_wifi_get_wep(unsigned int index_vlan, unsigned int index_ssid, wep_data_t* data);
int dal_wifi_set_wep(unsigned int index_vlan, unsigned int index_ssid, wep_data_t* data);

/*
PushButton  - PBC
Label       - use default pin
Display     - use random pin
Keypad      - set station pin
*/
typedef enum _wps_method_t {
    WPS_PBC,
    WPS_AP_STATIC_PIN,
    WPS_AP_RANDOM_PIN,
    WPS_DEVICE_PIN
} wps_method_t;

typedef enum _wps_pin_type_t {
    PIN_STATIC = 1,
    PIN_RANDOM,
    PIN_DEVICE
} wps_pin_type_t;

typedef struct _wps_data_t {
    unsigned int enable : 1;
    unsigned int pin;
    wps_method_t method;
} wps_data_t;
/*  028 - wps
    function    : dal_wifi_get_wps
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    data        : [out] pointer to wps data buffer
    return value : 0 - success, non-zero - fail
*/
int dal_wifi_get_wps(unsigned int index_vlan, unsigned int index_ssid, wps_data_t* data);
int dal_wifi_set_wps(unsigned int index_vlan, unsigned int index_ssid, wps_data_t* data);

/*  029 - channel list
    function    : dal_wifi_get_possible_channels
    parameter   :
    is5g        : [in] Freqency band is 5g or not
    bw          : [in] bandwidth
    channels    : [out] pointer to buffer receiving channel list
    size        : [in] the max count of channels can be received
    return value : 0 - success, non-zero - fail
*/
int dal_wifi_get_possible_channels(unsigned int index_vlan, unsigned int index_ssid, unsigned int is5g, band_width_t bw, int* channels, const unsigned int size);

typedef struct _wlan_sta_info {
    char   addr[SIZE_20];
    unsigned int    auth_state;
    unsigned long   tx_rate;//unit: Mbps
    unsigned long   rx_rate;//unit: Mbps
    unsigned long   tx_packets;
    unsigned long   rx_packets;
    unsigned long   tx_fail;
	int	            rssi;			// signal strength, unit: dBm
	int	            snr;			// link_quality
} wlan_sta_info;
/*  030 - associated device info
    function    : dal_wifi_get_associated_station_info
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    sta_info    : [in/out]    buffer used to receive station info
    max_count   : [in] max count of station info elements
    return value : associated device count on success, -1 on failure
*/
int dal_wifi_get_associated_station_info(unsigned int index_vlan, unsigned int index_ssid, wlan_sta_info* sta_info, unsigned int max_count);

typedef enum _spectrum_mgmt_mode {
    SPECT_MGMT_OFF,
    SPECT_MGMT_LOOSE_11H,
    SPECT_MGMT_STRICT_11H,
    SPECT_MGMT_STRICT_11D,
    SPECT_MGMT_LOOSE_11HD
} spectrum_mgmt_mode;
/*  031 - spectrum_management_mode
    function    :
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    mode        : [in] spectrum_management_mode
    return value :  for set:    0 on success, -1 on failure
                    for get:    spectrum_management_mode
*/
int dal_wifi_get_spectrum_management_mode(unsigned int index_vlan, unsigned int index_ssid);
int dal_wifi_set_spectrum_management_mode(unsigned int index_vlan, unsigned int index_ssid, spectrum_mgmt_mode mode);

/*  032 - tpc_mode
    function    :
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    mode        : [in] tpc_mode, bit0-BSS power control, bit1-AP power control
    return value :  for set:    0 on success, -1 on failure
                    for get:    tpc_mode
*/
int dal_wifi_set_tpc_mode(unsigned int index_vlan, unsigned int index_ssid, unsigned int mode);
int dal_wifi_get_tpc_mode(unsigned int index_vlan, unsigned int index_ssid);

typedef enum _dfs_state_t{
    DFS_IDLE        =0,   /* state for operating in non-radar channel */
    DFS_PREISM_CAC  =1,   /* CAC in progress */
    DFS_ISM         =2,   /* ISM in progress */
    DFS_CSA         =3,   /* csa */
    DFS_POSTISM_CAC =4,   /* ISM CAC */
    DFS_PREISM_OOC  =5,   /* PREISM OOC */
    DFS_POSTISM_OOC =6,   /* POSTISM OOC */
    DFS_STATES_COUNT      /* this many states exist */
}dfs_state_t;
/*  033 - dfs_state
    function    : dal_wifi_get_dfs_state
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    mode        : [in] tpc_mode, bit0-BSS power control, bit1-AP power control
    return value : dfs_state
*/
dfs_state_t dal_wifi_get_dfs_state(unsigned int index_vlan, unsigned int index_ssid);

/*  034 - tpc_txpower
    function    : dal_wifi_get_tpc_txpower
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    txpower     : [out] pointer to buffer receiving txpower, unit 0.01dBm
    count        : [in] the max count of txpower can be received
    return value : txpower count on success, -1 on fail
*/
int dal_wifi_get_tpc_txpower(unsigned int index_vlan, unsigned int index_ssid, int* txpower, unsigned int count);

/*  035 - ResetChannelWhenBoot
    function    : dal_wifi_get_tpc_txpower
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : ResetChannelWhenBoot, 0-disabled, 1 - enabled
*/
int dal_wifi_set_ResetChannelWhenBoot(unsigned int index_vlan, unsigned int index_ssid, unsigned int enabled);
unsigned int dal_wifi_get_ResetChannelWhenBoot(unsigned int index_vlan, unsigned int index_ssid);

//036 - Multiple SSID
int dal_wifi_set_mbss(unsigned int enabled);
unsigned int dal_wifi_get_mbss(void);

/*037 - SignalLevel
    function    : dal_wifi_get_signal_level
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    return value : signal level, unit dBm.
*/
int dal_wifi_get_signal_level(unsigned int index_vlan, unsigned int index_ssid);

/*038 - MACAddress
    function    : dal_wifi_get_mac_address
    parameter   :
    index_vlan  : [in] from wifi_get_ssid_list
    index_ssid  : [in] from wifi_get_ssid_list
    ssid        : [out] the buffer used to receive MACAddress
    size        : [in] size of buffer
    return value : 0 - success, otherwise - fail
*/
int dal_wifi_get_mac_address(unsigned int index_vlan, unsigned int index_ssid, char* mac, unsigned int size);

////////////////////////////////////////////////////////////////////////////////

#endif
