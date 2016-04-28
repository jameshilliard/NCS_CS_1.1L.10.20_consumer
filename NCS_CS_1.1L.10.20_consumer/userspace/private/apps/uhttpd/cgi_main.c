/*
 * deal with cgi
 */

#include "uhttpd.h"
#include "uhttpd-utils.h"

#include "cgi_main.h"
#include "cgi_firewall.h"
//new
#include "cgi_wan.h"
#include "cgi_dsl.h"
#include "cgi_localnet.h"
#include "cgi_voice.h"
#include "cgi_dev.h"
#include "cgi_wireless.h"
#include "cgi_diagnostics.h"
#include "cgi_ipv6.h"
#include "cgi_moca.h"
 #include "cgi_advanced.h"

#if HAVE_SESSION
#include "cgi_session.h"
#endif

WEB_PARAMS gWebParams;

PARAM_ITEM SetParams[] = {

    /* Common Post Parameters - START */
    {"sumbit_button_name", (void *)&gWebParams.sumbit_button_name, PARAM_TYPE_STR },
    {"submit_button_name", (void *)&gWebParams.submit_button_name, PARAM_TYPE_STR },
    {"apply_page", (void *)&gWebParams.apply_page, PARAM_TYPE_STR },
    {"waiting_page", (void *)&gWebParams.waiting_page, PARAM_TYPE_STR },
    {"waiting_page_topmenu", (void *)&gWebParams.waiting_page_topmenu, PARAM_TYPE_STR },
    {"waiting_page_leftmenu", (void *)&gWebParams.waiting_page_leftmenu, PARAM_TYPE_STR },
    {"confirm_page", (void *)&gWebParams.confirm_page, PARAM_TYPE_STR },
    {"isconfirm", (void *)&gWebParams.isconfirm, PARAM_TYPE_NUM },
    {"query_string_hidden", (void *)&gWebParams.query_string_hidden, PARAM_TYPE_STR },
    { "cur_lang_stat", (void *)&gWebParams.cur_lang_stat, PARAM_TYPE_NUM},    
    { "change_lang", (void *)&gWebParams.change_lang, PARAM_TYPE_NUM},    
    
    /* Common Post Parameters - END */

    //login
    {"login_username", (void *)&gWebParams.login_username, PARAM_TYPE_STR },
    {"login_password", (void *)&gWebParams.login_password, PARAM_TYPE_STR },
    {"adv_newpass", (void *)&gWebParams.adv_newpass, PARAM_TYPE_STR },
    {"adv_currentpass", (void *)&gWebParams.adv_currentpass, PARAM_TYPE_STR },    
    {"need_check_passwd", (void *)&gWebParams.need_check_passwd, PARAM_TYPE_NUM },

    //forgot password
    {"serial_number", (void *)&gWebParams.serial_number, PARAM_TYPE_STR },
    {"new_password", (void *)&gWebParams.new_password, PARAM_TYPE_STR },
    {"re_enter_password", (void *)&gWebParams.re_enter_password, PARAM_TYPE_STR },
    
    //firsttime login
    {"firsttime_login_username", (void *)&gWebParams.firsttime_login_username, PARAM_TYPE_STR },
    {"firsttime_login_password", (void *)&gWebParams.firsttime_login_password, PARAM_TYPE_STR },
        
    //mynetwork
    { "mn_del_ifname", (void *)&gWebParams.mn_del_ifname, PARAM_TYPE_STR},
    { "mn_del_type", (void *)&gWebParams.mn_del_type, PARAM_TYPE_STR},
    { "submit_oid", (void *)&gWebParams.submit_oid, PARAM_TYPE_STR},
    { "properties_ifname", (void *)&gWebParams.properties_ifname, PARAM_TYPE_STR},
    { "properties_descname", (void *)&gWebParams.properties_descname, PARAM_TYPE_STR},
    { "mn_enable", (void *)&gWebParams.mn_enable, PARAM_TYPE_STR},
    { "mn_is_single", (void *)&gWebParams.mn_is_single, PARAM_TYPE_NUM},
    
    { "mn_ifname", (void *)&gWebParams.mn_ifname, PARAM_TYPE_STR},
    { "mn_descname", (void *)&gWebParams.mn_descname, PARAM_TYPE_STR},
    { "mn_internet_protocol", (void *)&gWebParams.mn_internet_protocol, PARAM_TYPE_STR},
    { "mn_ipaddress", (void *)&gWebParams.mn_ipaddress, PARAM_TYPE_STR},
    { "mn_netmask", (void *)&gWebParams.mn_netmask, PARAM_TYPE_STR},
    { "mn_defaultgw", (void *)&gWebParams.mn_defaultgw, PARAM_TYPE_STR},
    { "mn_mtu", (void *)&gWebParams.mn_mtu, PARAM_TYPE_NUM},
    
    { "br_ifnameaddlist", (void *)&gWebParams.br_ifnameaddlist, PARAM_TYPE_STR},
    { "br_ifnamedellist", (void *)&gWebParams.br_ifnamedellist, PARAM_TYPE_STR},
    
    { "mn_dnstype", (void *)&gWebParams.mn_dnstype, PARAM_TYPE_STR},
    { "mn_dns1", (void *)&gWebParams.mn_dns1, PARAM_TYPE_STR},
    { "mn_dns2", (void *)&gWebParams.mn_dns2, PARAM_TYPE_STR},  
        
    { "mn_dhcptype", (void *)&gWebParams.mn_dhcptype, PARAM_TYPE_STR},
    { "mn_dhcpipstart", (void *)&gWebParams.mn_dhcpipstart, PARAM_TYPE_STR},
    { "mn_dhcpipend", (void *)&gWebParams.mn_dhcpipend, PARAM_TYPE_STR},
    { "mn_dhcpipsubnetmask", (void *)&gWebParams.mn_dhcpipsubnetmask, PARAM_TYPE_STR},
    { "mn_dhcpwinsserver", (void *)&gWebParams.mn_dhcpwinsserver, PARAM_TYPE_STR},
    { "mn_leasetime", (void *)&gWebParams.mn_leasetime, PARAM_TYPE_NUM},
    
    { "ppp_username", (void *)&gWebParams.ppp_username, PARAM_TYPE_STR},
    { "ppp_password", (void *)&gWebParams.ppp_password, PARAM_TYPE_STR},
    { "ppp_authproto", (void *)&gWebParams.ppp_authproto, PARAM_TYPE_STR},
    { "ppp_trigger", (void *)&gWebParams.ppp_trigger, PARAM_TYPE_STR},
    { "ppp_servicename", (void *)&gWebParams.ppp_servicename, PARAM_TYPE_STR},
    { "ppp_reconntime", (void *)&gWebParams.ppp_reconntime, PARAM_TYPE_NUM},
    
    { "mn_dslite_type", (void *)&gWebParams.mn_dslite_type, PARAM_TYPE_STR},
    { "mn_dslite_aftripv6", (void *)&gWebParams.mn_dslite_aftripv6, PARAM_TYPE_STR},
    { "mn_dslite_b4ipv4addr", (void *)&gWebParams.mn_dslite_b4ipv4addr, PARAM_TYPE_NUM},

    { "moca_auto_detection", (void *)&gWebParams.moca_auto_detection, PARAM_TYPE_NUM},
    { "moca_privacy", (void *)&gWebParams.moca_privacy, PARAM_TYPE_NUM},
    { "moca_passwd", (void *)&gWebParams.moca_passwd, PARAM_TYPE_STR},
    { "moca_cm_ratio", (void *)&gWebParams.moca_cm_ratio, PARAM_TYPE_NUM},
    { "moca_enable", (void *)&gWebParams.moca_enable, PARAM_TYPE_NUM},
    
    //wxx6000 use these setting
    { "lan_moca_channel", (void *)&gWebParams.lan_moca_channel, PARAM_TYPE_STR},
    { "lan_moca_privacy", (void *)&gWebParams.lan_moca_privacy, PARAM_TYPE_NUM},
    { "lan_moca_passwd", (void *)&gWebParams.lan_moca_passwd, PARAM_TYPE_STR},
    { "lan_moca_cm_ratio", (void *)&gWebParams.lan_moca_cm_ratio, PARAM_TYPE_NUM},
    { "lan_moca_enable", (void *)&gWebParams.lan_moca_enable, PARAM_TYPE_NUM},

    
    
    { "ext_ipmethod", (void *)&gWebParams.ext_ipmethod, PARAM_TYPE_NUM},
    { "ext_ipaddress", (void *)&gWebParams.ext_ipaddress, PARAM_TYPE_STR},
    { "ext_subnetmask", (void *)&gWebParams.ext_subnetmask, PARAM_TYPE_STR},
    { "ext_pid", (void *)&gWebParams.ext_pid, PARAM_TYPE_STR},

    { "mn_device_hostname", (void *)&gWebParams.mn_device_hostname, PARAM_TYPE_STR},
    { "mn_device_icon", (void *)&gWebParams.mn_device_icon, PARAM_TYPE_STR},
    
    { "mn_routing_method", (void *)&gWebParams.mn_routing_method, PARAM_TYPE_STR},
    { "mn_routing_mode", (void *)&gWebParams.mn_routing_mode, PARAM_TYPE_STR},
    { "mn_routing_metric", (void *)&gWebParams.mn_routing_metric, PARAM_TYPE_NUM},
    { "mn_routing_default", (void *)&gWebParams.mn_routing_default, PARAM_TYPE_NUM},
    { "mn_routing_des", (void *)&gWebParams.mn_routing_des, PARAM_TYPE_STR},
    { "mn_routing_mask", (void *)&gWebParams.mn_routing_mask, PARAM_TYPE_STR},
    { "mn_routing_gw", (void *)&gWebParams.mn_routing_gw, PARAM_TYPE_STR},
    
    { "mn_add_vlan_ifname", (void *)&gWebParams.mn_add_vlan_ifname, PARAM_TYPE_STR},
    { "mn_add_vlan_id", (void *)&gWebParams.mn_add_vlan_id, PARAM_TYPE_STR},
    { "mn_add_vlan_tag", (void *)&gWebParams.mn_add_vlan_tag, PARAM_TYPE_STR},
    { "mn_add_vlan_select", (void *)&gWebParams.mn_add_vlan_select, PARAM_TYPE_STR},
    { "mn_vlan_tag", (void *)&gWebParams.mn_vlan_tag, PARAM_TYPE_NUM},
    { "mn_port_isolation", (void *)&gWebParams.mn_port_isolation, PARAM_TYPE_NUM},
    { "mn_delete_vlanid", (void *)&gWebParams.mn_delete_vlanid, PARAM_TYPE_NUM},
    { "mn_ingress_policy", (void *)&gWebParams.mn_ingress_policy, PARAM_TYPE_NUM},
    { "mn_default_vlanid", (void *)&gWebParams.mn_default_vlanid, PARAM_TYPE_NUM},
    { "mn_vap_name", (void *)&gWebParams.mn_vap_name, PARAM_TYPE_STR},
    { "mn_addport_vlanid", (void *)&gWebParams.mn_addport_vlanid, PARAM_TYPE_NUM},
    { "mn_addport_ingress_policy", (void *)&gWebParams.mn_addport_ingress_policy, PARAM_TYPE_NUM},
    //mynetwork

    // wireless
    // wireless basic
    { "wireless_ssid_enable", (void *)&gWebParams.wireless_ssid_enable, PARAM_TYPE_NUM},
    { "wireless_ssid", (void *)&gWebParams.wireless_ssid, PARAM_TYPE_STR},
    { "wireless_channel", (void *)&gWebParams.wireless_channel, PARAM_TYPE_NUM},
    { "wireless_index_vlan", (void *)&gWebParams.wireless_index_vlan, PARAM_TYPE_NUM},
    { "wireless_index_ssid", (void *)&gWebParams.wireless_index_ssid, PARAM_TYPE_NUM},
    { "wireless_ssid_broadcast", (void *)&gWebParams.wireless_ssid_broadcast, PARAM_TYPE_NUM},
    { "wireless_radio_enable", (void *)&gWebParams.wireless_radio_enable, PARAM_TYPE_NUM},
    { "wireless_security_type", (void *)&gWebParams.wireless_security_type, PARAM_TYPE_NUM},
    { "wireless_key", (void *)&gWebParams.wireless_key, PARAM_TYPE_STR},
    { "wep_key_mode", (void *)&gWebParams.wep_key_mode, PARAM_TYPE_NUM},
    { "wep_auth_mode", (void *)&gWebParams.wep_auth_mode, PARAM_TYPE_NUM},    
    { "wpa_encrypt_mode", (void *)&gWebParams.wpa_encrypt_mode, PARAM_TYPE_NUM},    
    { "wep_key_len", (void *)&gWebParams.wep_key_len, PARAM_TYPE_NUM},
    { "wireless_channelwidth", (void *)&gWebParams.wireless_channelwidth, PARAM_TYPE_NUM},
    { "wireless_dot11_mode", (void *)&gWebParams.wireless_dot11_mode, PARAM_TYPE_NUM},
    { "wireless_trans_power", (void *)&gWebParams.wireless_trans_power, PARAM_TYPE_NUM},
    { "wireless_data_rate", (void *)&gWebParams.wireless_data_rate, PARAM_TYPE_NUM},

    { "wl_serverip", (void *)&gWebParams.wl_serverip, PARAM_TYPE_STR},
    { "wl_serverport", (void *)&gWebParams.wl_serverport, PARAM_TYPE_NUM},
    { "wl_serverpassword", (void *)&gWebParams.wl_serverpassword, PARAM_TYPE_STR},

    { "prissidchanged", (void *)&gWebParams.prissidchanged, PARAM_TYPE_NUM},

    //wireless advanced
    { "wl_protection", (void *)&gWebParams.wl_protection, PARAM_TYPE_NUM},
    { "wl_stbc", (void *)&gWebParams.wl_stbc, PARAM_TYPE_NUM},
    { "wl_aggregation", (void *)&gWebParams.wl_aggregation, PARAM_TYPE_NUM},
    { "wl_gi", (void *)&gWebParams.wl_gi, PARAM_TYPE_NUM},
    { "wl_broadcast", (void *)&gWebParams.wl_broadcast, PARAM_TYPE_NUM},
    { "wl_wmm", (void *)&gWebParams.wl_wmm, PARAM_TYPE_NUM},
    { "wl_beacon", (void *)&gWebParams.wl_beacon, PARAM_TYPE_NUM},


    // wireless wps
    { "wireless_wps_enable_type", (void *)&gWebParams.wireless_wps_enable_type, PARAM_TYPE_NUM},
    { "wireless_wps_pin", (void *)&gWebParams.wireless_wps_pin, PARAM_TYPE_STR},
    // wireless mac filter
    { "wireless_macPolicy", (void *)&gWebParams.wireless_macPolicy, PARAM_TYPE_NUM},
    { "wireless_macCount", (void *)&gWebParams.wireless_macCount, PARAM_TYPE_NUM},
    { "wireless_maclist", (void *)&gWebParams.wireless_maclist, PARAM_TYPE_STR},
    // wireless end

    //web 6000 advanced management
    { "adv_ip", (void *)&gWebParams.adv_ip, PARAM_TYPE_STR},
    { "adv_subnet", (void *)&gWebParams.adv_subnet, PARAM_TYPE_STR},
    { "adv_gateway", (void *)&gWebParams.adv_gateway, PARAM_TYPE_STR},
    { "adv_autoconfig", (void *)&gWebParams.adv_autoconfig, PARAM_TYPE_NUM},    
    // firewall
    // firewall general 
    { "firewall_radio_level", (void *)&gWebParams.firewall_radio_level, PARAM_TYPE_STR},
    { "firewall_block_ipfrags", (void *)&gWebParams.firewall_block_ipfrags, PARAM_TYPE_STR},

    // firewall access control
    { "firewall_access_control_action", (void *)&gWebParams.firewall_access_control_action, PARAM_TYPE_STR},
    { "firewall_access_blocked_indexes_enable", (void *)&gWebParams.firewall_access_blocked_indexes_enable, PARAM_TYPE_STR},
    { "firewall_access_blocked_indexes_disable", (void *)&gWebParams.firewall_access_blocked_indexes_disable, PARAM_TYPE_STR},
    { "firewall_access_allowed_indexes_enable", (void *)&gWebParams.firewall_access_allowed_indexes_enable, PARAM_TYPE_STR},
    { "firewall_access_allowed_indexes_disable", (void *)&gWebParams.firewall_access_allowed_indexes_disable, PARAM_TYPE_STR},
    { "firewall_access_control_title", (void *)&gWebParams.firewall_access_control_title, PARAM_TYPE_STR},
    { "firewall_access_control_index", (void *)&gWebParams.firewall_access_control_index, PARAM_TYPE_NUM},
    { "firewall_access_control_servicename", (void *)&gWebParams.firewall_access_control_servicename, PARAM_TYPE_STR},  
 
    { "firewall_access_protocol_indexes", (void *)&gWebParams.firewall_access_protocol_indexes, PARAM_TYPE_STR},
    { "firewall_access_localhost_list", (void *)&gWebParams.firewall_access_localhost_list, PARAM_TYPE_STR},
    { "firewall_access_localhost", (void *)&gWebParams.firewall_access_localhost, PARAM_TYPE_STR},  
    { "firewall_access_schedule_list", (void *)&gWebParams.firewall_access_schedule_list, PARAM_TYPE_STR},

    // firewall port forwarding
    { "firewall_forward_action", (void *)&gWebParams.firewall_forward_action, PARAM_TYPE_STR},
    { "firewall_forward_indexes_enable", (void *)&gWebParams.firewall_forward_indexes_enable, PARAM_TYPE_STR},
    { "firewall_forward_indexes_disable", (void *)&gWebParams.firewall_forward_indexes_disable, PARAM_TYPE_STR},
    { "firewall_forward_indexes_delete", (void *)&gWebParams.firewall_forward_indexes_delete, PARAM_TYPE_STR},
    
    { "firewall_forward_basic_advanced", (void *)&gWebParams.firewall_forward_basic_advanced, PARAM_TYPE_NUM},
    { "firewall_forward_localhost_list", (void *)&gWebParams.firewall_forward_localhost_list, PARAM_TYPE_STR},
    { "firewall_forward_localhost", (void *)&gWebParams.firewall_forward_localhost, PARAM_TYPE_STR},
    { "firewall_forward_protocol", (void *)&gWebParams.firewall_forward_protocol, PARAM_TYPE_STR},  
    { "firewall_forward_protocol_custom", (void *)&gWebParams.firewall_forward_protocol_custom, PARAM_TYPE_STR},
    { "firewall_forward_protocol_ports_custom", (void *)&gWebParams.firewall_forward_protocol_ports_custom, PARAM_TYPE_STR},
    { "firewall_forward_protocol_advanced", (void *)&gWebParams.firewall_forward_protocol_advanced, PARAM_TYPE_STR},

    { "firewall_forward_sourceports_advanced_list", (void *)&gWebParams.firewall_forward_sourceports_advanced_list, PARAM_TYPE_STR},
    { "firewall_forward_sourceports_advanced", (void *)&gWebParams.firewall_forward_sourceports_advanced, PARAM_TYPE_STR},
    { "firewall_forward_destports_advanced_list", (void *)&gWebParams.firewall_forward_destports_advanced_list, PARAM_TYPE_STR},
    { "firewall_forward_destports_advanced", (void *)&gWebParams.firewall_forward_destports_advanced, PARAM_TYPE_STR},
    { "firewall_forward_wanconnection_list", (void *)&gWebParams.firewall_forward_wanconnection_list, PARAM_TYPE_STR},

    { "firewall_forward_fwports_advanced_list", (void *)&gWebParams.firewall_forward_fwports_advanced_list, PARAM_TYPE_STR},
    { "firewall_forward_fwports_advanced", (void *)&gWebParams.firewall_forward_fwports_advanced, PARAM_TYPE_STR},
    { "firewall_forward_schedule_advanced", (void *)&gWebParams.firewall_forward_schedule_advanced, PARAM_TYPE_STR},
    { "firewall_forward_servicename", (void *)&gWebParams.firewall_forward_servicename, PARAM_TYPE_STR},
    { "firewall_forward_remote_ipaddress", (void *)&gWebParams.firewall_forward_remote_ipaddress, PARAM_TYPE_STR},
    
    
    // firewall dmz
    { "firewall_dmz_enable", (void *)&gWebParams.firewall_dmz_enable, PARAM_TYPE_STR},
    { "firewall_dmz_host_ip", (void *)&gWebParams.firewall_dmz_host_ip, PARAM_TYPE_STR},

    // firewall static nat      
    { "firewall_static_nat_action", (void *)&gWebParams.firewall_static_nat_action, PARAM_TYPE_STR},
    { "firewall_protocol_indexes", (void *)&gWebParams.firewall_protocol_indexes, PARAM_TYPE_STR},
    { "firewall_local_host_list", (void *)&gWebParams.firewall_local_host_list, PARAM_TYPE_STR},
    { "firewall_local_host", (void *)&gWebParams.firewall_local_host, PARAM_TYPE_STR},
    { "firewall_public_ip", (void *)&gWebParams.firewall_public_ip, PARAM_TYPE_STR},
    { "firewall_wan_connection_type", (void *)&gWebParams.firewall_wan_connection_type, PARAM_TYPE_STR},
    { "static_nat_local_server_enabled", (void *)&gWebParams.static_nat_local_server_enabled, PARAM_TYPE_STR},
    { "firewall_static_nat_indexes_enable", (void *)&gWebParams.firewall_static_nat_indexes_enable, PARAM_TYPE_STR},
    { "firewall_static_nat_indexes_disable", (void *)&gWebParams.firewall_static_nat_indexes_disable, PARAM_TYPE_STR},
    { "firewall_static_nat_index", (void *)&gWebParams.firewall_static_nat_index, PARAM_TYPE_NUM},
    { "firewall_static_nat_servicename", (void *)&gWebParams.firewall_static_nat_servicename, PARAM_TYPE_STR},

    // firewall protocol userdefined
    { "firewall_protocol_list", (void *)&gWebParams.firewall_protocol_list, PARAM_TYPE_STR},
    { "firewall_protocol_sourceports_list", (void *)&gWebParams.firewall_protocol_sourceports_list, PARAM_TYPE_STR},
    { "firewall_protocol_sourceports", (void *)&gWebParams.firewall_protocol_sourceports, PARAM_TYPE_STR},
    { "firewall_protocol_destports_list", (void *)&gWebParams.firewall_protocol_destports_list, PARAM_TYPE_STR},
    { "firewall_protocol_destports", (void *)&gWebParams.firewall_protocol_destports, PARAM_TYPE_STR},
    
    // firewall static nat user protocol 
    { "firewall_protocol_service", (void *)&gWebParams.firewall_protocol_service, PARAM_TYPE_STR},

    // firewall static nat user protocol add    
    { "firewall_protocol_entry", (void *)&gWebParams.firewall_protocol_entry, PARAM_TYPE_NUM},
    { "firewall_protocol_entry_exclude", (void *)&gWebParams.firewall_protocol_entry_exclude, PARAM_TYPE_NUM},
    { "firewall_protocol_number", (void *)&gWebParams.firewall_protocol_number, PARAM_TYPE_NUM},    

    // firewall port triggering
    /*
    { "porttrigger_object_edit_ids", (void *)&gWebParams.porttrigger_object_edit_ids, PARAM_TYPE_STR},
    { "porttrigger_edit_rules", (void *)&gWebParams.porttrigger_edit_rules, PARAM_TYPE_STR},
    { "del_predefined", (void *)&gWebParams.del_predefined, PARAM_TYPE_NUM},
    { "firewall_trigger_indexes_enable", (void *)&gWebParams.firewall_trigger_indexes_enable, PARAM_TYPE_STR},
    { "firewall_trigger_indexes_disable", (void *)&gWebParams.firewall_trigger_indexes_disable, PARAM_TYPE_STR},
    // firewall port triggering ports
    { "firewall_porttriggering_flag", (void *)&gWebParams.firewall_porttriggering_flag, PARAM_TYPE_STR},
    { "firewall_porttriggering_servicename", (void *)&gWebParams.firewall_porttriggering_servicename, PARAM_TYPE_STR},
    { "firewall_porttriggering_protocolexclude", (void *)&gWebParams.firewall_porttriggering_protocolexclude, PARAM_TYPE_STR},
    { "firewall_porttriggering_sourceports_list", (void *)&gWebParams.firewall_porttriggering_sourceports_list, PARAM_TYPE_STR},
    { "firewall_porttriggering_sourceports_start", (void *)&gWebParams.firewall_porttriggering_sourceports_start, PARAM_TYPE_NUM},
    { "firewall_porttriggering_sourceports_end", (void *)&gWebParams.firewall_porttriggering_sourceports_end, PARAM_TYPE_NUM},
    { "firewall_porttriggering_sourceexclude", (void *)&gWebParams.firewall_porttriggering_sourceexclude, PARAM_TYPE_STR},
    { "firewall_porttriggering_destports_list", (void *)&gWebParams.firewall_porttriggering_destports_list, PARAM_TYPE_STR},
    { "firewall_porttriggering_destports_start", (void *)&gWebParams.firewall_porttriggering_destports_start, PARAM_TYPE_NUM},
    { "firewall_porttriggering_destports_end", (void *)&gWebParams.firewall_porttriggering_destports_end, PARAM_TYPE_NUM},
    { "firewall_porttriggering_destexclude", (void *)&gWebParams.firewall_porttriggering_destexclude, PARAM_TYPE_STR},
    { "firewall_porttriggering_protocolnum", (void *)&gWebParams.firewall_porttriggering_protocolnum, PARAM_TYPE_NUM},
    { "firewall_porttriggering_icmp", (void *)&gWebParams.firewall_porttriggering_icmp, PARAM_TYPE_NUM},
    */
    // firewall security log setting
    { "firewall_securitylog_accepted_incoming", (void *)&gWebParams.firewall_securitylog_accepted_incoming, PARAM_TYPE_STR},
    { "firewall_securitylog_accepted_outgoing", (void *)&gWebParams.firewall_securitylog_accepted_outgoing, PARAM_TYPE_STR},
    { "firewall_securitylog_blocked_attempts", (void *)&gWebParams.firewall_securitylog_blocked_attempts, PARAM_TYPE_STR},
    { "firewall_securitylog_winnuke", (void *)&gWebParams.firewall_securitylog_winnuke, PARAM_TYPE_STR},
    { "firewall_securitylog_broadcast", (void *)&gWebParams.firewall_securitylog_broadcast, PARAM_TYPE_STR},
    { "firewall_securitylog_icmpreplay", (void *)&gWebParams.firewall_securitylog_icmpreplay, PARAM_TYPE_STR},
    { "firewall_securitylog_defragmentation", (void *)&gWebParams.firewall_securitylog_defragmentation, PARAM_TYPE_STR},
    { "firewall_securitylog_spoofed", (void *)&gWebParams.firewall_securitylog_spoofed, PARAM_TYPE_STR},
    { "firewall_securitylog_icmpredirect", (void *)&gWebParams.firewall_securitylog_icmpredirect, PARAM_TYPE_STR},
    { "firewall_securitylog_blockedfragments", (void *)&gWebParams.firewall_securitylog_blockedfragments, PARAM_TYPE_STR},
    { "firewall_securitylog_illegaloptions", (void *)&gWebParams.firewall_securitylog_illegaloptions, PARAM_TYPE_STR},
    { "firewall_securitylog_icmpmulticast", (void *)&gWebParams.firewall_securitylog_icmpmulticast, PARAM_TYPE_STR},
    { "firewall_securitylog_synflood", (void *)&gWebParams.firewall_securitylog_synflood, PARAM_TYPE_STR},
    { "firewall_securitylog_udpflood", (void *)&gWebParams.firewall_securitylog_udpflood, PARAM_TYPE_STR},
    { "firewall_securitylog_icmpflood", (void *)&gWebParams.firewall_securitylog_icmpflood, PARAM_TYPE_STR},
    { "firewall_securitylog_echochargen", (void *)&gWebParams.firewall_securitylog_echochargen, PARAM_TYPE_STR},
    { "firewall_securitylog_remoteadmin", (void *)&gWebParams.firewall_securitylog_remoteadmin, PARAM_TYPE_STR},
    { "firewall_securitylog_connstate", (void *)&gWebParams.firewall_securitylog_connstate, PARAM_TYPE_STR},
    { "firewall_securitylog_preventoverrun", (void *)&gWebParams.firewall_securitylog_preventoverrun, PARAM_TYPE_STR},
    { "firewall_securitylog_aggregation", (void *)&gWebParams.firewall_securitylog_aggregation, PARAM_TYPE_STR},    

    // firewall security log hazard
    { "firewall_securitylog_unviewed_indexes", (void *)&gWebParams.firewall_securitylog_unviewed_indexes, PARAM_TYPE_STR},

    // firewall advanced filtering
    { "firewall_advancedfiltering_index", (void *)&gWebParams.firewall_advancedfiltering_index, PARAM_TYPE_NUM},
    { "firewall_advancedfiltering_flag", (void *)&gWebParams.firewall_advancedfiltering_flag, PARAM_TYPE_STR},
    { "firewall_advancedfiltering_indexes_enable", (void *)&gWebParams.firewall_advancedfiltering_indexes_enable, PARAM_TYPE_STR},
    { "firewall_advancedfiltering_indexes_disable", (void *)&gWebParams.firewall_advancedfiltering_indexes_disable, PARAM_TYPE_STR},

    { "firewall_advancedfiltering_protocol_indexes", (void *)&gWebParams.firewall_advancedfiltering_protocol_indexes, PARAM_TYPE_STR},
    { "firewall_advancedfiltering_sourceaddress", (void *)&gWebParams.firewall_advancedfiltering_sourceaddress, PARAM_TYPE_STR},
    { "firewall_advancedfiltering_sourcea_input", (void *)&gWebParams.firewall_advancedfiltering_sourcea_input, PARAM_TYPE_STR},    
    { "firewall_advancedfiltering_destaddress", (void *)&gWebParams.firewall_advancedfiltering_destaddress, PARAM_TYPE_STR},
    { "firewall_advancedfiltering_desta_input", (void *)&gWebParams.firewall_advancedfiltering_desta_input, PARAM_TYPE_STR},    
    { "firewall_advancedfiltering_device", (void *)&gWebParams.firewall_advancedfiltering_device, PARAM_TYPE_STR},
    { "firewall_advancedfiltering_dscp", (void *)&gWebParams.firewall_advancedfiltering_dscp, PARAM_TYPE_STR},
    { "firewall_advancedfiltering_dscphex", (void *)&gWebParams.firewall_advancedfiltering_dscphex, PARAM_TYPE_STR},
    { "firewall_advancedfiltering_dscpmask", (void *)&gWebParams.firewall_advancedfiltering_dscpmask, PARAM_TYPE_STR},
    { "firewall_advancedfiltering_priority", (void *)&gWebParams.firewall_advancedfiltering_priority, PARAM_TYPE_STR},
    { "firewall_advancedfiltering_priorityqueue", (void *)&gWebParams.firewall_advancedfiltering_priorityqueue, PARAM_TYPE_STR},
    { "firewall_advancedfiltering_filterlength", (void *)&gWebParams.firewall_advancedfiltering_filterlength, PARAM_TYPE_STR},
    { "firewall_advancedfiltering_packetdata", (void *)&gWebParams.firewall_advancedfiltering_packetdata, PARAM_TYPE_STR},
    { "firewall_advancedfiltering_lengthbegin", (void *)&gWebParams.firewall_advancedfiltering_lengthbegin, PARAM_TYPE_NUM},
    { "firewall_advancedfiltering_lengthend", (void *)&gWebParams.firewall_advancedfiltering_lengthend, PARAM_TYPE_NUM},
    { "firewall_advancedfiltering_operation", (void *)&gWebParams.firewall_advancedfiltering_operation, PARAM_TYPE_STR},
    { "firewall_advancedfiltering_logging", (void *)&gWebParams.firewall_advancedfiltering_logging, PARAM_TYPE_STR},
    { "firewall_advancedfiltering_scheduler", (void *)&gWebParams.firewall_advancedfiltering_scheduler, PARAM_TYPE_STR},
    
    
    // firewall end

    // parental control
    { "parental_index", (void *)&gWebParams.parental_index, PARAM_TYPE_STR},
    { "parental_devicelist_dest", (void *)&gWebParams.parental_devicelist_dest, PARAM_TYPE_STR},
    { "parental_maclist_dest", (void *)&gWebParams.parental_maclist_dest, PARAM_TYPE_STR},  
    { "parental_limitaccess_dest", (void *)&gWebParams.parental_limitaccess_dest, PARAM_TYPE_STR},
    { "parental_limit_access", (void *)&gWebParams.parental_limit_access, PARAM_TYPE_STR},
    { "parental_weekdays", (void *)&gWebParams.parental_weekdays, PARAM_TYPE_STR},
    { "parental_is_enabling", (void *)&gWebParams.parental_is_enabling, PARAM_TYPE_STR},
    { "parental_timestart", (void *)&gWebParams.parental_timestart, PARAM_TYPE_STR},    
    { "parental_timeend", (void *)&gWebParams.parental_timeend, PARAM_TYPE_STR},    
    { "parental_rule_name", (void *)&gWebParams.parental_rule_name, PARAM_TYPE_STR},
    { "parental_rule_description", (void *)&gWebParams.parental_rule_description, PARAM_TYPE_STR},

    // parental end

    // advanced diagnostics
    { "advanced_ping_destination", (void *)&gWebParams.advanced_ping_destination, PARAM_TYPE_STR},
    { "advanced_ping_type", (void *)&gWebParams.advanced_ping_type, PARAM_TYPE_STR},
    { "advanced_ping_status", (void *)&gWebParams.advanced_ping_status, PARAM_TYPE_STR},
    { "advanced_ping_number", (void *)&gWebParams.advanced_ping_number, PARAM_TYPE_NUM},    

    // advanced mac cloning
    { "advanced_macaddress_index", (void *)&gWebParams.advanced_macaddress_index, PARAM_TYPE_STR},
    { "advanced_wandevices_clone", (void *)&gWebParams.advanced_wandevices_clone, PARAM_TYPE_STR},
    { "advanced_macaddress_clone", (void *)&gWebParams.advanced_macaddress_clone, PARAM_TYPE_STR},
    { "advanced_macaddress_clone_enable", (void *)&gWebParams.advanced_macaddress_clone_enable, PARAM_TYPE_STR},

    // advanced mac filter settings
    { "advanced_filter_mac_index", (void *)&gWebParams.advanced_filter_mac_index, PARAM_TYPE_STR},
    { "advanced_filter_macaddress_add", (void *)&gWebParams.advanced_filter_macaddress_add, PARAM_TYPE_STR},
    { "advanced_filter_mac_page", (void *)&gWebParams.advanced_filter_mac_page, PARAM_TYPE_STR},    
    
    // advanced dhcp access control     
    { "advanced_filter_mac_mode", (void *)&gWebParams.advanced_filter_mac_mode, PARAM_TYPE_STR},
    { "advanced_dhcp_accesscontrol_page", (void *)&gWebParams.advanced_dhcp_accesscontrol_page, PARAM_TYPE_STR},
    
    //advanced  dynamic dns
    { "advanced_ddns_index", (void *)&gWebParams.advanced_ddns_index, PARAM_TYPE_STR},
    { "advanced_ddns_host", (void *)&gWebParams.advanced_ddns_host, PARAM_TYPE_STR},
    { "advanced_ddns_device", (void *)&gWebParams.advanced_ddns_device, PARAM_TYPE_STR},
    { "advanced_ddns_provider", (void *)&gWebParams.advanced_ddns_provider, PARAM_TYPE_STR},
    { "advanced_ddns_username", (void *)&gWebParams.advanced_ddns_username, PARAM_TYPE_STR},
    { "advanced_ddns_password", (void *)&gWebParams.advanced_ddns_password, PARAM_TYPE_STR},
    { "advanced_ddns_system", (void *)&gWebParams.advanced_ddns_system, PARAM_TYPE_STR},
    { "advanced_ddns_wildcard", (void *)&gWebParams.advanced_ddns_wildcard, PARAM_TYPE_STR},
    { "advanced_ddns_exchanger", (void *)&gWebParams.advanced_ddns_exchanger, PARAM_TYPE_STR},
    { "advanced_ddns_backup", (void *)&gWebParams.advanced_ddns_backup, PARAM_TYPE_STR},
    { "advanced_ddns_offline", (void *)&gWebParams.advanced_ddns_offline, PARAM_TYPE_STR},
    { "advanced_ddns_sslmode", (void *)&gWebParams.advanced_ddns_sslmode, PARAM_TYPE_STR},
    { "advanced_ddns_validatetime", (void *)&gWebParams.advanced_ddns_validatetime, PARAM_TYPE_STR},

    // advanced dns server
    { "advanced_dnsserver_index", (void *)&gWebParams.advanced_dnsserver_index, PARAM_TYPE_STR},
    { "advanced_dnsserver_hostname", (void *)&gWebParams.advanced_dnsserver_hostname, PARAM_TYPE_STR},
    { "advanced_dnsserver_ipaddress", (void *)&gWebParams.advanced_dnsserver_ipaddress, PARAM_TYPE_STR},

    // advanced routing
    { "advanced_routing_index", (void *)&gWebParams.advanced_routing_index, PARAM_TYPE_STR},    
    { "advanced_routing_add_flag", (void *)&gWebParams.advanced_routing_add_flag, PARAM_TYPE_STR},
    { "advanced_routing_ipaddress", (void *)&gWebParams.advanced_routing_ipaddress, PARAM_TYPE_STR},
    { "ip_type", (void *)&gWebParams.ip_type, PARAM_TYPE_STR},
    { "advanced_routing_netmask", (void *)&gWebParams.advanced_routing_netmask, PARAM_TYPE_STR},
    { "advanced_routing_gateway", (void *)&gWebParams.advanced_routing_gateway, PARAM_TYPE_STR},
    { "advanced_routing_device", (void *)&gWebParams.advanced_routing_device, PARAM_TYPE_STR},
    { "advanced_routing_metric", (void *)&gWebParams.advanced_routing_metric, PARAM_TYPE_NUM},
    { "advanced_routing_igmp", (void *)&gWebParams.advanced_routing_igmp, PARAM_TYPE_STR},
    { "advanced_routing_domain", (void *)&gWebParams.advanced_routing_domain, PARAM_TYPE_STR},

    // advanced dhcp settings
    { "advanced_ipdistribution_index", (void *)&gWebParams.advanced_ipdistribution_index, PARAM_TYPE_STR},
    { "advanced_ipdistribution_interface", (void *)&gWebParams.advanced_ipdistribution_interface, PARAM_TYPE_STR},
    { "advanced_dhcp_start_ipaddress", (void *)&gWebParams.advanced_dhcp_start_ipaddress, PARAM_TYPE_STR},
    { "advanced_dhcp_end_ipaddress", (void *)&gWebParams.advanced_dhcp_end_ipaddress, PARAM_TYPE_STR},
    { "advanced_dhcp_subnet_mask", (void *)&gWebParams.advanced_dhcp_subnet_mask, PARAM_TYPE_STR},
    { "advanced_dhcp_wins_server", (void *)&gWebParams.advanced_dhcp_wins_server, PARAM_TYPE_STR},
    { "advanced_dhcp_relay_ipaddress", (void *)&gWebParams.advanced_dhcp_relay_ipaddress, PARAM_TYPE_STR},  
    { "advanced_dhcp_mode", (void *)&gWebParams.advanced_dhcp_mode, PARAM_TYPE_STR},
    { "advanced_dhcp_leasetime", (void *)&gWebParams.advanced_dhcp_leasetime, PARAM_TYPE_NUM},
    { "advanced_dhcp_provide_hostname", (void *)&gWebParams.advanced_dhcp_provide_hostname, PARAM_TYPE_STR},

    // advanced dhcp connections
    { "advanced_dhcpconnection_index", (void *)&gWebParams.advanced_dhcpconnection_index, PARAM_TYPE_STR},
    { "advanced_dhcpconnection_ipaddress", (void *)&gWebParams.advanced_dhcpconnection_ipaddress, PARAM_TYPE_STR},
    { "advanced_dhcpconnection_macaddress", (void *)&gWebParams.advanced_dhcpconnection_macaddress, PARAM_TYPE_STR},
    { "advanced_dhcpconnection_hostname", (void *)&gWebParams.advanced_dhcpconnection_hostname, PARAM_TYPE_STR},
    { "advanced_dhcpconnection_leasetype", (void *)&gWebParams.advanced_dhcpconnection_leasetype, PARAM_TYPE_STR},
    
    // advanced port configuration
    { "advanced_portconfiguration_speedduplex_wan", (void *)&gWebParams.advanced_portconfiguration_speedduplex_wan, PARAM_TYPE_STR},
    { "advanced_portconfiguration_speedduplex_lan1", (void *)&gWebParams.advanced_portconfiguration_speedduplex_lan1, PARAM_TYPE_STR},
    { "advanced_portconfiguration_speedduplex_lan2", (void *)&gWebParams.advanced_portconfiguration_speedduplex_lan2, PARAM_TYPE_STR},
    { "advanced_portconfiguration_speedduplex_lan3", (void *)&gWebParams.advanced_portconfiguration_speedduplex_lan3, PARAM_TYPE_STR},
    { "advanced_portconfiguration_speedduplex_lan4", (void *)&gWebParams.advanced_portconfiguration_speedduplex_lan4, PARAM_TYPE_STR},

    // advanced system settings  16 + 17
    { "advanced_system_hostname", (void *)&gWebParams.advanced_system_hostname, PARAM_TYPE_STR},
    { "advanced_system_domainname", (void *)&gWebParams.advanced_system_domainname, PARAM_TYPE_STR},
    { "advanced_system_autorefresh", (void *)&gWebParams.advanced_system_autorefresh, PARAM_TYPE_STR},
    { "advanced_system_promptpassword", (void *)&gWebParams.advanced_system_promptpassword, PARAM_TYPE_STR},
    { "advanced_system_warnchanged", (void *)&gWebParams.advanced_system_warnchanged, PARAM_TYPE_STR},  
    { "advanced_system_logging", (void *)&gWebParams.advanced_system_logging, PARAM_TYPE_STR},
    { "advanced_system_lowcapacity", (void *)&gWebParams.advanced_system_lowcapacity, PARAM_TYPE_STR},
    { "advanced_system_notifylevel", (void *)&gWebParams.advanced_system_notifylevel, PARAM_TYPE_STR},
    { "advanced_system_fwlowcapacity", (void *)&gWebParams.advanced_system_fwlowcapacity, PARAM_TYPE_STR},
    { "advanced_system_fwnotifylevel", (void *)&gWebParams.advanced_system_fwnotifylevel, PARAM_TYPE_STR},
    { "advanced_system_emailserver", (void *)&gWebParams.advanced_system_emailserver, PARAM_TYPE_STR},
    { "advanced_system_emailaddress", (void *)&gWebParams.advanced_system_emailaddress, PARAM_TYPE_STR},
    { "advanced_system_emailauth", (void *)&gWebParams.advanced_system_emailauth, PARAM_TYPE_STR},
    { "advanced_system_emailusername", (void *)&gWebParams.advanced_system_emailusername, PARAM_TYPE_STR},
    { "advanced_system_emailpassword", (void *)&gWebParams.advanced_system_emailpassword, PARAM_TYPE_STR},
    { "advanced_system_httpinterception", (void *)&gWebParams.advanced_system_httpinterception, PARAM_TYPE_STR},
    { "advanced_system_wandetection", (void *)&gWebParams.advanced_system_wandetection, PARAM_TYPE_STR},
    { "advanced_system_continuoustrying", (void *)&gWebParams.advanced_system_continuoustrying, PARAM_TYPE_STR},
        
    { "advanced_system_sessionlifetime", (void *)&gWebParams.advanced_system_sessionlifetime, PARAM_TYPE_NUM},
    { "advanced_system_concurrentusers", (void *)&gWebParams.advanced_system_concurrentusers, PARAM_TYPE_NUM},
    { "advanced_system_primary_http", (void *)&gWebParams.advanced_system_primary_http, PARAM_TYPE_NUM},
    { "advanced_system_secondary_http", (void *)&gWebParams.advanced_system_secondary_http, PARAM_TYPE_NUM},
    { "advanced_system_primary_https", (void *)&gWebParams.advanced_system_primary_https, PARAM_TYPE_NUM},
    { "advanced_system_secondary_https", (void *)&gWebParams.advanced_system_secondary_https, PARAM_TYPE_NUM},
    { "advanced_system_primary_telnet", (void *)&gWebParams.advanced_system_primary_telnet, PARAM_TYPE_NUM},
    { "advanced_system_secondary_telnet", (void *)&gWebParams.advanced_system_secondary_telnet, PARAM_TYPE_NUM},
    { "advanced_system_secure_telnet", (void *)&gWebParams.advanced_system_secure_telnet, PARAM_TYPE_NUM},  
    { "advanced_system_allowedcapacity", (void *)&gWebParams.advanced_system_allowedcapacity, PARAM_TYPE_NUM},
    { "advanced_system_logbuffer", (void *)&gWebParams.advanced_system_logbuffer, PARAM_TYPE_NUM},  
    { "advanced_system_fwallowedcapacity", (void *)&gWebParams.advanced_system_fwallowedcapacity, PARAM_TYPE_NUM},
    { "advanced_system_fwlogbuffer", (void *)&gWebParams.advanced_system_fwlogbuffer, PARAM_TYPE_NUM},  
    { "advanced_system_emailport", (void *)&gWebParams.advanced_system_emailport, PARAM_TYPE_NUM},
    { "advanced_system_ppptimeout", (void *)&gWebParams.advanced_system_ppptimeout, PARAM_TYPE_NUM},
    { "advanced_system_dhcptimeout", (void *)&gWebParams.advanced_system_dhcptimeout, PARAM_TYPE_NUM},
    { "advanced_system_cyclenumbers", (void *)&gWebParams.advanced_system_cyclenumbers, PARAM_TYPE_NUM},
    
    { "advanced_system_dnscache", (void *)&gWebParams.advanced_system_dnscache  , PARAM_TYPE_STR},
    { "advanced_system_primary_https_auth", (void *)&gWebParams.advanced_system_primary_https_auth  , PARAM_TYPE_STR},
    { "advanced_system_secondary_https_auth", (void *)&gWebParams.advanced_system_secondary_https_auth  , PARAM_TYPE_STR},
    { "advanced_system_secure_telnet_auth", (void *)&gWebParams.advanced_system_secure_telnet_auth  , PARAM_TYPE_STR},

    // advanced datetime timeserver
    { "advanced_timeserver_index", (void *)&gWebParams.advanced_timeserver_index, PARAM_TYPE_STR},
    { "advanced_timeserver_name", (void *)&gWebParams.advanced_timeserver_name, PARAM_TYPE_STR},

    // advanced datetime  6 + 2
    { "advanced_datetime_timezone", (void *)&gWebParams.advanced_datetime_timezone, PARAM_TYPE_STR},
    { "advanced_datetime_gmtoffset", (void *)&gWebParams.advanced_datetime_gmtoffset, PARAM_TYPE_NUM},
    { "advanced_datetime_daylight", (void *)&gWebParams.advanced_datetime_daylight, PARAM_TYPE_STR},
    { "advanced_datetime_starttime", (void *)&gWebParams.advanced_datetime_starttime, PARAM_TYPE_STR},
    { "advanced_datetime_endtime", (void *)&gWebParams.advanced_datetime_endtime, PARAM_TYPE_STR},  
    { "advanced_datetime_automatic", (void *)&gWebParams.advanced_datetime_automatic, PARAM_TYPE_STR},
    { "advanced_datetime_protocol", (void *)&gWebParams.advanced_datetime_protocol, PARAM_TYPE_STR},

    { "advanced_datetime_offset", (void *)&gWebParams.advanced_datetime_offset, PARAM_TYPE_NUM},
    { "advanced_datetime_updateperiod", (void *)&gWebParams.advanced_datetime_updateperiod, PARAM_TYPE_NUM},

    // advanced datetime clockset
    { "advanced_clockset_month", (void *)&gWebParams.advanced_clockset_month, PARAM_TYPE_STR},
    { "advanced_clockset_hour", (void *)&gWebParams.advanced_clockset_hour, PARAM_TYPE_STR},
    { "advanced_clockset_min", (void *)&gWebParams.advanced_clockset_min, PARAM_TYPE_STR},
    { "advanced_clockset_sec", (void *)&gWebParams.advanced_clockset_sec, PARAM_TYPE_STR},
    { "advanced_clockset_day", (void *)&gWebParams.advanced_clockset_day, PARAM_TYPE_NUM},
    { "advanced_clockset_year", (void *)&gWebParams.advanced_clockset_year, PARAM_TYPE_NUM},

    // advanced network objects
    { "advanced_networkobjects_index", (void *)&gWebParams.advanced_networkobjects_index, PARAM_TYPE_STR},
    { "advanced_networkobjects_name", (void *)&gWebParams.advanced_networkobjects_name, PARAM_TYPE_STR},
    { "advanced_networkobjects_itemindexes", (void *)&gWebParams.advanced_networkobjects_itemindexes, PARAM_TYPE_STR},  
    { "advanced_networkobjects_itemindex", (void *)&gWebParams.advanced_networkobjects_itemindex, PARAM_TYPE_STR},
    { "advanced_networkobjects_itemtype", (void *)&gWebParams.advanced_networkobjects_itemtype, PARAM_TYPE_STR},
    { "advanced_networkobjects_itemvalue1", (void *)&gWebParams.advanced_networkobjects_itemvalue1, PARAM_TYPE_STR},
    { "advanced_networkobjects_itemvalue2", (void *)&gWebParams.advanced_networkobjects_itemvalue2, PARAM_TYPE_STR},
    
    
    { "adv_configuration_file", (void *)&gWebParams.adv_configuration_file, PARAM_TYPE_STR},
    { "adv_upgradeimage_file", (void *)&gWebParams.adv_upgradeimage_file, PARAM_TYPE_STR},
    
    { "firmware_upgrade_type_sel", (void *)&gWebParams.firmware_upgrade_type_sel, PARAM_TYPE_STR},
    { "firmware_upgrade_checkhours", (void *)&gWebParams.firmware_upgrade_checkhours, PARAM_TYPE_STR},
    { "firmware_upgrade_checkurl", (void *)&gWebParams.firmware_upgrade_checkurl, PARAM_TYPE_STR},

    // advanced tr69
    { "advanced_tr69_reboot", (void *)&gWebParams.advanced_tr69_reboot, PARAM_TYPE_NUM},
    { "advanced_tr69_enable", (void *)&gWebParams.advanced_tr69_enable, PARAM_TYPE_NUM},
    { "advanced_tr69_url", (void *)&gWebParams.advanced_tr69_url, PARAM_TYPE_STR},
    { "advanced_tr69_username", (void *)&gWebParams.advanced_tr69_username, PARAM_TYPE_STR},
    { "advanced_tr69_password", (void *)&gWebParams.advanced_tr69_password, PARAM_TYPE_STR},
    { "advanced_tr69_informenable", (void *)&gWebParams.advanced_tr69_informenable, PARAM_TYPE_STR},
    { "advanced_tr69_informinterval", (void *)&gWebParams.advanced_tr69_informinterval, PARAM_TYPE_STR},
    { "advanced_tr69_informtime", (void *)&gWebParams.advanced_tr69_informtime, PARAM_TYPE_STR},
    { "advanced_tr69_backoff_interval", (void *)&gWebParams.advanced_tr69_backoff_interval, PARAM_TYPE_STR},
    { "advanced_tr69_connection_url", (void *)&gWebParams.advanced_tr69_connection_url, PARAM_TYPE_STR},
    { "advanced_tr69_connection_username", (void *)&gWebParams.advanced_tr69_connection_username, PARAM_TYPE_STR},
    { "advanced_tr69_connection_password", (void *)&gWebParams.advanced_tr69_connection_password, PARAM_TYPE_STR},
    { "advanced_tr69_soap", (void *)&gWebParams.advanced_tr69_soap, PARAM_TYPE_STR},


    //local ssh
    { "advanced_ssh_enable", (void *)&gWebParams.advanced_ssh_enable, PARAM_TYPE_NUM},
    { "advanced_ssh_username", (void *)&gWebParams.advanced_ssh_username, PARAM_TYPE_STR},
    { "advanced_ssh_password", (void *)&gWebParams.advanced_ssh_password, PARAM_TYPE_STR},
    { "advanced_ssh_port", (void *)&gWebParams.advanced_ssh_port, PARAM_TYPE_NUM},
    { "advanced_ssh_timeout", (void *)&gWebParams.advanced_ssh_timeout, PARAM_TYPE_NUM},

    //local ssh
    { "advanced_serialport_enable", (void *)&gWebParams.advanced_serialport_enable, PARAM_TYPE_NUM},

    //advanced swap image
    { "swap_currentversion", (void *)&gWebParams.swap_currentversion, PARAM_TYPE_NUM},

    //system log
    { "adv_systemlog_enable", (void *)&gWebParams.adv_systemlog_enable, PARAM_TYPE_NUM},
    { "adv_systemlog_ip", (void *)&gWebParams.adv_systemlog_ip, PARAM_TYPE_STR},

    //hnap log
    { "hnaplog_action", (void *)&gWebParams.hnaplog_action, PARAM_TYPE_STR},

    //auto upgrade
    { "auto_upgrade_enable", (void *)&gWebParams.auto_upgrade_enable, PARAM_TYPE_NUM},
    { "auto_upgrade_url", (void *)&gWebParams.auto_upgrade_url, PARAM_TYPE_STR},
    { "auto_upgrade_ver", (void *)&gWebParams.auto_upgrade_ver, PARAM_TYPE_STR},

    //sendmail settings
    { "adv_mailsettings_smtpserver_name", (void *)&gWebParams.adv_mailsettings_smtpserver_name, PARAM_TYPE_STR},
    { "adv_mailsettings_smtpserver_port", (void *)&gWebParams.adv_mailsettings_smtpserver_port, PARAM_TYPE_STR},
    { "adv_mailsettings_smtpserver_security", (void *)&gWebParams.adv_mailsettings_smtpserver_security, PARAM_TYPE_STR},
    { "adv_mailsettings_smtpserver_auth", (void *)&gWebParams.adv_mailsettings_smtpserver_auth, PARAM_TYPE_STR},
    { "adv_mailsettings_smtpserver_accountname", (void *)&gWebParams.adv_mailsettings_smtpserver_accountname, PARAM_TYPE_STR},
    { "adv_mailsettings_smtpserver_accountpassword", (void *)&gWebParams.adv_mailsettings_smtpserver_accountpassword, PARAM_TYPE_STR},
    { "adv_mailsettings_receive_email", (void *)&gWebParams.adv_mailsettings_receive_email, PARAM_TYPE_STR},


    //option 60
    { "adv_option60_idx", (void *)&gWebParams.adv_option60_idx, PARAM_TYPE_NUM},
    { "adv_option60_enable", (void *)&gWebParams.adv_option60_enable, PARAM_TYPE_NUM},
    { "adv_option60_alias", (void *)&gWebParams.adv_option60_alias, PARAM_TYPE_STR},
    { "adv_option60_value", (void *)&gWebParams.adv_option60_value, PARAM_TYPE_STR},


    //advanced IGMP Proxy
    { "igmp_proxy_status_sel", (void *)&gWebParams.igmp_proxy_status_sel, PARAM_TYPE_NUM},
    { "igmp_proxy_version_sel", (void *)&gWebParams.igmp_proxy_version_sel, PARAM_TYPE_NUM},
    { "igmp_proxy_fastleave_sel", (void *)&gWebParams.igmp_proxy_fastleave_sel, PARAM_TYPE_NUM},
    { "igmp_proxy_robustness", (void *)&gWebParams.igmp_proxy_robustness, PARAM_TYPE_STR},
    { "igmp_proxy_query_interval", (void *)&gWebParams.igmp_proxy_query_interval, PARAM_TYPE_STR},
    { "igmp_proxy_query_resp_interval", (void *)&gWebParams.igmp_proxy_query_resp_interval, PARAM_TYPE_STR},
    { "igmp_proxy_lastquery_interval", (void *)&gWebParams.igmp_proxy_lastquery_interval, PARAM_TYPE_STR},
    { "igmp_proxy_max_groups", (void *)&gWebParams.igmp_proxy_max_groups, PARAM_TYPE_STR},
    { "igmp_proxy_max_datasources", (void *)&gWebParams.igmp_proxy_max_datasources, PARAM_TYPE_STR},
    { "igmp_proxy_max_groupmembers", (void *)&gWebParams.igmp_proxy_max_groupmembers, PARAM_TYPE_STR},
    { "igmp_proxy_lan2lan_sel", (void *)&gWebParams.igmp_proxy_lan2lan_sel, PARAM_TYPE_NUM},

    //advanced igmp interface
    { "igmp_interface1", (void *)&gWebParams.igmp_interface1, PARAM_TYPE_STR},
    { "igmp_interface2", (void *)&gWebParams.igmp_interface2, PARAM_TYPE_STR},
    { "igmp_interface3", (void *)&gWebParams.igmp_interface3, PARAM_TYPE_STR},
    { "igmp_interface_status1", (void *)&gWebParams.igmp_interface_status1, PARAM_TYPE_STR},
    { "igmp_interface_status2", (void *)&gWebParams.igmp_interface_status2, PARAM_TYPE_STR},
    { "igmp_interface_status3", (void *)&gWebParams.igmp_interface_status3, PARAM_TYPE_STR},

    //advanced igmp host filtering
    { "igmp_host_edit_igmpallowed", (void *)&gWebParams.igmp_host_edit_igmpallowed, PARAM_TYPE_STR},
    { "igmp_host_edit_maxchannel_allowed", (void *)&gWebParams.igmp_host_edit_maxchannel_allowed, PARAM_TYPE_STR},
    { "igmp_host_edit_ids", (void *)&gWebParams.igmp_host_edit_ids, PARAM_TYPE_STR},


    //advanced igmp host service
    { "igmp_service_ids", (void *)&gWebParams.igmp_service_ids, PARAM_TYPE_STR},
    { "igmp_service_edit_ids", (void *)&gWebParams.igmp_service_edit_ids, PARAM_TYPE_STR},
    { "igmp_service_edit_otype", (void *)&gWebParams.igmp_service_edit_otype, PARAM_TYPE_STR},
    { "igmp_service_edit_rangelist", (void *)&gWebParams.igmp_service_edit_rangelist, PARAM_TYPE_STR},
    { "igmp_service_edit_servicename", (void *)&gWebParams.igmp_service_edit_servicename, PARAM_TYPE_STR},
    { "igmp_service_edit_maxstb", (void *)&gWebParams.igmp_service_edit_maxstb, PARAM_TYPE_STR},
    { "igmp_service_edit_maxnonstb", (void *)&gWebParams.igmp_service_edit_maxnonstb, PARAM_TYPE_STR},
    
    // advanced IGMP Acl
    { "igmp_acl_filter_white", (void *)&gWebParams.igmp_acl_filter_white, PARAM_TYPE_STR},  
    { "igmp_acl_filter_black", (void *)&gWebParams.igmp_acl_filter_black, PARAM_TYPE_STR},  
    { "igmp_acl_whitelist", (void *)&gWebParams.igmp_acl_whitelist, PARAM_TYPE_STR},
    { "igmp_acl_blacklist", (void *)&gWebParams.igmp_acl_blacklist, PARAM_TYPE_STR},

    
    // advanced port forwarding rules
    { "adv_portforwarding_ids", (void *)&gWebParams.adv_portforwarding_ids, PARAM_TYPE_STR},
    { "adv_portforwarding_edit_otype", (void *)&gWebParams.adv_portforwarding_edit_otype, PARAM_TYPE_STR},
    { "adv_portforwarding_edit_ids", (void *)&gWebParams.adv_portforwarding_edit_ids, PARAM_TYPE_STR},  
    { "adv_portforwarding_edit_servicename", (void *)&gWebParams.adv_portforwarding_edit_servicename, PARAM_TYPE_STR},
    { "adv_portforwarding_edit_servicename_old", (void *)&gWebParams.adv_portforwarding_edit_servicename_old, PARAM_TYPE_STR},
    { "adv_portforwarding_edit_servicedesc", (void *)&gWebParams.adv_portforwarding_edit_servicedesc, PARAM_TYPE_STR},
    { "adv_portforwarding_edit_rules", (void *)&gWebParams.adv_portforwarding_edit_rules, PARAM_TYPE_STR},

    //new network object
    { "adv_network_object_ids", (void *)&gWebParams.adv_network_object_ids, PARAM_TYPE_STR},    
    { "adv_networkobject_edit_ids", (void *)&gWebParams.adv_networkobject_edit_ids, PARAM_TYPE_STR},    
    { "adv_network_edit_rules", (void *)&gWebParams.adv_network_edit_rules, PARAM_TYPE_STR},    
    { "adv_networkobject_edit_objectname", (void *)&gWebParams.adv_networkobject_edit_objectname, PARAM_TYPE_STR},  
    

    // advanced scheduler rules
    { "scheduler_rules_ids", (void *)&gWebParams.scheduler_rules_ids, PARAM_TYPE_STR},
    { "scheduler_rules_set_otype", (void *)&gWebParams.scheduler_rules_set_otype, PARAM_TYPE_STR},
    { "scheduler_rules_set_ids", (void *)&gWebParams.scheduler_rules_set_ids, PARAM_TYPE_STR},  
    { "scheduler_rules_set_rulename", (void *)&gWebParams.scheduler_rules_set_rulename, PARAM_TYPE_STR},
    { "scheduler_rules_set_rulestatus", (void *)&gWebParams.scheduler_rules_set_rulestatus, PARAM_TYPE_STR},
    { "scheduler_rules_set_rules", (void *)&gWebParams.scheduler_rules_set_rules, PARAM_TYPE_STR},

    // advanced Universal Plug and Play
    { "universal_upnp", (void *)&gWebParams.universal_upnp, PARAM_TYPE_STR},
    { "universal_upnp_autoclean", (void *)&gWebParams.universal_upnp_autoclean, PARAM_TYPE_STR},

    // advanced SIP ALG
    { "sip_alg_enable", (void *)&gWebParams.sip_alg_enable, PARAM_TYPE_STR},

    // advanced Local Administration
    { "local_admin_telnet_pri", (void *)&gWebParams.local_admin_telnet_pri, PARAM_TYPE_STR},
    { "local_admin_telnet_sec", (void *)&gWebParams.local_admin_telnet_sec, PARAM_TYPE_STR},
    { "local_admin_telnet_ssl", (void *)&gWebParams.local_admin_telnet_ssl, PARAM_TYPE_STR},

    // advanced Remote Administration
    { "remote_admin_telnet_pri", (void *)&gWebParams.remote_admin_telnet_pri, PARAM_TYPE_STR},
    { "remote_admin_telnet_sec", (void *)&gWebParams.remote_admin_telnet_sec, PARAM_TYPE_STR},
    { "remote_admin_telnet_ssl", (void *)&gWebParams.remote_admin_telnet_ssl, PARAM_TYPE_STR},
    { "remote_admin_http_pri", (void *)&gWebParams.remote_admin_http_pri, PARAM_TYPE_STR},
    { "remote_admin_http_sec", (void *)&gWebParams.remote_admin_http_sec, PARAM_TYPE_STR},
    { "remote_admin_https_pri", (void *)&gWebParams.remote_admin_https_pri, PARAM_TYPE_STR},
    { "remote_admin_https_sec", (void *)&gWebParams.remote_admin_https_sec, PARAM_TYPE_STR},
    { "remote_admin_ping", (void *)&gWebParams.remote_admin_ping, PARAM_TYPE_STR},
    { "remote_admin_traceroute", (void *)&gWebParams.remote_admin_traceroute, PARAM_TYPE_STR},

    // advanced Users
    { "users_setting_fullname", (void *)&gWebParams.users_setting_fullname, PARAM_TYPE_STR},
    { "users_setting_username", (void *)&gWebParams.users_setting_username, PARAM_TYPE_STR},
    { "users_setting_newpassword", (void *)&gWebParams.users_setting_newpassword, PARAM_TYPE_STR},
    { "users_setting_userlevel_sel", (void *)&gWebParams.users_setting_userlevel_sel, PARAM_TYPE_NUM},
    { "users_selected_ids", (void *)&gWebParams.users_selected_ids, PARAM_TYPE_STR},
    { "users_setting_selected_ids", (void *)&gWebParams.users_setting_selected_ids, PARAM_TYPE_STR},
    { "users_login_attempt", (void *)&gWebParams.users_login_attempt, PARAM_TYPE_NUM},
    { "users_setting_email", (void *)&gWebParams.users_setting_email, PARAM_TYPE_STR},
    { "users_setting_system_notifylevel_sel", (void *)&gWebParams.users_setting_system_notifylevel_sel, PARAM_TYPE_NUM},
    { "users_setting_security_notifylevel_sel", (void *)&gWebParams.users_setting_security_notifylevel_sel, PARAM_TYPE_NUM},

    // advanced end

    //WAN Settings
    {"wan_ipType", (void *)&gWebParams.wan_ipType, PARAM_TYPE_STR},
    {"wan_ipAddress", (void *)&gWebParams.wan_ipAddress, PARAM_TYPE_STR},
    {"wan_ipMask", (void *)&gWebParams.wan_ipMask, PARAM_TYPE_STR},
    {"wan_ipDefGtw", (void *)&gWebParams.wan_ipDefGtw, PARAM_TYPE_STR},
    {"wan_dnsType", (void *)&gWebParams.wan_dnsType, PARAM_TYPE_STR},
    {"wan_dns1", (void *)&gWebParams.wan_dns1, PARAM_TYPE_STR},
    {"wan_dns2", (void *)&gWebParams.wan_dns2, PARAM_TYPE_STR},

    //Broadband Configure
    {"bb_ispType", (void *)&gWebParams.bb_ispType, PARAM_TYPE_STR},
    {"bb_lineMode", (void *)&gWebParams.bb_lineMode, PARAM_TYPE_STR},
    {"bb_transMode", (void *)&gWebParams.bb_transMode, PARAM_TYPE_STR},
    {"bb_vlanId", (void *)&gWebParams.bb_vlanId, PARAM_TYPE_STR},
    {"bb_mtu", (void *)&gWebParams.bb_mtu, PARAM_TYPE_STR},
 
    // System Monitoring Log
    {"sysmonitoring_log_flag", (void *)&gWebParams.sysmonitoring_log_flag, PARAM_TYPE_STR },
    
    //Local Network Subnet&DHCP    
    {"dev_ipv4_addr_value", (void *)&gWebParams.dev_ipv4_addr_value, PARAM_TYPE_STR },
    {"subnet_mask_value", (void *)&gWebParams.subnet_mask_value, PARAM_TYPE_STR },
    {"dhcpv4_start_addr_value", (void *)&gWebParams.dhcpv4_start_addr_value, PARAM_TYPE_STR },
    {"dhcpv4_end_addr_value", (void *)&gWebParams.dhcpv4_end_addr_value, PARAM_TYPE_STR },
    {"pub_sub_enable_sel", (void *)&gWebParams.pub_sub_enable_sel, PARAM_TYPE_NUM },
    {"pub_ipv4_addr_value", (void *)&gWebParams.pub_ipv4_addr_value, PARAM_TYPE_STR },
    {"pub_sub_mask_value", (void *)&gWebParams.pub_sub_mask_value, PARAM_TYPE_STR },
    {"pub_dhcpv4_start_addr_value", (void *)&gWebParams.pub_dhcpv4_start_addr_value, PARAM_TYPE_STR },
    {"pub_dhcpv4_end_addr_value", (void *)&gWebParams.pub_dhcpv4_end_addr_value, PARAM_TYPE_STR },
    {"dhcp_pool_radio", (void *)&gWebParams.dhcp_pool_radio, PARAM_TYPE_NUM },
    
    {"cascad_router_enable_sel", (void *)&gWebParams.cascad_router_enable_sel, PARAM_TYPE_NUM },
    {"cascad_router_addr_value", (void *)&gWebParams.cascad_router_addr_value, PARAM_TYPE_STR },
    {"cas_net_addr_value", (void *)&gWebParams.cas_net_addr_value, PARAM_TYPE_STR },
    {"cascad_sub_mask_value", (void *)&gWebParams.cascad_sub_mask_value, PARAM_TYPE_STR },

    {"localnet_lease0_value", (void *)&gWebParams.localnet_lease0_value, PARAM_TYPE_STR },
    {"localnet_lease1_value", (void *)&gWebParams.localnet_lease1_value, PARAM_TYPE_STR },
    {"localnet_lease2_value", (void *)&gWebParams.localnet_lease2_value, PARAM_TYPE_STR },
    {"localnet_lease3_value", (void *)&gWebParams.localnet_lease3_value, PARAM_TYPE_STR },
 
    //Local Network IP Address Allocation
    {"dev_dns_hostname", (void *)&gWebParams.dev_dns_hostname, PARAM_TYPE_STR },
    {"dev_dns_ip", (void *)&gWebParams.dev_dns_ip, PARAM_TYPE_STR },
    {"dev_dns_mac", (void *)&gWebParams.dev_dns_mac, PARAM_TYPE_STR },

    //Local Network IP Address Allocation
    {"local_allo_button", (void *)&gWebParams.local_allo_button, PARAM_TYPE_STR },
    {"local_allo_rm_id", (void *)&gWebParams.local_allo_rm_id, PARAM_TYPE_STR },
    {"man_add_mac_value", (void *)&gWebParams.man_add_mac_value, PARAM_TYPE_STR },
    {"man_add_ip_value", (void *)&gWebParams.man_add_ip_value, PARAM_TYPE_STR },

    //Firewall Rules
    {"ena_disable_filter", (void *)&gWebParams.ena_disable_filter, PARAM_TYPE_STR },
    {"action_radio", (void *)&gWebParams.action_radio, PARAM_TYPE_STR },
    {"rule_order_value", (void *)&gWebParams.rule_order_value, PARAM_TYPE_STR },
    {"src_ip_addr_value", (void *)&gWebParams.src_ip_addr_value, PARAM_TYPE_STR },
    {"dst_ip_addr_value", (void *)&gWebParams.dst_ip_addr_value, PARAM_TYPE_STR },
    {"ing_inter_value", (void *)&gWebParams.ing_inter_value, PARAM_TYPE_STR },
    {"eg_inter_value", (void *)&gWebParams.eg_inter_value, PARAM_TYPE_STR },
    {"src_mac_addr_value", (void *)&gWebParams.src_mac_addr_value, PARAM_TYPE_STR },
    {"dst_mac_addr_value", (void *)&gWebParams.dst_mac_addr_value, PARAM_TYPE_STR },
    {"firewall_protocol_sel", (void *)&gWebParams.firewall_protocol_sel, PARAM_TYPE_STR },
    {"src_port_value", (void *)&gWebParams.src_port_value, PARAM_TYPE_STR },
    {"dst_port_value", (void *)&gWebParams.dst_port_value, PARAM_TYPE_STR },
    {"filter_icmp_type_value", (void *)&gWebParams.filter_icmp_type_value, PARAM_TYPE_STR },    
    {"firewall_button_act", (void *)&gWebParams.firewall_button_act, PARAM_TYPE_STR },
    {"firewall_rule_act_id", (void *)&gWebParams.firewall_rule_act_id, PARAM_TYPE_STR },
    {"src_mask_addr_value", (void *)&gWebParams.src_mask_addr_value, PARAM_TYPE_STR },
    {"dst_mask_addr_value", (void *)&gWebParams.dst_mask_addr_value, PARAM_TYPE_STR },
    {"packetLength_value", (void *)&gWebParams.packetLength_value, PARAM_TYPE_STR },
    {"tcpFlags_value", (void *)&gWebParams.tcpFlags_value, PARAM_TYPE_STR },

    // firewall pinholes
    { "fw_delPid", (void *)&gWebParams.fw_delPid, PARAM_TYPE_STR},
    { "fw_pinholes_serviceName", (void *)&gWebParams.fw_pinholes_serviceName, PARAM_TYPE_STR},
    { "fw_pinholes_srcPorts_start", (void *)&gWebParams.fw_pinholes_srcPorts_start, PARAM_TYPE_STR},
    { "fw_pinholes_srcPorts_end", (void *)&gWebParams.fw_pinholes_srcPorts_end, PARAM_TYPE_STR},
    { "fw_pinholes_desPorts_start", (void *)&gWebParams.fw_pinholes_desPorts_start, PARAM_TYPE_STR},
    { "fw_pinholes_desPorts_end", (void *)&gWebParams.fw_pinholes_desPorts_end, PARAM_TYPE_STR},
    { "fw_pinholes_protocol", (void *)&gWebParams.fw_pinholes_protocol, PARAM_TYPE_STR},
    { "fw_pinholes_serviveId", (void *)&gWebParams.fw_pinholes_serviveId, PARAM_TYPE_STR},
    { "fw_pinholes_rule_clientIp", (void *)&gWebParams.fw_pinholes_rule_clientIp, PARAM_TYPE_STR},
    { "fw_pinholes_rule_oid", (void *)&gWebParams.fw_pinholes_rule_oid, PARAM_TYPE_STR},

    //Firewall Advanced
    {"fw_adv_enable", (void *)&gWebParams.fw_adv_enable, PARAM_TYPE_NUM },
    {"fw_adv_invalidIp", (void *)&gWebParams.fw_adv_invalidIp, PARAM_TYPE_NUM },
    {"fw_adv_portscan", (void *)&gWebParams.fw_adv_portscan, PARAM_TYPE_NUM },
    {"fw_drop_unkownType", (void *)&gWebParams.fw_drop_unkownType, PARAM_TYPE_NUM },
    {"fw_drop_invalid", (void *)&gWebParams.fw_drop_invalid, PARAM_TYPE_NUM },
    {"fw_drop_iier", (void *)&gWebParams.fw_drop_iier, PARAM_TYPE_NUM },
    {"fw_fl_limit", (void *)&gWebParams.fw_fl_limit, PARAM_TYPE_NUM },
    {"fw_fl_rate", (void *)&gWebParams.fw_fl_rate, PARAM_TYPE_NUM },
    {"fw_fl_burst", (void *)&gWebParams.fw_fl_burst, PARAM_TYPE_NUM },
    {"fw_fl_icmp", (void *)&gWebParams.fw_fl_icmp, PARAM_TYPE_NUM },
    {"fw_fl_udp", (void *)&gWebParams.fw_fl_udp, PARAM_TYPE_NUM },
    {"fw_fl_lupm", (void *)&gWebParams.fw_fl_lupm, PARAM_TYPE_NUM },
    {"fw_fl_ltcpe", (void *)&gWebParams.fw_fl_ltcpe, PARAM_TYPE_NUM },
    {"fw_fl_lisyn", (void *)&gWebParams.fw_fl_lisyn, PARAM_TYPE_NUM },
    {"fw_level", (void *)&gWebParams.fw_level, PARAM_TYPE_STR },

    //Firewall IP Passthru
    {"ippass_alloc_mode_sel", (void *)&gWebParams.ippass_alloc_mode_sel, PARAM_TYPE_STR },
    {"def_serv_internal_addr_value", (void *)&gWebParams.def_serv_internal_addr_value, PARAM_TYPE_STR },
    {"ippass_pass_mode_sel", (void *)&gWebParams.ippass_pass_mode_sel, PARAM_TYPE_STR },
    {"pass_fixed_mac_addr_value", (void *)&gWebParams.pass_fixed_mac_addr_value, PARAM_TYPE_STR },
    {"pass_lease_day", (void *)&gWebParams.pass_lease_day, PARAM_TYPE_STR },
    {"pass_lease_hour", (void *)&gWebParams.pass_lease_hour, PARAM_TYPE_STR },
    {"pass_lease_min", (void *)&gWebParams.pass_lease_min, PARAM_TYPE_STR },
    {"pass_lease_sec", (void *)&gWebParams.pass_lease_sec, PARAM_TYPE_STR },

    //Access Code
    {"old_pwd", (void *)&gWebParams.old_pwd, PARAM_TYPE_STR },
    {"new_pwd", (void *)&gWebParams.new_pwd, PARAM_TYPE_STR },
    {"action", (void *)&gWebParams.action, PARAM_TYPE_STR },
    {"btn_action", (void *)&gWebParams.btn_action, PARAM_TYPE_STR },
    {"pwd_reset_result", (void *)&gWebParams.pwd_reset_result, PARAM_TYPE_STR },

    //Voice
    {"voice_line_type", (void *)&gWebParams.voice_line_type, PARAM_TYPE_STR },

    //IPV6
    //IPV6 - WAN
    {"ipv6_wan_enable", (void *)&gWebParams.ipv6_wan_enable, PARAM_TYPE_NUM},
    {"ipv6_wan_pro", (void *)&gWebParams.ipv6_wan_pro, PARAM_TYPE_NUM},
    {"ipv6_wan_6rd_dhcp", (void *)&gWebParams.ipv6_wan_6rd_dhcp, PARAM_TYPE_NUM},
    {"ipv6_wan_6rd_prefix", (void *)&gWebParams.ipv6_wan_6rd_prefix, PARAM_TYPE_STR},
    {"ipv6_wan_6rd_prefixLen", (void *)&gWebParams.ipv6_wan_6rd_prefixLen, PARAM_TYPE_NUM},
    {"ipv6_wan_6rd_ipv4masklen", (void *)&gWebParams.ipv6_wan_6rd_ipv4masklen, PARAM_TYPE_NUM},
    {"ipv6_wan_6rd_ipv4RouterAddr", (void *)&gWebParams.ipv6_wan_6rd_ipv4RouterAddr, PARAM_TYPE_STR},
    {"ipv6_wan_6rd_mtu", (void *)&gWebParams.ipv6_wan_6rd_mtu, PARAM_TYPE_NUM},

    //IPV6 - Firewall
    {"ipv6_6rd_fwESP", (void *)&gWebParams.ipv6_6rd_fwESP, PARAM_TYPE_NUM},
    {"ipv6_6rd_fwAH", (void *)&gWebParams.ipv6_6rd_fwAH, PARAM_TYPE_NUM},
    {"ipv6_6rd_fwHIP", (void *)&gWebParams.ipv6_6rd_fwHIP, PARAM_TYPE_NUM},
    {"ipv6_6rd_fwIKEP", (void *)&gWebParams.ipv6_6rd_fwIKEP, PARAM_TYPE_NUM},
    {"ipv6_adv_fwICMP", (void *)&gWebParams.ipv6_adv_fwICMP, PARAM_TYPE_NUM},

    //IPV6 LAN
    {"ipv6_lan_enable", (void *)&gWebParams.ipv6_lan_enable, PARAM_TYPE_NUM},
    {"ipv6_lan_dhcpPd", (void *)&gWebParams.ipv6_lan_dhcpPd, PARAM_TYPE_NUM},
    {"ipv6_lan_dhcpAddress", (void *)&gWebParams.ipv6_lan_dhcpAddress, PARAM_TYPE_NUM},
    //IPV6 end

    //Diagnostics Troubleshoot
    {"troubleshoot_address", (void *)&gWebParams.troubleshoot_address, PARAM_TYPE_STR},
    {"troubleshoot_protocol", (void *)&gWebParams.troubleshoot_protocol, PARAM_TYPE_STR},
    {"alg_vpn", (void *)&gWebParams.alg_vpn, PARAM_TYPE_STR},
    {"alg_voipSip", (void *)&gWebParams.alg_voipSip, PARAM_TYPE_STR},
    {"alg_voipH323", (void *)&gWebParams.alg_voipH323, PARAM_TYPE_STR},

    //Local Network - Configure
    {"configure_port1", (void *)&gWebParams.configure_port1, PARAM_TYPE_NUM},
    {"configure_port2", (void *)&gWebParams.configure_port2, PARAM_TYPE_NUM},
    {"configure_port3", (void *)&gWebParams.configure_port3, PARAM_TYPE_NUM},
    {"configure_port4", (void *)&gWebParams.configure_port4, PARAM_TYPE_NUM},

    //Remote GUI
    {"remote_state", (void *)&gWebParams.remote_state, PARAM_TYPE_STR},
    {"remote_name", (void *)&gWebParams.remote_name, PARAM_TYPE_STR},
    {"remote_pwd", (void *)&gWebParams.remote_pwd, PARAM_TYPE_STR},
    {"remote_port", (void *)&gWebParams.remote_port, PARAM_TYPE_STR},
    {"remote_timeout", (void *)&gWebParams.remote_timeout, PARAM_TYPE_STR},

    //Diagnostics Troubleshoot
    {"syslog_stats", (void *)&gWebParams.syslog_stats, PARAM_TYPE_STR},
    {"syslog_server", (void *)&gWebParams.syslog_server, PARAM_TYPE_STR},

    { NULL, NULL, PARAM_TYPE_NONE }
};


