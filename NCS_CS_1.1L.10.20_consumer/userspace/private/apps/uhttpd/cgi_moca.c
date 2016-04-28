#include "uhttpd.h"
#include "uhttpd-utils.h"
#include "cgi_main.h"
#include "cgi_moca.h"
#include "cgi_act_tr69.h"

/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetMocaInfo
 *
 *  [DESCRIPTION]:
 *
 *   cli -g Device.MoCA.Interface.1.Status
 *   cli -g Device.MoCA.Interface.1.FreqCurrentMaskSetting
 *   cli -g Device.MoCA.Interface.1.BeaconPowerLimit
 *   cli -g Device.MoCA.Interface.1.PreferredNC
 *   cli -g Device.MoCA.Interface.1.PrivacyEnabledSetting
 *   cli -g Device.MoCA.Interface.1.KeyPassphrase 
 *
 *       Get Moca infomation.
 *
 *  [return]:status|FreqCurrentMaskSetting|BeaconPowerLimit|PreferredNC|PrivacyEnabledSetting|KeyPassphrase|mac|
 **************************************************************************/
void cgiGetMocaInfo(int argc, char **argv, char *varValue)
{
    gui_debug("cgiGetMocaInfo start");

    char moca_node[BUFLEN_64] = {0};

    char moca_status[BUFLEN_8] = {0};
    char moca_freqcurrentmasksetting[BUFLEN_32] = {0};
    char moca_beaconpowerlimit[BUFLEN_4] = {0};
    char moca_preferrednc[BUFLEN_4] = {0};
    char moca_privacyenable[BUFLEN_4] = {0};
    char moca_keypass[BUFLEN_24] = {0};

    char moca_ncinator[BUFLEN_4] = {0};

    //for popen, get nc mac
    char cmd[BUFLEN_128] = {'\0'};
    FILE * fp = NULL;
    char line[BUFLEN_128] = {'\0'};    
    char *p = NULL;

    char buf[BUFLEN_128] = {0};
    char bufmac[BUFLEN_32] = {0};
    int iNcId = 0;

    sprintf(moca_node, "%s.%d", "Device.MoCA.Interface", 1);

    
    if(tr69GetUnfreshLeafData(moca_status, moca_node, "Status")) {
        gui_debug("get Status error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, moca_status);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(moca_freqcurrentmasksetting, moca_node, "FreqCurrentMaskSetting")) {
        gui_debug("get FreqCurrentMaskSetting error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, moca_freqcurrentmasksetting);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(moca_beaconpowerlimit, moca_node, "BeaconPowerLimit")) {
        gui_debug("get BeaconPowerLimit error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, moca_beaconpowerlimit);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(moca_preferrednc, moca_node, "PreferredNC")) {
        gui_debug("get PreferredNC error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, moca_preferrednc);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(moca_privacyenable, moca_node, "PrivacyEnabledSetting")) {
        gui_debug("get PrivacyEnabledSetting error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, moca_privacyenable);
        strcat(varValue, "|");
    }

    if(tr69GetUnfreshLeafData(moca_keypass, moca_node, "KeyPassphrase")) {
        gui_debug("get KeyPassphrase error");
        strcat(varValue, " |");
    }else{
        strcat(varValue, moca_keypass);
        strcat(varValue, "|");
    }


/* sdk 4.1601a, support "moca show --nodestatus"
    //mac
    if(tr69GetUnfreshLeafData(moca_ncinator, moca_node, "NetworkCoordinator")) {
        gui_debug("get NetworkCoordinator error");
        strcat(varValue, " |");

        return;
    }
    
    sprintf(cmd, "moca show --nodestatus %s", moca_ncinator);
    gui_debug("cmd = %s", cmd);
    fp = popen(cmd, "r");

    if(NULL == fp) {
        strcat(varValue, " |");
        goto mac_end;
    }

    while (fgets(line, BUFLEN_128, fp) != NULL) {
        if(strlen(line) > 0)
        {
            if (line[strlen(line)-1] == '\n')
            line[strlen(line)-1] = '\0'; 
            gui_debug("line => %s", line);
            
            if(strstr(line, "MAC Address") != NULL)
            {
                p = strchr(line, ':');
                if(NULL != p) {
                    gui_debug("p+1 => %s", p+1);

                    strcat(varValue, p+1);
                    strcat(varValue, "|");
                }                

                memset(line, '\0', sizeof(line));
                goto mac_end;
            }
        }
        memset(line, '\0', sizeof(line));
    }
    strcat(varValue, " |");

mac_end:
    pclose(fp);
*/


    //mocap get --network_status: get current moca id
    sprintf(cmd, "mocap get --network_status");
    gui_debug("cmd = %s", cmd);
    fp = popen(cmd, "r");

    if(NULL == fp) {        
        goto error;
    }

    while (fgets(line, BUFLEN_128, fp) != NULL) {
        if(strlen(line) > 0)
        {
            if (line[strlen(line)-1] == '\n')
            line[strlen(line)-1] = '\0'; 
            gui_debug("line => %s", line);
            
            if(strstr(line, "nc_node_id") != NULL) {
                p = strchr(line, ':');
                if(NULL != p) {
                    iNcId= atoi(p+1);
                    gui_debug("iNcId=> %d", iNcId);
                }
            } 
        }
        memset(line, '\0', sizeof(line));
    }
    memset(buf, '\0', sizeof(buf));
    memset(bufmac, '\0', sizeof(bufmac));
    pclose(fp);



    memset(cmd, '\0', sizeof(cmd));
    sprintf(cmd, "mocap get --node_stats index %d", iNcId);
    sprintf(buf, "Node %d  ", iNcId);
    gui_debug("cmd = %s", cmd);        
    fp = popen(cmd, "r");

    if(NULL == fp) {
        goto error;
    }

    while (fgets(line, BUFLEN_128, fp) != NULL) {
        if(strlen(line) > 0)
        {
            if (line[strlen(line)-1] == '\n')
            line[strlen(line)-1] = '\0'; 
            gui_debug("line => %s", line);
            
            if((p = strstr(line, buf)) != NULL) {                
                if(NULL != p) {
                    int _offset = safe_strlen(buf);
                    gui_debug("p+%d => %s", _offset, p+_offset);
                    sprintf(bufmac, "%s", p+_offset);
                }
            }
        }
        memset(line, '\0', sizeof(line));
    }

    strcat(varValue, bufmac);
    strcat(varValue, " |");

    memset(bufmac, '\0', sizeof(bufmac));
    memset(buf, '\0', sizeof(buf));
    memset(cmd, '\0', sizeof(cmd));

    pclose(fp);

    goto end;

error:
    pclose(fp);
    gui_debug("popen error");
    strcat(varValue, " |");

end:
    gui_debug("\n\n\n----------------------------");
    gui_debug("%s", varValue);  
    gui_debug(">>>>>>%s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);

}

#define MoCA_VERSION_2_0                 0x20
/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiGetMocaStatus
 *
 *  [DESCRIPTION]:
 *
 *   mocap get --network_status     //
 *      connected_nodes:  node count
 *      node_id:  self id
 *   mocap get --gen_node_ext_status index 1 profile_type 3     
 *      index: node number
 *      profile_type: 0: RX Unicast;    3:TX Unicast
            0 = RX Unicast 
            1 = RX Broadcast 
            2 = RX Map 
            3 = TX Unicast 
            4 = TX Broadcast 
            5 = TX Map 
            6 = RX Unicast VLPER 
            7 = RX Unicast NPER 
            8 = RX Broadcast VLPER 
            9 = RX Broadcast NPER 
            10 = RX Map 2.0 
            11 = RX OFDMA 
            12 = TX Unicast VLPER 
            13 = TX Unicast NPER 
            14 = TX Broadcast VLPER 
            15 = TX Broadcast NPER 
            16 = TX Map 2.0 
            17 = TX OFDMA 
            18 = RX Unicast NPER Secondary Channel 
            19 = RX Unicast VLPER Secondary Channel 
            20 = TX Unicast NPER Secondary Channel 
            21 = TX Unicast VLPER Secondary Channel
 *       Get Moca Status
 *
 *  [return]:
 **************************************************************************/
void cgiGetMocaStatus(int argc, char **argv, char *varValue)
{
    gui_debug("cgiGetMocaStatus start");
    
    char buf[BUFLEN_128] = {0};
    char buftx[BUFLEN_32] = {0};
    char bufrx[BUFLEN_32] = {0};
    char bufmac[BUFLEN_32] = {0};
    int iNodeCount = 0;
    int iSelfId = 0;
    int iNCId = 0;

    char cmd[BUFLEN_128] = {'\0'};
    FILE * fp = NULL;
    char line[BUFLEN_128] = {'\0'};    
    char *p = NULL;

    //mocap get --network_status: get total node count, current moca id
    sprintf(cmd, "mocap get --network_status");
    gui_debug("cmd = %s", cmd);
    fp = popen(cmd, "r");

    if(NULL == fp) {        
        goto error;
    }

    while (fgets(line, BUFLEN_128, fp) != NULL) {
        if(strlen(line) > 0)
        {
            if (line[strlen(line)-1] == '\n')
            line[strlen(line)-1] = '\0'; 
            gui_debug("line => %s", line);
            
            if(strstr(line, "connected_nodes") != NULL) {
                p = strchr(line, ':');
                if(NULL != p) {
                    iNodeCount = atoi(p+1);
                    gui_debug("iNodeCount => %d", iNodeCount);
                }
            } else if(strstr(line, "node_id") != NULL && strstr(line, "nc_node_id") == NULL) {
                p = strchr(line, ':');
                if(NULL != p) {
                    iSelfId = atoi(p+1);
                    gui_debug("iSelfId => %d", iSelfId);
                }
            } else if(strstr(line, "nc_node_id") != NULL) {
                p = strchr(line, ':');
                if(NULL != p) {
                    iNCId = atoi(p+1);
                    gui_debug("iNCId => %d", iNCId);
                }
            } 
        }
        memset(line, '\0', sizeof(line));
    }
    if(tr69GetUnfreshLeafData(bufmac, "Device.MoCA.Interface.1", "MACAddress")) {
        gui_debug("get MACAddress error");
        sprintf(bufmac, "%s", " ");
    }


    sprintf(buf, "%d |%d |%s |%d ||", iNodeCount, iSelfId, bufmac, iNCId);
    strcat(varValue, buf);
    memset(buf, '\0', sizeof(buf));
    memset(bufmac, '\0', sizeof(bufmac));
    pclose(fp);


    //get others moca node status
    int i = 0;
    int moca_version = 0;
    for (i=0; i<iNodeCount; i++){
        if (i == iSelfId)   continue;

        // get moca version
        memset(cmd, '\0', sizeof(cmd));
        sprintf(cmd, "mocap get --nodeinfo %d", i);
        gui_debug("cmd = %s", cmd);
        fp = popen(cmd, "r");

        if(NULL == fp) {
            goto error;
        }

        while (fgets(line, BUFLEN_128, fp) != NULL) {
            if(strlen(line) > 0)
            {
                if (line[strlen(line)-1] == '\n')
                line[strlen(line)-1] = '\0';
                gui_debug("line => %s", line);

                if(strstr(line, "active_moca_version: ") != NULL) {
                    p = strchr(line, ':');
                    if(NULL != p) {
                        gui_debug("p+1 => %s", p+1);
                        moca_version = atoi(p+1);
                    }
                }
            }
            memset(line, '\0', sizeof(line));
        }
        pclose(fp);

        //get TX and MAC
        memset(cmd, '\0', sizeof(cmd));
        if(moca_version >= MoCA_VERSION_2_0)
        {
            sprintf(cmd, "mocap get --gen_node_ext_status index %d profile_type 13", i);
        }
        else
        {
            sprintf(cmd, "mocap get --gen_node_ext_status index %d profile_type 3", i);
        }
        gui_debug("cmd = %s", cmd);        
        fp = popen(cmd, "r");

        if(NULL == fp) {        
            goto error;
        }

        while (fgets(line, BUFLEN_128, fp) != NULL) {
            if(strlen(line) > 0)
            {
                if (line[strlen(line)-1] == '\n')
                line[strlen(line)-1] = '\0'; 
                gui_debug("line => %s", line);
                
                if(strstr(line, "node           :") != NULL) {
                    p = strchr(line, ':');
                    if(NULL != p) {
                        gui_debug("p+1 => %s", p+1);
                        sprintf(bufmac, "%s", p+1);
                    }
                } else if(strstr(line, "phy_rate") != NULL) {
                    p = strchr(line, ':');
                    if(NULL != p) {
                        gui_debug("p+1 => %s", p+1);
                        sprintf(buftx, "%s", p+1);
                    }
                }
            }
            memset(line, '\0', sizeof(line));
        }
        pclose(fp);

        //get RX
        memset(cmd, '\0', sizeof(cmd));
        if(moca_version >= MoCA_VERSION_2_0)
        {
            sprintf(cmd, "mocap get --gen_node_ext_status index %d profile_type 7", i);
        }
        else
        {
            sprintf(cmd, "mocap get --gen_node_ext_status index %d profile_type 0", i);
        }
        gui_debug("cmd = %s", cmd);
        fp = popen(cmd, "r");

        if(NULL == fp) {        
            goto error;
        }

        while (fgets(line, BUFLEN_128, fp) != NULL) {
            if(strlen(line) > 0)
            {
                if (line[strlen(line)-1] == '\n')
                line[strlen(line)-1] = '\0'; 
                gui_debug("line => %s", line);
                
                if(strstr(line, "phy_rate") != NULL) {
                    p = strchr(line, ':');
                    if(NULL != p) {
                        gui_debug("p+1 => %s", p+1);
                        sprintf(bufrx, "%s", p+1);
                    }
                } 
            }
            memset(line, '\0', sizeof(line));
        }

        sprintf(buf, "%s |%s |%s ||", buftx, bufrx, bufmac);
        strcat(varValue, buf);
        memset(bufmac, '\0', sizeof(bufmac));
        memset(buftx, '\0', sizeof(buftx));
        memset(bufrx, '\0', sizeof(bufrx));

        pclose(fp);
    }

    goto end;


error:
    pclose(fp);
    gui_debug("popen error");

end:
    gui_debug("\n\n\n----------------------------");
    gui_debug("%s", varValue);  
    gui_debug(">>>>>>%s():%d, varValue=[%s]\n\n\n", __func__, __LINE__, varValue);
}


/**************************************************************************
 *  [FUNCTION NAME]:
 *      cgiPostMocaSetup
 *
 *  [DESCRIPTION]:
 *
 *       Moca Setup
 *
 *  [return]:
 **************************************************************************/
int cgiPostMocaSetup()
{
    gui_debug("FreqCurrentMaskSetting => %s", gWebParams.lan_moca_channel);
    gui_debug("BeaconPowerLimit => %d", gWebParams.lan_moca_cm_ratio);
    gui_debug("PreferredNC => %d", gWebParams.lan_moca_enable);
    gui_debug("PrivacyEnabledSetting => %d", gWebParams.lan_moca_privacy);
    gui_debug("KeyPassphrase => %s", gWebParams.lan_moca_passwd);

    tr69CommitSetLeafDataString("Device.MoCA.Interface", 1, "FreqCurrentMaskSetting", gWebParams.lan_moca_channel);
    tr69CommitSetLeafDataInt("Device.MoCA.Interface", 1, "BeaconPowerLimit", gWebParams.lan_moca_cm_ratio);
    tr69CommitSetLeafDataInt("Device.MoCA.Interface", 1, "PreferredNC", gWebParams.lan_moca_enable);
    tr69CommitSetLeafDataInt("Device.MoCA.Interface", 1, "PrivacyEnabledSetting", gWebParams.lan_moca_privacy);

    if (1 == gWebParams.lan_moca_privacy)
        tr69CommitSetLeafDataString("Device.MoCA.Interface", 1, "KeyPassphrase", gWebParams.lan_moca_passwd);

    tr69CommitSetLeafDataEnd("Device.MoCA.Interface", 1);
    tr69SaveNow();

    return 1;
}

