/*
 * deal with cgi
 *
 */

#ifndef _CGI_MAIN_H
#define _CGI_MAIN_H

#include "uhttpd.h"

#define UPLOAD_IMAGE_FREE_SPACE_KB      15000
#define WEB_BUF_SIZE_MAX        40960
#define DAL_BUFFER_SIZE         1024
#define DAL_BUFFER_LENGTH       64
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

#define NETWORK_INTERFACE_ETH0      "eth0"      // LAN ethernet port 1
#define NETWORK_INTERFACE_ETH1      "eth1"      // LAN ethernet port 2
#define NETWORK_INTERFACE_ETH2      "eth2"      // LAN ethernet port 3
#define NETWORK_INTERFACE_ETH3      "eth3"      // LAN ethernet port 4
#define NETWORK_INTERFACE_ETH10     "eth10"     // WAN ethernet port
#define NETWORK_INTERFACE_BR0       "br0"       // Bridge 1
#define NETWORK_INTERFACE_BR1       "br1"       // Bridge 2
#define NETWORK_INTERFACE_BR2       "br2"       // Bridge 3
#define NETWORK_INTERFACE_BR3       "br3"       // Bridge 4
#define NETWORK_INTERFACE_ATH0      "ath0"      // Wireless 1
#define NETWORK_INTERFACE_ATH1      "ath1"      // Wireless 2
#define NETWORK_INTERFACE_ATH2      "ath2"      // Wireless 3
#define NETWORK_INTERFACE_ATH3      "ath3"      // Wireless 4
#define NETWORK_INTERFACE_PPP0      "ppp0"      // WAN PPPoE 1
#define NETWORK_INTERFACE_PPP1      "ppp1"      // WAN PPPoE 2
#define NETWORK_INTERFACE_CLINK0    "clink0"    // LAN Coax
#define NETWORK_INTERFACE_CLINK1    "clink1"    // WAN Coax

#define UPLOAD_FILE_PREFIX          "uploadfile_"
#define CONFIG_FILENAME             "backup_cfg"
#define UPGRADEIMAGE_FILENAME       "upgrade.img"

#include "../ctl/files/include/libtr69_client.h"
#include "../ctl/files/include/OID.h"
#include "../ctl/files/include/ctl.h"
#include "../ctl/files/include/tsl_strconv.h"

#define BUFLEN_2048     2048  //!< buffer length 1024


//#include "../../../package/ctl/files/include/OID.h"
//#include "../../../package/ctl/files/include/ctl.h"
//#include "../../../package/ctl/files/include/tsl_strconv.h"
//#include "dal_api.h"

#include "cgi_utils.h"
#if HAVE_SESSION
#include "uhttpd-session.h"
#endif

#define ennull_out(x) \
    do { if((x) == NULL) goto out; } while(0)

#define gui_debug(arg...)    do { \
            SHOWDATE(); \
            fprintf(stdout,"--GUI %s(%d):%s: ", __FILE__, __LINE__, __FUNCTION__);\
            fprintf(stdout,##arg);\
            fprintf(stdout,"\n" );\
        } while(0)


#define TSL_CURRENT_TIME() \
({ \
    struct timespec _t0; \
    clock_gettime(CLOCK_MONOTONIC, &_t0); \
    _t0.tv_sec; \
})

typedef void (*TAG_FUNC) (int argc, char **argv, char *varValue);
typedef struct {
   char *tagName;
   TAG_FUNC tagFunc;
} TAG_VAR;

struct tag_set {
    char *tagname;
    void (*output)(int argc, char **argv, char *buf);
};

void cgiGetUserPermissions(char *buf);
void include(int argc, char **argv, char *buf);
void echoParameter(int argc, char **argv, char *buf);
void cgiEchoParameter(int argc, char **argv, char *varValue);

void cgiGetDataFromDM(char *key, char *varValue);

void cgiGetQueryString(int argc, char **argv, char *varValue);
void getValueFromQueryString(char *des, char *query_string, char *key);
void getSpecialOidFromSubmitOid(char *des, char *submit_oid, int idx);

char *getValueFromDal(char *v);

typedef enum {
    PARAM_TYPE_NONE = 0,
    PARAM_TYPE_STR,
    PARAM_TYPE_NUM,
} PARAM_TYPE;

typedef struct {
    char *key;
    void *value;
    PARAM_TYPE type;
} PARAM_ITEM;

