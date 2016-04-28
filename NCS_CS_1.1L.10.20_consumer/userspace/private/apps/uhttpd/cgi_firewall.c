
#include "cgi_common.h"
#include "cgi_firewall.h"
#include "uhttpd-file.h"


/*
int cgiPostFirewallAdvancedFiltering()
{
    char *token = NULL;
    char *index_token = NULL;
    dal_ret_t advanced_filter;
    tsl_rv_t ret = tsl_rv_suc;
    char buf[BUFLEN_24] = {0};
    char lengthbegin[BUFLEN_24] = {0};
    char lengthend[BUFLEN_24] = {0};
    char token_enable[BUFLEN_256][BUFLEN_32];
    char src_ip_enable[BUFLEN_256][BUFLEN_32];
    char dst_ip_enable[BUFLEN_256][BUFLEN_32];
    char token_disable[BUFLEN_256][BUFLEN_32];
    char src_ip_disable[BUFLEN_256][BUFLEN_32];
    char dst_ip_disable[BUFLEN_256][BUFLEN_32];
      tsl_char_t log_buff[BUFLEN_256] = {0};
    int input = 0;
    int index = 0;
    int index_enable = 0;
    int index_disable = 0;
    int i = 0;

    memset(&advanced_filter, 0, sizeof(dal_ret_t));

    gui_debug("gWebParams.sumbit_button_name = %s", gWebParams.sumbit_button_name);

    if(tsl_strcmp(gWebParams.sumbit_button_name, "list") == 0)
    {
        gui_debug("gWebParams.firewall_advancedfiltering_indexes_enable = %s", gWebParams.firewall_advancedfiltering_indexes_enable);
        gui_debug("gWebParams.firewall_advancedfiltering_indexes_disable = %s", gWebParams.firewall_advancedfiltering_indexes_disable);

        token = strtok(gWebParams.firewall_advancedfiltering_indexes_enable, "|");
        while(token != NULL) {
            gui_debug("token_enable: %s", token);
            char *p = strstr(token, ";");
            p[0] = '\0';
            char *q = strstr(p+1, ";");
            q[0] = '\0';
            strcpy(token_enable[index_enable], token);
            strcpy(src_ip_enable[index_enable], p+1);
            strcpy(dst_ip_enable[index_enable], q+1);
            index_enable++;
            token = strtok(NULL, "|");
        }

        for(i = 0; i < index_enable; i++)
        {
            index = 1;
            index_token = strtok(token_enable[i], "_");
            while(index_token != NULL)
            {
                gui_debug("index_token_enable: %s", index_token);
                if(index == 1)
                {
                    if(!tsl_strcmp(index_token, "I"))
                        input = 1;
                    else
                        input = 0;
                }
                else if(index == 2)
                {
                    strncpy(buf, index_token, BUFLEN_24 - 1);
                }
                else if(index == 3)
                {
                    advanced_filter.param[FW_ADV_FILTER_PID] = &index_token[0];
                }

                index++;
                index_token = strtok(NULL, "_");
            }

            if (strstr(src_ip_enable[i], "unresolved") != NULL || strstr(dst_ip_enable[i], "unresolved") != NULL)
            {
                advanced_filter.param[FW_ADV_FILTER_ENABLE] = "false";
            }
            else
            {
                advanced_filter.param[FW_ADV_FILTER_ENABLE] = "true";
                if (safe_strlen(&src_ip_enable[i][0]) > 0)
                {
                    advanced_filter.param[FW_ADV_FILTER_SRCIP] = &src_ip_enable[i][0];
                    advanced_filter.param[FW_ADV_FILTER_DSTIP] = &dst_ip_enable[i][0];
                }
            }
            ret = dal_fw_addset_adv_filter(&advanced_filter, buf, input);
        }

        token = strtok(gWebParams.firewall_advancedfiltering_indexes_disable, "|");
        while(token != NULL) {
            gui_debug("token_disable: %s", token);
            char *p = strstr(token, ";");
            p[0] = '\0';
            char *q = strstr(p+1, ";");
            q[0] = '\0';
            strcpy(token_disable[index_disable], token);
            strcpy(src_ip_disable[index_disable], p+1);
            strcpy(dst_ip_disable[index_disable], q+1);
            index_disable++;
            token = strtok(NULL, "|");
        }

        for(i = 0; i < index_disable; i++)
        {
            index = 1;
            index_token = strtok(token_disable[i], "_");
            while(index_token != NULL)
            {
                gui_debug("index_token_disable: %s", index_token);
                if(index == 1)
                {
                    if(!tsl_strcmp(index_token, "I"))
                        input = 1;
                    else
                        input = 0;
                }
                else if(index == 2)
                {
                    strncpy(buf, index_token, BUFLEN_24 - 1);
                }
                else if(index == 3)
                {
                    advanced_filter.param[FW_ADV_FILTER_PID] = &index_token[0];
                }

                index++;
                index_token = strtok(NULL, "_");
            }

            advanced_filter.param[FW_ADV_FILTER_ENABLE] = "false";
            ret = dal_fw_addset_adv_filter(&advanced_filter, buf, input);
        }

        gui_debug("Done:  Advanced Filtering Enable/Disable");
    }
    else if(tsl_strcmp(gWebParams.sumbit_button_name, "delete") == 0)
    {
        gui_debug("gWebParams.firewall_advancedfiltering_index = %d", gWebParams.firewall_advancedfiltering_index);
        gui_debug("gWebParams.firewall_advancedfiltering_flag = %s", gWebParams.firewall_advancedfiltering_flag);
        gui_debug("gWebParams.firewall_advancedfiltering_device = %s", gWebParams.firewall_advancedfiltering_device);

        sprintf(buf, "%d", gWebParams.firewall_advancedfiltering_index);
        advanced_filter.param[FW_ADV_FILTER_PID] = &buf[0];

        if(!tsl_strcmp(gWebParams.firewall_advancedfiltering_flag, "input"))
            input = 1;

        ret = dal_fw_del_adv_filter(&advanced_filter, gWebParams.firewall_advancedfiltering_device, input);

        gui_debug("Done:  Advanced Filtering Delete");
    }
    else if(tsl_strcmp(gWebParams.sumbit_button_name, "add_edit") == 0)
    {
        gui_debug("gWebParams.firewall_advancedfiltering_index = %d", gWebParams.firewall_advancedfiltering_index);
        gui_debug("gWebParams.firewall_advancedfiltering_flag = %s", gWebParams.firewall_advancedfiltering_flag);
        gui_debug("gWebParams.firewall_advancedfiltering_protocol_indexes = %s", gWebParams.firewall_advancedfiltering_protocol_indexes);
        gui_debug("gWebParams.firewall_advancedfiltering_sourceaddress = %s", gWebParams.firewall_advancedfiltering_sourceaddress);
        gui_debug("gWebParams.firewall_advancedfiltering_sourcea_input = %s", gWebParams.firewall_advancedfiltering_sourcea_input);
        gui_debug("gWebParams.firewall_advancedfiltering_destaddress = %s", gWebParams.firewall_advancedfiltering_destaddress);
        gui_debug("gWebParams.firewall_advancedfiltering_desta_input = %s", gWebParams.firewall_advancedfiltering_desta_input);
        gui_debug("gWebParams.firewall_advancedfiltering_device = %s", gWebParams.firewall_advancedfiltering_device);
        gui_debug("gWebParams.firewall_advancedfiltering_dscp = %s", gWebParams.firewall_advancedfiltering_dscp);
        gui_debug("gWebParams.firewall_advancedfiltering_dscphex = %s", gWebParams.firewall_advancedfiltering_dscphex);
        gui_debug("gWebParams.firewall_advancedfiltering_dscpmask = %s", gWebParams.firewall_advancedfiltering_dscpmask);
        gui_debug("gWebParams.firewall_advancedfiltering_priority = %s", gWebParams.firewall_advancedfiltering_priority);
        gui_debug("gWebParams.firewall_advancedfiltering_priorityqueue = %s", gWebParams.firewall_advancedfiltering_priorityqueue);
        gui_debug("gWebParams.firewall_advancedfiltering_filterlength = %s", gWebParams.firewall_advancedfiltering_filterlength);
        gui_debug("gWebParams.firewall_advancedfiltering_packetdata = %s", gWebParams.firewall_advancedfiltering_packetdata);
        gui_debug("gWebParams.firewall_advancedfiltering_lengthbegin = %d", gWebParams.firewall_advancedfiltering_lengthbegin);
        gui_debug("gWebParams.firewall_advancedfiltering_lengthend = %d", gWebParams.firewall_advancedfiltering_lengthend);
        gui_debug("gWebParams.firewall_advancedfiltering_operation = %s", gWebParams.firewall_advancedfiltering_operation);
        gui_debug("gWebParams.firewall_advancedfiltering_logging = %s", gWebParams.firewall_advancedfiltering_logging);
        gui_debug("gWebParams.firewall_advancedfiltering_scheduler = %s", gWebParams.firewall_advancedfiltering_scheduler);
        gui_debug("gWebParams.firewall_protocol_list = %s", gWebParams.firewall_protocol_list);
        gui_debug("gWebParams.firewall_protocol_sourceports_list = %s", gWebParams.firewall_protocol_sourceports_list);
        gui_debug("gWebParams.firewall_protocol_sourceports = %s", gWebParams.firewall_protocol_sourceports);
        gui_debug("gWebParams.firewall_protocol_destports_list = %s", gWebParams.firewall_protocol_destports_list);
        gui_debug("gWebParams.firewall_protocol_destports = %s", gWebParams.firewall_protocol_destports);

        if(!tsl_strcmp(gWebParams.firewall_advancedfiltering_flag, "input"))
            input = 1;

        advanced_filter.param[FW_ADV_FILTER_ENABLE] = "true";

        sprintf(buf, "%d", gWebParams.firewall_advancedfiltering_index);
        advanced_filter.param[FW_ADV_FILTER_PID] = &buf[0];

        if(tsl_strcmp(gWebParams.firewall_advancedfiltering_sourceaddress, "-1") == 0)
        {           
            advanced_filter.param[FW_ADV_FILTER_NetworkObj_FLAG] = "false";
            advanced_filter.param[FW_ADV_FILTER_SRCIP] = &gWebParams.firewall_advancedfiltering_sourcea_input[0];
        }
        else
        {           
            if(tsl_strcmp(gWebParams.firewall_advancedfiltering_sourceaddress, "0.0.0.0") == 0)
            {
                advanced_filter.param[FW_ADV_FILTER_NetworkObj_FLAG] = "false";
                advanced_filter.param[FW_ADV_FILTER_SRCIP] = &gWebParams.firewall_advancedfiltering_sourceaddress[0];
            }      
            else
            {
                advanced_filter.param[FW_ADV_FILTER_NetworkObj_FLAG] = "true";
                advanced_filter.param[FW_ADV_FILTER_NetworkObjectName] = &gWebParams.firewall_advancedfiltering_sourceaddress[0];
            }            
        }        

        if(tsl_strcmp(gWebParams.firewall_advancedfiltering_destaddress, "-1") == 0)
        {           
            advanced_filter.param[FW_ADV_FILTER_DestAddress_NetworkObj_FLAG] = "false";
            advanced_filter.param[FW_ADV_FILTER_DSTIP] = &gWebParams.firewall_advancedfiltering_desta_input[0];
        }
        else
        {           
            if(tsl_strcmp(gWebParams.firewall_advancedfiltering_destaddress, "0.0.0.0") == 0)
            {
                advanced_filter.param[FW_ADV_FILTER_DestAddress_NetworkObj_FLAG] = "false";
                advanced_filter.param[FW_ADV_FILTER_DSTIP] = &gWebParams.firewall_advancedfiltering_destaddress[0];
            }   
            else
            {
                advanced_filter.param[FW_ADV_FILTER_DestAddress_NetworkObj_FLAG] = "true";
                advanced_filter.param[FW_ADV_FILTER_DestAddress_NetworkObjectName] = &gWebParams.firewall_advancedfiltering_destaddress[0];
            }   
        }

        if(tsl_strcmp(gWebParams.firewall_advancedfiltering_protocol_indexes, "") == 0)
        {
            advanced_filter.param[FW_ADV_FILTER_PROTOCOL] = &gWebParams.firewall_protocol_list[0];

            if(tsl_strcmp(gWebParams.firewall_protocol_sourceports_list, "-1") == 0)
            {
                advanced_filter.param[FW_ADV_FILTER_SRCPORT] = &gWebParams.firewall_protocol_sourceports[0];
            }
            else
            {
                advanced_filter.param[FW_ADV_FILTER_SRCPORT] = &gWebParams.firewall_protocol_sourceports_list[0];
            }

            if(tsl_strcmp(gWebParams.firewall_protocol_destports_list, "-1") == 0)
            {
                advanced_filter.param[FW_ADV_FILTER_DSTPORT] = &gWebParams.firewall_protocol_destports[0];
            }
            else
            {
                advanced_filter.param[FW_ADV_FILTER_DSTPORT] = &gWebParams.firewall_protocol_destports_list[0];
            }
            advanced_filter.param[FW_ADV_FILTER_SERVICENAME_FLAG] = "false";
            advanced_filter.param[FW_ADV_FILTER_SERVICENAME] = &gWebParams.firewall_advancedfiltering_protocol_indexes[0];
        }
        else if(tsl_strcmp(gWebParams.firewall_advancedfiltering_protocol_indexes, "Any") == 0)
        {
            advanced_filter.param[FW_ADV_FILTER_PROTOCOL] = "Any";
            advanced_filter.param[FW_ADV_FILTER_SERVICENAME_FLAG] = "false";
            advanced_filter.param[FW_ADV_FILTER_SERVICENAME] = "Any";
        }
        else
        {
            advanced_filter.param[FW_ADV_FILTER_SERVICENAME_FLAG] = "true";
            advanced_filter.param[FW_ADV_FILTER_SERVICENAME] = &gWebParams.firewall_advancedfiltering_protocol_indexes[0];
        }

        if(tsl_strcmp(gWebParams.firewall_advancedfiltering_scheduler, "") == 0)
        {
            advanced_filter.param[FW_ADV_FILTER_SCHEDULERNAME_FLAG] = "false";
        }
        else
        {
            advanced_filter.param[FW_ADV_FILTER_SCHEDULERNAME_FLAG] = "true";
        }

        advanced_filter.param[FW_ADV_FILTER_SCHEDULERNAME] = &gWebParams.firewall_advancedfiltering_scheduler[0];
        advanced_filter.param[FW_ADV_FILTER_DSCPVALUE_CHECK] = &gWebParams.firewall_advancedfiltering_dscp[0];
        advanced_filter.param[FW_ADV_FILTER_DSCPVALUE] = &gWebParams.firewall_advancedfiltering_dscphex[0];
        advanced_filter.param[FW_ADV_FILTER_PRIORITY_CHECK] = &gWebParams.firewall_advancedfiltering_priority[0];
        advanced_filter.param[FW_ADV_FILTER_PRIORITY] = &gWebParams.firewall_advancedfiltering_priorityqueue[0];
        advanced_filter.param[FW_ADV_FILTER_MATCHLENGTH_CHECK] = &gWebParams.firewall_advancedfiltering_filterlength[0];
        sprintf(lengthbegin, "%d", gWebParams.firewall_advancedfiltering_lengthbegin);
        advanced_filter.param[FW_ADV_FILTER_MATCHLENGTH] = &lengthbegin[0];
        sprintf(lengthend, "%d", gWebParams.firewall_advancedfiltering_lengthend);
        advanced_filter.param[FW_ADV_FILTER_MATCHLENGTH_MAX] = &lengthend[0];
        advanced_filter.param[FW_ADV_FILTER_LOGGING] = &gWebParams.firewall_advancedfiltering_logging[0];
        advanced_filter.param[FW_ADV_FILTER_ACTION] = &gWebParams.firewall_advancedfiltering_operation[0];
        advanced_filter.param[FW_ADV_FILTER_DEVICENAME] = &gWebParams.firewall_advancedfiltering_device[0];

        ret = dal_fw_addset_adv_filter(&advanced_filter, gWebParams.firewall_advancedfiltering_device, input);
    }

      snprintf(log_buff, sizeof(log_buff), "WBM user admin (%s) has changed security settings(Advanced Filtering)", gWebParams.cli_addr);
      dal_write_log(OMT_LOG_INFO, LOG_SECURITY, "Firewall Info", "Configuration change", log_buff);
    if(ret == tsl_rv_suc) {
        gui_debug("Advanced Filtering (%s) success!", gWebParams.sumbit_button_name);
        return 0;
    } else {
        gui_debug("Advanced Filtering (%s) error!", gWebParams.sumbit_button_name);
        return -1;
    }
}


/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *          Firewall Network Computer
 *           
 * 
 **************************************************************************/