void cgiSetParam(char *key, char *value) {
    int i = 0;
    for ( ; SetParams[i].key != NULL; i++ )
    {
        if ( safe_strcmp(key, SetParams[i].key) == 0 ) {
            break;
        }
    }
    
    if ( SetParams[i].key != NULL ) {       
        switch ( SetParams[i].type ) {
            case PARAM_TYPE_STR:
                strcpy((char *)SetParams[i].value, value);
                break;
            case PARAM_TYPE_NUM:
                *((int *)SetParams[i].value) = atoi(value);
                break;
            default:
                break;
        }
    }
}

void cgiParsePostData(char *peekbuf) {
    char *key, *value, *next;
    
    if (peekbuf) {
        gui_debug("peekbuf=%s\n", peekbuf);
        for (value = peekbuf++; value; value = next) {
            key = strsep(&value, "=");
            if (key) {
                next = value;
                value = strsep(&next, "&");
                if (!value) {
                    value = next;
                    next = NULL;
                }
                
                uh_urldecode(value, safe_strlen(value), value, safe_strlen(value));
                gui_debug("urldecode %s=%s", key, value);
                cgiSetParam(key, value);
            } else
                next = NULL;
        }
    }
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      do_post
 *
 *  [DESCRIPTION]:
 *      save post data to gWebParams
 *  [return]:
 **************************************************************************/
void do_post(struct client *cl, struct http_request *req)
{
    if (cl->peeklen <= 0)   return;
    
    cgiParsePostData(cl->peekbuf);
    
    int len = sizeof(gWebParams.action_page);
    memset(gWebParams.action_page, 0, len);
    strncpy(gWebParams.action_page, req->url, (strlen(req->url)<(len-1)?strlen(req->url):(len-1)));
    
    char* ext = NULL;
    ext = strrchr(gWebParams.action_page, '.');                         
    if (ext != NULL) {
        *ext = '\0';
        strcat(gWebParams.action_page, ".act");
        
        if (safe_strlen(gWebParams.query_string) > 0 && safe_strcmp(gWebParams.query_string, "NULL") != 0)
        {
            strcat(req->url, "?");
            strcat(req->url, gWebParams.query_string);
        }
    }
    gui_debug("gWebParams.action_page=%s", gWebParams.action_page);
}
    
/**************************************************************************
 *  [FUNCTION NAME]:
 *      do_act
 *
 *  [DESCRIPTION]:
 *      
 *  [return]:
 **************************************************************************/
int do_cgiact(struct client *cl, struct http_request *req)
{
    int result = 0;
    gui_debug("req->url=%s", req->url);

#if 0
    //WAN Settings
    if (strstr(req->url, "broadband_wansettings.cgi") != NULL){
        cgiPostWANSettings();
    }
    //Broadband Configure
    else if (strstr(req->url, "broadband_configure.cgi") != NULL){
        cgiPostBroadbandSettings();
    }
    //Local Network
    else if (strstr(req->url, "localnet_sub_dhcp.cgi") != NULL){
        cgiPostLANSubnetDhcp();
    }
    else if (strstr(req->url, "localnet_dev_dns.cgi") != NULL){
        cgiPostLANDeviceDNS();
    }
    else if (strstr(req->url, "localnet_ip.cgi") != NULL){
        cgiPostLANIPAllocation();
    }
    //Firewall
    else if (strstr(req->url, "firewall_rules.cgi") != NULL){
        cgiPostFirewallRules();
    }
    //Firewall ip passthru
    else if (strstr(req->url, "firewall_ip_passthru.cgi") != NULL){
        cgiPostFirewallIPPassthru();
    }
    //Firewall - Pinholes
    else if (strstr(req->url, "pinholesRule.cgi") != NULL){
        cgiPostPinholesRule();
    }
    else if (strstr(req->url, "pinholesService.cgi") != NULL){
        cgiPostPinholesService();
    }
    //Firewall Advanced
    else if (strstr(req->url, "firewall_adv_set.cgi") != NULL){
        cgiPostFirewallAdvancedSettings();
    }
    //reboot
    else if (strstr(req->url, "restart.cgi") != NULL){
        cgiPostRestart();
    }
#endif    
    //WiFi - Basic
    if (strstr(req->url, "wl_basic.cgi") != NULL){
       cgiPostWirelessBasic();
    }

    //WiFi - Security
    else if (strstr(req->url, "wl_security.cgi") != NULL){
       cgiPostWirelessSecurity();
    }
    //WiFi - Adv
    else if (strstr(req->url, "wl_adv.cgi") != NULL){
       cgiPostWirelessAdv();
    }  
    //WiFi - WPS
    else if (strstr(req->url, "wl_wfps.cgi") != NULL){
       cgiPostWirelessWPS();
    }  
    //WiFi - WAC
    else if (strstr(req->url, "wl_wac.cgi") != NULL){
       cgiPostWirelessMacFilter();
    }           
    //Advanced - Management
    else if (strstr(req->url, "adv_manage.cgi") != NULL){
       cgiPostAdvManagement();
    }
    else if (strstr(req->url, "adv_reboot.cgi") != NULL){
       cgiPostAdvReboot();
    }
    else if(strstr(req->url, "adv_default.cgi") != NULL){
        cgiPostFactoryDefault();
    }
    else if(strstr(req->url, "adv_language.cgi") != NULL){
        cgiPostAdvLanguage();
    }    
    else if(strstr(req->url, "adv_password.cgi") != NULL){
        cgiPostAdvPassword();
    }    
    else if(strstr(req->url, "forgot_password.cgi") != NULL){
        cgiPostForgotPassword();
    }     
    else if(strstr(req->url, "adv_pwd_err_clear.cgi") != NULL){
        cgiPostAdvPwdErrClear();
    }    
    else if(strstr(req->url, "adv_tr69.cgi") != NULL){
        cgiPostAdvTR69();
    }
    else if(strstr(req->url, "adv_localssh.cgi") != NULL){
        cgiPostAdvLocalssh();
    }
    else if (strstr(req->url, "adv_swap.cgi") != NULL){
       cgiPostAdvSwap();
    }
    else if(strstr(req->url, "adv_systemlog.cgi") != NULL){
        cgiPostAdvSystemLog();
    }
    else if(strstr(req->url, "hnap_log.cgi") != NULL){
        cgiPostHnapLog();
    }
    else if(strstr(req->url, "adv_autoupgrade.cgi") != NULL){
        cgiPostAdvAutoUpgrade();
    }

    //Moca Setup
    else if (strstr(req->url, "moca_setup.cgi") != NULL){
       cgiPostMocaSetup();
    }
    //upgrade
    else if (strstr(req->url, "adv_upgrade.cgi") != NULL){
       result = gWebParams.upgrade_status;
       gui_debug("result = %d", result);
    }
    else if (strstr(req->url, "adv_option60_add.cgi") != NULL){
       cgiPostOption60Add();
    }
    else if (strstr(req->url, "adv_option60_modify.cgi") != NULL){
       cgiPostOption60Modify();
    }
    else if (strstr(req->url, "adv_option60_delete.cgi") != NULL){
       cgiPostOption60Delete();
    }
    else if (strstr(req->url, "adv_serial.cgi") != NULL){
       cgiPostAdvSerialPort();
    }
#if 0    
    //WiFi - Basic - Radio Reset
    else if (strstr(req->url, "localnet_wifi_radioReset.cgi") != NULL){
       cgiPostWirelessRadioReset();
    }
    //WiFi - WPS
    else if (strstr(req->url, "localnet_wifi_wps.cgi") != NULL){
       cgiPostWirelessAdvancedWps();
    }
    //WiFi - MAC Filter
    else if (strstr(req->url, "localnet_mac.cgi") != NULL){
       cgiPostMacFilter();
    }
    //Access Code
    else if(strstr(req->url, "changeUserInfo.cgi") != NULL){
        cgiPostChangeUserInfo(cl, req);
    }
    //Diagnostic - Resets
    else if(strstr(req->url, "diagnostic_reset_ip.cgi") != NULL){
        cgiPostResets();
    }
    else if(strstr(req->url, "diagnostic_reset_device.cgi") != NULL){
        cgiPostResets();
    }
    else if(strstr(req->url, "diagnostic_reset_conn.cgi") != NULL){
        cgiPostResets();
    }
    //Diagnostics Logs
    else if(strstr(req->url, "clearLogs.cgi") != NULL){
        cgiClearLogs();
    }
    //Diagnostics Troubleshoot
    else if(strstr(req->url, "diagnostic_troubleshoot.cgi") != NULL){
        cgiPostTroubleShoot();
    }
    //Diagnostics NAT ALG
    else if(strstr(req->url, "diagnostic_nat_alg.cgi") != NULL){
        cgiPostNatALGSettings();    
    }
    //Voice
    else if(strstr(req->url, "voice_line.cgi") != NULL){
        cgiPostVoiceLine();    
    }
    //IPV6 - WAN
    else if(strstr(req->url, "broadband_ipv6_wansettings.cgi") != NULL){
        cgiPostIPv6WANSettings();    
    }
    //IPV6 - Firewall
    else if(strstr(req->url, "firewall_adv_set_ipv6.cgi") != NULL){
        cgiPostIPv6FirewallAdvSettings();    
    }
    //IPV6 - LAN
    else if(strstr(req->url, "localnet_ipv6_config.cgi") != NULL){
        cgiPostIPv6LANSettings();    
    }
    //Local Network - Configure
    else if(strstr(req->url, "localnet_config.cgi") != NULL){
        cgiPostPortConfigure();    
    }
    //Remote GUI
    else if(strstr(req->url, "device_remote_access.cgi") != NULL){
        cgiPostRemoteGUIGSettings();    
    }
    //Diagnostics Troubleshoot
    else if(strstr(req->url, "diagnostic_syslog.cgi") != NULL){
        cgiPostSyslogSettings();    
    }
    //Connection to AT&T
    else if(strstr(req->url, "device_status_wan.cgi") != NULL){
        cgiPostRestartWAN();
    }
#endif
    return result;
}


//get data from glbal parameters(in memory).
PARAM_ITEM GetParams[] = {
    //get cli_addr
    { "cli_addr", (void *)&gWebParams.cli_addr, PARAM_TYPE_STR},
    //save settings
    { "g_apply_page", (void *)&gWebParams.apply_page, PARAM_TYPE_STR},
    { "g_waiting_page", (void *)&gWebParams.waiting_page, PARAM_TYPE_STR},
    { "g_waitingtopmenu", (void *)&gWebParams.waiting_page_topmenu, PARAM_TYPE_STR},
    { "g_waitingleftmenu", (void *)&gWebParams.waiting_page_leftmenu, PARAM_TYPE_STR},
    
    { "g_action_page", (void *)&gWebParams.action_page, PARAM_TYPE_STR},
 
    { "login_username", (void *)&gWebParams.login_username, PARAM_TYPE_STR },
    { "wcb_main_login_success", (void *)&gWebParams.wcb_main_login_success, PARAM_TYPE_NUM},
    { "cur_lang_stat", (void *)&gWebParams.cur_lang_stat, PARAM_TYPE_NUM},    
    { "change_lang", (void *)&gWebParams.change_lang, PARAM_TYPE_NUM},    
    { "change_pwd_old_error", (void *)&gWebParams.change_pwd_old_error, PARAM_TYPE_NUM},
    

    // parental
    { "parental_index", (void *)&gWebParams.parental_index, PARAM_TYPE_STR},
    // end parental
    
    // advanced             
    { "advanced_filter_mac_index", (void *)&gWebParams.advanced_filter_mac_index, PARAM_TYPE_STR },
    { "advanced_filter_macaddress_add", (void *)&gWebParams.advanced_filter_macaddress_add, PARAM_TYPE_STR },
    { "advanced_filter_mac_page", (void *)&gWebParams.advanced_filter_mac_page, PARAM_TYPE_STR },
    { "advanced_dhcp_accesscontrol_page", (void *)&gWebParams.advanced_dhcp_accesscontrol_page, PARAM_TYPE_STR },
    { "advanced_ddns_index", (void *)&gWebParams.advanced_ddns_index, PARAM_TYPE_STR },
    { "advanced_dnsserver_index", (void *)&gWebParams.advanced_dnsserver_index, PARAM_TYPE_STR },
    { "advanced_routing_index", (void *)&gWebParams.advanced_routing_index, PARAM_TYPE_STR},
    { "advanced_routing_add_flag", (void *)&gWebParams.advanced_routing_add_flag, PARAM_TYPE_STR},
    { "advanced_ipdistribution_index", (void *)&gWebParams.advanced_ipdistribution_index, PARAM_TYPE_STR},
    { "advanced_dhcpconnection_index", (void *)&gWebParams.advanced_dhcpconnection_index, PARAM_TYPE_STR},
    { "advanced_timeserver_index", (void *)&gWebParams.advanced_timeserver_index, PARAM_TYPE_STR},
    { "advanced_networkobjects_index", (void *)&gWebParams.advanced_networkobjects_index, PARAM_TYPE_STR},
    { "advanced_networkobjects_itemindex", (void *)&gWebParams.advanced_networkobjects_itemindex, PARAM_TYPE_STR},

    //upgrade status, -9:init;  9:upload finished.  -1:image error; 0:success
    { "upgrade_status", (void *)&gWebParams.upgrade_status, PARAM_TYPE_NUM},
    { "advanced_tr69_reboot", (void *)&gWebParams.advanced_tr69_reboot, PARAM_TYPE_NUM},

    // end advanced
    //Access Code
    {"pwd_reset_result", (void *)&gWebParams.pwd_reset_result, PARAM_TYPE_STR },
    //Diagnostics Troubleshoot
    {"troubleshoot_protocol", (void *)&gWebParams.troubleshoot_protocol, PARAM_TYPE_STR },
    {"action", (void *)&gWebParams.action, PARAM_TYPE_STR },
    {"btn_action", (void *)&gWebParams.btn_action, PARAM_TYPE_STR },    
    {"troubleshoot_address", (void *)&gWebParams.troubleshoot_address, PARAM_TYPE_STR},
    { NULL, NULL, PARAM_TYPE_NONE }
};

//get data form custom function.(datamodel)
TAG_VAR GetParamsTable[] = {
#if 0
    //utils
    {"query_string", cgiGetQueryString},
    //Local Network
    {"local_net_status", cgiGetLocalNetStatus},
    {"local_net_v4_static", cgiGetLANIPv4Statistics}, 
    {"local_net_wifi_status", cgiGetLANWiFiStatus}, 
    {"local_net_wifi_statistics", cgiGetLANWiFiStatistics}, 
    {"local_net_eth_statistics", cgiGetLANEthStatistics},
    {"local_sub_dhcp", cgiGetLANSubnetDhcp},
    {"local_dns_mapping_list", cgiGetLANDNSHostMapping},
    {"local_dev_list", cgiGetLANDeviceList},
    {"local_ip_allo_list", cgiGetLANIPAllocationList},

    //Firewall status
    {"firewall_status", cgiGetFirewallStatus},

    //Firewall ip passthru
    {"firewall_ip_passthru", cgiGetFirewallIPPassthru},
    
    //Firewall rules
    {"firewall_rule_enable", cgiGetFirewallRuleEnable},
    {"firewall_rules", cgiGetFirewallRules},
    
    //Firewall Pinholes
    {"getAllPinholesServices",getAllPinholesServices},
    {"getAllPinholesRules", getAllPinholesRules},
    //Firewall Advanced
    {"getFirewallAdvancedSettings", cgiGetFirewallAdvancedSettings},
    //WAN Settings
    {"ip_settings", cgiGetIPSettings},
    {"dns_settings", cgiGetDNSSettings},
    //Broadband Configure
    {"broadband_settings", cgiGetBroadbandSettings},
    {"autoMode", cgiGetAutoMode},
    //Broadband Status
    {"broadband_status", cgiGetBroadbandStatus},
    //Broadband IGMP
    {"igmp_proxyInfo", cgiGetIGMPProxyInfo},
    {"igmp_snooping_stats", cgiGetIGMPSnoopingStats},
    {"igmp_summary", cgiGetIGMPSummary},
    {"igmp_group_hosts", cgiGetIGMPGroupHosts},
    //Device
    {"sysInfo", cgiGetSystemInfo},
#endif 
    //Wireless Basic
    {"wireless_multiple_ssid", cgiGetSSIDList},
    {"wireless_channel_list", cgiGetChannelList},
    {"wireless_basic_status", cgiGetWirelessBasic},
    {"wireless_net_list", cgiGetWiFiNetList}, 
    //Wcb Main
    {"main_product_info", cgiGetMainProductInfo},  
    {"main_net_station", cgiGetMainNetStation},    
    

    //Wireless Security
    {"wireless_security_status", cgiGetWirelessSecurity},
    //Wireless Advanced
    {"wireless_advanced", cgiGetWirelessAdvanced},    
    //Wireless WPS
    {"wireless_wps_status", cgiGetWirelessWPS},   
    //Wireless Access Control
    {"wireless_access_control", cgiGetWirelessMacFilter},       
    //Advanced Management
    {"adv_management", cgiGetAdvManagement}, 
    {"main_cur_ipinfo", cgiGetCurIPInfo}, 
    {"dev_serial_num", cgiGetDevSerialNum},     
    {"adv_tr69", cgiGetAdvTR69},
    {"adv_localssh", cgiGetAdvLocalssh},
    {"adv_swapimageversion", cgiGetSwapImageVersion},
    {"adv_systemlog", cgiGetAdvSystemLog},
    {"hanplog_info", cgiGetHnapLogInfo},
    {"hanplog_content", cgiGetHnapLogContent},

    {"isQuantennaCard", cgiGetQuantennaCard},

    {"option60list", cgiGetOption60List},
    {"adv_autoupgrade", cgiGetAdvAutoUpgrade},
    
    //Factory Default Redirect Page
    {"redirect_ip", cgiGetRedirectIP}, 
    //Zero Conf
    {"act_zeroconf_upgrade", actZeroConfUpgrade}, 
    {"act_zeroconf_autosync", actZeroConfAutoSync},    

    //moca infomation
    {"moca_info", cgiGetMocaInfo},
    {"moca_status", cgiGetMocaStatus},

    //user_level
    {"user_level", cgiGetUserLevel},

    //DeviceInfo
    {"model_number", cgiGetModelNumber},

    //seiral port
    {"adv_serialport", cgiGetAdvSerialPort},
#if 0    
    //Wireless WPS
    {"wireless_wps", cgiGetWirelessAdvancedWps},

    //Voice
    {"voice_status",cgiGetVoiceStatus},
    {"voice_call_summary",cgiGetVoiceCallSummary},
    {"voice_cumulative_last",cgiGetVoiceCumuLast},
    {"voice_statistics",cgiGetVoiceStatistics},
    
    //Wireless Status
    {"wireless_status", cgiGetWirelessStatus},
    //Wireless MAC Filter
    {"wireless_macFilter", cgiGetMacFilter},
    //Diagnostics Logs
    {"diagnosticsLogs", cgiGetLogs},
    //Diagnostics NAT Table
    {"natTable", cgiGetNatTable},
    {"natALGSettings", cgiGetNatALGSettings},
    //IPV6 Broadband Status
    {"ipv6_wan_status", cgiGetIPv6WANStatus},
    {"ipv6_wan_protocol", cgiGetIPv6WANCurMode},
    {"ipv6_wan_enabled", cgiGetIPv6WANEnabled},
    {"ipv6_wan_6rdNUDResult", cgiGet6rdNUDResult},
    {"ipv6_wan_6rdSettings", cgiGet6rd},
    {"ipv6_wan_staticSettings", cgiGetIPv6WANStatic},
    //IPV6 - Firewall
    {"ipv6_firewallAdvancedSettings", cgiGetIPv6FirewallAdvSettings},
    //IPV6 - LAN
    {"local_net_ipv6_stat", cgiGetLocalNetIPv6Status},
    //{"local_net_v6_static", cgiGetLANIPv6Statistics},
    {"ipv6_lan_settings", cgiGetIPv6LANSettings},
#endif
    
    //Update firmware
    {"update_progress", cgiGetUpdateProgress},
#if 0
    {"currentFirmVer", cgiGetCurFirmVersion},
    
    //Diagnostics Troubleshoot
    {"pingResult", cgiGetPingResult},
    {"tracerouteResult", cgiGetTracerouteResult},
    {"NSLookupResult", cgiGetNSLookupResult},
    //Local Network - Configure
    {"getPortConfigure", cgiGetPortConfigure},
    //Broadband connection status
    {"bb_PhysicalLinkStatus", cgiGetBBPhysicalLinkStatus},
    {"bb_IPConnectionStatus", cgiGetBBIPConnectionStatus},
    {"bb_TotalBytesReceived", cgiGetBBTotalBytesReceived},
    //Remote GUI
    {"remoteGUISettings", cgiGetRemoteGUIGSettings},
    //Diagnostics Troubleshoot
    {"syslogSettings", cgiGetSyslogSettings},
#ifdef AEI_SYSTEM_BATTERY
    {"BatteryStatus", cgiGetBatteryStatus},
    {"BatteryInfo", cgiGetBatteryInfo},
    {"TemperatureStatus", cgiGetTemperatureInfo},
#endif
#endif
    { NULL, NULL }
};

void cgiEchoParameter(int argc, char **argv, char *varValue)
{
    int i = 0;
    
    if ( argc < 2) return;
    
    for ( ; GetParams[i].key != NULL; i++ )
        if ( safe_strcmp(argv[1], GetParams[i].key) == 0 )
            break;
            
    if ( i < (ARRAY_SIZE(GetParams)-1) )
    {
        if ( GetParams[i].key != NULL ) {
            switch ( GetParams[i].type ) {
                case PARAM_TYPE_STR:
                    strcpy(varValue, (char *)GetParams[i].value);
                    break;
                case PARAM_TYPE_NUM:
                    sprintf(varValue, "%d", *((int *)GetParams[i].value));
                    break;
                default:
                    varValue[0] = '\0';
                    break;
            }
        }
    }
    else
    {
        for (i=0 ; i < ARRAY_SIZE(GetParamsTable); i++ )
        {
            if ( safe_strcmp(argv[1], GetParamsTable[i].tagName) == 0 )
                break;
        }
        //printf("--%s: i=%d\n", __FUNCTION__, i);
        if (i < ARRAY_SIZE(GetParamsTable)-1)
        {
            struct timeval tv1;
            struct timeval tv2;
            gettimeofday(&tv1, NULL);
            
            (*(GetParamsTable[i].tagFunc))(argc, argv, varValue);
            //gui_debug("cgi get end, table index=%d, argc=%d, argv[1]=%s, varValue=%s\n", i, argc, argv[1], varValue);
            
            gettimeofday(&tv2, NULL);
            gui_debug("GetParamsTable[%d].tagName=%s, get time=%f", i, GetParamsTable[i].tagName, gui_distime(tv1, tv2));
        }
        else    
            varValue[0] = '\0';
    }
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      do_download
 *
 *  [DESCRIPTION]:
 *      download file
 *  [return]:
 **************************************************************************/
void do_download(struct client *cl, struct http_request *req)
{
    if (strstr(req->url, "saveLogs.conf") != NULL)
    {
        //cgiSaveLog(cl, req);
    }
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetQueryString
 *
 *  [DESCRIPTION]:
 *      get query string from url path
 *  [return]:
 **************************************************************************/
void cgiGetQueryString(int argc, char **argv, char *varValue)
{
    sprintf(varValue, "%s", gWebParams.query_string);
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      getValueFromQueryString
 *
 *  [DESCRIPTION]:
 *      get value with key from QUERY_STRING
 *  [return]:
 **************************************************************************/
void getValueFromQueryString(char *des, char *query_string, char *key)
{
    char *name, *value, *next;
    char buf[BUFLEN_1024] = {0};
    char *pbuf = &buf[0];
    strncpy(buf, query_string, safe_strlen(query_string));
     
    if (pbuf) 
    {
        for (value = pbuf++; value; value = next) 
        {
            name = strsep(&value, "=");
            if (name) {
                next = value;
                value = strsep(&next, "&");
                if (!value) {
                    value = next;
                    next = NULL;
                }
                gui_debug("key=%s, value=%s, strlen(value)=%d", key, value, safe_strlen(value));
                if (strcmp(name, key) == 0)
                {
                    strcpy(des, value);
                    break;
                }
            } else
                next = NULL;
        }
    }
    gui_debug("query_string=%s, des=%s", query_string, des);
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      getSpecialOidFromSubmitOid
 *
 *  [DESCRIPTION]:
 *      get oid; eg: submit_oid is IDG.LANXXX.1.XXX|IDG.WANXXX.1.XXXX|IDG.BRIXXX.1.XXX, idx is 1, des will get IDG.WANXXX.1.XXXX
 *  [return]:
 **************************************************************************/
void getSpecialOidFromSubmitOid(char *des, char *submit_oid, int idx)
{
    char *del = "|";
    char *p;
    char str[256];
    int num = 0;

    strncpy(str, submit_oid, safe_strlen(submit_oid));
    p = strtok(str, del);
    while (p )
    {       
        if (num == idx) 
        {               
            sprintf(des, "%s", p);
            break;
        }
        p=strtok(NULL, del);
        num ++;
    }
}


void cgiGetDataFromDM(char *key, char *varValue)
{
    char *parameter;
    int type;
    
    int r1 = tr69_get_leaf_data(key, (void *)&parameter, &type);
    if (r1 == 0){
        if (type ==  TR69_NODE_LEAF_TYPE_STRING){
            if (parameter == NULL)
                sprintf(varValue, "%s", "NULL");
            else {
                sprintf(varValue, "%s", parameter);
                free(parameter);
            }
        }else {
            if (parameter == NULL)
                sprintf(varValue, "%s", "NULL");
            else
                sprintf(varValue, "%d", (int)parameter);        
        }
    }else {
        sprintf(varValue, "%s", "NULL");
    }
    printf("%s: key=%s, r1=%d, varValue=%s\n", __FUNCTION__, key, r1, varValue);
}


char *getValueFromDal(char *v)
{   
    if (v && safe_strlen(v)>0)
    {
#if HAVE_XSS
        int idx = gWebParams.dal_buffer_idx % DAL_BUFFER_LENGTH;
        memset(gWebParams.dal_buffer[idx], 0, sizeof(gWebParams.dal_buffer[idx]));
        
        if (safe_strlen(v) >= sizeof(gWebParams.dal_buffer[idx]))
            return v;
        else
        {
            strncpy(gWebParams.dal_buffer[idx], v, safe_strlen(v));
            //gui_debug("before--> %s", gWebParams.dal_buffer[idx]);
            GUI_SpeciCharEncode(gWebParams.dal_buffer[idx], sizeof(gWebParams.dal_buffer[idx]));
            //gui_debug("idx = %d, after--> %s", idx, gWebParams.dal_buffer[idx]);
            
            gWebParams.dal_buffer_idx++;
            gWebParams.dal_buffer_idx = gWebParams.dal_buffer_idx % DAL_BUFFER_LENGTH;
            
            return &gWebParams.dal_buffer[idx][0];
        }       
#else       
        return v;
#endif                      
    }
    
    return "NULL";
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      isxdigit_str
 *
 *  [DESCRIPTION]:
 *      isxdigit for string
 *
 *  [return]:1:HEX 0:ASCII
 **************************************************************************/
int isxdigit_str(char* str)
{
    int i = 0;

    for (; i<safe_strlen(str); i++)
    {
        if (isxdigit(str[i]))
        {
            continue;
        }
        else
        {
            return 0;
        }
    }

    return 1;
}

unsigned int Ascii2Hex(char *ascii, unsigned int ascii_len, char *hex)
{
 int i, j;
 char data[2];
 
 if(NULL==ascii||0==ascii_len||NULL==hex)
 {
  return 0;
 }

 for(i=ascii_len-1; i>=0; i--)
 {
  data[0] = ascii[i]&0x0F;
  data[1] = ascii[i] >> 4;
  for(j=0; j<2; j++)
  {
   if(data[j]>=0&&data[j]<=9)
   {
    data[j] += 0x30;
   }
   else if(data[j]>=0x0a&&data[j]<=0x0f)
   {
    data[j] += 'a'-0x0a;
   }
   else
   {
    return 0;
   }
  }
  
  j = i<<1;
  hex[j+1] = data[0];
  hex[j+0] = data[1];
 }

 return (ascii_len<<1);
}

unsigned int Hex2Ascii(char *hex, unsigned int hex_len, char *ascii)
{
    int i, j;
    char data[2];
    gui_debug("hex=%s hex_len=%d ascii=%s", hex, hex_len, ascii);

    if(NULL==hex||0==hex_len||hex_len%2!=0||NULL==ascii)
    {
        return 0;
    }

    for (i=0; i<hex_len; i+=2)
    {
        data[0] = hex[i];
        data[1] = hex[i+1];

        for (j=0; j<2; j++)
        {
            if (data[j]>='0'&&data[j]<='9')
            {
                data[j] -= 0x30;
            }
            else if (data[j]>='a'&&data[j]<='f')
            {
                data[j] -= 'a' - 0x0a;
            }
            else
            {
                return 0;
            }
        }
        
        ascii[i>>1] = (data[0]&0x0F) << 4 | (data[1]&0x0F);
    }
    
    gui_debug("hex=%s hex_len=%d ascii=%s", hex, hex_len, ascii);
    return (hex_len>>1);
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      getMemInfo
 *
 *  [DESCRIPTION]:
 *      get value from /proc/meminfo by name
 *  [return]:
 *      meminfo value(int)
 **************************************************************************/
int getMemInfo(char *key)
{
    char line[96] = {0};
    int value = 0;
    FILE *fp = NULL;
    
    fp = fopen("/proc/meminfo", "r");
    if(fp == NULL){
        return 0;
    }
    
    while(fgets(line, sizeof(line), fp))
    {
        char *rs;
        if((rs = strstr(line, key)) != NULL)
        {
            value = atoi(rs+strlen(key));
            break;
        }
    }
    
    fclose(fp);
    return value;
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgigetfirsttimelogin
 *
 *  [DESCRIPTION]:
 *       get InternetGatewayDevice.X_ACTIONTEC_SystemConfig.HttpFirstTimeLogin
 *  [return]:
 **************************************************************************/
void cgigetfirsttimelogin(int argc, char **argv, char *varValue)
{
#if 0 //sharko should modify here.
    dal_ret_t *ret_get = NULL;

    ret_get = dal_sys_get_basic_router_cfg();
    if (ret_get == NULL)
        return;

    gui_debug("ret_get->param[SYS_CFG_FIRSTTIME_LOGIN]=%s", ret_get->param[SYS_CFG_FIRSTTIME_LOGIN]);
    strcat(varValue, getValueFromDal(ret_get->param[SYS_CFG_FIRSTTIME_LOGIN]));

    if (ret_get != NULL)
        free_dal_ret(&ret_get);
#endif
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgipostfirsttimelogin
 *
 *  [DESCRIPTION]:
 *      firsttime_login.html form post
 *  [return]:
 *      TSL_RV_SUC/TSL_RV_FAIL
 **************************************************************************/
int cgipostfirsttimelogin(void)
{
#if 0 //sharko should modify here.
    dal_ret_t *ret_get = NULL;
    int num = 0;
    dal_ret_t ret_post;
    char encode_pw[BUFLEN_256] = {0};
    tsl_rv_t flag = TSL_RV_SUC;

    gui_debug("gWebParams.firsttime_login_password=%s", &gWebParams.firsttime_login_password[0]);

    ret_get = dal_sys_get_user_list(&num);
    if (ret_get == NULL)
        return TSL_RV_FAIL;

    for (int i=0; i<num; i++)
    {
        if (!tsl_strcmp(getValueFromDal(ret_get[i].param[USER_NAME]), "admin"))
        {
            memset(&ret_post, 0, sizeof(ret_post));
            ret_post.param[USER_FULLNAME] = ret_get[i].param[USER_FULLNAME];
            ret_post.param[USER_NAME] = ret_get[i].param[USER_NAME];
            ret_post.param[USER_PASSWORD] = &gWebParams.firsttime_login_password[0];
            ret_post.param[USER_AUTH_LEVEL] = ret_get[i].param[USER_AUTH_LEVEL];
            ret_post.param[USER_PID] = ret_get[i].param[USER_PID];
            tsl_encode(gWebParams.firsttime_login_password, encode_pw);
            ret_post.param[USER_CRYPT_PWD] = &encode_pw[0];
        }
    }

    gui_debug("ret_post.param[USER_FULLNAME]=%s", ret_post.param[USER_FULLNAME]);
    gui_debug("ret_post.param[USER_NAME]=%s", ret_post.param[USER_NAME]);
    gui_debug("ret_post.param[USER_PASSWORD]=%s", ret_post.param[USER_PASSWORD]);
    gui_debug("ret_post.param[USER_AUTH_LEVEL]=%s", ret_post.param[USER_AUTH_LEVEL]);
    gui_debug("ret_post.param[USER_PID]=%s", ret_post.param[USER_PID]);
    gui_debug("ret_post.param[USER_CRYPT_PWD]=%s", ret_post.param[USER_CRYPT_PWD]);

    flag = dal_sys_set_user(&ret_post);

    memset(&ret_post, 0, sizeof(ret_post));
    ret_post.param[SYS_CFG_FIRSTTIME_LOGIN] = "false";
    gui_debug("ret_post.param[SYS_CFG_FIRSTTIME_LOGIN]=%s", ret_post.param[SYS_CFG_FIRSTTIME_LOGIN]);
    flag = dal_sys_set_basic_router_cfg(&ret_post);

    flag = dal_SaveConfig();

    if (ret_get != NULL)
        free_dal_ret(&ret_get);

    return flag;
#endif
    return 0;
}


/* 0: LAN 1: WAN */
int getDeviceType(char *ifname)
{
    if (tsl_strcmp(ifname, NETWORK_INTERFACE_ETH0) == 0
        || tsl_strcmp(ifname, NETWORK_INTERFACE_ETH2) == 0
        || tsl_strncmp(ifname, "ath", 3) == 0)
        return 0;
    else if (tsl_strcmp(ifname, NETWORK_INTERFACE_ETH10) == 0
        || tsl_strncmp(ifname, "ppp", 3) == 0)
        return 1;

    return 0;
}