typedef struct {
    //client address
    char cli_addr[BUFLEN_64];

    
    /* Common Post Parameters - START */
    char dal_buffer[DAL_BUFFER_LENGTH][DAL_BUFFER_SIZE];
    int dal_buffer_idx;
    char firmware_version[BUFLEN_32];
    
    char sumbit_button_name[BUFLEN_24];
    char submit_button_name[BUFLEN_24];
    char apply_page[BUFLEN_256];
    char waiting_page[BUFLEN_40];
    char waiting_page_topmenu[BUFLEN_40];
    char waiting_page_leftmenu[BUFLEN_40];
    
    char action_page[BUFLEN_64];

    char confirm_page[BUFLEN_40];
    int isconfirm;
    char query_string_hidden[BUFLEN_256];
    /* Common Post Parameters - END */

    //login
    char login_username[BUFLEN_32];
    char adv_currentpass[BUFLEN_64];
    char adv_newpass[BUFLEN_64];
    char login_password[BUFLEN_64];    
    int need_check_passwd;
    int wcb_main_login_success;
    int change_pwd_old_error;
    int cur_lang_stat;
    int change_lang;

    //forgot password
    char serial_number[BUFLEN_64];
    char new_password[BUFLEN_64];
    char re_enter_password[BUFLEN_64];    

    //firsttime login
    char firsttime_login_username[BUFLEN_32];
    char firsttime_login_password[BUFLEN_32];

    //mynetwork
    char mn_del_ifname[BUFLEN_16];
    char mn_del_type[BUFLEN_4];
    
    char submit_oid[BUFLEN_256];
    char properties_ifname[BUFLEN_24];
    char properties_descname[BUFLEN_64];
    char mn_enable[BUFLEN_16];
    int mn_is_single;
    
    char mn_ifname[BUFLEN_24];
    char mn_descname[BUFLEN_64];
    char mn_internet_protocol[BUFLEN_48];
    char mn_ipaddress[BUFLEN_48];
    char mn_netmask[BUFLEN_48];
    char mn_defaultgw[BUFLEN_48];
    int mn_mtu;
    
    char br_ifnameaddlist[BUFLEN_64];
    char br_ifnamedellist[BUFLEN_64];
    
    char mn_dnstype[BUFLEN_16];
    char mn_dns1[BUFLEN_64];
    char mn_dns2[BUFLEN_64];
    
    char mn_dhcptype[BUFLEN_16];
    char mn_dhcpipstart[BUFLEN_48];
    char mn_dhcpipend[BUFLEN_48];
    char mn_dhcpipsubnetmask[BUFLEN_48];
    char mn_dhcpwinsserver[BUFLEN_48];
    int mn_leasetime;
    
    char ppp_username[BUFLEN_64];
    char ppp_password[BUFLEN_64];
    char ppp_authproto[BUFLEN_32];
    char ppp_trigger[BUFLEN_16];
    char ppp_servicename[BUFLEN_64];
    int ppp_reconntime;
    
    char mn_dslite_type[BUFLEN_16];
    char mn_dslite_aftripv6[BUFLEN_64];
    int mn_dslite_b4ipv4addr;
    
    int moca_auto_detection;
    int moca_privacy;
    char moca_passwd[BUFLEN_64+1];
    int moca_cm_ratio;
    int moca_enable;
    
    //wxx6000 use these setting
    char lan_moca_channel[BUFLEN_48];
    int lan_moca_privacy;
    char lan_moca_passwd[BUFLEN_64+1];
    int lan_moca_cm_ratio;
    int lan_moca_enable;
        
    int ext_ipmethod;
    char ext_ipaddress[BUFLEN_48];
    char ext_subnetmask[BUFLEN_48];
    char ext_pid[BUFLEN_48];
    
    char mn_device_hostname[BUFLEN_48];
    char mn_device_icon[BUFLEN_48];
    
    char mn_routing_method[BUFLEN_16];
    char mn_routing_mode[BUFLEN_16];
    int mn_routing_metric;
    int mn_routing_default;
    char mn_routing_des[BUFLEN_48];
    char mn_routing_mask[BUFLEN_48];
    char mn_routing_gw[BUFLEN_48];
    
    char mn_add_vlan_ifname[BUFLEN_16];
    char mn_add_vlan_id[BUFLEN_16];
    char mn_add_vlan_tag[BUFLEN_16];
    char mn_add_vlan_select[BUFLEN_256];
    int mn_vlan_tag;

    int mn_port_isolation;
    int mn_delete_vlanid;
    int mn_ingress_policy;
    int mn_default_vlanid;
    char mn_vap_name[BUFLEN_64];
    int mn_addport_vlanid;
    int mn_addport_ingress_policy;
    //mynetwork
    
    char query_string[BUFLEN_256];

    //web6000 wireless basic
    int wireless_ssid_enable;
    int wireless_index_vlan;
    int wireless_index_ssid;
    char wireless_ssid[BUFLEN_64];
    int wireless_channel;
    int wireless_dot11_mode;
    int wireless_ssid_broadcast;
    int wireless_radio_enable;
    int wireless_security_type;
    int wireless_channelwidth;
    int wireless_data_rate;    
    int wireless_trans_power;
    char wireless_key[BUFLEN_64];
    int wep_key_mode;
    int wpa_encrypt_mode;    
    int wep_key_len;
    int wep_auth_mode;
    int prissidchanged;

    char wl_serverip[BUFLEN_64];
    int wl_serverport;
    char wl_serverpassword[BUFLEN_64];

    //wireless advanced
    int wl_protection;
    int wl_stbc;
    int wl_aggregation;
    int wl_gi;
    int wl_broadcast;
    int wl_wmm;  
    int wl_beacon;  
    
    // wireless wps
    int wireless_wps_enable_type;
    char wireless_wps_pin[BUFLEN_64];
    // wireless mac filter
    int wireless_macPolicy;
    int wireless_macCount;
    char wireless_maclist[BUFLEN_1024];
    // wireless end

    //web6000 advanced management
    char adv_ip[BUFLEN_64];
    char adv_subnet[BUFLEN_64];
    char adv_gateway[BUFLEN_64];
    int adv_autoconfig;

    // firewall general
    char firewall_radio_level[BUFLEN_16];
    char firewall_block_ipfrags[BUFLEN_16];

    // firewall access control
    char firewall_access_control_action[BUFLEN_32];
    char firewall_access_blocked_indexes_enable[BUFLEN_256];
    char firewall_access_blocked_indexes_disable[BUFLEN_256];
    char firewall_access_allowed_indexes_enable[BUFLEN_256];
    char firewall_access_allowed_indexes_disable[BUFLEN_256];
    char firewall_access_control_title[BUFLEN_16];
    int firewall_access_control_index;  
    char firewall_access_control_servicename[BUFLEN_64]; 

    char firewall_access_protocol_indexes[BUFLEN_256];
    char firewall_access_localhost_list[BUFLEN_24];
    char firewall_access_localhost[BUFLEN_24];
    char firewall_access_schedule_list[BUFLEN_24];

    // firewall port forwarding
    char firewall_forward_action[BUFLEN_32]; 
    char firewall_forward_indexes_enable[BUFLEN_256];
    char firewall_forward_indexes_disable[BUFLEN_256];
    char firewall_forward_indexes_delete[BUFLEN_256];

    int firewall_forward_basic_advanced;
    char firewall_forward_localhost_list[BUFLEN_24];
    char firewall_forward_localhost[BUFLEN_24];
    char firewall_forward_protocol[BUFLEN_24];
    char firewall_forward_protocol_custom[BUFLEN_16];
    char firewall_forward_protocol_ports_custom[BUFLEN_16];
    char firewall_forward_protocol_advanced[BUFLEN_16];
    char firewall_forward_sourceports_advanced_list[BUFLEN_16];
    char firewall_forward_sourceports_advanced[BUFLEN_16]; 
    char firewall_forward_destports_advanced_list[BUFLEN_16]; 
    char firewall_forward_destports_advanced[BUFLEN_16]; 
    char firewall_forward_wanconnection_list[BUFLEN_16]; 
    char firewall_forward_fwports_advanced_list[BUFLEN_16]; 
    char firewall_forward_fwports_advanced[BUFLEN_16];
    char firewall_forward_schedule_advanced[BUFLEN_16];     
    char firewall_forward_servicename[BUFLEN_64]; 
    char firewall_forward_remote_ipaddress[BUFLEN_24];


    // firewall dmz
    char firewall_dmz_enable[BUFLEN_16];
    char firewall_dmz_host_ip[BUFLEN_24];

    // firewall static nat
    char firewall_static_nat_action[BUFLEN_32];
    char firewall_protocol_indexes[BUFLEN_256];
    char firewall_local_host_list[BUFLEN_24];
    char firewall_local_host[BUFLEN_24];
    char firewall_public_ip[BUFLEN_24];
    char firewall_wan_connection_type[BUFLEN_24];
    char static_nat_local_server_enabled[BUFLEN_16];
    char firewall_static_nat_indexes_enable[BUFLEN_256];
    char firewall_static_nat_indexes_disable[BUFLEN_256];
    int firewall_static_nat_index;
    char firewall_static_nat_servicename[BUFLEN_64];

    // firewall protocol userdefined
    char firewall_protocol_list[BUFLEN_24];
    char firewall_protocol_sourceports_list[BUFLEN_24];
    char firewall_protocol_sourceports[BUFLEN_128];
    char firewall_protocol_destports_list[BUFLEN_24];
    char firewall_protocol_destports[BUFLEN_128];

    // firewall static nat user protocol 
    char firewall_protocol_service[BUFLEN_64];

    // firewall static nat user protocol add
    int firewall_protocol_entry;
    int firewall_protocol_entry_exclude;
    int firewall_protocol_number;

    // firewall security log setting
    char firewall_securitylog_accepted_incoming[BUFLEN_8];
    char firewall_securitylog_accepted_outgoing[BUFLEN_8];
    char firewall_securitylog_blocked_attempts[BUFLEN_8];
    char firewall_securitylog_winnuke[BUFLEN_8];
    char firewall_securitylog_broadcast[BUFLEN_8];
    char firewall_securitylog_icmpreplay[BUFLEN_8];
    char firewall_securitylog_defragmentation[BUFLEN_8];
    char firewall_securitylog_spoofed[BUFLEN_8];
    char firewall_securitylog_icmpredirect[BUFLEN_8];
    char firewall_securitylog_blockedfragments[BUFLEN_8];
    char firewall_securitylog_illegaloptions[BUFLEN_8];
    char firewall_securitylog_icmpmulticast[BUFLEN_8];
    char firewall_securitylog_synflood[BUFLEN_8];
    char firewall_securitylog_udpflood[BUFLEN_8];
    char firewall_securitylog_icmpflood[BUFLEN_8];
    char firewall_securitylog_echochargen[BUFLEN_8];
    char firewall_securitylog_remoteadmin[BUFLEN_8];
    char firewall_securitylog_connstate[BUFLEN_8];
    char firewall_securitylog_preventoverrun[BUFLEN_8];
    char firewall_securitylog_aggregation[BUFLEN_8];

    // firewall security log hazard
    char firewall_securitylog_unviewed_indexes[BUFLEN_256];

    // firewall advanced filtering
    int firewall_advancedfiltering_index;
    char firewall_advancedfiltering_flag[BUFLEN_8]; 
    char firewall_advancedfiltering_indexes_enable[BUFLEN_256];
    char firewall_advancedfiltering_indexes_disable[BUFLEN_256];

    char firewall_advancedfiltering_protocol_indexes[BUFLEN_256];
    char firewall_advancedfiltering_sourceaddress[BUFLEN_24];
    char firewall_advancedfiltering_sourcea_input[BUFLEN_24];
    char firewall_advancedfiltering_destaddress[BUFLEN_24];
    char firewall_advancedfiltering_desta_input[BUFLEN_24];
    char firewall_advancedfiltering_device[BUFLEN_24];
    char firewall_advancedfiltering_dscp[BUFLEN_8];
    char firewall_advancedfiltering_dscphex[BUFLEN_18];
    char firewall_advancedfiltering_dscpmask[BUFLEN_18];
    char firewall_advancedfiltering_priority[BUFLEN_8];
    char firewall_advancedfiltering_priorityqueue[BUFLEN_48];
    char firewall_advancedfiltering_filterlength[BUFLEN_8];
    char firewall_advancedfiltering_packetdata[BUFLEN_8];
    int firewall_advancedfiltering_lengthbegin;
    int firewall_advancedfiltering_lengthend;
    char firewall_advancedfiltering_operation[BUFLEN_18];
    char firewall_advancedfiltering_logging[BUFLEN_8];
    char firewall_advancedfiltering_scheduler[BUFLEN_18];
    
        
    // firewall end

    // parental control
    char parental_index[BUFLEN_32];
    char parental_devicelist_dest[BUFLEN_1024];
    char parental_maclist_dest[BUFLEN_1024];
    char parental_limitaccess_dest[BUFLEN_1024];
    char parental_limit_access[BUFLEN_16];
    char parental_weekdays[BUFLEN_32];
    char parental_is_enabling[BUFLEN_16];
    char parental_timestart[BUFLEN_16];
    char parental_timeend[BUFLEN_16];
    char parental_rule_name[BUFLEN_64];
    char parental_rule_description[BUFLEN_128];
    
    // parental end

    // advanced diagnostics
    char advanced_ping_destination[BUFLEN_64];
    char advanced_ping_type[BUFLEN_64];
    char advanced_ping_status[BUFLEN_64];
    int advanced_ping_number;
    char advanced_ping_result[BUFLEN_1024];

    // advanced mac cloning
    char advanced_macaddress_index[BUFLEN_32];
    char advanced_wandevices_clone[BUFLEN_24];
    char advanced_macaddress_clone[BUFLEN_24];
    char advanced_macaddress_clone_enable[BUFLEN_24];

    // advanced mac filter settings
    char advanced_filter_mac_index[BUFLEN_32];
    char advanced_filter_macaddress_add[BUFLEN_24];
    char advanced_filter_mac_page[BUFLEN_48];

    // advanced dhcp access control 
    char advanced_filter_mac_mode[BUFLEN_24];
    char advanced_dhcp_accesscontrol_page[BUFLEN_48];
    
    //advanced  dynamic dns
    char advanced_ddns_index[BUFLEN_32];
    char advanced_ddns_host[BUFLEN_64];
    char advanced_ddns_device[BUFLEN_24];
    char advanced_ddns_provider[BUFLEN_24];
    char advanced_ddns_username[BUFLEN_24];
    char advanced_ddns_password[BUFLEN_24];
    char advanced_ddns_system[BUFLEN_24];
    char advanced_ddns_wildcard[BUFLEN_24];
    char advanced_ddns_exchanger[BUFLEN_48];
    char advanced_ddns_backup[BUFLEN_24];
    char advanced_ddns_offline[BUFLEN_24];
    char advanced_ddns_sslmode[BUFLEN_24];
    char advanced_ddns_validatetime[BUFLEN_24];

    // advanced dns server
    char advanced_dnsserver_index[BUFLEN_32];
    char advanced_dnsserver_hostname[BUFLEN_64];
    char advanced_dnsserver_ipaddress[BUFLEN_24];

    // advanced routing
    char advanced_routing_index[BUFLEN_32];
    char advanced_routing_add_flag[BUFLEN_32];
    char ip_type[BUFLEN_24];
    char advanced_routing_ipaddress[BUFLEN_24];
    char advanced_routing_netmask[BUFLEN_24];
    char advanced_routing_gateway[BUFLEN_24];
    char advanced_routing_device[BUFLEN_24];
    int advanced_routing_metric;
    char advanced_routing_igmp[BUFLEN_24];
    char advanced_routing_domain[BUFLEN_24];

    // advanced dhcp settings
    char advanced_ipdistribution_index[BUFLEN_32];
    char advanced_ipdistribution_interface[BUFLEN_24];
    char advanced_dhcp_start_ipaddress[BUFLEN_24];
    char advanced_dhcp_end_ipaddress[BUFLEN_24];
    char advanced_dhcp_subnet_mask[BUFLEN_24];
    char advanced_dhcp_wins_server[BUFLEN_24];
    char advanced_dhcp_relay_ipaddress[BUFLEN_24];
    char advanced_dhcp_mode[BUFLEN_24];
    int advanced_dhcp_leasetime;
    char advanced_dhcp_provide_hostname[BUFLEN_24]; 

    // advanced dhcp connections
    char advanced_dhcpconnection_index[BUFLEN_32];
    char advanced_dhcpconnection_ipaddress[BUFLEN_24];
    char advanced_dhcpconnection_macaddress[BUFLEN_24];
    char advanced_dhcpconnection_hostname[BUFLEN_64];
    char advanced_dhcpconnection_leasetype[BUFLEN_24];

    // advanced port configuration
    char advanced_portconfiguration_speedduplex_wan[BUFLEN_24];
    char advanced_portconfiguration_speedduplex_lan1[BUFLEN_24];
    char advanced_portconfiguration_speedduplex_lan2[BUFLEN_24];
    char advanced_portconfiguration_speedduplex_lan3[BUFLEN_24];
    char advanced_portconfiguration_speedduplex_lan4[BUFLEN_24];

    // advanced system settings
    char advanced_system_hostname[BUFLEN_64];
    char advanced_system_domainname[BUFLEN_64];
    char advanced_system_autorefresh[BUFLEN_8];
    char advanced_system_promptpassword[BUFLEN_8];
    char advanced_system_warnchanged[BUFLEN_8];
    int advanced_system_sessionlifetime;
    int advanced_system_concurrentusers;
    int advanced_system_primary_http;
    int advanced_system_secondary_http;
    int advanced_system_primary_https;
    int advanced_system_secondary_https;
    int advanced_system_primary_telnet;
    int advanced_system_secondary_telnet;
    int advanced_system_secure_telnet;
    char advanced_system_logging[BUFLEN_8];
    char advanced_system_lowcapacity[BUFLEN_8];
    int advanced_system_allowedcapacity;
    int advanced_system_logbuffer;
    char advanced_system_notifylevel[BUFLEN_16];
    char advanced_system_fwlowcapacity[BUFLEN_8];
    int advanced_system_fwallowedcapacity;
    int advanced_system_fwlogbuffer;
    char advanced_system_fwnotifylevel[BUFLEN_16];
    char advanced_system_emailserver[BUFLEN_32];
    char advanced_system_emailaddress[BUFLEN_32];
    int advanced_system_emailport;
    char advanced_system_emailauth[BUFLEN_8];
    char advanced_system_emailusername[BUFLEN_128];
    char advanced_system_emailpassword[BUFLEN_128];
    char advanced_system_httpinterception[BUFLEN_8];
    char advanced_system_wandetection[BUFLEN_8];
    int advanced_system_ppptimeout;
    int advanced_system_dhcptimeout;
    int advanced_system_cyclenumbers;
    char advanced_system_continuoustrying[BUFLEN_8];
    
    char advanced_system_dnscache[BUFLEN_8];
    char advanced_system_primary_https_auth[BUFLEN_16];
    char advanced_system_secondary_https_auth[BUFLEN_16];
    char advanced_system_secure_telnet_auth[BUFLEN_16];

    // advanced datetime timeserver
    char advanced_timeserver_index[BUFLEN_32];
    char advanced_timeserver_name[BUFLEN_64];

    // advanced datetime
    char advanced_datetime_timezone[BUFLEN_32];
    int advanced_datetime_gmtoffset;
    char advanced_datetime_daylight[BUFLEN_8];
    char advanced_datetime_starttime[BUFLEN_32];    
    char advanced_datetime_endtime[BUFLEN_32];  
    int advanced_datetime_offset;
    char advanced_datetime_automatic[BUFLEN_8];
    char advanced_datetime_protocol[BUFLEN_8];
    int advanced_datetime_updateperiod;

    // advanced datetime clockset
    char advanced_clockset_month[BUFLEN_4];
    int advanced_clockset_day;
    int advanced_clockset_year;
    char advanced_clockset_hour[BUFLEN_4];
    char advanced_clockset_min[BUFLEN_4];
    char advanced_clockset_sec[BUFLEN_4];

    // advanced network objects
    char advanced_networkobjects_index[BUFLEN_32];
    char advanced_networkobjects_name[BUFLEN_64];
    char advanced_networkobjects_itemindexes[BUFLEN_256];

    char advanced_networkobjects_itemindex[BUFLEN_32];
    char advanced_networkobjects_itemtype[BUFLEN_16];
    char advanced_networkobjects_itemvalue1[BUFLEN_128];
    char advanced_networkobjects_itemvalue2[BUFLEN_128];
    
    char adv_configuration_file[BUFLEN_128];
    char adv_upgradeimage_file[BUFLEN_128];
    char firmware_upgrade_type_sel[BUFLEN_8];
    char firmware_upgrade_checkhours[BUFLEN_8];
    char firmware_upgrade_checkurl[BUFLEN_128];

    int upgrade_status;

    // advanced tr69
    int advanced_tr69_reboot;
    int advanced_tr69_enable;
    char advanced_tr69_url[BUFLEN_256+1];
    char advanced_tr69_username[BUFLEN_256+1];
    char advanced_tr69_password[BUFLEN_256+1];
    char advanced_tr69_informenable[BUFLEN_8];
    char advanced_tr69_informinterval[BUFLEN_16];
    char advanced_tr69_informtime[BUFLEN_64];
    char advanced_tr69_backoff_interval[BUFLEN_16];
    char advanced_tr69_connection_url[BUFLEN_256+1];
    char advanced_tr69_connection_username[BUFLEN_256+1];
    char advanced_tr69_connection_password[BUFLEN_256+1];
    char advanced_tr69_soap[BUFLEN_8];

    //local ssh
    int advanced_ssh_enable;
    char advanced_ssh_username[BUFLEN_64];
    char advanced_ssh_password[BUFLEN_64];
    int advanced_ssh_port;
    int advanced_ssh_timeout;

    //serial port
    int advanced_serialport_enable;

    //advanced swap image
    int swap_currentversion;

    //system log
    int adv_systemlog_enable;
    char adv_systemlog_ip[BUFLEN_64+1];

    //hnap log
    char hnaplog_action[BUFLEN_32];

    //auto upgrade
    int auto_upgrade_enable ;
    char auto_upgrade_url[BUFLEN_256];
    char auto_upgrade_ver[BUFLEN_256];

  
    // advanced port forwarding rules
    char adv_portforwarding_ids[BUFLEN_32];
    char adv_portforwarding_edit_otype[BUFLEN_16];
    char adv_portforwarding_edit_ids[BUFLEN_32];
    char adv_portforwarding_edit_servicename[BUFLEN_32];
    char adv_portforwarding_edit_servicename_old[BUFLEN_32];
    char adv_portforwarding_edit_servicedesc[BUFLEN_64];
    char adv_portforwarding_edit_rules[BUFLEN_1024];


    //advanced send mail settings
    char adv_mailsettings_smtpserver_name[BUFLEN_128];
    char adv_mailsettings_smtpserver_port[BUFLEN_8];
    char adv_mailsettings_smtpserver_security[BUFLEN_32];
    char adv_mailsettings_smtpserver_auth[BUFLEN_32];
    char adv_mailsettings_smtpserver_accountname[BUFLEN_32];
    char adv_mailsettings_smtpserver_accountpassword[BUFLEN_32];
    char adv_mailsettings_receive_email[BUFLEN_128];


    //option60 
    int adv_option60_idx;
    int adv_option60_enable;
    char adv_option60_alias[BUFLEN_64+1];
    char adv_option60_value[BUFLEN_256];    


    //new network object
    char adv_network_object_ids[BUFLEN_256];
    char adv_networkobject_edit_ids[BUFLEN_256];
    char adv_network_edit_rules[BUFLEN_1024];
    char adv_networkobject_edit_objectname[BUFLEN_64];

    // advanced scheduler rules
    char scheduler_rules_ids[BUFLEN_32];
    char scheduler_rules_set_otype[BUFLEN_16];
    char scheduler_rules_set_ids[BUFLEN_32];
    char scheduler_rules_set_rulename[BUFLEN_64];
    char scheduler_rules_set_rulestatus[BUFLEN_8];
    char scheduler_rules_set_rules[BUFLEN_1024];
        
    //advanced IGMP Proxy
    int igmp_proxy_status_sel;
    int igmp_proxy_version_sel;
    int igmp_proxy_fastleave_sel;
    char igmp_proxy_robustness[BUFLEN_16];
    char igmp_proxy_query_interval[BUFLEN_16];
    char igmp_proxy_query_resp_interval[BUFLEN_16];
    char igmp_proxy_lastquery_interval[BUFLEN_16];
    char igmp_proxy_max_groups[BUFLEN_16];
    char igmp_proxy_max_datasources[BUFLEN_16];
    char igmp_proxy_max_groupmembers[BUFLEN_16];
    int igmp_proxy_lan2lan_sel;

    //advanced igmp interface
    char igmp_interface1[BUFLEN_32];
    char igmp_interface2[BUFLEN_32];
    char igmp_interface3[BUFLEN_32];
    char igmp_interface_status1[BUFLEN_8];
    char igmp_interface_status2[BUFLEN_8];
    char igmp_interface_status3[BUFLEN_8];

    //advanced igmp host filtering
    char igmp_host_edit_igmpallowed[BUFLEN_32];
    char igmp_host_edit_maxchannel_allowed[BUFLEN_32];
    char igmp_host_edit_ids[BUFLEN_32];

    //advanced igmp service
    char igmp_service_ids[BUFLEN_32];
    char igmp_service_edit_ids[BUFLEN_32];
    char igmp_service_edit_otype[BUFLEN_32];
    char igmp_service_edit_rangelist[BUFLEN_1024];
    char igmp_service_edit_servicename[BUFLEN_32];
    char igmp_service_edit_maxstb[BUFLEN_32];
    char igmp_service_edit_maxnonstb[BUFLEN_32];

    
    // advanced IGMP ACL
    char igmp_acl_whitelist[BUFLEN_2048];
    char igmp_acl_blacklist[BUFLEN_2048];   
    char igmp_acl_filter_white[BUFLEN_8];
    char igmp_acl_filter_black[BUFLEN_8];   

    // advanced Universal Plug and Play
    char universal_upnp[BUFLEN_16];
    char universal_upnp_autoclean[BUFLEN_16];

    // advanced SIP ALG
    char sip_alg_enable[BUFLEN_16];

    // advanced Local Administration
    char local_admin_telnet_pri[BUFLEN_16];
    char local_admin_telnet_sec[BUFLEN_16];
    char local_admin_telnet_ssl[BUFLEN_16];

    // advanced Remote Administration
    char remote_admin_telnet_pri[BUFLEN_16];
    char remote_admin_telnet_sec[BUFLEN_16];
    char remote_admin_telnet_ssl[BUFLEN_16];
    char remote_admin_http_pri[BUFLEN_16];
    char remote_admin_http_sec[BUFLEN_16];
    char remote_admin_https_pri[BUFLEN_16];
    char remote_admin_https_sec[BUFLEN_16];
    char remote_admin_ping[BUFLEN_16];
    char remote_admin_traceroute[BUFLEN_16];

    // advanced Users
    char users_setting_fullname[BUFLEN_64];
    char users_setting_username[BUFLEN_64];
    char users_setting_newpassword[BUFLEN_64];
    int users_setting_userlevel_sel;
    char users_selected_ids[BUFLEN_64];
    char users_setting_selected_ids[BUFLEN_64];
    int users_login_attempt;
    char users_setting_email[BUFLEN_64];
    int users_setting_system_notifylevel_sel;
    int users_setting_security_notifylevel_sel;

    // advanced end

    // System Monitoring Log
    char sysmonitoring_log_flag[BUFLEN_16];
    
    // WAN Settings
    char wan_ipType[BUFLEN_16];
    char wan_ipAddress[BUFLEN_48];
    char wan_ipMask[BUFLEN_48];
    char wan_ipDefGtw[BUFLEN_48];
    // DNS
    char wan_dnsType[BUFLEN_16];
    char wan_dns1[BUFLEN_64];
    char wan_dns2[BUFLEN_64];
	
    //Broadband Configure
    char bb_ispType[BUFLEN_48];
    char bb_lineMode[BUFLEN_48];
    char bb_transMode[BUFLEN_48];
    char bb_vlanId[BUFLEN_16];
    char bb_mtu[BUFLEN_16];

    //Broadband Connection Status
    char bb_phy_link_stat[BUFLEN_48];
    char bb_ip_con_stat[BUFLEN_48];
    char bb_toltal_bytes_rev[BUFLEN_48];    
    
    //Local Network Subnet&DHCP
    char dev_ipv4_addr_value[BUFLEN_48];
    char subnet_mask_value[BUFLEN_48];
    char dhcpv4_start_addr_value[BUFLEN_48];
    char dhcpv4_end_addr_value[BUFLEN_48];
    int pub_sub_enable_sel;
    
    char pub_ipv4_addr_value[BUFLEN_48];
    char pub_sub_mask_value[BUFLEN_48];
    char pub_dhcpv4_start_addr_value[BUFLEN_48];
    char pub_dhcpv4_end_addr_value[BUFLEN_48];
    int dhcp_pool_radio;

    int cascad_router_enable_sel;    
    char cascad_router_addr_value[BUFLEN_48];
    char cas_net_addr_value[BUFLEN_48];
    char cascad_sub_mask_value[BUFLEN_48];

    char localnet_lease0_value[BUFLEN_8];
    char localnet_lease1_value[BUFLEN_8];
    char localnet_lease2_value[BUFLEN_8];
    char localnet_lease3_value[BUFLEN_8];

     //Local Dns Host Mapping Allocation
    char dev_dns_ip[BUFLEN_48];
    char dev_dns_mac[BUFLEN_48];
    char dev_dns_hostname[BUFLEN_256];
 
     //Local Network IP Address Allocation
    char local_allo_button[BUFLEN_8];
    char local_allo_rm_id[BUFLEN_48];
    char man_add_mac_value[BUFLEN_48];
    char man_add_ip_value[BUFLEN_48];


    //Firewall Rules
    char ena_disable_filter[BUFLEN_8];
    char action_radio[BUFLEN_8];
    char rule_order_value[BUFLEN_8];
    char src_ip_addr_value[BUFLEN_48];
    char dst_ip_addr_value[BUFLEN_48];
    char ing_inter_value[BUFLEN_48];
    char eg_inter_value[BUFLEN_48];
    char src_mac_addr_value[BUFLEN_48];
    char dst_mac_addr_value[BUFLEN_48];
    char firewall_protocol_sel[BUFLEN_8];
    char src_port_value[BUFLEN_48];
    char dst_port_value[BUFLEN_48];
    char filter_icmp_type_value[BUFLEN_48];
    char firewall_button_act[BUFLEN_48];
    char firewall_rule_act_id[BUFLEN_48];
    char src_mask_addr_value[BUFLEN_48];
    char dst_mask_addr_value[BUFLEN_48];
    char packetLength_value[BUFLEN_48];
    char tcpFlags_value[BUFLEN_256];

    //Firewall pinholes
    char fw_delPid[BUFLEN_64];
    char fw_pinholes_serviceName[BUFLEN_64];
    char fw_pinholes_srcPorts_start[BUFLEN_16];
    char fw_pinholes_srcPorts_end[BUFLEN_16];
    char fw_pinholes_desPorts_start[BUFLEN_16];
    char fw_pinholes_desPorts_end[BUFLEN_16];
    char fw_pinholes_protocol[BUFLEN_16];;
    char fw_pinholes_serviveId[BUFLEN_16];
    char fw_pinholes_rule_clientIp[BUFLEN_48];
    char fw_pinholes_rule_oid[BUFLEN_48];

    //Firewall Advanced
    int fw_adv_enable;
    int fw_adv_invalidIp;
    int fw_adv_portscan;
    int fw_drop_unkownType;
    int fw_drop_invalid;
    int fw_drop_iier;
    int fw_fl_limit;
    int fw_fl_rate;
    int fw_fl_burst;
    int fw_fl_icmp;
    int fw_fl_udp;
    int fw_fl_lupm;
    int fw_fl_ltcpe;
    int fw_fl_lisyn;
    char fw_level[BUFLEN_16];

    //Firewall IP Passthru
    char ippass_alloc_mode_sel[BUFLEN_8];
    char def_serv_internal_addr_value[BUFLEN_48];
    char ippass_pass_mode_sel[BUFLEN_8];
    char pass_fixed_mac_addr_value[BUFLEN_48];
    char pass_lease_day[BUFLEN_16];
    char pass_lease_hour[BUFLEN_16];
    char pass_lease_min[BUFLEN_16];
    char pass_lease_sec[BUFLEN_16];

    //Access Code
    char old_pwd[BUFLEN_64];
    char new_pwd[BUFLEN_64];
    char action[BUFLEN_64];
    char btn_action[BUFLEN_64];
    char pwd_reset_result[BUFLEN_16];

    //Voice
    char voice_line_type[BUFLEN_24];

    //IPV6 - WAN
    int ipv6_wan_enable;
    int ipv6_wan_pro;
    int ipv6_wan_6rd_dhcp;
    char ipv6_wan_6rd_prefix[BUFLEN_48];
    int ipv6_wan_6rd_prefixLen;
    int ipv6_wan_6rd_ipv4masklen;
    char ipv6_wan_6rd_ipv4RouterAddr[BUFLEN_48];
    int ipv6_wan_6rd_mtu;

    //IPV6 - Firewall
    int ipv6_6rd_fwESP;
    int ipv6_6rd_fwAH;
    int ipv6_6rd_fwHIP;
    int ipv6_6rd_fwIKEP;
    int ipv6_adv_fwICMP;

    //IPV6 - LAN
    int ipv6_lan_enable;
    int ipv6_lan_dhcpPd;
    int ipv6_lan_dhcpAddress;

    //Diagnostics Troubleshoot
    char troubleshoot_address[BUFLEN_48];
    char troubleshoot_protocol[BUFLEN_16];
    char alg_vpn[BUFLEN_8];
    char alg_voipSip[BUFLEN_8];
    char alg_voipH323[BUFLEN_8];

    //Local Network - Configure
    int configure_port1;
    int configure_port2;
    int configure_port3;
    int configure_port4;

    //Remote GUI
    char remote_state[BUFLEN_8];
    char remote_name[BUFLEN_8];
    char remote_pwd[BUFLEN_8];
    char remote_port[BUFLEN_8];
    char remote_timeout[BUFLEN_8];

    //Diagnostics Troubleshoot
    char syslog_stats[BUFLEN_16];
    char syslog_server[BUFLEN_48];
    
} WEB_PARAMS;

extern WEB_PARAMS gWebParams;

void cgiSetParam(char *key, char *value);
void cgiParsePostData(char *peekbuf);
void do_post(struct client *cl, struct http_request *req);
int do_cgiact(struct client *cl, struct http_request *req);
void do_download(struct client *cl, struct http_request *req);

int isxdigit_str(char* str);
unsigned int Ascii2Hex(char *ascii, unsigned int ascii_len, char *hex);
unsigned int Hex2Ascii(char *hex, unsigned int hex_len, char *ascii);

int getMemInfo(char *key);
void cgigetfirsttimelogin(int argc, char **argv, char *varValue);
int cgipostfirsttimelogin(void);
int getDeviceType(char *ifname);

#endif /* _CGI_MAIN_H */