/*
void cgiGetFirewallNetwork(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    int entry_nums = 0;
    int i = 0;
    char resolveip[BUFLEN_24] = {0};
    char resolveip_buf[BUFLEN_256] = {0};
    int items_num = 0;
    char type[BUFLEN_4] = {0};

    dal_ret_t getRet;

    //Network Object
    dal_ret_t *network = dal_get_fw_networkobj(&entry_nums);

    if(network != NULL)
    {
        kprintf("entry_nums: %d\n", entry_nums);

        for(i = 0; i < entry_nums; i++)
        {
            items_num = 0;
            dal_ret_t *items_ret = dal_get_fw_networkItems(getValueFromDal(network[i].param[NETWORK_GET_OID]), &items_num);
            if (items_ret == NULL)
            {
                continue;
            }

            memset(resolveip_buf, 0, sizeof(resolveip_buf));
                        memset(buf, 0, sizeof(buf));
            for (int j=0; j<items_num; j++)
            {
                memset(resolveip, 0, sizeof(resolveip));
                                memset(type, 0, sizeof(type));

                strcpy(type, getValueFromDal(items_ret[j].param[NETWORK_ITEMS_TYPE]));

                //ip address
                if (!strcmp(type, "1"))
                {
                    snprintf(resolveip, sizeof(resolveip), "%s", items_ret[j].param[NETWORK_ITEMS_FIRSTVALUE]);
                }
                //ip subnet
                else if (!strcmp(type, "2"))
                {
                    snprintf(resolveip, sizeof(resolveip), "%s/%s", items_ret[j].param[NETWORK_ITEMS_FIRSTVALUE], items_ret[j].param[NETWORK_ITEMS_SECONDVALUE]);
                }
                //ip range
                else if (!strcmp(type, "3"))
                {
                    snprintf(resolveip, sizeof(resolveip), "%s-%s", items_ret[j].param[NETWORK_ITEMS_FIRSTVALUE], items_ret[j].param[NETWORK_ITEMS_SECONDVALUE]);
                }
                //mac address
                else if (!strcmp(type, "4"))
                {
                    snprintf(resolveip, sizeof(resolveip), "%s/%s", items_ret[j].param[NETWORK_ITEMS_FIRSTVALUE], items_ret[j].param[NETWORK_ITEMS_SECONDVALUE]);
                }
                //hostname(resolve)
                else if (!strcmp(type, "5"))
                {
                    memset(&getRet, 0, sizeof(dal_ret_t));
                    getRet.param[RESOLV_HOST_NAME] = items_ret[j].param[NETWORK_ITEMS_FIRSTVALUE];

                    if (dal_resolve_hostname_local(&getRet, 1) == TSL_RV_SUC)
                    {
                        snprintf(resolveip, sizeof(resolveip), "%s", getRet.param[RESOLV_HOST_IP]);

                        gui_debug("resolveip=%s", resolveip);
                        TSL_FREE(getRet.param[RESOLV_HOST_IP]);
                        TSL_FREE(getRet.param[RESOLV_HOST_NAME_STATUS_CODE]);
                    }
                    else
                    {
                        snprintf(resolveip, sizeof(resolveip), "%s", "Unresolved");
                    }

                    if (!safe_strcmp(resolveip, "0.0.0.0") || !safe_strcmp(resolveip, "Unresolved"))
                    {
                        memset(&getRet, 0, sizeof(dal_ret_t));
                        getRet.param[RESOLV_HOST_NAME] = items_ret[j].param[NETWORK_ITEMS_FIRSTVALUE];

                        if (dal_get_resolve_status(&getRet) == TSL_RV_SUC)
                        {
                            if (atoi(getRet.param[RESOLV_HOST_NAME_STATUS_CODE]) == FW_RESOLV_NOT_FOUND)
                            {
                                snprintf(resolveip, sizeof(resolveip), "%s", "Unresolved");
                            }
                            else if (atoi(getRet.param[RESOLV_HOST_NAME_STATUS_CODE]) == FW_RESOLV_IN_PROGRESS)
                            {
                                snprintf(resolveip, sizeof(resolveip), "%s", "Resolving");
                            }
                            else if (atoi(getRet.param[RESOLV_HOST_NAME_STATUS_CODE]) == FW_RESOLV_RESOLVED)
                            {
                                snprintf(resolveip, sizeof(resolveip), "%s", getRet.param[RESOLV_HOST_IP]);
                            }
                            else
                            {
                                snprintf(resolveip, sizeof(resolveip), "%s", getRet.param[RESOLV_HOST_NAME_STATUS_STRING]);
                            }

                            gui_debug("resolveip=%s", resolveip);
                            TSL_FREE(getRet.param[RESOLV_HOST_IP]);
                            TSL_FREE(getRet.param[RESOLV_HOST_NAME_STATUS_CODE]);
                            TSL_FREE(getRet.param[RESOLV_HOST_NAME_STATUS_STRING]);
                        }
                        else
                        {
                            snprintf(resolveip, sizeof(resolveip), "%s", "Unresolved");
                        }
                    }
                }
                //dhcp option
                else if (!strcmp(type, "6"))
                {
                    snprintf(resolveip, sizeof(resolveip), "%s", items_ret[j].param[NETWORK_ITEMS_FIRSTVALUE]);
                }

                strcat(resolveip_buf, type);
                strcat(resolveip_buf, ",");
                strcat(resolveip_buf, resolveip);
                strcat(resolveip_buf, ";");
            }

            sprintf(buf, "%s|%s|%s||",
                    network[i].param[NETWORK_GET_OID],
                    resolveip_buf,
                    getValueFromDal(network[i].param[NETWORK_NAME])
                   );

            gui_debug("buf=%s", buf);
            strcat(varValue, buf);
                        free_multiple_dal_ret(&items_ret, items_num, DAL_RET_CONTINUOUS);
        }

        kprintf("varValue: %s\n", varValue);
        free_multiple_dal_ret(&network, entry_nums, DAL_RET_CONTINUOUS);
    }
}

/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *          Firewall WAN Connection Type
 *           
 * 
 **************************************************************************/
