#include "uhttpd.h"
#include "uhttpd-utils.h"
#include "cgi_main.h"
#include "cgi_wireless.h"
#include "dal_wifi.h"

#define INDEX_SSID 1
#define INDEX_VLAN 0
#define MAX_LIST_COUNT  16
ssid_info_t ssid_list_info[MAX_LIST_COUNT] = {0}; 

/**************************************************************************
 *  [FUNCTION NAME]:
 *      get_cur_vlan_ssid_index
 *
 *  [DESCRIPTION]:
 *
 *           Get Current Index VLAN and Index SSID
 *
 *  [return]:
 **************************************************************************/
static void get_cur_vlan_ssid_index(unsigned int *index_vlan, unsigned int *index_ssid)
{
    char get_from_vlan[8] = {0};
    char get_from_ssid[8] = {0};


    getValueFromQueryString(get_from_vlan, gWebParams.query_string, "index_vlan");
    getValueFromQueryString(get_from_ssid, gWebParams.query_string, "index_ssid");
    gui_debug("get_from_vlan=%s,get_from_ssid=%s\n",get_from_vlan,get_from_ssid);
    if(strlen(get_from_vlan)>=0&&strlen(get_from_ssid)>0)
    {
        *index_vlan = atoi(get_from_vlan);
        *index_ssid = atoi(get_from_ssid);
        gui_debug("111vlan=%d,ssid=%d\n",*index_vlan,*index_ssid);
    }/*
    else if(gWebParams.wireless_index_vlan>=0&&gWebParams.wireless_index_ssid>0)
    {
        *index_vlan = gWebParams.wireless_index_vlan;
        *index_ssid = gWebParams.wireless_index_ssid;
        gui_debug("222vlan=%d,ssid=%d\n",*index_vlan,*index_ssid);
    }*/
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      get_cur_vlan_ssid_index_bypost
 *
 *  [DESCRIPTION]:
 *
 *           Get Current Index VLAN and Index SSID by Post
 *
 *  [return]:
 **************************************************************************/
static void get_cur_vlan_ssid_index_bypost(unsigned int *index_vlan, unsigned int *index_ssid)
{
    *index_vlan = gWebParams.wireless_index_vlan;
    *index_ssid = gWebParams.wireless_index_ssid;
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostWirelessBasic
 *
 *  [DESCRIPTION]:
 *
 *           Post wifi-basic settings
 *
 *  [return]:
 **************************************************************************/
int cgiPostWirelessBasic()
{
    tsl_rv_t flag = TSL_RV_SUC;
    unsigned int index_vlan = INDEX_VLAN;
    unsigned int index_ssid = INDEX_SSID;
    wep_data_t data_wep = {0};
    wpa_data_t data_wpa = {0};
    unsigned int standard = 0;
    unsigned int securityType = 0;

    //get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
    get_cur_vlan_ssid_index_bypost(&index_vlan,&index_ssid);

    gui_debug("Post basic vlan=%d,ssid=%d\n",index_vlan,index_ssid);

    //Radio Enable
    gui_debug("Radio Enable: value=%d\n",gWebParams.wireless_radio_enable);
    if(dal_wifi_set_RadioEnabled(index_vlan, index_ssid, gWebParams.wireless_radio_enable)){
        gui_debug("--%s-- FAILED: dal_wifi_set_RadioEnabled--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        flag = -1;
    }

    if (gWebParams.wireless_radio_enable == 1)
    {
        //SSID Enable
        gui_debug("SSID Enable: value=%d\n",gWebParams.wireless_ssid_enable);
        if(dal_wifi_set_SSIDEnable(index_vlan, index_ssid, gWebParams.wireless_ssid_enable)){
            gui_debug("--%s-- FAILED: dal_wifi_set_SSIDEnable--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
            flag = -1;
        }
    }

    if (gWebParams.wireless_radio_enable == 1 && gWebParams.wireless_ssid_enable == 1)
    {
        //Set SSID
        gui_debug("Set SSID: value=%s\n",gWebParams.wireless_ssid);
        if(dal_wifi_set_ssid(index_vlan, index_ssid, gWebParams.wireless_ssid)) {
            gui_debug("--%s-- FAILED: dal_wifi_set_ssid--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
            flag = -1;
        }
        

        //SSID Broadcast
        gui_debug("wl_broadcast: value=%d\n",gWebParams.wl_broadcast);
        if(dal_wifi_set_SSIDAdvertisementEnabled(index_vlan, index_ssid, gWebParams.wl_broadcast)){
            gui_debug("--%s-- FAILED: dal_wifi_set_SSIDAdvertisementEnabled--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
            flag = -1;
        }  

        //Transmit Power
        //wireless_trans_power
        gui_debug("wireless_trans_power: value=%d\n",gWebParams.wireless_trans_power);
        if(dal_wifi_set_TransmitPower(index_vlan, index_ssid, gWebParams.wireless_trans_power)){
            gui_debug("--%s-- FAILED: dal_wifi_set_TransmitPower--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
            flag = -1;
        }

        //Set Security
        gui_debug("Set Security: security type=%d; key=%s; wep_key_len=%d\n",gWebParams.wireless_security_type,gWebParams.wireless_key, gWebParams.wep_key_len);
        switch(gWebParams.wireless_security_type)
        {
            case 0:
                securityType = SECURITY_MODE_OFF;
                break;
            case 1:
                data_wep.key_index = 0;
                data_wep.eap_port = 0;
                
                if(gWebParams.wep_key_len==5||gWebParams.wep_key_len==13)
                {
                    data_wep.key[data_wep.key_index].type = 1;
                    if(gWebParams.wep_key_len==5)
                        securityType = SECURITY_MODE_WEP64;
                    else
                        securityType = SECURITY_MODE_WEP128;
                }
                else if(gWebParams.wep_key_len==10||gWebParams.wep_key_len==26)
                {
                    data_wep.key[data_wep.key_index].type = 0;
                    if(gWebParams.wep_key_len==10)
                        securityType = SECURITY_MODE_WEP64;
                    else
                        securityType = SECURITY_MODE_WEP128;
                }

                if(gWebParams.wep_auth_mode == 0)
                {
                    data_wep.wep_auth_mode.open = 1;
                    data_wep.wep_auth_mode.share= 0;
                }
                else if(gWebParams.wep_auth_mode == 1)
                {
                    data_wep.wep_auth_mode.open = 0;
                    data_wep.wep_auth_mode.share= 1;
                }
                else
                {
                    data_wep.wep_auth_mode.open = 1;
                    data_wep.wep_auth_mode.share= 1;
                }                   
                
                snprintf(data_wep.key[data_wep.key_index].psk, sizeof(data_wep.key[data_wep.key_index].psk), "%s", gWebParams.wireless_key);
                snprintf(data_wep.eap_server, sizeof(data_wep.eap_server), "%s", "\0");
                snprintf(data_wep.eap_secret, sizeof(data_wep.eap_secret), "%s", "\0");
                gui_debug("++++++dal_wifi_set_wep+++key=%s;mode=%d\n",data_wep.key[data_wep.key_index].psk, data_wep.key[data_wep.key_index].type);
                if(dal_wifi_set_wep(index_vlan, index_ssid, &data_wep)) {
                    gui_debug("--%s-- FAILED: dal_wifi_set_wep--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
                    flag = -1;
                }
                break;
            case 2:
            case 3:
            case 4:
                if(gWebParams.wireless_security_type == 2)
                {
                    securityType = SECURITY_MODE_PSK_WPA;
                    data_wpa.wpa = 1;
                    data_wpa.wpa2 = 0;
#if defined(AEI_WECB_CUSTOMER_TELUS)
                    //#195926
                    data_wpa.wpa_encrypt_mode.tkip = 1;
                    data_wpa.wpa_encrypt_mode.aes = 0;
#else
                    if(gWebParams.wpa_encrypt_mode==0)
                    {
                        data_wpa.wpa_encrypt_mode.tkip = 1;
                        data_wpa.wpa_encrypt_mode.aes = 0;
                    }
                    else if(gWebParams.wpa_encrypt_mode==1)
                    {
                        data_wpa.wpa_encrypt_mode.tkip = 0;
                        data_wpa.wpa_encrypt_mode.aes = 1;
                    }
                    else
                    {
                        data_wpa.wpa_encrypt_mode.tkip = 1;
                        data_wpa.wpa_encrypt_mode.aes = 1;
                    } 
#endif
                }
                else if(gWebParams.wireless_security_type == 3)
                {
                    securityType = SECURITY_MODE_PSK_WPA2;
                    data_wpa.wpa = 0;
                    data_wpa.wpa2 = 1;
#if defined(AEI_WECB_CUSTOMER_TELUS)
                    //#195926
                    data_wpa.wpa2_encrypt_mode.tkip = 0;
                    data_wpa.wpa2_encrypt_mode.aes = 1;
#else
                    if(gWebParams.wpa_encrypt_mode==0)
                    {
                        data_wpa.wpa2_encrypt_mode.tkip = 1;
                        data_wpa.wpa2_encrypt_mode.aes = 0;
                    }
                    else if(gWebParams.wpa_encrypt_mode==1)
                    {
                        data_wpa.wpa2_encrypt_mode.tkip = 0;
                        data_wpa.wpa2_encrypt_mode.aes = 1;
                    }
                    else
                    {
                        data_wpa.wpa2_encrypt_mode.tkip = 1;
                        data_wpa.wpa2_encrypt_mode.aes = 1;
                    }                    
#endif
                }
                else
                {
                    securityType = SECURITY_MODE_PSK_WPAWPA2;
                    data_wpa.wpa = 1;
                    data_wpa.wpa2 = 1;
#if defined(AEI_WECB_CUSTOMER_TELUS)
                    //#195926
                    data_wpa.wpa_encrypt_mode.tkip = 1;
                    data_wpa.wpa_encrypt_mode.aes = 1;                    
                    data_wpa.wpa2_encrypt_mode.tkip = 1;
                    data_wpa.wpa2_encrypt_mode.aes = 1;
#else
                    if(gWebParams.wpa_encrypt_mode==0)
                    {
                        data_wpa.wpa_encrypt_mode.tkip = 1;
                        data_wpa.wpa_encrypt_mode.aes = 0;                    
                        data_wpa.wpa2_encrypt_mode.tkip = 1;
                        data_wpa.wpa2_encrypt_mode.aes = 0;
                    }
                    else if(gWebParams.wpa_encrypt_mode==1)
                    {
                        data_wpa.wpa_encrypt_mode.tkip = 0;
                        data_wpa.wpa_encrypt_mode.aes = 1;                    
                        data_wpa.wpa2_encrypt_mode.tkip = 0;
                        data_wpa.wpa2_encrypt_mode.aes = 1;
                    }
                    else
                    {
                        data_wpa.wpa_encrypt_mode.tkip = 1;
                        data_wpa.wpa_encrypt_mode.aes = 1;                    
                        data_wpa.wpa2_encrypt_mode.tkip = 1;
                        data_wpa.wpa2_encrypt_mode.aes = 1;
                    }        
#endif
                }
   
                data_wpa.key_index = 0;
                data_wpa.group_key_update_interval = 1200;
                data_wpa.eap_port = 0;
                /*
                data_wpa.wpa_encrypt_mode.tkip = 0;
                data_wpa.wpa_encrypt_mode.aes = 1;
                data_wpa.wpa2_encrypt_mode.tkip = 0;
                data_wpa.wpa2_encrypt_mode.aes = 1;
                */
                data_wpa.wpa_auth_mode.psk = 1;
                data_wpa.wpa_auth_mode.eap = 0;
                data_wpa.wpa2_auth_mode.psk = 1;
                data_wpa.wpa2_auth_mode.eap = 0;
                snprintf(data_wpa.key[data_wpa.key_index].psk, sizeof(data_wpa.key[data_wpa.key_index].psk), "%s", gWebParams.wireless_key);
                snprintf(data_wpa.eap_server, sizeof(data_wpa.eap_server), "%s", "\0");
                snprintf(data_wpa.eap_secret, sizeof(data_wpa.eap_secret), "%s", "\0");
                gui_debug("++++++dal_wifi_set_wpa+++key=%s\n",data_wpa.key[data_wpa.key_index].psk);
                if(dal_wifi_set_wpa(index_vlan, index_ssid, &data_wpa)) {
                    gui_debug("--%s-- FAILED: dal_wifi_set_wpa--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
                    flag = -1;
                }
                break;
        }
        gui_debug("security Type=%d\n",securityType);
        if(dal_wifi_set_security_mode(index_vlan, index_ssid, securityType)) {
            gui_debug("--%s-- FAILED: dal_wifi_set_security_mode--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
            flag = -1;
        }

        //Set Mode
        gui_debug("Mode=%d\n",gWebParams.wireless_dot11_mode);
        switch(gWebParams.wireless_dot11_mode)
        {
            case 0:
                standard = IEEE80211_B;
                break;
            case 1:
                standard = IEEE80211_G;
                break;
            case 2:
                standard = IEEE80211_N;
                break;
            case 3:
                standard = IEEE80211_B|IEEE80211_G;
                break;
            case 4:
                standard = IEEE80211_G|IEEE80211_N;
                break;
            case 5:
                standard = IEEE80211_B|IEEE80211_G|IEEE80211_N;
                break;
            case 6:
                standard = IEEE80211_A|IEEE80211_N;
                break;
            case 7:
                standard = IEEE80211_A|IEEE80211_N|IEEE80211_AC;
                break;
        }

        if(dal_wifi_set_standard(index_vlan, index_ssid, standard)) {
            gui_debug("--%s-- FAILED: dal_wifi_set_standard--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
            flag = -1;
        }

        //Set Channel && Bandwidth
        unsigned int bandwidth = 0;
        gui_debug("Channel=%d;wireless_channelwidth=%d\n",gWebParams.wireless_channel, gWebParams.wireless_channelwidth);
        switch(gWebParams.wireless_channelwidth){
            case 20:
                bandwidth = BAND_WIDTH_20MHZ;
                break;
            case 40:
                bandwidth = BAND_WIDTH_40MHZ;
                break;
            case 80:
                bandwidth = BAND_WIDTH_80MHZ;
                break;
            default:
                bandwidth = BAND_WIDTH_AUTO;
                break;
        }
        if(dal_wifi_set_channel(index_vlan, index_ssid, gWebParams.wireless_channel, bandwidth, 0)) {
            gui_debug("--%s-- FAILED: dal_wifi_set_channel--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
            flag = -1;
        }
        if(dal_wifi_set_AutoChannelEnable(index_vlan, index_ssid, 0==gWebParams.wireless_channel)) {
            gui_debug("--%s-- FAILED: dal_wifi_set_AutoChannelEnable--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
            flag = -1;
        }
        
        //security type is open and wep, will disable wps
        if (gWebParams.wireless_security_type == 0 || gWebParams.wireless_security_type == 1)
        {
            tsl_rv_t result = TSL_RV_SUC;
            wps_data_t data = {0};
            //unsigned int index_vlan = INDEX_VLAN;
            //unsigned int index_ssid = INDEX_SSID;

            get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
            data.enable = 0;

            result = dal_wifi_set_wps(index_vlan, index_ssid, &data);
        }

        //disable Auto Configure #192793  
        gui_debug("gWebParams.prissidchanged = %d", gWebParams.prissidchanged);
        if (gWebParams.prissidchanged == 1){
            if(tr69SetLeafDataInt("Device.X_ACTIONTEC_COM_ZeroConf.Extender", 0, "Enable", 0)){
                gui_debug("Set Device.X_ACTIONTEC_COM_ZeroConf.Extender Enable error!");
            }
            tr69SaveNow();
        }
        
    }


    flag = dal_wifi_set_commit(index_vlan, index_ssid);
//    flag = dal_SaveConfig();

    gui_debug("\n\n>>>> Leave %s():%d\n\n", __func__, __LINE__);
    return flag;
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetWirelessBasic
 *
 *  [DESCRIPTION]:
 *
 *           Get Wireless Basic Status
 *
 *  [return]:index_vlan|index_ssid|SSID Enable|Radio Enable|SSID Name|SSID Broadcast|Security Mode|KEY|KEY MODE|WEP Key Length|Wireless band|Channel|Bandwidth|Transmit Rate|Power Level|
 **************************************************************************/
void cgiGetWirelessBasic(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    char str[BUFLEN_256]= {0};
    unsigned int value = 0;
    unsigned int ch = 0;
    unsigned int index_vlan = INDEX_VLAN;
    unsigned int index_ssid = INDEX_SSID;
    gui_debug(">>>>>>Enter %s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
    get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
    gui_debug("status get cur vlan=%d,ssid=%d\n",index_vlan,index_ssid);

    //Current VLAN
    sprintf(buf, "%d|", index_vlan);
    strcat(varValue, buf);

    //Current SSID
    sprintf(buf, "%d|", index_ssid);
    strcat(varValue, buf);

    //SSID Enabled
    value = dal_wifi_get_SSIDEnable(index_vlan, index_ssid);
    gui_debug("SSID Enable: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    //Radio Enable
    value = dal_wifi_get_RadioEnabled(index_vlan, index_ssid);
    gui_debug("Radio Enable: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    //SSID
    if(dal_wifi_get_ssid(index_vlan, index_ssid, str, sizeof(str))) {
        gui_debug("--%s-- FAILED: dal_wifi_get_ssid--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        strcat(varValue, "|");
    } else {
        gui_debug("Set SSID: value=%s\n",str);
        sprintf(buf, "%s|", getValueFromDal(str));
        strcat(varValue, buf);
    }
    
    //SSID Broadcast
    value = dal_wifi_get_SSIDAdvertisementEnabled(index_vlan, index_ssid);
    gui_debug("Advertisement SSID: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);


    //Security mode
    value = dal_wifi_get_security_mode(index_vlan, index_ssid);
    gui_debug("Security: value=%d\n",value);
    if (SECURITY_MODE_PSK_WPAWPA2==value || SECURITY_MODE_EAP_WPAWPA2==value)
    {
        strcat(varValue, "4|");
    }
    else if (SECURITY_MODE_PSK_WPA2==value || SECURITY_MODE_EAP_WPA2==value)
    {
        strcat(varValue, "3|");
    }
    else if (SECURITY_MODE_PSK_WPA==value || SECURITY_MODE_EAP_WPA==value)
    {
        strcat(varValue, "2|");
    }
    else if (SECURITY_MODE_WEP64==value||SECURITY_MODE_WEP128==value)
    {
        strcat(varValue, "1|");
    }
    else
    {
        strcat(varValue, "0|");
    }

    //Key and Type
    wep_data_t data_wep = {0};
    wpa_data_t data_wpa = {0};
    //#195926, need support open mode.
    //if(SECURITY_MODE_WEP64==value || SECURITY_MODE_WEP128==value ||SECURITY_MODE_PSK_WPA==value || SECURITY_MODE_PSK_WPA2==value || SECURITY_MODE_PSK_WPAWPA2==value)
    {
        if(dal_wifi_get_wep(index_vlan, index_ssid, &data_wep)) {
            gui_debug("--%s-- FAILED: dal_wifi_get_wep--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
            //Key
            strcat(varValue, "|");
            //wepauth mode
            strcat(varValue, "|");
        }else{
            //Key
            sprintf(buf, "%s|", getValueFromDal(data_wep.key[0].psk));
            strcat(varValue, buf);
            
            //wepauth mode
            if(data_wep.wep_auth_mode.open == 1&&data_wep.wep_auth_mode.share== 0)
               sprintf(buf, "%d|", 0);
            else if(data_wep.wep_auth_mode.open == 0&&data_wep.wep_auth_mode.share== 1)
               sprintf(buf, "%d|", 1);
            else
               sprintf(buf, "%d|", 2); 
               strcat(varValue, buf);              
        }
        
        if(dal_wifi_get_wpa(index_vlan, index_ssid, &data_wpa)) {
            gui_debug("--%s-- FAILED: dal_wifi_get_wpa--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
            //Key
            strcat(varValue, "|");
            //wpa encrypt mode
            strcat(varValue, "|");
        }else{
            //Key
            sprintf(buf, "%s|", getValueFromDal(data_wpa.key[0].psk));
            strcat(varValue, buf);
            //wpa encrypt mode
            if (data_wpa.wpa == 1){
                if(data_wpa.wpa_encrypt_mode.tkip == 1&&data_wpa.wpa_encrypt_mode.aes == 0)
                    strcat(varValue, "0|"); 
                else if(data_wpa.wpa_encrypt_mode.tkip == 0&&data_wpa.wpa_encrypt_mode.aes == 1)
                      strcat(varValue, "1|");
                else
                      strcat(varValue, "2|");            
            }
            else {
                if(data_wpa.wpa2_encrypt_mode.tkip == 1&&data_wpa.wpa2_encrypt_mode.aes == 0)
                      strcat(varValue, "0|"); 
                else if(data_wpa.wpa2_encrypt_mode.tkip == 0&&data_wpa.wpa2_encrypt_mode.aes == 1)
                      strcat(varValue, "1|");
                else
                      strcat(varValue, "2|");        
            }
            
        }         
        
    }
    //else{
        //strcat(varValue, "||||");
    //}

    //Wireless band
    value = dal_wifi_get_standard(index_vlan, index_ssid);
    gui_debug("Mode: value=%d\n",value);
    switch(value)
    {
        case IEEE80211_B:
                strcat(varValue, "0|");
                break;
            case IEEE80211_G:
                strcat(varValue, "1|");
                break;
            case IEEE80211_N:
                strcat(varValue, "2|");
                break;
            case IEEE80211_B|IEEE80211_G:
                strcat(varValue, "3|");
                break;
            case IEEE80211_G|IEEE80211_N:
                strcat(varValue, "4|");
                break;
            case IEEE80211_B|IEEE80211_G|IEEE80211_N:
                strcat(varValue, "5|");
                break;
            case IEEE80211_A|IEEE80211_N:
                strcat(varValue, "6|");
                break;
            case IEEE80211_A|IEEE80211_N|IEEE80211_AC:
                strcat(varValue, "7|");
                break;
            default:
                strcat(varValue, "|");
                break;
    }

    //Channel
    if(dal_wifi_get_channel(index_vlan, index_ssid, &value, &ch, NULL)) {
        gui_debug("--%s-- FAILED: dal_wifi_get_channel--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        value = 0;
    }
    gui_debug("Menu Channel: value=%d; Bandwidth: value=%d\n",value, ch);
    if(dal_wifi_get_AutoChannelEnable(index_vlan, index_ssid))
    {
        gui_debug("Auto Channel.\n");
        value = 0;
    }
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    //Bandwidth
    switch(ch){
        case BAND_WIDTH_20MHZ:
            strcat(varValue, "20|");
            break;
        case BAND_WIDTH_40MHZ:
            strcat(varValue, "40|");
            break;
        case BAND_WIDTH_80MHZ:
            strcat(varValue, "80|");
            break;
        default:
            strcat(varValue, "0|");
            break;
    }

    //transmit Power Level
    value = dal_wifi_get_TransmitPower(index_vlan, index_ssid);
    gui_debug("Transmit Power: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    //Wireless MAC
    value = dal_wifi_get_mac_address(index_vlan, index_ssid,buf,sizeof(buf));
    gui_debug("Data rate: value=%d,buf=%s\n",value,buf);
    strcat(varValue, buf);
    
    gui_debug(">>>>>>%s():%d,varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostWirelessSecurity
 *
 *  [DESCRIPTION]:
 *
 *           Post wifi-security settings
 *
 *  [return]:
 **************************************************************************/
int cgiPostWirelessSecurity()
{
    tsl_rv_t flag = TSL_RV_SUC;
    unsigned int index_vlan = INDEX_VLAN;
    unsigned int index_ssid = INDEX_SSID;
    wep_data_t data_wep = {0};
    wpa_data_t data_wpa = {0};
    unsigned int standard = 0;
    unsigned int securityType = 0;

    //get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
    get_cur_vlan_ssid_index_bypost(&index_vlan,&index_ssid);

    gui_debug("Post security vlan=%d,ssid=%d\n",index_vlan,index_ssid);
        
    if (gWebParams.wireless_radio_enable == 1&&gWebParams.wireless_ssid_enable == 1)
    {
        //Set Security
        gui_debug("Set Security: security type=%d; key=%s; wep_auth_mode=%d,wep_key_len=%d\n",gWebParams.wireless_security_type,gWebParams.wireless_key, gWebParams.wep_auth_mode,gWebParams.wep_key_len);
        switch(gWebParams.wireless_security_type)
        {
            case 0:
                securityType = SECURITY_MODE_OFF;
                break;
            case 1:
                data_wep.key_index = 0;
                data_wep.eap_port = 0;
                if(gWebParams.wep_key_len==5||gWebParams.wep_key_len==13)
                {
                    data_wep.key[data_wep.key_index].type = 1;
                    if(gWebParams.wep_key_len==5)
                        securityType = SECURITY_MODE_WEP64;
                    else
                        securityType = SECURITY_MODE_WEP128;
                }
                else if(gWebParams.wep_key_len==10||gWebParams.wep_key_len==26)
                {
                    data_wep.key[data_wep.key_index].type = 0;
                    if(gWebParams.wep_key_len==10)
                        securityType = SECURITY_MODE_WEP64;
                    else
                        securityType = SECURITY_MODE_WEP128;
                }

                if(gWebParams.wep_auth_mode == 0)
                {
                    data_wep.wep_auth_mode.open = 1;
                    data_wep.wep_auth_mode.share= 0;
                }
                else if(gWebParams.wep_auth_mode == 1)
                {
                    data_wep.wep_auth_mode.open = 0;
                    data_wep.wep_auth_mode.share= 1;
                }
                else
                {
                    data_wep.wep_auth_mode.open = 1;
                    data_wep.wep_auth_mode.share= 1;
                }                   
                snprintf(data_wep.key[data_wep.key_index].psk, sizeof(data_wep.key[data_wep.key_index].psk), "%s", gWebParams.wireless_key);
                snprintf(data_wep.eap_server, sizeof(data_wep.eap_server), "%s", "\0");
                snprintf(data_wep.eap_secret, sizeof(data_wep.eap_secret), "%s", "\0");
                gui_debug("++++++dal_wifi_set_wep+++key=%s;key_type=%d,mode=%d\n",data_wep.key[data_wep.key_index].psk, data_wep.key[data_wep.key_index].type,gWebParams.wep_auth_mode);
                if(dal_wifi_set_wep(index_vlan, index_ssid, &data_wep)) {
                    gui_debug("--%s-- FAILED: dal_wifi_set_wep--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
                    flag = -1;
                }
                break;

            case 2:     //psk wpa
                securityType = SECURITY_MODE_PSK_WPA;
                data_wpa.wpa = 1;
                data_wpa.wpa2 = 0;
                if(gWebParams.wpa_encrypt_mode==0)
                {
                    data_wpa.wpa_encrypt_mode.tkip = 1;
                    data_wpa.wpa_encrypt_mode.aes = 0;
                }
                else if(gWebParams.wpa_encrypt_mode==1)
                {
                    data_wpa.wpa_encrypt_mode.tkip = 0;
                    data_wpa.wpa_encrypt_mode.aes = 1;
                }
                else
                {
                    data_wpa.wpa_encrypt_mode.tkip = 1;
                    data_wpa.wpa_encrypt_mode.aes = 1;
                }

                data_wpa.key_index = 0;
                data_wpa.group_key_update_interval = 1200;

                data_wpa.wpa_auth_mode.psk = 1;
                data_wpa.wpa_auth_mode.eap = 0;
                data_wpa.wpa2_auth_mode.psk = 0;
                data_wpa.wpa2_auth_mode.eap = 0;

                data_wpa.eap_port = 0; 
                snprintf(data_wpa.key[data_wpa.key_index].psk, sizeof(data_wpa.key[data_wpa.key_index].psk), "%s", gWebParams.wireless_key);
                snprintf(data_wpa.eap_server, sizeof(data_wpa.eap_server), "%s", "\0");
                snprintf(data_wpa.eap_secret, sizeof(data_wpa.eap_secret), "%s", "\0");
                gui_debug("++++++dal_wifi_set_wpa+++key=%s\n",data_wpa.key[data_wpa.key_index].psk);
                if(dal_wifi_set_wpa(index_vlan, index_ssid, &data_wpa)) {
                    gui_debug("--%s-- FAILED: dal_wifi_set_wpa--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
                    flag = -1;
                }

                break;

            case 3:     //psk wpa2
                securityType = SECURITY_MODE_PSK_WPA2;
                data_wpa.wpa = 0;
                data_wpa.wpa2 = 1;
                if(gWebParams.wpa_encrypt_mode==0)
                {
                    data_wpa.wpa2_encrypt_mode.tkip = 1;
                    data_wpa.wpa2_encrypt_mode.aes = 0;
                }
                else if(gWebParams.wpa_encrypt_mode==1)
                {
                    data_wpa.wpa2_encrypt_mode.tkip = 0;
                    data_wpa.wpa2_encrypt_mode.aes = 1;
                }
                else
                {
                    data_wpa.wpa2_encrypt_mode.tkip = 1;
                    data_wpa.wpa2_encrypt_mode.aes = 1;
                }

                data_wpa.key_index = 0;
                data_wpa.group_key_update_interval = 1200;

                data_wpa.wpa_auth_mode.psk = 0;
                data_wpa.wpa_auth_mode.eap = 0;
                data_wpa.wpa2_auth_mode.psk = 1;
                data_wpa.wpa2_auth_mode.eap = 0;

                data_wpa.eap_port = 0; 
                snprintf(data_wpa.key[data_wpa.key_index].psk, sizeof(data_wpa.key[data_wpa.key_index].psk), "%s", gWebParams.wireless_key);
                snprintf(data_wpa.eap_server, sizeof(data_wpa.eap_server), "%s", "\0");
                snprintf(data_wpa.eap_secret, sizeof(data_wpa.eap_secret), "%s", "\0");
                gui_debug("++++++dal_wifi_set_wpa+++key=%s\n",data_wpa.key[data_wpa.key_index].psk);
                if(dal_wifi_set_wpa(index_vlan, index_ssid, &data_wpa)) {
                    gui_debug("--%s-- FAILED: dal_wifi_set_wpa--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
                    flag = -1;
                }

                break;

            case 4:     //psk wpa/wpa2
                securityType = SECURITY_MODE_PSK_WPAWPA2;
                data_wpa.wpa = 1;
                data_wpa.wpa2 = 1;
                if(gWebParams.wpa_encrypt_mode==0)
                {
                    data_wpa.wpa_encrypt_mode.tkip = 1;
                    data_wpa.wpa_encrypt_mode.aes = 0;
                    data_wpa.wpa2_encrypt_mode.tkip = 1;
                    data_wpa.wpa2_encrypt_mode.aes = 0;
                }
                else if(gWebParams.wpa_encrypt_mode==1)
                {
                    data_wpa.wpa_encrypt_mode.tkip = 0;
                    data_wpa.wpa_encrypt_mode.aes = 1;
                    data_wpa.wpa2_encrypt_mode.tkip = 0;
                    data_wpa.wpa2_encrypt_mode.aes = 1;
                }
                else
                {
                    data_wpa.wpa_encrypt_mode.tkip = 1;
                    data_wpa.wpa_encrypt_mode.aes = 1;
                    data_wpa.wpa2_encrypt_mode.tkip = 1;
                    data_wpa.wpa2_encrypt_mode.aes = 1;
                }
                
                data_wpa.key_index = 0;
                data_wpa.group_key_update_interval = 1200;

                data_wpa.wpa_auth_mode.psk = 1;
                data_wpa.wpa_auth_mode.eap = 0;
                data_wpa.wpa2_auth_mode.psk = 1;
                data_wpa.wpa2_auth_mode.eap = 0;

                data_wpa.eap_port = 0; 
                snprintf(data_wpa.key[data_wpa.key_index].psk, sizeof(data_wpa.key[data_wpa.key_index].psk), "%s", gWebParams.wireless_key);
                snprintf(data_wpa.eap_server, sizeof(data_wpa.eap_server), "%s", "\0");
                snprintf(data_wpa.eap_secret, sizeof(data_wpa.eap_secret), "%s", "\0");
                gui_debug("++++++dal_wifi_set_wpa+++key=%s\n",data_wpa.key[data_wpa.key_index].psk);
                if(dal_wifi_set_wpa(index_vlan, index_ssid, &data_wpa)) {
                    gui_debug("--%s-- FAILED: dal_wifi_set_wpa--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
                    flag = -1;
                }

                break;

            case 5:     //eap wpa
                securityType = SECURITY_MODE_EAP_WPA;
                data_wpa.wpa = 1;
                data_wpa.wpa2 = 0;
                if(gWebParams.wpa_encrypt_mode==0)
                {
                    data_wpa.wpa_encrypt_mode.tkip = 1;
                    data_wpa.wpa_encrypt_mode.aes = 0;
                }
                else if(gWebParams.wpa_encrypt_mode==1)
                {
                    data_wpa.wpa_encrypt_mode.tkip = 0;
                    data_wpa.wpa_encrypt_mode.aes = 1;
                }
                else
                {
                    data_wpa.wpa_encrypt_mode.tkip = 1;
                    data_wpa.wpa_encrypt_mode.aes = 1;
                }

                data_wpa.key_index = 0;
                data_wpa.group_key_update_interval = 1200;

                data_wpa.wpa_auth_mode.psk = 0;
                data_wpa.wpa_auth_mode.eap = 1;
                data_wpa.wpa2_auth_mode.psk = 0;
                data_wpa.wpa2_auth_mode.eap = 0;

                snprintf(data_wpa.key[data_wpa.key_index].psk, sizeof(data_wpa.key[data_wpa.key_index].psk), "%s", "\0");
                data_wpa.eap_port = gWebParams.wl_serverport;
                snprintf(data_wpa.eap_server, sizeof(data_wpa.eap_server), "%s", gWebParams.wl_serverip);
                snprintf(data_wpa.eap_secret, sizeof(data_wpa.eap_secret), "%s", gWebParams.wl_serverpassword);
                gui_debug("eap_server=%s, secret=%s\n",data_wpa.eap_server, data_wpa.eap_secret);
                if(dal_wifi_set_wpa(index_vlan, index_ssid, &data_wpa)) {
                    gui_debug("--%s-- FAILED: dal_wifi_set_wpa--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
                    flag = -1;
                }

                break;

            case 6:     //eap wpa2
                securityType = SECURITY_MODE_EAP_WPA2;
                data_wpa.wpa = 0;
                data_wpa.wpa2 = 1;
                if(gWebParams.wpa_encrypt_mode==0)
                {
                    data_wpa.wpa2_encrypt_mode.tkip = 1;
                    data_wpa.wpa2_encrypt_mode.aes = 0;
                }
                else if(gWebParams.wpa_encrypt_mode==1)
                {
                    data_wpa.wpa2_encrypt_mode.tkip = 0;
                    data_wpa.wpa2_encrypt_mode.aes = 1;
                }
                else
                {
                    data_wpa.wpa2_encrypt_mode.tkip = 1;
                    data_wpa.wpa2_encrypt_mode.aes = 1;
                }

                data_wpa.key_index = 0;
                data_wpa.group_key_update_interval = 1200;

                data_wpa.wpa_auth_mode.psk = 0;
                data_wpa.wpa_auth_mode.eap = 1;
                data_wpa.wpa2_auth_mode.psk = 0;
                data_wpa.wpa2_auth_mode.eap = 0;

                snprintf(data_wpa.key[data_wpa.key_index].psk, sizeof(data_wpa.key[data_wpa.key_index].psk), "%s", "\0");
                data_wpa.eap_port = gWebParams.wl_serverport;
                snprintf(data_wpa.eap_server, sizeof(data_wpa.eap_server), "%s", gWebParams.wl_serverip);
                snprintf(data_wpa.eap_secret, sizeof(data_wpa.eap_secret), "%s", gWebParams.wl_serverpassword);
                gui_debug("eap_server=%s, secret=%s\n",data_wpa.eap_server, data_wpa.eap_secret);
                if(dal_wifi_set_wpa(index_vlan, index_ssid, &data_wpa)) {
                    gui_debug("--%s-- FAILED: dal_wifi_set_wpa--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
                    flag = -1;
                }

                break;

            case 7:
                securityType = SECURITY_MODE_EAP_WPAWPA2;
                data_wpa.wpa = 1;
                data_wpa.wpa2 = 1;
                if(gWebParams.wpa_encrypt_mode==0)
                {
                    data_wpa.wpa_encrypt_mode.tkip = 1;
                    data_wpa.wpa_encrypt_mode.aes = 0;
                    data_wpa.wpa2_encrypt_mode.tkip = 1;
                    data_wpa.wpa2_encrypt_mode.aes = 0;
                }
                else if(gWebParams.wpa_encrypt_mode==1)
                {
                    data_wpa.wpa_encrypt_mode.tkip = 0;
                    data_wpa.wpa_encrypt_mode.aes = 1;
                    data_wpa.wpa2_encrypt_mode.tkip = 0;
                    data_wpa.wpa2_encrypt_mode.aes = 1;
                }
                else
                {
                    data_wpa.wpa_encrypt_mode.tkip = 1;
                    data_wpa.wpa_encrypt_mode.aes = 1;
                    data_wpa.wpa2_encrypt_mode.tkip = 1;
                    data_wpa.wpa2_encrypt_mode.aes = 1;
                }
                
                data_wpa.key_index = 0;
                data_wpa.group_key_update_interval = 1200;

                data_wpa.wpa_auth_mode.psk = 0;
                data_wpa.wpa_auth_mode.eap = 1;
                data_wpa.wpa2_auth_mode.psk = 0;
                data_wpa.wpa2_auth_mode.eap = 1;

                snprintf(data_wpa.key[data_wpa.key_index].psk, sizeof(data_wpa.key[data_wpa.key_index].psk), "%s", "\0");
                data_wpa.eap_port = gWebParams.wl_serverport;
                snprintf(data_wpa.eap_server, sizeof(data_wpa.eap_server), "%s", gWebParams.wl_serverip);
                snprintf(data_wpa.eap_secret, sizeof(data_wpa.eap_secret), "%s", gWebParams.wl_serverpassword);
                gui_debug("eap_server=%s, secret=%s\n",data_wpa.eap_server, data_wpa.eap_secret);
                if(dal_wifi_set_wpa(index_vlan, index_ssid, &data_wpa)) {
                    gui_debug("--%s-- FAILED: dal_wifi_set_wpa--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
                    flag = -1;
                }

                break;
        }
        gui_debug("security Type=%d\n",securityType);
        if(dal_wifi_set_security_mode(index_vlan, index_ssid, securityType)) {
            gui_debug("--%s-- FAILED: dal_wifi_set_security_mode--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
            flag = -1;
        }
        
        //security type is open/wep/wpa , will disable wps
        if (gWebParams.wireless_security_type == 0 || \
            gWebParams.wireless_security_type == 1 || \
            gWebParams.wireless_security_type == 2 || \
            gWebParams.wireless_security_type == 5 || \
            gWebParams.wpa_encrypt_mode == 0)
        {
            tsl_rv_t result = TSL_RV_SUC;
            wps_data_t data = {0};
            //unsigned int index_vlan = INDEX_VLAN;
            //unsigned int index_ssid = INDEX_SSID;

            get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
            data.enable = 0;

            result = dal_wifi_set_wps(index_vlan, index_ssid, &data);
        }
    }


    flag = dal_wifi_set_commit(index_vlan, index_ssid);
//    flag = dal_SaveConfig();

    gui_debug("\n\n>>>> Leave %s():%d\n\n", __func__, __LINE__);
    return flag;
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetWirelessAdvanced
 *
 *  [DESCRIPTION]:
 *
 *           Get Wireless Advanced
 *
 *  [return]:index_vlan|index_ssid|SSID Enable|Radio Enable|SSID Name|Security mode|Key|Key Type|WEP Key Length|
 **************************************************************************/
void cgiGetWirelessAdvanced(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    char str[BUFLEN_256]= {0};
    unsigned int value = 0;
    unsigned int ch = 0;
    unsigned int index_vlan = INDEX_VLAN;
    unsigned int index_ssid = INDEX_SSID;
    int wep_keyLen = 0;
    gui_debug(">>>>>>Enter %s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
    get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
    gui_debug("status get cur vlan=%d,ssid=%d\n",index_vlan,index_ssid);

    //Current VLAN
    sprintf(buf, "%d|", index_vlan);
    strcat(varValue, buf);

    //Current SSID
    sprintf(buf, "%d|", index_ssid);
    strcat(varValue, buf);

    //SSID Enabled
    value = dal_wifi_get_SSIDEnable(index_vlan, index_ssid);
    gui_debug("SSID Enable: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    //Radio Enable
    value = dal_wifi_get_RadioEnabled(index_vlan, index_ssid);
    gui_debug("Radio Enable: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    //SSID
    if(dal_wifi_get_ssid(index_vlan, index_ssid, str, sizeof(str))) {
        gui_debug("--%s-- FAILED: dal_wifi_get_ssid--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        strcat(varValue, "|");
    } else {
        gui_debug("Set SSID: value=%s\n",str);
        sprintf(buf, "%s|", getValueFromDal(str));
        strcat(varValue, buf);
    }

    //Protection
    //value = dal_wifi_get_AMSDU(index_vlan, index_ssid);
   // gui_debug("dal_wifi_get_AMSDU: value=%d\n",value);
    sprintf(buf, "%d|", 1);
    strcat(varValue, buf);  

    //STBC
    //value = dal_wifi_get_AMSDU(index_vlan, index_ssid);
   // gui_debug("dal_wifi_get_AMSDU: value=%d\n",value);
    sprintf(buf, "%d|", 1);
    strcat(varValue, buf);  
    
    //Aggregation MSDU
    value = dal_wifi_get_AMSDU(index_vlan, index_ssid);
    gui_debug("dal_wifi_get_AMSDU: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);    

    //Short GI
    value = dal_wifi_get_ShortGuardInterval(index_vlan, index_ssid);
    gui_debug("dal_wifi_get_ShortGuardInterval: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf); 

    //SSID Broadcast
    value = dal_wifi_get_SSIDAdvertisementEnabled(index_vlan, index_ssid);
    gui_debug("dal_wifi_get_SSIDAdvertisementEnabled: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);     

    //WMM
    value = dal_wifi_get_WMMEnable(index_vlan, index_ssid);
    gui_debug("dal_wifi_get_WMMEnable: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    //BeaconInterval
    value = dal_wifi_get_BeaconInterval(index_vlan, index_ssid);
    gui_debug("dal_wifi_get_BeaconInterval: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    //Transmit Power
    value = dal_wifi_get_TransmitPower(index_vlan, index_ssid);
    gui_debug("dal_wifi_get_TransmitPower: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    gui_debug(">>>>>>%s():%d,varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostWirelessAdv
 *
 *  [DESCRIPTION]:
 *
 *           Post wifi-advanced settings
 *
 *  [return]:
 **************************************************************************/
int cgiPostWirelessAdv()
{
    tsl_rv_t flag = TSL_RV_SUC;
    unsigned int index_vlan = INDEX_VLAN;
    unsigned int index_ssid = INDEX_SSID;
    wpa_data_t data_wpa = {0};
    unsigned int standard = 0;
    unsigned int securityType = 0;

    //get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
    get_cur_vlan_ssid_index_bypost(&index_vlan,&index_ssid);

    gui_debug("Post basic vlan=%d,ssid=%d\n",index_vlan,index_ssid);

    //Protection
    gui_debug("Protection: value=%d\n",gWebParams.wl_protection);
    //if(dal_wifi_set_WiFiProtection(index_vlan, index_ssid, gWebParams.wl_protection)){
    //    gui_debug("--%s-- FAILED: dal_wifi_set_WiFiProtection--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
    //    flag = -1;
    //}

    //STBC
    gui_debug("wl_stbc: value=%d\n",gWebParams.wl_stbc);
    //if(dal_wifi_set_WiFiSTBC(index_vlan, index_ssid, gWebParams.wl_stbc)){
    //    gui_debug("--%s-- FAILED: dal_wifi_set_WiFiSTBC--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
    //    flag = -1;
    //}        

    //Aggregation
    gui_debug("wl_aggregation: value=%d\n",gWebParams.wl_aggregation);
    if(dal_wifi_set_AMSDU(index_vlan, index_ssid, gWebParams.wl_aggregation)){
        gui_debug("--%s-- FAILED: dal_wifi_set_AMSDU--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        flag = -1;
    }    

    //Short GI
    gui_debug("wl_gi: value=%d\n",gWebParams.wl_gi);
    if(dal_wifi_set_ShortGuardInterval(index_vlan, index_ssid, gWebParams.wl_gi)){
        gui_debug("--%s-- FAILED: dal_wifi_set_ShortGuardInterval--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        flag = -1;
    }  
/*
    //SSID Broadcast
    gui_debug("wl_broadcast: value=%d\n",gWebParams.wl_broadcast);
    if(dal_wifi_set_SSIDAdvertisementEnabled(index_vlan, index_ssid, gWebParams.wl_broadcast)){
        gui_debug("--%s-- FAILED: dal_wifi_set_SSIDAdvertisementEnabled--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        flag = -1;
    }  
*/
    //WMM
    gui_debug("wl_wmm: value=%d\n",gWebParams.wl_wmm);
    if(dal_wifi_set_WMMEnable(index_vlan, index_ssid, gWebParams.wl_wmm)){
        gui_debug("--%s-- FAILED: dal_wifi_set_WMMEnable--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        flag = -1;
    } 

    //beacon
    gui_debug("wl_beacon: value=%d\n",gWebParams.wl_beacon);
    if(dal_wifi_set_BeaconInterval(index_vlan, index_ssid, gWebParams.wl_beacon)){
        gui_debug("--%s-- FAILED: dal_wifi_set_BeaconInterval--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        flag = -1;
    }
/*
    //Transmit Power
    //wireless_trans_power
    gui_debug("wireless_trans_power: value=%d\n",gWebParams.wireless_trans_power);
    if(dal_wifi_set_TransmitPower(index_vlan, index_ssid, gWebParams.wireless_trans_power)){
        gui_debug("--%s-- FAILED: dal_wifi_set_TransmitPower--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        flag = -1;
    }
*/    
    flag = dal_wifi_set_commit(index_vlan, index_ssid);
//    flag = dal_SaveConfig();

    gui_debug("\n\n>>>> Leave %s():%d\n\n", __func__, __LINE__);
    return flag;
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetWirelessWPS
 *
 *  [DESCRIPTION]:
 *
 *           Get Wireless WPS Status
 *
 **************************************************************************/
void cgiGetWirelessWPS(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    char str[BUFLEN_256]= {0};
    wps_data_t wps_data = {0};
    unsigned int value = 0;
    unsigned int ch = 0;
    unsigned int index_vlan = INDEX_VLAN;
    unsigned int index_ssid = INDEX_SSID;
    gui_debug(">>>>>>Enter %s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
    get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
    gui_debug("status get cur vlan=%d,ssid=%d\n",index_vlan,index_ssid);

    //Current VLAN
    sprintf(buf, "%d|", index_vlan);
    strcat(varValue, buf);

    //Current SSID
    sprintf(buf, "%d|", index_ssid);
    strcat(varValue, buf);

    //SSID Enabled
    value = dal_wifi_get_SSIDEnable(index_vlan, index_ssid);
    gui_debug("SSID Enable: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    //Radio Enable
    value = dal_wifi_get_RadioEnabled(index_vlan, index_ssid);
    gui_debug("Radio Enable: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    //SSID
    if(dal_wifi_get_ssid(index_vlan, index_ssid, str, sizeof(str))) {
        gui_debug("--%s-- FAILED: dal_wifi_get_ssid--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        strcat(varValue, "|");
    } else {
        gui_debug("Set SSID: value=%s\n",str);
        sprintf(buf, "%s|", getValueFromDal(str));
        strcat(varValue, buf);
    }

    //WPS enable/disable
    if(dal_wifi_get_wps(index_vlan, index_ssid, &wps_data)) {
        gui_debug("--%s-- FAILED: dal_wifi_get_wps--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        strcat(varValue, "|");
    } else {
        gui_debug("Set wps enalbe: value=%d\n",wps_data.enable);
        sprintf(buf, "%d|", wps_data.enable);
        strcat(varValue, buf);
    }

    //security type
    value = dal_wifi_get_security_mode(index_vlan, index_ssid);
    gui_debug("Security: value=%d\n",value);
    if (SECURITY_MODE_PSK_WPAWPA2==value || SECURITY_MODE_EAP_WPAWPA2==value)
    {
        strcat(varValue, "4|");
    }
    else if (SECURITY_MODE_PSK_WPA2==value || SECURITY_MODE_EAP_WPA2==value)
    {
        strcat(varValue, "3|");
    }
    else if (SECURITY_MODE_PSK_WPA==value || SECURITY_MODE_EAP_WPA==value)
    {
        strcat(varValue, "2|");
    }
    else if (SECURITY_MODE_WEP64==value||SECURITY_MODE_WEP128==value)
    {
        strcat(varValue, "1|");
    }
    else
    {
        strcat(varValue, "0|");
    }


    //encryption/key type
    wep_data_t data_wep = {0};
    wpa_data_t data_wpa = {0};
    if(SECURITY_MODE_WEP64==value||SECURITY_MODE_WEP128==value){
        if(dal_wifi_get_wep(index_vlan, index_ssid, &data_wep)) {
            gui_debug("--%s-- FAILED: dal_wifi_get_wep--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
            //Key
            strcat(varValue, "|");
            //wepauth/ecrypt_mode
            strcat(varValue, "|");
        }else{
            //Key
            sprintf(buf, "%s|", getValueFromDal(data_wep.key[0].psk));
            strcat(varValue, buf);
            //wepauth/ecrypt_mode
            if(data_wep.wep_auth_mode.open == 1&&data_wep.wep_auth_mode.share== 0)
               sprintf(buf, "%d|", 0);
            else if(data_wep.wep_auth_mode.open == 0&&data_wep.wep_auth_mode.share== 1)
               sprintf(buf, "%d|", 1);
            else
               sprintf(buf, "%d|", 2); 
               
            strcat(varValue, buf);              
        }
        
    }
    else if(SECURITY_MODE_PSK_WPA==value || SECURITY_MODE_PSK_WPA2==value || SECURITY_MODE_PSK_WPAWPA2==value)
    {
        if(dal_wifi_get_wpa(index_vlan, index_ssid, &data_wpa)) {
            gui_debug("--%s-- FAILED: dal_wifi_get_wpa--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
            //Key
            strcat(varValue, "|");
            //Type
            strcat(varValue, "|");
        }else{
            //Key
            sprintf(buf, "%s|", getValueFromDal(data_wpa.key[0].psk));
            strcat(varValue, buf);
            //wepauth/ecrypt_mode
            if(SECURITY_MODE_PSK_WPA==value )
            {
                if(data_wpa.wpa_encrypt_mode.tkip == 1&&data_wpa.wpa_encrypt_mode.aes == 0)
                      strcat(varValue, "0|"); 
                else if(data_wpa.wpa_encrypt_mode.tkip == 0&&data_wpa.wpa_encrypt_mode.aes == 1)
                      strcat(varValue, "1|");
                else
                    strcat(varValue, "2|");
            }   
            else
            {
                if(data_wpa.wpa2_encrypt_mode.tkip == 1&&data_wpa.wpa2_encrypt_mode.aes == 0)
                      strcat(varValue, "0|"); 
                else if(data_wpa.wpa2_encrypt_mode.tkip == 0&&data_wpa.wpa2_encrypt_mode.aes == 1)
                      strcat(varValue, "1|");
                else
                    strcat(varValue, "2|");
            }                
            
        }
        
    }
    else{
        strcat(varValue, "||");
    }


    //WPS status
    //value = dal_wifi_get_wps_status(index_vlan, index_ssid);
    //gui_debug("dal_wifi_get_wps_status: value=%d\n",value);
    sprintf(buf, "%d|", 1);
    strcat(varValue, buf);     

    //AP PIN
    sprintf(buf, "%.08d|", wps_data.pin);
    strcat(varValue, buf);     
    

    gui_debug(">>>>>>%s():%d,varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}

int cgiPostWirelessWPS()
{
    gui_debug(">>>>>>Enter %s():%d, wireless_wps_enable_type=%d; submit_button_name=%s; wireless_wps_pin=%s\n\n\n", __func__, __LINE__, gWebParams.wireless_wps_enable_type, gWebParams.submit_button_name, gWebParams.wireless_wps_pin);
    tsl_rv_t result = TSL_RV_SUC;
    tsl_rv_t flag = TSL_RV_SUC;
    
    unsigned int index_vlan = INDEX_VLAN;
    unsigned int index_ssid = INDEX_SSID;
    wps_data_t data = {0};

    //get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
    get_cur_vlan_ssid_index_bypost(&index_vlan,&index_ssid);

    if (gWebParams.wireless_wps_enable_type == 1)
    {
        data.enable = 1;
        if(!tsl_strcmp(gWebParams.submit_button_name, "push"))
        {
            data.method = WPS_PBC;
        }
        else if(!tsl_strcmp(gWebParams.submit_button_name, "generate_ap_pin"))
        {
            data.method = WPS_AP_RANDOM_PIN;
        }
        else if(!tsl_strcmp(gWebParams.submit_button_name, "use_device_pin"))
        {
            data.method = WPS_DEVICE_PIN;
            data.pin= atoi(gWebParams.wireless_wps_pin);
        }
        else if(!tsl_strcmp(gWebParams.submit_button_name, "static_ap_pin"))
        {
            data.method = WPS_AP_STATIC_PIN;
            data.pin= atoi(gWebParams.wireless_wps_pin);
        } 
    }
    else if (gWebParams.wireless_wps_enable_type == 0)
    {
        data.enable = 0;
    }

    result = dal_wifi_set_wps(index_vlan, index_ssid, &data);
    
    flag = dal_wifi_set_commit(index_vlan, index_ssid);

    gui_debug(">>>>>>%s():%d, result=[%d]\n\n\n", __func__, __LINE__, result);
    return flag;    
}
    

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetWirelessSecurity
 *
 *  [DESCRIPTION]:
 *
 *           Get Wireless Security Status
 *
 **************************************************************************/
void cgiGetWirelessSecurity(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    char str[BUFLEN_256]= {0};
    unsigned int value = 0;
    unsigned int ch = 0;
    unsigned int index_vlan = INDEX_VLAN;
    unsigned int index_ssid = INDEX_SSID;
    int wep_keyLen = 0;
    gui_debug(">>>>>>Enter %s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
    get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
    gui_debug("status get cur vlan=%d,ssid=%d\n",index_vlan,index_ssid);

    //Current VLAN
    sprintf(buf, "%d|", index_vlan);
    strcat(varValue, buf);

    //Current SSID
    sprintf(buf, "%d|", index_ssid);
    strcat(varValue, buf);

    //SSID Enabled
    value = dal_wifi_get_SSIDEnable(index_vlan, index_ssid);
    gui_debug("SSID Enable: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    //Radio Enable
    value = dal_wifi_get_RadioEnabled(index_vlan, index_ssid);
    gui_debug("Radio Enable: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    //SSID
    if(dal_wifi_get_ssid(index_vlan, index_ssid, str, sizeof(str))) {
        gui_debug("--%s-- FAILED: dal_wifi_get_ssid--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        strcat(varValue, "|");
    } else {
        gui_debug("Set SSID: value=%s\n",str);
        sprintf(buf, "%s|", getValueFromDal(str));
        strcat(varValue, buf);
    }
    
    //Security mode
    value = dal_wifi_get_security_mode(index_vlan, index_ssid);
    gui_debug("Security: value=%d\n",value);
    switch(value){
        case SECURITY_MODE_EAP_WPAWPA2:
            strcat(varValue, "7|");
            break;
        case SECURITY_MODE_EAP_WPA2:
            strcat(varValue, "6|");
            break;
        case SECURITY_MODE_EAP_WPA:
            strcat(varValue, "5|");
            break;
        case SECURITY_MODE_PSK_WPAWPA2:
            strcat(varValue, "4|");
            break;
        case SECURITY_MODE_PSK_WPA2:
            strcat(varValue, "3|");
            break;
        case SECURITY_MODE_PSK_WPA:
            strcat(varValue, "2|");
            break;
        case SECURITY_MODE_WEP64:
        case SECURITY_MODE_WEP128:
            strcat(varValue, "1|");
            break;
        default:
            strcat(varValue, "0|");
            break;
    }

    //Key and Type
    wep_data_t data_wep = {0};
    wpa_data_t data_wpa = {0};
    switch (value){
        case SECURITY_MODE_WEP64:
        case SECURITY_MODE_WEP128:

            if(dal_wifi_get_wep(index_vlan, index_ssid, &data_wep)) {
                gui_debug("--%s-- FAILED: dal_wifi_get_wep--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
                //Key
                strcat(varValue, "|");
                //wepauth/ecrypt_mode
                strcat(varValue, "|");
            }else{
                //Key
                sprintf(buf, "%s|", getValueFromDal(data_wep.key[0].psk));
                strcat(varValue, buf);
                //wepauth/ecrypt_mode
                if(data_wep.wep_auth_mode.open == 1&&data_wep.wep_auth_mode.share== 0)
                   sprintf(buf, "%d|", 0);
                else if(data_wep.wep_auth_mode.open == 0&&data_wep.wep_auth_mode.share== 1)
                   sprintf(buf, "%d|", 1);
                else
                   sprintf(buf, "%d|", 2); 
                   
                strcat(varValue, buf);

                if(SECURITY_MODE_WEP64==value)
                {
                    if(data_wep.key[0].type == 1)
                        wep_keyLen = 5;
                    else
                        wep_keyLen = 10;
                }
                else
                {
                    if(data_wep.key[0].type == 1)
                        wep_keyLen = 13;
                    else
                        wep_keyLen = 26;
                }                
            }
            
            //WEP Key Length
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%d|", wep_keyLen);
            strcat(varValue, buf);

            break;
        case SECURITY_MODE_PSK_WPA:
        case SECURITY_MODE_PSK_WPA2:
        case SECURITY_MODE_PSK_WPAWPA2:
        case SECURITY_MODE_EAP_WPA:
        case SECURITY_MODE_EAP_WPA2:
        case SECURITY_MODE_EAP_WPAWPA2:

            if(dal_wifi_get_wpa(index_vlan, index_ssid, &data_wpa)) {
                gui_debug("--%s-- FAILED: dal_wifi_get_wpa--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
                //Key
                strcat(varValue, "|");
                //Type
                strcat(varValue, "|");
                //WEP Key length
                strcat(varValue, "|");

                //WPA+802.1x   Authentication Mode = Enterprise (RADIUS)
                strcat(varValue, " | | |");
            }else{
                //Key
                sprintf(buf, "%s|", getValueFromDal(data_wpa.key[0].psk));
                strcat(varValue, buf);
                //wepauth/ecrypt_mode
                if(SECURITY_MODE_PSK_WPA==value )
                {
                    if(data_wpa.wpa_encrypt_mode.tkip == 1&&data_wpa.wpa_encrypt_mode.aes == 0)
                          strcat(varValue, "0|"); 
                    else if(data_wpa.wpa_encrypt_mode.tkip == 0&&data_wpa.wpa_encrypt_mode.aes == 1)
                          strcat(varValue, "1|");
                    else
                        strcat(varValue, "2|");
                }   
                else
                {
                    if(data_wpa.wpa2_encrypt_mode.tkip == 1&&data_wpa.wpa2_encrypt_mode.aes == 0)
                          strcat(varValue, "0|"); 
                    else if(data_wpa.wpa2_encrypt_mode.tkip == 0&&data_wpa.wpa2_encrypt_mode.aes == 1)
                          strcat(varValue, "1|");
                    else
                        strcat(varValue, "2|");
                }

                //WEP Key length
                strcat(varValue, "|");

                gui_debug("server = %s, port = %d, password = %s", getValueFromDal(data_wpa.eap_server), data_wpa.eap_port, getValueFromDal(data_wpa.eap_secret));

                //WPA+802.1x   Authentication Mode = Enterprise (RADIUS)
                memset(buf, 0, sizeof(buf));
                sprintf(buf, "%s|", getValueFromDal(data_wpa.eap_server));
                strcat(varValue, buf);

                memset(buf, 0, sizeof(buf));
                sprintf(buf, "%d|", data_wpa.eap_port);
                strcat(varValue, buf);

                memset(buf, 0, sizeof(buf));
                sprintf(buf, "%s|", getValueFromDal(data_wpa.eap_secret));
                strcat(varValue, buf);

            }

            break;
        default:
            strcat(varValue, "||");
            break;
    }

    gui_debug(">>>>>>%s():%d,varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}


void cgiGetQuantennaCard(int argc, char **argv, char *varValue) {
    if (0 == system("ifconfig host0 >/dev/null 2>/dev/null"))
        strcat(varValue, "1");
    else
        strcat(varValue, "0");
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostWirelessMacFilter
 *
 *  [DESCRIPTION]:
 *
 *       MAC Filter.
 *
 *  [return]:
 **************************************************************************/
int cgiPostWirelessMacFilter()
{
    tsl_rv_t result = TSL_RV_SUC;
    unsigned int index_vlan = INDEX_VLAN;
    unsigned int index_ssid = INDEX_SSID;
    acl_info_t acl = {0};
    char mac_list[SIZE_32*SIZE_20]="";

    //get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
    get_cur_vlan_ssid_index_bypost(&index_vlan,&index_ssid);

    //MAC Filtering Type
    if(gWebParams.wireless_macPolicy == 1)
        acl.policy = ACL_ALLOW;
    else if(gWebParams.wireless_macPolicy == 2)
        acl.policy = ACL_DENY;
    else
        acl.policy = ACL_DISABLE;

    gui_debug("\nwireless_macPolicy=%d,wireless_macCount=%d\n", gWebParams.wireless_macPolicy,gWebParams.wireless_macCount);
        
    //MAC Filter List
    if(gWebParams.wireless_macCount >0)
    {
        char *p = NULL;
        char *blank = " ";
        int i = 0;
    gui_debug("\nwireless_macPolicy=%s\n", gWebParams.wireless_maclist);
        
        p = strtok(gWebParams.wireless_maclist, blank);
        while(p) {
            if(i >= SIZE_32) break;
            snprintf(acl.mac[i], SIZE_20, "%s", p);
            gui_debug("\nacl.mac[%d]=%s\n", i,acl.mac[i]);
            i++;
            p = strtok(NULL, blank);
        }
    }

    //MAC Filter Count
    acl.count = gWebParams.wireless_macCount;

    result = dal_wifi_set_ACL(index_vlan, index_ssid, &acl);
    result = dal_wifi_set_commit(index_vlan, index_ssid);
    gui_debug(">>>>>>%s():%d, result=[%d]\n\n\n", __func__, __LINE__, result);
    return result;
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetWirelessMacFilter
 *
 *  [DESCRIPTION]:
 *
 *       Get MAC Filter Settings.
 *
 *  [return]:index vlan|index ssid|wps enable|appin
 **************************************************************************/
void cgiGetWirelessMacFilter(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_64] = {0};
    unsigned int index_vlan = INDEX_VLAN;
    unsigned int index_ssid = INDEX_SSID;
    acl_info_t acl = {0};
    char mac_list[SIZE_32*SIZE_20] = {0};

    gui_debug(">>>>>>Enter %s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
    get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
    gui_debug("status get cur vlan=%d,ssid=%d\n",index_vlan,index_ssid);

    //Current VLAN
    sprintf(buf, "%d|", index_vlan);
    strcat(varValue, buf);

    //Current SSID
    sprintf(buf, "%d|", index_ssid);
    strcat(varValue, buf);

    if(dal_wifi_get_ACL(index_vlan, index_ssid, &acl)) {
        gui_debug("--%s-- FAILED: dal_wifi_get_ACL--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        return;
    }

    //MAC Filtering Type
    if(acl.policy == ACL_DISABLE)
        strcat(varValue, "0|");
    else if(acl.policy == ACL_ALLOW)
        strcat(varValue, "1|");
    else if(acl.policy == ACL_DENY)
        strcat(varValue, "2|");

    //MAC Filter List
    if(acl.count>0)
    {
        for(int i=0; i<acl.count; i++) {
            snprintf(mac_list+strlen(mac_list), sizeof(mac_list)-strlen(mac_list), "%s ", acl.mac[i]);
        }
        strcat(varValue, mac_list);
    }else{
        strcat(varValue, "|");
    }

    gui_debug(">>>>>>%s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}



/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetSSIDList
 *
 *  [DESCRIPTION]:
 *
 *       Get SSID list
 *
 *  [return]: index|ifname|ssid name|index vlan|index ssid|is5G||
 **************************************************************************/
void cgiGetSSIDList(int argc, char **argv, char *varValue)
{   
    char buf[256] = {0};
    
    memset(ssid_list_info,0,sizeof(ssid_list_info));
    gui_debug(">>>>>>Enter %s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
    int ret = 0;
    int ssid_num=0;
    ret = dal_wifi_get_ssid_list(ssid_list_info,MAX_LIST_COUNT);
    if (ret == 0) return;

    for(ssid_num = 0; ssid_num<ret;ssid_num++)
    {
        gui_debug("%s():%d - ssid[%d]: ifname=[%s], SSID=[%s], \
            index_vlan=[%d], index_ssid=[%d], is5g=[%d]\n",\
                __func__, __LINE__, ssid_num, ssid_list_info[ssid_num].ifname, ssid_list_info[ssid_num].ssid,
                ssid_list_info[ssid_num].index_vlan, ssid_list_info[ssid_num].index_ssid, ssid_list_info[ssid_num].is5g);

        snprintf(buf,sizeof(buf),"%d|%s|%s|%d|%d|%d||",ssid_num,ssid_list_info[ssid_num].ifname,getValueFromDal(ssid_list_info[ssid_num].ssid), ssid_list_info[ssid_num].index_vlan, ssid_list_info[ssid_num].index_ssid, ssid_list_info[ssid_num].is5g);
        strcat(varValue,buf);
    }

    gui_debug(">>>>>>%s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetChannelList
 *
 *  [DESCRIPTION]:
 *
 *       Get all channels by bandwidth
 *
 *  [return]: 20/channels||40/channels||80/channels||
 **************************************************************************/
void cgiGetChannelList(int argc, char **argv, char *varValue)
{
    gui_debug(">>>>>>Enter %s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
    int  channels[64]= {0};
    char list_buf[256]="";
    unsigned int cur_index_vlan = INDEX_VLAN;
    unsigned int cur_index_ssid = INDEX_SSID;
    unsigned int is5g_chan_list = 0;
    unsigned int value = 0;
    int chan_ele_num = 0;
    int ssid_list_num = 0;
    band_width_t bandwidth = 0;
    //get current index ssid and vlan
    get_cur_vlan_ssid_index(&cur_index_vlan,&cur_index_ssid);


    //Judge the current ssid is 5g or not.
    if(strlen(ssid_list_info[ssid_list_num].ssid)<=0)
    {
        memset(ssid_list_info,0,sizeof(ssid_list_info));
        int ret = 0;
        ret = dal_wifi_get_ssid_list(ssid_list_info,MAX_LIST_COUNT);
        if (ret == 0) 
        {
            gui_debug("Get chan List dal_wifi_get_ssid_list error");
            return;
        }
    }
    for(ssid_list_num = 0; strlen(ssid_list_info[ssid_list_num].ssid);ssid_list_num++)
    {
        if(ssid_list_info[ssid_list_num].index_ssid==cur_index_ssid)
        {
            is5g_chan_list = ssid_list_info[ssid_list_num].is5g;
            gui_debug("\n####000000ChannelList get cur vlan=%d,ssid=%d,is5g_chan_list=%d\n",cur_index_vlan,cur_index_ssid,is5g_chan_list);
            break;
        }
    }
    gui_debug("\nChannelList get cur vlan=%d,ssid=%d,is5g_chan_list=%d\n",cur_index_vlan,cur_index_ssid,is5g_chan_list);



    //Get all channels by bandwidth: 20/channels||40/channels||80/channels||
    for(int i=0; i<3; i++){
        memset(list_buf,0,sizeof(list_buf));
        memset(channels,0,sizeof(channels));

        if(i == 0)
        {
            bandwidth = BAND_WIDTH_20MHZ;
            sprintf(list_buf, "%d:", 20);
        }
        else if(i == 1)
        {
            bandwidth = BAND_WIDTH_40MHZ;
            sprintf(list_buf, "%d:", 40);
        }
        else if(i == 2)
        {
            bandwidth = BAND_WIDTH_80MHZ;
            sprintf(list_buf, "%d:", 80);
        }

        strcat(varValue, list_buf);

        value = dal_wifi_get_possible_channels(cur_index_vlan, cur_index_ssid,is5g_chan_list, bandwidth ,channels,sizeof(channels));//need clare modify
        gui_debug("\nvalue=%d,chan0=%d,chan1=%d,cha2=%d\n",value,channels[0],channels[1],channels[2]);

        if (value>=0)
        {
            for(chan_ele_num = 0; chan_ele_num < sizeof(channels); chan_ele_num++)
            {
                if(channels[chan_ele_num] > 0)
                {
                    sprintf(list_buf, "%d|", channels[chan_ele_num]);
                    strcat(varValue, list_buf);
                }
                else
                    break;
            }  
        }
        strcat(varValue, "/");
    }
    gui_debug(">>>>>>%s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetWiFiNetList
 *
 *  [DESCRIPTION]:
 *
 *           Get Wireless Network List
 *
 *  [return]:index_vlan|index_ssid|SSID Enable|Radio Enable|SSID Name|SSID Broadcast|Security Mode|KEY|KEY MODE|WEP Key Length|Wireless band|Channel|Bandwidth|Transmit Rate|Power Level|
 **************************************************************************/
void cgiGetWiFiNetList(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    char str[BUFLEN_256]= {0};
    unsigned int value = 0;
    unsigned int ch = 0;
    int ssid_list_num = 0;
    unsigned int index_vlan = INDEX_VLAN;
    unsigned int index_ssid = INDEX_SSID;
    gui_debug(">>>>>>Enter %s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
    
    if(strlen(ssid_list_info[ssid_list_num].ssid)<=0)
    {
        memset(ssid_list_info,0,sizeof(ssid_list_info));
        int ret = 0;
        ret = dal_wifi_get_ssid_list(ssid_list_info,MAX_LIST_COUNT);
        if (ret == 0) 
        {
            gui_debug("Get WiFi Net List dal_wifi_get_ssid_list error");
            return;
        }
    }
    for(ssid_list_num = 0; strlen(ssid_list_info[ssid_list_num].ssid);ssid_list_num++)
    {
        gui_debug("%s():%d - ssid[%d]: ifname=[%s], SSID=[%s], \
            index_vlan=[%d], index_ssid=[%d], is5g=[%d]\n",\
                __func__, __LINE__, ssid_list_num, ssid_list_info[ssid_list_num].ifname, ssid_list_info[ssid_list_num].ssid,
                ssid_list_info[ssid_list_num].index_vlan, ssid_list_info[ssid_list_num].index_ssid, ssid_list_info[ssid_list_num].is5g);
    
        index_vlan = ssid_list_info[ssid_list_num].index_vlan;
        index_ssid = ssid_list_info[ssid_list_num].index_ssid;
        //Current VLAN
        sprintf(buf, "%d|", index_vlan);
        strcat(varValue, buf);
    
        //Current SSID
        sprintf(buf, "%d|", index_ssid);
        strcat(varValue, buf);
    
        //SSID Enabled
        value = dal_wifi_get_SSIDEnable(index_vlan, index_ssid);
        gui_debug("SSID Enable: value=%d\n",value);
        sprintf(buf, "%d|", value);
        strcat(varValue, buf);
    
        //Radio Enable
        value = dal_wifi_get_RadioEnabled(index_vlan, index_ssid);
        gui_debug("Radio Enable: value=%d\n",value);
        sprintf(buf, "%d|", value);
        strcat(varValue, buf);
    
        //SSID Name
        if(dal_wifi_get_ssid(index_vlan, index_ssid, str, sizeof(str))) {
            gui_debug("--%s-- FAILED: dal_wifi_get_ssid--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
            strcat(varValue, "|");
        } else {
            gui_debug("Set SSID: value=%s\n",str);
            sprintf(buf, "%s|", getValueFromDal(str));
            strcat(varValue, buf);
        }

        //Frequency is 5G
        sprintf(buf, "%u|", ssid_list_info[ssid_list_num].is5g);
        strcat(varValue, buf);   

        //Wireless band
        value = dal_wifi_get_standard(index_vlan, index_ssid);
        gui_debug("Mode: value=%d\n",value);
        switch(value)
        {
            case IEEE80211_B:
                    strcat(varValue, "0|");
                    break;
                case IEEE80211_G:
                    strcat(varValue, "1|");
                    break;
                case IEEE80211_N:
                    strcat(varValue, "2|");
                    break;
                case IEEE80211_B|IEEE80211_G:
                    strcat(varValue, "3|");
                    break;
                case IEEE80211_G|IEEE80211_N:
                    strcat(varValue, "4|");
                    break;
                case IEEE80211_B|IEEE80211_G|IEEE80211_N:
                    strcat(varValue, "5|");
                    break;
                case IEEE80211_A|IEEE80211_N:
                    strcat(varValue, "6|");
                    break;
                case IEEE80211_A|IEEE80211_N|IEEE80211_AC:
                    strcat(varValue, "7|");
                    break;
                default:
                    strcat(varValue, " |");     //value: 0 - auto, 1 - 802.11a
                    break;
        }        

        //Wireless MAC
        value = dal_wifi_get_mac_address(index_vlan, index_ssid,buf,sizeof(buf));
        gui_debug("WiFi MAC: value=%d,buf=%s\n",value,buf);
        if(safe_strlen(buf) > 0){
            strcat(varValue, buf);
            strcat(varValue, "|");
        } else{
            strcat(varValue, " |");
        }
        

        //Channel
        if(dal_wifi_get_channel(index_vlan, index_ssid, &value, &ch, NULL)) {
            gui_debug("--%s-- FAILED: dal_wifi_get_channel--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
            value = 0;
        }
        gui_debug("Menu Channel: value=%d; Bandwidth: value=%d\n",value, ch);
        if(dal_wifi_get_AutoChannelEnable(index_vlan, index_ssid))
        {
            gui_debug("Auto Channel. channel=%d\n", value);            
            sprintf(buf, "Auto,%d|", value);
            strcat(varValue, buf);
        } else {
            sprintf(buf, "%d|", value);
            strcat(varValue, buf);
        }
        
               
        //Security mode
        value = dal_wifi_get_security_mode(index_vlan, index_ssid);
        gui_debug("Security: value=%d\n",value);
        if (SECURITY_MODE_PSK_WPAWPA2==value || SECURITY_MODE_EAP_WPAWPA2==value)
        {
            strcat(varValue, "4|");
        }
        else if (SECURITY_MODE_PSK_WPA2==value || SECURITY_MODE_EAP_WPA2==value)
        {
            strcat(varValue, "3|");
        }
        else if (SECURITY_MODE_PSK_WPA==value || SECURITY_MODE_EAP_WPA==value)
        {
            strcat(varValue, "2|");
        }
        else if (SECURITY_MODE_WEP64==value||SECURITY_MODE_WEP128==value)
        {
            strcat(varValue, "1|");
        }
        else
        {
            strcat(varValue, "0|");
        }

        //get  Associated Clients count
         /*  030 - associated device info
        function    : dal_wifi_get_associated_station_info
        parameter   :
        index_vlan  : [in] from wifi_get_ssid_list
        index_ssid  : [in] from wifi_get_ssid_list
        sta_info    : [in/out]    buffer used to receive station info
        max_count   : [in] max count of station info elements
        return value : associated device count on success, -1 on failure
        */
        wlan_sta_info sta_info[32];
        value = dal_wifi_get_associated_station_info(index_vlan, index_ssid, sta_info, 32);
        gui_debug("Associated Clients count = %d", value);
        sprintf(buf, "%d||", value);
        strcat(varValue, buf);

    }

    gui_debug(">>>>>>%s():%d,varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}


#if 0
/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostWirelessAdvancedWps
 *
 *  [DESCRIPTION]:
 *
 *       Post wps settings.
 *
 *  [return]:
 **************************************************************************/
int cgiPostWirelessAdvancedWps()
{
    gui_debug(">>>>>>Enter %s():%d, wireless_wps_enable_type=%d; submit_button_name=%s; wireless_wps_pin=%s\n\n\n", __func__, __LINE__, gWebParams.wireless_wps_enable_type, gWebParams.submit_button_name, gWebParams.wireless_wps_pin);
    tsl_rv_t result = TSL_RV_SUC;
    unsigned int index_vlan = INDEX_VLAN;
    unsigned int index_ssid = INDEX_SSID;
    wps_data_t data = {0};

    get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
    if (gWebParams.wireless_wps_enable_type == 1)
    {
        data.enable = 1;
        if(!tsl_strcmp(gWebParams.submit_button_name, "push"))
        {
            data.method = WPS_PBC;
        }
        else if(!tsl_strcmp(gWebParams.submit_button_name, "generate_ap_pin"))
        {
            data.method = WPS_AP_RANDOM_PIN;
        }
        else if(!tsl_strcmp(gWebParams.submit_button_name, "use_device_pin"))
        {
            data.method = WPS_DEVICE_PIN;
            data.pin= atoi(gWebParams.wireless_wps_pin);
        }
        else if(!tsl_strcmp(gWebParams.submit_button_name, "static_ap_pin"))
        {
            data.method = WPS_AP_STATIC_PIN;
            data.pin= atoi(gWebParams.wireless_wps_pin);
        } 
    }
    else if (gWebParams.wireless_wps_enable_type == 0)
    {
        data.enable = 0;
    }

    result = dal_wifi_set_wps(index_vlan, index_ssid, &data);
    result = dal_SaveConfig();
    gui_debug(">>>>>>%s():%d, result=[%d]\n\n\n", __func__, __LINE__, result);
    return result;
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostWirelessAdvancedWps
 *
 *  [DESCRIPTION]:
 *
 *       Get wps settings.
 *
 *  [return]:index vlan|index ssid|wps enable|appin
 **************************************************************************/
void cgiGetWirelessAdvancedWps(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_64] = {0};
    unsigned int index_vlan = INDEX_VLAN;
    unsigned int index_ssid = INDEX_SSID;
    wps_data_t data = {0};

    get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
    gui_debug(">>>>>>Enter %s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
    if(dal_wifi_get_wps(index_vlan, index_ssid, &data)) {
        return;
    }

    //Current SSID adn VLAN
    sprintf(buf, "%d|", index_vlan);
    strcat(varValue, buf);

    sprintf(buf, "%d|", index_ssid);
    strcat(varValue, buf);

    //WPS Enable
    sprintf(buf, "%d|", data.enable);
    strcat(varValue, buf);

    //APPIN
    sprintf(buf, "%.08d", data.pin);
    strcat(varValue, buf);

    gui_debug(">>>>>>%s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiWiFiStatus
 *
 *  [DESCRIPTION]:
 *
 *       Get WiFi Status.
 *
 *  [return]:index vlan|index ssid|radio enable|ssid|user Type|security type|wpa/wep key|auth type|ssid status
 **************************************************************************/
void cgiGetWirelessStatus(int argc, char **argv, char *varValue)
{

    char buf[BUFLEN_256] = {0};
    char str[BUFLEN_256]= {0};
    unsigned int value = 0;
    unsigned int ch = 0;
    unsigned int index_vlan = INDEX_VLAN;
    unsigned int index_ssid = INDEX_SSID;
    gui_debug(">>>>>>Enter %s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
    get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
    gui_debug("status get cur vlan=%d,ssid=%d\n",index_vlan,index_ssid);

    for (int i = 0; i < 4; ++i)
    {
        //5: 2.4G User; 6: 2.4G Guest; 1: 5G User; 2: 5G Guest
        if(i == 0)
            index_ssid = 5;
        else if(i == 1)
            index_ssid = 6;
        else if(i == 2)
            index_ssid = 1;
        else if(i == 3)
            index_ssid = 2;

        //VLAN Index
        sprintf(buf, "%d|", index_vlan);
        strcat(varValue, buf);

        //SSID Index
        sprintf(buf, "%d|", index_ssid);
        strcat(varValue, buf);

        //Radio Enable
        value = dal_wifi_get_RadioEnabled(index_vlan, index_ssid);
        gui_debug("Radio Enable: value=%d\n",value);
        sprintf(buf, "%d|", value);
        strcat(varValue, buf);

        //SSID
        if(dal_wifi_get_ssid(index_vlan, index_ssid, str, sizeof(str))) {
            gui_debug("--%s-- FAILED: dal_wifi_get_ssid--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
            strcat(varValue, "|");
        } else {
            gui_debug("Get SSID: value=%s\n",str);
            sprintf(buf, "%s|", str);
            strcat(varValue, buf);
        }
        

        if(index_ssid == 1 || index_ssid == 5)
            strcat(varValue, "User|");
        else
            strcat(varValue, "Guest|");

        //Authentication type & Network password
        //Security Enabled
        value = dal_wifi_get_security_mode(index_vlan, index_ssid);
        gui_debug("Security: value=%d\n",value);
        if (SECURITY_MODE_PSK_WPAWPA2==value)
        {
            strcat(varValue, "4|");
        }
        else if (SECURITY_MODE_PSK_WPA2==value)
        {
            strcat(varValue, "3|");
        }
        else if (SECURITY_MODE_PSK_WPA==value)
        {
            strcat(varValue, "2|");
        }
        else if (SECURITY_MODE_WEP64==value)
        {
            strcat(varValue, "1|");
        }
        else
        {
            strcat(varValue, "0|");
        }

        //Key and Type
        wep_data_t data_wep = {0};
        wpa_data_t data_wpa = {0};
        if(SECURITY_MODE_WEP64==value){
            if(dal_wifi_get_wep(index_vlan, index_ssid, &data_wep)) {
                gui_debug("--%s-- FAILED: dal_wifi_get_wep--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
                //Key
                strcat(varValue, "|");
                //Type
                strcat(varValue, "|");
            }else{
                //Key
                sprintf(buf, "%s|", data_wep.key[0].psk);
                strcat(varValue, buf);
                //Type
                sprintf(buf, "%d|", data_wep.key[0].type);
                strcat(varValue, buf);
            }
            
        }
        else if(SECURITY_MODE_PSK_WPA==value || SECURITY_MODE_PSK_WPA2==value || SECURITY_MODE_PSK_WPAWPA2==value)
        {
            if(dal_wifi_get_wpa(index_vlan, index_ssid, &data_wpa)) {
                gui_debug("--%s-- FAILED: dal_wifi_get_wpa--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
                //Key
                strcat(varValue, "|");
                //Type
                strcat(varValue, "|");
            }else{
                //Key
                sprintf(buf, "%s|", data_wpa.key[0].psk);
                strcat(varValue, buf);
                //Type
                strcat(varValue, "|");
            }
            
        }else{
            strcat(varValue, "||");
        }

        //SSID Enabled
        value = dal_wifi_get_SSIDEnable(index_vlan, index_ssid);
        gui_debug("SSID Enable: value=%d\n",value);
        sprintf(buf, "%d/", value);
        strcat(varValue, buf);
    }
    gui_debug(">>>>>>%s():%d,varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetLANWiFiStatus
 *
 *  [DESCRIPTION]:
 *
 *       Get WiFi Status for Local Network status page.
 *
 *  [return]:index vlan|index ssid|ssid enable|radio enable|ssid|SSID Broadcast|Mode|Channel type|current channel|Bandwidth|Security type|key|key type|Power Level|MAC Address Filtering Enable|Configured Data Rate|Supported Data Rates|Current Signal Strength(dBm)
 **************************************************************************/
void cgiGetLANWiFiStatus(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    char str[BUFLEN_256]= {0};
    unsigned int value = 0;
    unsigned int ch = 0;
    unsigned int index_vlan = INDEX_VLAN;
    unsigned int index_ssid = INDEX_SSID;
    gui_debug(">>>>>>Enter %s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
    get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
    gui_debug("status get cur vlan=%d,ssid=%d\n",index_vlan,index_ssid);

    //Current SSID adn VLAN
    sprintf(buf, "%d|", index_vlan);
    strcat(varValue, buf);

    sprintf(buf, "%d|", index_ssid);
    strcat(varValue, buf);

    //SSID Enabled
    value = dal_wifi_get_SSIDEnable(index_vlan, index_ssid);
    gui_debug("SSID Enable: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    //Radio Enable
    value = dal_wifi_get_RadioEnabled(index_vlan, index_ssid);
    gui_debug("Radio Enable: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    //SSID
    if(dal_wifi_get_ssid(index_vlan, index_ssid, str, sizeof(str))) {
        gui_debug("--%s-- FAILED: dal_wifi_get_ssid--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        strcat(varValue, "|");
    } else {
        gui_debug("Set SSID: value=%s\n",str);
        sprintf(buf, "%s|", str);
        strcat(varValue, buf);
        
    }
    
    //SSID Broadcast
    value = dal_wifi_get_SSIDAdvertisementEnabled(index_vlan, index_ssid);
    gui_debug("Advertisement SSID: value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    //Mode
    value = dal_wifi_get_standard(index_vlan, index_ssid);
    gui_debug("Mode: value=%d\n",value);
    switch(value)
    {
        case IEEE80211_B:
                strcat(varValue, "B|");
                break;
            case IEEE80211_G:
                strcat(varValue, "G|");
                break;
            case IEEE80211_N:
                strcat(varValue, "N|");
                break;
            case IEEE80211_B|IEEE80211_G:
                strcat(varValue, "B,G|");
                break;
            case IEEE80211_G|IEEE80211_N:
                strcat(varValue, "G,N|");
                break;
            case IEEE80211_B|IEEE80211_G|IEEE80211_N:
                strcat(varValue, "B,G,N|");
                break;
            case IEEE80211_A|IEEE80211_N:
                strcat(varValue, "A,N|");
                break;
            case IEEE80211_A|IEEE80211_N|IEEE80211_AC:
                strcat(varValue, "A,N,AC|");
                break;
            default:
                strcat(varValue, "|");
                break;
    }

    //Channel
    if(dal_wifi_get_AutoChannelEnable(index_vlan, index_ssid))
    {
        //Auto Channel.
        gui_debug("Auto Channel.\n");
        value = 0;
    }else{
        //Manual Channel
        value = 1;
    }
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    if(dal_wifi_get_channel(index_vlan, index_ssid, &value, &ch, NULL)) {
        gui_debug("--%s-- FAILED: dal_wifi_get_channel--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        value = 0;
    }
    gui_debug("Current Channel: value=%d; Bandwidth: value=%d\n",value, ch);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    //Bandwidth
    switch(ch){
        case BAND_WIDTH_20MHZ:
            strcat(varValue, "20|");
            break;
        case BAND_WIDTH_40MHZ:
            strcat(varValue, "40|");
            break;
        case BAND_WIDTH_80MHZ:
            strcat(varValue, "80|");
            break;
        default:
            strcat(varValue, "0|");
            break;
    }


    //Security Enabled
    value = dal_wifi_get_security_mode(index_vlan, index_ssid);
    gui_debug("Security: value=%d\n",value);
    if (SECURITY_MODE_PSK_WPAWPA2==value)
    {
        strcat(varValue, "WPA/WPA-2 Mixed|");
    }
    else if (SECURITY_MODE_PSK_WPA2==value)
    {
        strcat(varValue, "WPA2|");
    }
    else if (SECURITY_MODE_PSK_WPA==value)
    {
        strcat(varValue, "WPA|");
    }
    else if (SECURITY_MODE_WEP64==value)
    {
        strcat(varValue, "WEP|");
    }
    else
    {
        strcat(varValue, "Off|");
    }

    //Key and Type
    wep_data_t data_wep = {0};
    wpa_data_t data_wpa = {0};
    if(SECURITY_MODE_WEP64==value){
        if(dal_wifi_get_wep(index_vlan, index_ssid, &data_wep)) {
            gui_debug("--%s-- FAILED: dal_wifi_get_wep--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
            //Key
            strcat(varValue, "|");
            //Type
            strcat(varValue, "|");
        }else{
            //Key
            sprintf(buf, "%s|", data_wep.key[0].psk);
            strcat(varValue, buf);
            //Type
            sprintf(buf, "%d|", data_wep.key[0].type);
            strcat(varValue, buf);
        }
        
    }
    else if(SECURITY_MODE_PSK_WPA==value || SECURITY_MODE_PSK_WPA2==value || SECURITY_MODE_PSK_WPAWPA2==value)
    {
        if(dal_wifi_get_wpa(index_vlan, index_ssid, &data_wpa)) {
            gui_debug("--%s-- FAILED: dal_wifi_get_wpa--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
            //Key
            strcat(varValue, "|");
            //Type
            strcat(varValue, "|");
        }else{
            //Key
            sprintf(buf, "%s|", data_wpa.key[0].psk);
            strcat(varValue, buf);
            //Type
            strcat(varValue, buf);
        }
        
    }else{
        strcat(varValue, "||");
    }
    
    //Power Level
    strcat(varValue, "|");
    //MAC Address Filtering Enable
    acl_info_t acl = {0};
    if(dal_wifi_get_ACL(index_vlan, index_ssid, &acl)) {
        gui_debug("--%s-- FAILED: dal_wifi_get_ACL--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        strcat(varValue, "0|");
    }else{
        sprintf(buf, "%d|", acl.policy);
        gui_debug("MAC Address Filtering Enable: value=%d\n",acl.policy);
        strcat(varValue, buf);
    }

    //Configured Data Rate
    value = dal_wifi_get_TxRate(index_vlan, index_ssid);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    //Supported Data Rates
    char buf_rates[BUFLEN_256] = {0};
    if(dal_wifi_get_PossibleDataTransmitRates(index_vlan, index_ssid, &buf_rates, sizeof(buf_rates))){
        gui_debug("--%s-- FAILED: dal_wifi_get_PossibleDataTransmitRates--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        strcat(varValue, "|");
    }
    else{
        sprintf(buf, "%s|", buf_rates);
        strcat(varValue, buf);
    }
    //Current Signal Strength(dBm)
    value = dal_wifi_get_signal_level(index_vlan, index_ssid);
    gui_debug("Current Signal Strength(dBm): value=%d\n",value);
    sprintf(buf, "%d|", value);
    strcat(varValue, buf);

    gui_debug(">>>>>>%s():%d,varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetLANWiFiStatistics
 *
 *  [DESCRIPTION]:
 *
 *       Get LAN Wireless Statistics for Local Network status page.
 *
 *  [return]:Transmit Bytes|Receive Bytes|Transmit Packets|Receive Packets|Transmit Error Packets|Receive Error Packets|Transmit Discard Packets|Receive Discard Packets
 **************************************************************************/
void cgiGetLANWiFiStatistics(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_64] = {0};
    wifi_stats_t data_wifi_stats = {0};

    unsigned int index_vlan = INDEX_VLAN;
    unsigned int index_ssid = INDEX_SSID;
    gui_debug(">>>>>>Enter %s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
    get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
    gui_debug("status get cur vlan=%d,ssid=%d\n",index_vlan,index_ssid);
    if(dal_wifi_get_stats(index_vlan, index_ssid, &data_wifi_stats))
    {
        gui_debug("--%s-- FAILED: dal_wifi_get_stats--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        return;
    }
    sprintf(buf, "%d|", data_wifi_stats.tx_total_bytes);
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%d|", data_wifi_stats.rx_total_bytes);
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%d|", data_wifi_stats.tx_total_packets);
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%d|", data_wifi_stats.rx_total_packets);
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%d|", data_wifi_stats.tx_errors);
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%d|", data_wifi_stats.rx_errors);
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%d|", data_wifi_stats.tx_discard);
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%d|", data_wifi_stats.rx_discard);
    strcat(varValue, buf);

    gui_debug("Local net wifi statistics varvalue = %s",varValue);
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostMacFilter
 *
 *  [DESCRIPTION]:
 *
 *       MAC Filter.
 *
 *  [return]:
 **************************************************************************/
int cgiPostMacFilter()
{
    tsl_rv_t result = TSL_RV_SUC;
    unsigned int index_vlan = INDEX_VLAN;
    unsigned int index_ssid = INDEX_SSID;
    acl_info_t acl = {0};
    char mac_list[SIZE_32*SIZE_20]="";

    get_cur_vlan_ssid_index(&index_vlan,&index_ssid);

    //MAC Filtering Type
    if(gWebParams.wireless_macPolicy == 1)
        acl.policy = ACL_ALLOW;
    else if(gWebParams.wireless_macPolicy == 2)
        acl.policy = ACL_DENY;
    else
        acl.policy = ACL_DISABLE;

    //MAC Filter List
    if(gWebParams.wireless_macCount >0)
    {
        char *p = NULL;
        char *blank = " ";
        int i = 0;
        p = strtok(gWebParams.wireless_maclist, blank);
        while(p) {
            if(i >= SIZE_32) break;
            snprintf(acl.mac[i], SIZE_20, "%s", p);
            i++;
            p = strtok(NULL, blank);
        }
    }

    //MAC Filter Count
    acl.count = gWebParams.wireless_macCount;

    result = dal_wifi_set_ACL(index_vlan, index_ssid, &acl);
    result = dal_wifi_set_commit(index_vlan, index_ssid);
    gui_debug(">>>>>>%s():%d, result=[%d]\n\n\n", __func__, __LINE__, result);
    return result;
}

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetMacFilter
 *
 *  [DESCRIPTION]:
 *
 *       Get MAC Filter Settings.
 *
 *  [return]:index vlan|index ssid|wps enable|appin
 **************************************************************************/
void cgiGetMacFilter(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_64] = {0};
    unsigned int index_vlan = 1;
    unsigned int index_ssid = INDEX_SSID;
    acl_info_t acl = {0};
    char mac_list[SIZE_32*SIZE_20] = {0};

    get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
    gui_debug(">>>>>>Enter %s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
    if(dal_wifi_get_ACL(index_vlan, index_ssid, &acl)) {
        gui_debug("--%s-- FAILED: dal_wifi_get_ACL--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
        return;
    }

    //Current SSID adn VLAN
    sprintf(buf, "%d|", index_vlan);
    strcat(varValue, buf);

    sprintf(buf, "%d|", index_ssid);
    strcat(varValue, buf);

    //MAC Filtering Type
    if(acl.policy == ACL_DISABLE)
        strcat(varValue, "0|");
    else if(acl.policy == ACL_ALLOW)
        strcat(varValue, "1|");
    else if(acl.policy == ACL_DENY)
        strcat(varValue, "2|");

    //MAC Filter List
    if(acl.count>0)
    {
        for(int i=0; i<acl.count; i++) {
            snprintf(mac_list+strlen(mac_list), sizeof(mac_list)-strlen(mac_list), "%s ", acl.mac[i]);
        }
        strcat(varValue, mac_list);
    }else{
        strcat(varValue, "|");
    }

    gui_debug(">>>>>>%s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}

 /**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostWirelessRadioReset
 *
 *  [DESCRIPTION]:
 *
 *           Radio Reset
 *
 *  [return]:
 **************************************************************************/
int cgiPostWirelessRadioReset()
{
    char buf[BUFLEN_64] = {0};
    unsigned int index_vlan = 1;
    unsigned int index_ssid = INDEX_SSID;
    acl_info_t acl = {0};
    char mac_list[SIZE_32*SIZE_20] = {0};

    get_cur_vlan_ssid_index(&index_vlan,&index_ssid);
    gui_debug("vlan=%d,ssid=%d\n",index_vlan,index_ssid);
    if(dal_wifi_radio_reset(index_vlan, index_ssid))
    {
        gui_debug("--%s-- FAILED: dal_wifi_radio_reset--index_vlan=%d, index_ssid=%d\n", __FUNCTION__, index_vlan, index_ssid);
    }
}
#endif