/*
void cgiGetFirewallWanConnectionType(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_128] = {0};
    dal_ret_t *wan_device = NULL;
    dal_ret_t *wan_device_info = NULL;
    int param_index = 1;

    wan_device = dal_get_wan_conn_list_sample();

    if(wan_device == NULL) return;

    while (wan_device->param[param_index] != NULL)
    {
        sprintf(buf, "%d|", param_index);
        strcat(varValue, buf);

        sprintf(buf, "%s|", getValueFromDal(wan_device->param[param_index]));
        strcat(varValue, buf);

        wan_device_info = dal_if_get_ip_settings(wan_device->param[param_index], DAL_WAN);
        if(wan_device_info != NULL)
        {
            sprintf(buf, "%s|%s||", getValueFromDal(wan_device_info->param[IF_NAME]),
                    getValueFromDal(wan_device_info->param[IF_MAC_ADDR]));
            free_dal_ret(&wan_device_info);
        } else {
            strcpy(buf, "NULL|NULL||");
        }

        strcat(varValue, buf);
        param_index++;
    }

    kprintf("varValue: %s\n", varValue);
    free_dal_ret(&wan_device);

#if 0
    strcat(varValue, "0|eth0|Broadband Connection (Ethernet)|00:00:00:00:00:00||");
    strcat(varValue, "1|clink0|Broadband Connection (Coax)|00:00:00:00:00:00||");
    strcat(varValue, "2|wan0|WAN PPPoE|00:00:00:00:00:00||");
    strcat(varValue, "3|wan1|WAN PPPoE 2|00:00:00:00:00:00||");
#endif
}



/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *          Firewall Schedulers
 *           
 * 
 **************************************************************************/
 /*
void cgiGetFirewallSchedulers(int argc, char **argv, char *varValue)
{
    int entry_nums = 0;
    char buf[BUFLEN_256] = {0};

    dal_ret_t *scheduler = dal_ipt_get_sche_rule_list(&entry_nums);

    if(scheduler != NULL)
    {
        kprintf("entry_nums: %d\n", entry_nums);

        for(int i = 0; i < entry_nums; i++)
        {
            /*
            if(!tsl_strcmp(getValueFromDal(scheduler[i].param[SCHEDULER_RULE_STATUS]), "0"))
                continue;
            */
/*
            sprintf(buf, "%s|%s|%s||",
                    getValueFromDal(scheduler[i].param[SCHEDULER_RULE_PID]),
                    getValueFromDal(scheduler[i].param[SCHEDULER_RULE_NAME]),
                    getValueFromDal(scheduler[i].param[SCHEDULER_RULE_STATUS])
                   );
            strcat(varValue, buf);
        }
        kprintf("varValue: %s\n", varValue);
        free_multiple_dal_ret(&scheduler, entry_nums, DAL_RET_CONTINUOUS);
    }
}
*/

/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *         Get Firewall Status
 *           
 * 
 **************************************************************************/
void cgiGetFirewallStatus(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};

    dal_ret_t *dal_get = dal_get_firewall_modules_status();

    if(dal_get == NULL)
    {
        gui_debug("cgiGetFirewallStatus dal_get is null");
        return;
    }

    snprintf(buf, sizeof(buf), "%s|%s|%s|%s",
            getValueFromDal(dal_get->param[FW_STATUS_PACKET_FILTER]),
            getValueFromDal(dal_get->param[FW_STATUS_IP_PASSTHRU]),
            getValueFromDal(dal_get->param[FW_STATUS_NAT_DEFAULT_SERVER]),
            getValueFromDal(dal_get->param[FW_STATUS_ADVANCED])            
         );

    strcat(varValue, buf);
    free_dal_ret(&dal_get);
    gui_debug("Firewall status varValue: %s\n", varValue);
}


/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *         Get Firewall Rule Enable/Disable
 *           
 * 
 **************************************************************************/
void cgiGetFirewallRuleEnable(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};


    dal_ret_t *dal_get = dal_get_firewall_modules_status();

    if(dal_get == NULL)
    {
        gui_debug("cgiGetFirewallRuleEnable dal_get is null");
        return;
    }

    snprintf(buf, sizeof(buf), "%s",getValueFromDal(dal_get->param[FW_STATUS_PACKET_FILTER]));
    strcat(varValue, buf);
    free_dal_ret(&dal_get);

    gui_debug("Firewall Rule Enable varValue: %s\n", varValue);
}

/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *         Get Firewall Rules
 *           
 * 
 **************************************************************************/
void cgiGetFirewallRules(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    int num = 0;
    int i = 0;

    //snprintf(buf, sizeof(buf), "1111|0|1|192.168.1.22|192.168.1.23|NULL|NULL|NULL|NULL|0|2326|2328|icmp_type1||2222|0|2|192.168.1.23|192.168.1.24|NULL|NULL|NULL|NULL|0|3326|3328|icmp_type2||");

    dal_ret_t *dal_get = dal_get_packet_filter_rule(&num);

    if(dal_get == NULL)
    {
        gui_debug("cgiGetFirewallRuleEnable dal_get is null");
        return;
    }
    
    for(i=0; i<num; i++)
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[FW_PACKET_FILTER_RULE_OID]));
        strcat(varValue, buf);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[FW_PACKET_FILTER_RULE_TARGET]));
        strcat(varValue, buf);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[FW_PACKET_FILTER_RULE_ORDER]));
        strcat(varValue, buf);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[FW_PACKET_FILTER_RULE_SRCIP]));
        strcat(varValue, buf);
    
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[FW_PACKET_FILTER_RULE_DESTIP]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[FW_PACKET_FILTER_RULE_SRCINTERFACE]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[FW_PACKET_FILTER_RULE_DESTINTERFACE]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[FW_PACKET_FILTER_RULE_PROTOCOL]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[FW_PACKET_FILTER_RULE_SRCPORT]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[FW_PACKET_FILTER_RULE_DESTPORT]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[FW_PACKET_FILTER_RULE_ICMPTYPE]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[FW_PACKET_FILTER_RULE_SRCMASK]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[FW_PACKET_FILTER_RULE_DESTMASK]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[FW_PACKET_FILTER_RULE_SOURCEMAC]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[FW_PACKET_FILTER_RULE_DESTMAC]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(dal_get[i].param[FW_PACKET_FILTER_RULE_PACKETLENGTH]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s||", getValueFromDal(dal_get[i].param[FW_PACKET_FILTER_RULE_TCPFLAGS]));
        strcat(varValue, buf);
    }
    free_dal_ret(&dal_get);    
    gui_debug("Firewall Rule Enable varValue: %s\n", varValue);
}

int cgiPostFirewallRules()
{
    tsl_rv_t result = 0;
    int ret_size = sizeof(dal_ret_t);
    dal_ret_t dal_post;
    memset(&dal_post, 0, ret_size);

    gui_debug(" firewall_button_act=%s, firewall_rule_act_id=%s", gWebParams.firewall_button_act, gWebParams.firewall_rule_act_id);
    gui_debug(" ena_disable_filter=%s, action_radio=%s,rule_order_value=%s", gWebParams.ena_disable_filter, gWebParams.action_radio, gWebParams.rule_order_value);
    gui_debug(" src_ip_addr_value=%s, dst_ip_addr_value=%s,ing_inter_value=%s", gWebParams.src_ip_addr_value, gWebParams.dst_ip_addr_value, gWebParams.ing_inter_value);
    gui_debug(" eg_inter_value=%s,src_mac_addr_value=%s,dst_mac_addr_value=%s", gWebParams.eg_inter_value, gWebParams.src_mac_addr_value, gWebParams.dst_mac_addr_value);
    gui_debug(" firewall_protocol_sel=%s, src_port_value=%s,dst_port_value=%s,filter_icmp_type_value=%s", gWebParams.firewall_protocol_sel, gWebParams.src_port_value, gWebParams.dst_port_value,gWebParams.filter_icmp_type_value);
    gui_debug(" packetLength_value=%s; tcpFlags_value=%s\n",gWebParams.packetLength_value, gWebParams.tcpFlags_value);

    char eblFilter[BUFLEN_16] = {0};
    char interface_all[BUFLEN_16] = {0};
    strcpy(eblFilter, "1");
    strcpy(interface_all, "1");
    if(!strcmp(gWebParams.firewall_button_act,"apply"))
    {
        if(!strcmp(gWebParams.ena_disable_filter, "1"))
        {
            dal_enable_packet_filter();
        }
        else if(!strcmp(gWebParams.ena_disable_filter, "0"))
        {
            dal_disable_packet_filter();
        }
    }
    else if((!strcmp(gWebParams.firewall_button_act,"add")&&!strcmp(gWebParams.firewall_rule_act_id,"-1"))||(!strcmp(gWebParams.firewall_button_act,"edit")&&strcmp(gWebParams.firewall_rule_act_id,"-1")))
    {

        dal_post.param[FW_PACKET_FILTER_RULE_ENABLE] = &eblFilter[0];
        dal_post.param[FW_PACKET_FILTER_RULE_TARGET] = &gWebParams.action_radio[0];
        dal_post.param[FW_PACKET_FILTER_RULE_ORDER] = &gWebParams.rule_order_value[0];
        dal_post.param[FW_PACKET_FILTER_RULE_SRCIP] = &gWebParams.src_ip_addr_value[0];
        dal_post.param[FW_PACKET_FILTER_RULE_DESTIP] = &gWebParams.dst_ip_addr_value[0];
        dal_post.param[FW_PACKET_FILTER_RULE_SRCINTERFACE] = &gWebParams.ing_inter_value[0];
        dal_post.param[FW_PACKET_FILTER_RULE_DESTINTERFACE] = &gWebParams.eg_inter_value[0];
        dal_post.param[FW_PACKET_FILTER_RULE_PROTOCOL] = &gWebParams.firewall_protocol_sel[0];
        dal_post.param[FW_PACKET_FILTER_RULE_SRCPORT] = &gWebParams.src_port_value[0];
        dal_post.param[FW_PACKET_FILTER_RULE_DESTPORT] = &gWebParams.dst_port_value[0];
        dal_post.param[FW_PACKET_FILTER_RULE_OID] = &gWebParams.firewall_rule_act_id[0];
        dal_post.param[FW_PACKET_FILTER_RULE_SRCMASK] = &gWebParams.src_mask_addr_value[0];
        dal_post.param[FW_PACKET_FILTER_RULE_DESTMASK] = &gWebParams.dst_mask_addr_value[0];
        dal_post.param[FW_PACKET_FILTER_RULE_ICMPTYPE] = &gWebParams.filter_icmp_type_value[0];
        dal_post.param[FW_PACKET_FILTER_RULE_SOURCEMAC] = &gWebParams.src_mac_addr_value[0];
        dal_post.param[FW_PACKET_FILTER_RULE_DESTMAC] = &gWebParams.dst_mac_addr_value[0];
        dal_post.param[FW_PACKET_FILTER_RULE_PACKETLENGTH] = &gWebParams.packetLength_value[0];
        dal_post.param[FW_PACKET_FILTER_RULE_TCPFLAGS] = &gWebParams.tcpFlags_value[0];
        if (safe_strlen(gWebParams.ing_inter_value) == 0)
            dal_post.param[FW_PACKET_FILTER_RULE_SRCALLINTERFACE] = &interface_all[0];
        if (safe_strlen(gWebParams.eg_inter_value) == 0)
            dal_post.param[FW_PACKET_FILTER_RULE_DESTALLINTERFACE] = &interface_all[0];

        gui_debug(" \nFW_PACKET_FILTER_RULE_ENABLE=%s\n",dal_post.param[FW_PACKET_FILTER_RULE_ENABLE]);
        gui_debug(" \nFW_PACKET_FILTER_RULE_TARGET=%s\n FW_PACKET_FILTER_RULE_ORDER=%s\n",dal_post.param[FW_PACKET_FILTER_RULE_TARGET],dal_post.param[FW_PACKET_FILTER_RULE_ORDER]);
        gui_debug(" \nFW_PACKET_FILTER_RULE_SRCIP=%s\n FW_PACKET_FILTER_RULE_DESTIP=%s\n",dal_post.param[FW_PACKET_FILTER_RULE_SRCIP], dal_post.param[FW_PACKET_FILTER_RULE_DESTIP]);
        gui_debug(" \nFW_PACKET_FILTER_RULE_SRCINTERFACE=%s\n FW_PACKET_FILTER_RULE_DESTINTERFACE=%s\n",dal_post.param[FW_PACKET_FILTER_RULE_SRCINTERFACE],dal_post.param[FW_PACKET_FILTER_RULE_DESTINTERFACE]);
        gui_debug(" \nFW_PACKET_FILTER_RULE_PROTOCOL=%s\n",dal_post.param[FW_PACKET_FILTER_RULE_PROTOCOL]);
        gui_debug(" \nFW_PACKET_FILTER_RULE_SRCPORT=%s\n FW_PACKET_FILTER_RULE_DESTPORT=%s\n",dal_post.param[FW_PACKET_FILTER_RULE_SRCPORT], dal_post.param[FW_PACKET_FILTER_RULE_DESTPORT]); 
        gui_debug(" \nFW_PACKET_FILTER_RULE_SRCALLINTERFACE=%s\n",dal_post.param[FW_PACKET_FILTER_RULE_SRCALLINTERFACE]);
        gui_debug(" \nFW_PACKET_FILTER_RULE_DESTALLINTERFACE=%s\n",dal_post.param[FW_PACKET_FILTER_RULE_DESTALLINTERFACE]);
        gui_debug(" \nFW_PACKET_FILTER_RULE_SRCMASK=%s\n",dal_post.param[FW_PACKET_FILTER_RULE_SRCMASK]);
        gui_debug(" \nFW_PACKET_FILTER_RULE_DESTMASK=%s\n",dal_post.param[FW_PACKET_FILTER_RULE_DESTMASK]);
        gui_debug(" \nFW_PACKET_FILTER_RULE_OID=%s\n",dal_post.param[FW_PACKET_FILTER_RULE_OID]);
        gui_debug(" \nFW_PACKET_FILTER_RULE_ICMPTYPE=%s\n",dal_post.param[FW_PACKET_FILTER_RULE_ICMPTYPE]);
        gui_debug(" \nFW_PACKET_FILTER_RULE_SOURCEMAC=%s;FW_PACKET_FILTER_RULE_DESTMAC=%s\n",dal_post.param[FW_PACKET_FILTER_RULE_SOURCEMAC],dal_post.param[FW_PACKET_FILTER_RULE_DESTMAC]);
        gui_debug(" \nFW_PACKET_FILTER_RULE_PACKETLENGTH=%s\n",dal_post.param[FW_PACKET_FILTER_RULE_PACKETLENGTH]);
        gui_debug(" \nFW_PACKET_FILTER_RULE_TCPFLAGS=%s\n",dal_post.param[FW_PACKET_FILTER_RULE_TCPFLAGS]);
        if((!strcmp(gWebParams.firewall_button_act,"add"))&&!strcmp(gWebParams.firewall_rule_act_id,"-1"))
        {gui_debug("+++++++++add\n");
            result = dal_add_packet_filter_rule(&dal_post);
        }
        else
        {gui_debug("+++++++++modify\n");
            result = dal_modify_packet_filter_rule(&dal_post);
        }
    }
    else if(!strcmp(gWebParams.firewall_button_act,"remove")&&strcmp(gWebParams.firewall_rule_act_id,"-1"))
    {gui_debug("+++++++++delete rule id=%s\n",gWebParams.firewall_rule_act_id);
        dal_post.param[FW_PACKET_FILTER_RULE_OID] = &gWebParams.firewall_rule_act_id[0];
        result = dal_del_packet_filter_rule(&dal_post);
    }
    
    result = dal_SaveConfig();
    return result;
}

/****************************Firewall Pinholes Begin****************************/

/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *         Add a custom services for firewall pinholes.
 *           
 * 
 **************************************************************************/

static int cgiAddModPinholesCustomServ()
{
    int result = 0;
    ServiceObjectPtr serviceObjectPtr = NULL;
    ServicePortsObjectPtr servicePortsObjectPtr = NULL;
    char service_name_old[BUFLEN_64] = {0};
    int num = 0;

    gui_debug("gWebParams.fw_pinholes_serviceName=%s\n",gWebParams.fw_pinholes_serviceName);
    gui_debug("gWebParams.fw_pinholes_srcPorts_start=%s\n",gWebParams.fw_pinholes_srcPorts_start);
    gui_debug("gWebParams.fw_pinholes_srcPorts_end=%s\n",gWebParams.fw_pinholes_srcPorts_end);
    gui_debug("gWebParams.fw_pinholes_desPorts_start=%s\n",gWebParams.fw_pinholes_desPorts_start);
    gui_debug("gWebParams.fw_pinholes_desPorts_end=%s\n",gWebParams.fw_pinholes_desPorts_end);
    gui_debug("gWebParams.fw_pinholes_protocol=%s\n",gWebParams.fw_pinholes_protocol);
    gui_debug("gWebParams.fw_pinholes_serviveId=%s\n",gWebParams.fw_pinholes_serviveId);

    serviceObjectPtr = (ServiceObjectPtr)malloc(sizeof(ServiceObject));
    if (!serviceObjectPtr)
    {
        gui_debug("out of memory");
        return  -1;
    }

    memset(serviceObjectPtr, 0, sizeof(ServiceObject));
    serviceObjectPtr->enable = (unsigned char *)strdup("1");
    serviceObjectPtr->name = (unsigned char *)strdup(&gWebParams.fw_pinholes_serviceName[0]);
    serviceObjectPtr->isCustom = (unsigned char *)strdup("1");
    strcpy(service_name_old, gWebParams.fw_pinholes_serviveId);

    gui_debug("num=%d", num);
    serviceObjectPtr->servicePortEntries[num] = (ServicePortsObjectPtr)malloc(sizeof(ServicePortsObject));
    if (!serviceObjectPtr->servicePortEntries[num])
    {
        gui_debug("out of memory");
        return -1;
    }

    memset(serviceObjectPtr->servicePortEntries[num], 0, sizeof(ServicePortsObject));

    serviceObjectPtr->servicePortEntries[num]->enable = (unsigned char *)strdup("1");
    serviceObjectPtr->servicePortEntries[num]->protocol = (unsigned char *)strdup(&gWebParams.fw_pinholes_protocol[0]);
    serviceObjectPtr->servicePortEntries[num]->sourcePortStart = (unsigned char *)strdup(&gWebParams.fw_pinholes_srcPorts_start[0]);
    serviceObjectPtr->servicePortEntries[num]->sourcePortEnd = (unsigned char *)strdup(&gWebParams.fw_pinholes_srcPorts_end[0]);
    serviceObjectPtr->servicePortEntries[num]->destinationPortStart = (unsigned char *)strdup(&gWebParams.fw_pinholes_desPorts_start[0]);
    serviceObjectPtr->servicePortEntries[num]->destinationPortEnd = (unsigned char *)strdup(&gWebParams.fw_pinholes_desPorts_end[0]);
    serviceObjectPtr->param_num = ++num;

    if (safe_strlen(service_name_old) == 0)
    {
        if (dal_add_one_service_entry(serviceObjectPtr) != SERV_RV_SUC)
            return -1;
    }
    else
    {
        if (dal_modify_one_service_entry((unsigned char *)service_name_old, serviceObjectPtr) != SERV_RV_SUC)
            return -1;
    }

    result = dal_SaveConfig();

    if (serviceObjectPtr)
    {
        aei_free_service_entry(&serviceObjectPtr);
        serviceObjectPtr = NULL;
        servicePortsObjectPtr = NULL;
    }
    return result;
}

/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *         Delete a custom services for firewall pinholes.
 *           
 * 
 **************************************************************************/

static int cgiDelPinholesCustomServ()
{
    int result = 0;
    result = dal_del_one_service_entry(&gWebParams.fw_pinholes_serviveId[0]);
    result = dal_SaveConfig();
    gui_debug("\nYUKI======cgiDelPinholesCustomServ:delete service %s :result=%d\n",gWebParams.fw_pinholes_serviveId, result);
    return result;
}

int cgiPostPinholesService()
{
    int result = 0;

    gui_debug("\nYUKI=========action=%s\n",gWebParams.action_radio);

    if(!safe_strcmp(gWebParams.action_radio, "delete"))
        result = cgiDelPinholesCustomServ();
    else
        result = cgiAddModPinholesCustomServ();
    return result;

}

/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *         Get all firewall pinholes services
 *           
 * 
 **************************************************************************/
void getAllPinholesServices(int argc, char **argv, char *varValue)
{
    AppServiceEntriesPtr appserventry = NULL;
    ServicePortsObjectPtr portobj = NULL;
    ServiceObjectPtr servobj = NULL;
    char buf[BUFLEN_64] = {0};
    char portInfo[BUFLEN_256] = {0};

    appserventry = dal_get_service_entry_list();
    if (appserventry == NULL) 
        return;

    gui_debug("appserventry->param_num=%d", appserventry->param_num);
    for (int i=0; i<appserventry->param_num; i++)
    {
        servobj = appserventry->serviceEntries[i];
        //Service name
        strcat(varValue, getValueFromDal((char *)servobj->name));
        strcat(varValue, "|");

        //Service is enable?
        strcat(varValue, (char *)servobj->enable);
        strcat(varValue, "|");

        //Service is custom?
        strcat(varValue, (char *)servobj->isCustom);
        strcat(varValue, "|");

        //ports
        memset(portInfo, 0, sizeof(portInfo));
        for (int j=0; j<servobj->param_num; j++)
        {
            portobj = servobj->servicePortEntries[j];

            sprintf(buf, "%s,", (char *)portobj->enable);
            strcat(portInfo, buf);

            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s,", (char *)portobj->protocol);
            strcat(portInfo, buf);

            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s,", (char *)portobj->sourcePortStart);
            strcat(portInfo, buf);

            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s,", (char *)portobj->sourcePortEnd);
            strcat(portInfo, buf);

            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s,", (char *)portobj->destinationPortStart);
            strcat(portInfo, buf);

            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s,", (char *)portobj->destinationPortEnd);
            strcat(portInfo, buf);

            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s,", (char *)portobj->icmpType);
            strcat(portInfo, buf);

            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s+", (char *)portobj->icmpCode);
            strcat(portInfo, buf);
        }
        
        strcat(varValue, portInfo);
        strcat(varValue, "||");
    }

    //formart: service enable|service name|service custom|ports
    gui_debug("\nYUKI===Get All Pinholes Pr Service===varValue=%s\n",varValue);

    if (appserventry != NULL)
    {
        aei_free_service_entry_result(&appserventry);
        appserventry = NULL;
        servobj = NULL;
        portobj = NULL;
    }
}

/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *         Add a port forwarding rule for firewall pinholes.
 *           
 * 
 **************************************************************************/
static int cgiAddCustomRule()
{
    int result = 0;
    int ret_size = sizeof(dal_ret_t);
    dal_ret_t ret_fw;
    memset(&ret_fw, 0, ret_size);
    char fwEnabel[BUFLEN_16] = {0};
    char useService[BUFLEN_16] = {0};

    strcpy(fwEnabel,"1");
    strcpy(useService, "1");
    ret_fw.param[FW_FORWARDING_ENABLED] = &fwEnabel[0];
    ret_fw.param[FW_FORWARDING_USE_SERVICE] = &useService[0];
    ret_fw.param[FW_FORWARDING_INTERNAL_CLIENT] = &gWebParams.fw_pinholes_rule_clientIp[0];
    ret_fw.param[FW_FORWARDING_SERVICE_NAME] = &gWebParams.fw_pinholes_serviceName[0];

    gui_debug("\nYUKI======\n");
    gui_debug("FW_FORWARDING_ENABLED=%s\n",ret_fw.param[FW_FORWARDING_ENABLED]);
    gui_debug("FW_FORWARDING_USE_SERVICE=%s\n",ret_fw.param[FW_FORWARDING_USE_SERVICE]);
    gui_debug("FW_FORWARDING_INTERNAL_CLIENT=%s\n",ret_fw.param[FW_FORWARDING_INTERNAL_CLIENT]);
    gui_debug("FW_FORWARDING_SERVICE_NAME=%s\n",ret_fw.param[FW_FORWARDING_SERVICE_NAME]);
    
    result = dal_add_portforwarding_rule(&ret_fw);
    result = dal_SaveConfig();
    gui_debug("\nYUKI======cgiAddPinholesCustomServ:result=%d\n",result);
    return result;
}

/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *         Delete a port forwarding rule for firewall pinholes.
 *           
 * 
 **************************************************************************/
static int cgiDelCustomRule()
{
    int result = 0;
    int ret_size = sizeof(dal_ret_t);
    dal_ret_t ret_fw;
    memset(&ret_fw, 0, ret_size);
    char fwEnabel[BUFLEN_16] = {0};
    char useService[BUFLEN_16] = {0};

    strcpy(fwEnabel,"1");
    strcpy(useService, "0");
    ret_fw.param[FW_FORWARDING_ENABLED] = &fwEnabel[0];
    ret_fw.param[FW_FORWARDING_USE_SERVICE] = &useService[0];
    ret_fw.param[FW_FORWARDING_INTERNAL_CLIENT] = &gWebParams.fw_pinholes_rule_clientIp[0];
    ret_fw.param[FW_FORWARDING_SERVICE_NAME] = &gWebParams.fw_pinholes_serviceName[0];
    ret_fw.param[FW_FORWARDING_OID] = &gWebParams.fw_pinholes_rule_oid[0];

    gui_debug("\nYUKI======\n");
    gui_debug("FW_FORWARDING_ENABLED=%s\n",ret_fw.param[FW_FORWARDING_ENABLED]);
    gui_debug("FW_FORWARDING_USE_SERVICE=%s\n",ret_fw.param[FW_FORWARDING_USE_SERVICE]);
    gui_debug("FW_FORWARDING_INTERNAL_CLIENT=%s\n",ret_fw.param[FW_FORWARDING_INTERNAL_CLIENT]);
    gui_debug("FW_FORWARDING_SERVICE_NAME=%s\n",ret_fw.param[FW_FORWARDING_SERVICE_NAME]);
    gui_debug("FW_FORWARDING_OID=%s\n",ret_fw.param[FW_FORWARDING_OID]);

    result = dal_del_portforwarding_rule(&ret_fw);
    result = dal_SaveConfig();
    gui_debug("\nYUKI======cgiDelPinholesCustomServ:result=%d\n",result);
    return result;
}

int cgiPostPinholesRule()
{
    int result = 0;
    gui_debug("\nYUKI=========action=%s\n",gWebParams.action_radio);
    if(!safe_strcmp(gWebParams.action_radio, "add"))
        result = cgiAddCustomRule();
    else if(!safe_strcmp(gWebParams.action_radio, "delete"))
        result = cgiDelCustomRule();
    return result;
}

/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *         Get all port forwarding rules.
 *           
 * 
 **************************************************************************/
void getAllPinholesRules(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};
    int count =0;
    dal_ret_t *ret_cusSer = NULL;
    ret_cusSer = dal_get_portforwarding_rule(&count);

    gui_debug("\nYUKI===count=%d\n",count);
    if(ret_cusSer == NULL)
        return;
    for(int i=0; i<count; i++)
    {
        sprintf(buf, "%s|", getValueFromDal(ret_cusSer[i].param[FW_FORWARDING_SERVICE_NAME]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(ret_cusSer[i].param[FW_FORWARDING_INTERNAL_CLIENT]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(ret_cusSer[i].param[FW_FORWARDING_ENABLED]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(ret_cusSer[i].param[FW_FORWARDING_LEASE_DURATION]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(ret_cusSer[i].param[FW_FORWARDING_REMOTE_HOST]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(ret_cusSer[i].param[FW_FORWARDING_EXTERNAL_PORT]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(ret_cusSer[i].param[FW_FORWARDING_EXTERNAL_PORT_END]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(ret_cusSer[i].param[FW_FORWARDING_INTERNAL_PORT]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(ret_cusSer[i].param[FW_FORWARDING_PROTOCOL]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(ret_cusSer[i].param[FW_FORWARDING_DESCRIPTION]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(ret_cusSer[i].param[FW_FORWARDING_USE_SERVICE]));
        strcat(varValue, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s||", getValueFromDal(ret_cusSer[i].param[FW_FORWARDING_OID]));
        strcat(varValue, buf);
    }
    gui_debug("\nYUKI===Get Firewall Pinholes Custom Services===varValue=%s\n",varValue);
    free_dal_ret(&ret_cusSer);
}
/****************************Firewall Pinholes End****************************/
/****************************Firewall Advanced Settings Begin****************************/
/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *         Set Firewall Advanced Settings.
 *           
 * 
 **************************************************************************/
int cgiPostFirewallAdvancedSettings(){
    int result = 0;
    char adv_enable[BUFLEN_4] = {0};
    char invalidIp[BUFLEN_4] = {0};
    char portscan[BUFLEN_4] = {0};
    char drop_unkownType[BUFLEN_4] = {0};
    char drop_invalid[BUFLEN_4] = {0};
    char drop_iier[BUFLEN_4] = {0};
    char fl_limit[BUFLEN_4] = {0};
    char fl_rate[BUFLEN_16] = {0};
    char fl_burst[BUFLEN_16] = {0};
    char fl_icmp[BUFLEN_4] = {0};
    char fl_udp[BUFLEN_4] = {0};
    char fl_lupm[BUFLEN_4] = {0};
    char fl_ltcpe[BUFLEN_4] = {0};
    char fl_lisyn[BUFLEN_4] = {0};
    int ret_size = sizeof(dal_ret_t);
    dal_ret_t ret_fw;
    memset(&ret_fw, 0, ret_size);

    gui_debug("\nYUKI===If enable Advanced Settings:%d\n",gWebParams.fw_adv_enable);
    if(gWebParams.fw_adv_enable == 1){
        
        sprintf(adv_enable, "%d", gWebParams.fw_adv_enable);
        sprintf(invalidIp, "%d", gWebParams.fw_adv_invalidIp);
        sprintf(portscan, "%d", gWebParams.fw_adv_portscan);
        sprintf(drop_unkownType, "%d", gWebParams.fw_drop_unkownType);
        sprintf(drop_invalid, "%d", gWebParams.fw_drop_invalid);
        sprintf(drop_iier, "%d", gWebParams.fw_drop_iier);
        sprintf(fl_limit, "%d", gWebParams.fw_fl_limit);
        sprintf(fl_rate, "%d", gWebParams.fw_fl_rate);
        sprintf(fl_burst, "%d", gWebParams.fw_fl_burst);
        sprintf(fl_icmp, "%d", gWebParams.fw_fl_icmp);
        sprintf(fl_udp, "%d", gWebParams.fw_fl_udp);
        sprintf(fl_lupm, "%d", gWebParams.fw_fl_lupm);
        sprintf(fl_ltcpe, "%d", gWebParams.fw_fl_ltcpe);
        sprintf(fl_lisyn, "%d", gWebParams.fw_fl_lisyn);

        ret_fw.param[FW_ADV_ENABLED] = &adv_enable[0];
        ret_fw.param[FW_ADV_INVALIDIP] = &invalidIp[0];
        ret_fw.param[FW_ADV_PORTSCAN] = &portscan[0];
        ret_fw.param[FW_ADV_UNKNOWTYPE] = &drop_unkownType[0];
        ret_fw.param[FW_ADV_INVALIDTCPFLAGS] = &drop_invalid[0];
        ret_fw.param[FW_ADV_INVALIDICMPREQU] = &drop_iier[0];
        ret_fw.param[FW_ADV_FLOODLIMIT] = &fl_limit[0];
        if(gWebParams.fw_fl_limit == 1)
        {
            ret_fw.param[FW_ADV_FLOODRATELIMIT] = &fl_rate[0];
            ret_fw.param[FW_ADV_FLOODBURSTLIMIT] = &fl_burst[0];
            ret_fw.param[FW_ADV_FL_ICMP] = &fl_icmp[0];
            ret_fw.param[FW_ADV_FL_UDP] = &fl_udp[0];
            ret_fw.param[FW_ADV_FL_UDPPASS] = &fl_lupm[0];
            ret_fw.param[FW_ADV_FL_TCP] = &fl_ltcpe[0];
            ret_fw.param[FW_ADV_FL_TCPSYN] = &fl_lisyn[0];
        }
        gui_debug("\nYUKI======\n");
        gui_debug("FW_ADV_ENABLED=%s\n",ret_fw.param[FW_ADV_ENABLED]);
        gui_debug("FW_ADV_INVALIDIP=%s\n",ret_fw.param[FW_ADV_INVALIDIP]);
        gui_debug("FW_ADV_PORTSCAN=%s\n",ret_fw.param[FW_ADV_PORTSCAN]);
        gui_debug("FW_ADV_UNKNOWTYPE=%s\n",ret_fw.param[FW_ADV_UNKNOWTYPE]);
        gui_debug("FW_ADV_INVALIDTCPFLAGS=%s\n",ret_fw.param[FW_ADV_INVALIDTCPFLAGS]);
        gui_debug("FW_ADV_INVALIDICMPREQU=%s\n",ret_fw.param[FW_ADV_INVALIDICMPREQU]);
        gui_debug("FW_ADV_FLOODLIMIT=%s\n",ret_fw.param[FW_ADV_FLOODLIMIT]);
        gui_debug("FW_ADV_FLOODRATELIMIT=%s\n",ret_fw.param[FW_ADV_FLOODRATELIMIT]);
        gui_debug("FW_ADV_FLOODBURSTLIMIT=%s\n",ret_fw.param[FW_ADV_FLOODBURSTLIMIT]);
        gui_debug("FW_ADV_FL_ICMP=%s\n",ret_fw.param[FW_ADV_FL_ICMP]);
        gui_debug("FW_ADV_FL_UDP=%s\n",ret_fw.param[FW_ADV_FL_UDP]);
        gui_debug("FW_ADV_FL_UDPPASS=%s\n",ret_fw.param[FW_ADV_FL_UDPPASS]);
        gui_debug("FW_ADV_FL_TCP=%s\n",ret_fw.param[FW_ADV_FL_TCP]);
        gui_debug("FW_ADV_FL_TCPSYN=%s\n",ret_fw.param[FW_ADV_FL_TCPSYN]);

        result = dal_enable_firewall_advanced();
        result = dal_set_firewall_advanced(&ret_fw);
    }else{
        result = dal_disable_firewall_advanced();
    }
    gui_debug("Security Level=%s\n",gWebParams.fw_level);
    result = dal_set_fw_level(gWebParams.fw_level);
    result = dal_SaveConfig();
    return result;
}

/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *         Get Firewall Advanced Settings.
 *           
 * 
 **************************************************************************/
void cgiGetFirewallAdvancedSettings(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_64] = {0};
    dal_ret_t *ret_fw = NULL;
    dal_ret_t *ret_fw_sts = NULL;
    char security_level[BUFLEN_16] = {0};
    ret_fw = dal_get_firewall_advanced();
    ret_fw_sts = dal_get_firewall_modules_status();
    
    if(ret_fw == NULL || ret_fw_sts == NULL)
        return;

    sprintf(buf, "%s|", getValueFromDal(ret_fw_sts->param[FW_STATUS_ADVANCED]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_fw->param[FW_ADV_INVALIDIP]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_fw->param[FW_ADV_PORTSCAN]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_fw->param[FW_ADV_UNKNOWTYPE]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_fw->param[FW_ADV_INVALIDTCPFLAGS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_fw->param[FW_ADV_INVALIDICMPREQU]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_fw->param[FW_ADV_FLOODLIMIT]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_fw->param[FW_ADV_FLOODRATELIMIT]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_fw->param[FW_ADV_FLOODBURSTLIMIT]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_fw->param[FW_ADV_FL_ICMP]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_fw->param[FW_ADV_FL_UDP]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_fw->param[FW_ADV_FL_UDPPASS]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_fw->param[FW_ADV_FL_TCP]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(ret_fw->param[FW_ADV_FL_TCPSYN]));
    strcat(varValue, buf);
    //Get Security Level
    dal_get_fw_level(&security_level);
    strcat(varValue, security_level);
    gui_debug("\nYUKI===Get Firewall Advanced Settings===varValue=%s\n",varValue);
    free_dal_ret(&ret_fw);
    free_dal_ret(&ret_fw_sts);
}
/****************************Firewall Advanced Settings End****************************/

/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *         Get Firewall IP Passthru
 *           
 * 
 **************************************************************************/
void cgiGetFirewallIPPassthru(int argc, char **argv, char *varValue)
{
    char buf[BUFLEN_256] = {0};

    //snprintf(buf, sizeof(buf), "1|192.168.1.156|172.16.10.221|2|00:23:cd:92:db:85|1|2|3|4||");
    
    dal_ret_t *dal_get = dal_get_ip_passthru();
    if(dal_get == NULL)
    {
        gui_debug("cgiGetFirewallIPPassthru dal_get is null");
        return;
    }
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(dal_get->param[FW_IP_PASSTHRU_ALLOCATION_MODE]));
    strcat(varValue, buf);
        
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(dal_get->param[FW_IP_PASSTHRU_INTERADDR]));
    strcat(varValue, buf);
        
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(dal_get->param[FW_IP_PASSTHRU_PASSTHRU_MODE]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(dal_get->param[FW_IP_PASSTHRU_FIXMACADDR]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(dal_get->param[FW_IP_PASSTHRU_DHCPLEASE_DAY]));
    strcat(varValue, buf); 

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(dal_get->param[FW_IP_PASSTHRU_DHCPLEASE_HOUR]));
    strcat(varValue, buf); 

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(dal_get->param[FW_IP_PASSTHRU_DHCPLEASE_MINUTE]));
    strcat(varValue, buf); 

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(dal_get->param[FW_IP_PASSTHRU_DHCPLEASE_SECOND]));
    strcat(varValue, buf); 

    free_dal_ret(&dal_get);
    gui_debug("Firewall IP Passthru varValue: %s\n", varValue);
}


int cgiPostFirewallIPPassthru()
{
    tsl_rv_t result = 0;
    dal_ret_t dal_post;
    int ret_size = sizeof(dal_ret_t);
    memset(&dal_post, 0, ret_size);
    
    dal_post.param[FW_IP_PASSTHRU_ALLOCATION_MODE] = &gWebParams.ippass_alloc_mode_sel[0];
    if(!safe_strcmp("0", gWebParams.ippass_alloc_mode_sel)){
        //Default Server
        dal_post.param[FW_IP_PASSTHRU_INTERADDR] = &gWebParams.def_serv_internal_addr_value[0];

    }else if(!safe_strcmp("2", gWebParams.ippass_alloc_mode_sel)){
        //Passthrough
        dal_post.param[FW_IP_PASSTHRU_PASSTHRU_MODE] = &gWebParams.ippass_pass_mode_sel[0];
        if(!safe_strcmp("0", gWebParams.ippass_pass_mode_sel))
        {
            //DHCPS-dynamic
            dal_post.param[FW_IP_PASSTHRU_DHCPLEASE_DAY] = &gWebParams.pass_lease_day[0];
            dal_post.param[FW_IP_PASSTHRU_DHCPLEASE_HOUR] = &gWebParams.pass_lease_hour[0];
            dal_post.param[FW_IP_PASSTHRU_DHCPLEASE_MINUTE] = &gWebParams.pass_lease_min[0];
            dal_post.param[FW_IP_PASSTHRU_DHCPLEASE_SECOND] = &gWebParams.pass_lease_sec[0];
        }
        else if(!safe_strcmp("1", gWebParams.ippass_pass_mode_sel))
        {
            //DHCPS-fixed
            dal_post.param[FW_IP_PASSTHRU_FIXMACADDR] = &gWebParams.pass_fixed_mac_addr_value[0];
            dal_post.param[FW_IP_PASSTHRU_DHCPLEASE_DAY] = &gWebParams.pass_lease_day[0];
            dal_post.param[FW_IP_PASSTHRU_DHCPLEASE_HOUR] = &gWebParams.pass_lease_hour[0];
            dal_post.param[FW_IP_PASSTHRU_DHCPLEASE_MINUTE] = &gWebParams.pass_lease_min[0];
            dal_post.param[FW_IP_PASSTHRU_DHCPLEASE_SECOND] = &gWebParams.pass_lease_sec[0];
        }
        else{
            //Manual
        }  
    }

    result = dal_set_ip_passthru(&dal_post);

    gui_debug("allocation mode=%s; default server inter address=%s\n",dal_post.param[FW_IP_PASSTHRU_ALLOCATION_MODE],dal_post.param[FW_IP_PASSTHRU_INTERADDR]);  
    gui_debug("passThr mode=%s; fixed mac=%s\n",dal_post.param[FW_IP_PASSTHRU_PASSTHRU_MODE],dal_post.param[FW_IP_PASSTHRU_FIXMACADDR]);
    gui_debug("dhcp lease day=%s; hours=%s; minutes=%s; second=%s\n",dal_post.param[FW_IP_PASSTHRU_DHCPLEASE_DAY], dal_post.param[FW_IP_PASSTHRU_DHCPLEASE_HOUR], dal_post.param[FW_IP_PASSTHRU_DHCPLEASE_MINUTE], dal_post.param[FW_IP_PASSTHRU_DHCPLEASE_SECOND]);
    gui_debug("set dal result=%d\n",result);


    dal_SaveConfig();
    
    return result;
}
