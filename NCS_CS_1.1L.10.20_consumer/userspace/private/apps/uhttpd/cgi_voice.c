/*****CGI for Local Network****/
#include "cgi_common.h"
#include "cgi_voice.h"
#include "uhttpd-file.h"


/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *          Get voice status 
 *           
 * 
 **************************************************************************/
 void cgiGetVoiceStatus(int argc, char **argv, char *varValue)
 {
    char buf[BUFLEN_1024] = {0};
    unsigned int line_num = 1;
   
    dal_ret_t *voice_stat_line1 = NULL;

    
    voice_stat_line1 = dal_get_line_voice_status(line_num);
    if (voice_stat_line1 == NULL) 
    {
        gui_debug("dal_get_line_voice_status 1 voice_stat_line1 is null");
        return;
    }
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(voice_stat_line1->param[LINE_STATUS]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(voice_stat_line1->param[LINE_CALL_STATE]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(voice_stat_line1->param[LINE_PHONE_NUMBER]));
    strcat(varValue, buf);
    
    free_dal_ret(&voice_stat_line1); 


    dal_ret_t *voice_stat_line2 = NULL;
    line_num = 2;
    voice_stat_line2 = dal_get_line_voice_status(line_num);
    if (voice_stat_line2 == NULL) 
    {
        gui_debug("dal_get_line_voice_status 2 voice_stat_line2 is null");
        return;
    }
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(voice_stat_line2->param[LINE_STATUS]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(voice_stat_line2->param[LINE_CALL_STATE]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(voice_stat_line2->param[LINE_PHONE_NUMBER]));
    strcat(varValue, buf);
    
    free_dal_ret(&voice_stat_line2); 


    
    
    dal_ret_t *regist_stat_line1 = NULL;
    line_num = 1;
    regist_stat_line1 = dal_get_line_reg_status(line_num);
    if (regist_stat_line1 == NULL) 
    {
        gui_debug("dal_get_line_reg_status 1 regist_stat_line1 is null");
        return;
    }
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(regist_stat_line1->param[REGISTRATION_STATUS_TIMESTAMP]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(regist_stat_line1->param[REGISTRATION_STATUS_SERVERIP]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(regist_stat_line1->param[REGISTRATION_STATUS_LAST_REG_MSG]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(regist_stat_line1->param[REGISTRATION_STATUS_LAST_RETRY_INTERVAL]));
    strcat(varValue, buf);
    
    free_dal_ret(&regist_stat_line1); 


    dal_ret_t *regist_stat_line2 = NULL;
    line_num = 2;
    regist_stat_line2 = dal_get_line_reg_status(line_num);
    if (regist_stat_line2 == NULL) 
    {
        gui_debug("dal_get_line_reg_status 2 regist_stat_line2 is null");
        return;
    }
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(regist_stat_line2->param[REGISTRATION_STATUS_TIMESTAMP]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(regist_stat_line2->param[REGISTRATION_STATUS_SERVERIP]));
    strcat(varValue, buf);
    
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s|", getValueFromDal(regist_stat_line2->param[REGISTRATION_STATUS_LAST_REG_MSG]));
    strcat(varValue, buf);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s||", getValueFromDal(regist_stat_line2->param[REGISTRATION_STATUS_LAST_RETRY_INTERVAL]));
    strcat(varValue, buf);
    
    free_dal_ret(&regist_stat_line2);  
   
    //snprintf(buf,sizeof(buf),"N/A|0.0.0.0|N/A|N/A|N/A|192.168.1.6|N/A|N/A||");
   // strcat(varValue, buf);
    
    gui_debug("Local net stat varvalue = %s",varValue);
    

}


/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *          Get voice call summary 
 *           
 * 
 **************************************************************************/
 void cgiGetVoiceCallSummary(int argc, char **argv, char *varValue)
 {
    char buf[BUFLEN_1024] = {0};
    unsigned int line_num = 1;
   
    dal_ret_t *vc_call_sum = NULL;
    summary_call_type call_type = CurrentCall;

    for(line_num = 1;line_num<=2;)
    {   
    
        vc_call_sum = dal_get_line_call_summary(line_num,call_type);
        if (vc_call_sum == NULL) 
        {
            gui_debug("dal_get_line_call_summary vc_call_sum is null");
            return;
        }
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(vc_call_sum->param[CALL_SUMMARY_TIMESTAMP]));
        strcat(varValue, buf);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(vc_call_sum->param[CALL_SUMMARY_TYPE]));
        strcat(varValue, buf);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(vc_call_sum->param[CALL_SUMMARY_DURATION]));
        strcat(varValue, buf);
    
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(vc_call_sum->param[CALL_SUMMARY_CODEC_INUSE]));
        strcat(varValue, buf);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(vc_call_sum->param[CALL_SUMMARY_FAR_END_HOST_INFO]));
        strcat(varValue, buf);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(vc_call_sum->param[CALL_SUMMARY_FAR_END_CALLER_INFO]));
        strcat(varValue, buf);
    
        free_dal_ret(&vc_call_sum); 
        vc_call_sum = NULL;

        if(line_num == 1&&call_type == CurrentCall)
        {
            line_num = 1;
            call_type = LastCompletedCall;
        }
        else if(line_num == 1&&call_type == LastCompletedCall)
        {
            line_num = 2;
            call_type = CurrentCall;
        }
        else if(line_num == 2&&call_type == CurrentCall)
        {
            line_num = 2;
            call_type = LastCompletedCall;
        }
        else
            break;
    }
    
    gui_debug("Voice Call Summary varvalue = %s",varValue);
}

/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *          Get voice Cumulative Since Last Reset  
 *           
 * 
 **************************************************************************/
 void cgiGetVoiceCumuLast(int argc, char **argv, char *varValue)
 {
    char buf[BUFLEN_1024] = {0};
    unsigned int line_num = 1;
   
    dal_ret_t *vc_cum_last = NULL;

    for(line_num = 1;line_num<=2;line_num++)
    {   
    
        vc_cum_last = dal_get_line_cumulative_summary(line_num);
        if (vc_cum_last == NULL) 
        {
            gui_debug("dal_get_line_cumulative_summary vc_cum_last is null");
            return;
        }
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(vc_cum_last->param[CUMULATIVE_SUMMARY_LAST_RESET_TIMESTAMP]));
        strcat(varValue, buf);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(vc_cum_last->param[CUMULATIVE_SUMMARY_NUMBER_OF_CALLS]));
        strcat(varValue, buf);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(vc_cum_last->param[CUMULATIVE_SUMMARY_DURATION]));
        strcat(varValue, buf);
    
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(vc_cum_last->param[CUMULATIVE_SUMMARY_NUMBER_OF_INCOMING_CALLS_FAILED]));
        strcat(varValue, buf);
        
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%s|", getValueFromDal(vc_cum_last->param[CUMULATIVE_SUMMARY_NUMBER_OF_OUTGOING_CALLS_FAILED]));
        strcat(varValue, buf);
    
        free_dal_ret(&vc_cum_last); 
        vc_cum_last = NULL;
    }
    
    gui_debug("Voice cum last varvalue = %s",varValue);
}


/**************************************************************************
 *  [DESCRIPTION]:
 *           
 *          Get voice Statistics 
 *           
 * 
 **************************************************************************/
 void cgiGetVoiceStatistics(int argc, char **argv, char *varValue)
 {
    char buf[BUFLEN_64] = {0};
    unsigned int line_num = 1;
    statistics_type stat_type = 0;
   
    dal_ret_t *statis_ret = NULL;

    for(stat_type =IncomingLastCall; stat_type<=OutgoingCumulativeCalls; stat_type++)
    {   

        for(line_num = 1;line_num<=2;line_num++)
        {
            statis_ret = dal_get_line_statistics(line_num,stat_type);
            if (statis_ret == NULL) 
            {
                gui_debug("dal_get_line_cumulative_summary statis_ret is null,stat_type=%d,line_num=%u",stat_type,line_num);
                return;
            }
            
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s|", getValueFromDal(statis_ret->param[CALL_STATISTICS_RTP_PACKET_LOSS]));
            strcat(varValue, buf);
            
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s|", getValueFromDal(statis_ret->param[CALL_STATISTICS_RTP_PACKET_LOSS_RATE]));
            strcat(varValue, buf);
            
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s|", getValueFromDal(statis_ret->param[CALL_STATISTICS_TOTAL_RTCP_PACKETS]));
            strcat(varValue, buf);
        
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s|", getValueFromDal(statis_ret->param[CALL_STATISTICS_AVG_INTER_ARRIVAL_JITTER]));
            strcat(varValue, buf);
            
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s|", getValueFromDal(statis_ret->param[CALL_STATISTICS_MAX_INTER_ARRIVAL_JITTER]));
            strcat(varValue, buf);
    
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s|", getValueFromDal(statis_ret->param[CALL_STATISTICS_SUM_OF_INTER_ARRIVAL_JITTER]));
            strcat(varValue, buf);
            
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s|", getValueFromDal(statis_ret->param[CALL_STATISTICS_SUMSQR_OF_INTER_ARRIVAL_JITTER]));
            strcat(varValue, buf);
            
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s|", getValueFromDal(statis_ret->param[CALL_STATISTICS_SUM_FRACTION_LOSS]));
            strcat(varValue, buf);
        
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s|", getValueFromDal(statis_ret->param[CALL_STATISTICS_SUMSQR_FRACTION_LOSS]));
            strcat(varValue, buf);
            
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s|", getValueFromDal(statis_ret->param[CALL_STATISTICS_MAX_ONE_WAY_DELAY]));
            strcat(varValue, buf);
    
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s|", getValueFromDal(statis_ret->param[CALL_STATISTICS_SUM_ONE_WAY_DELAY]));
            strcat(varValue, buf);
            
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s|", getValueFromDal(statis_ret->param[CALL_STATISTICS_SUMSQR_ONE_WAY_DELAY]));
            strcat(varValue, buf);
            
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s|", getValueFromDal(statis_ret->param[CALL_STATISTICS_AVG_ROUND_TRIP_TIME]));
            strcat(varValue, buf);
        
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s|", getValueFromDal(statis_ret->param[CALL_STATISTICS_MAX_ROUND_TRIP_TIME]));
            strcat(varValue, buf);
            
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s|", getValueFromDal(statis_ret->param[CALL_STATISTICS_SUM_ROUND_TRIP_TIME]));
            strcat(varValue, buf);
    
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s|", getValueFromDal(statis_ret->param[CALL_STATISTICS_SUMSQR_ROUND_TRIP_TIME]));
            strcat(varValue, buf);  

            free_dal_ret(&statis_ret); 
            statis_ret = NULL;

        }
        strcat(varValue, "|"); 
    }
    
    gui_debug("Voice statistics varvalue = %s",varValue);
}




/**************************************************************************
 *
 *  [DESCRIPTION]:
 *      Voice Line settings.
 *
 **************************************************************************/
int cgiPostVoiceLine()
{
    tsl_rv_t result = 0;
    unsigned int line_num = 0;
    
    gui_debug("voice_line_type=%s", gWebParams.voice_line_type);

    if(strcmp(gWebParams.voice_line_type,"ringLine1") == 0)
    {
        line_num = 1;
        result = dal_ring_line(line_num);
    }
    else if(strcmp(gWebParams.voice_line_type,"ringLine2") == 0)
    {
        line_num = 2;
        result = dal_ring_line(line_num);
    }
    else if(strcmp(gWebParams.voice_line_type,"resetLine1") == 0)
    {
        line_num = 1;
        result = dal_reset_line(line_num);
    }
    else if(strcmp(gWebParams.voice_line_type,"resetLine2") == 0)
    {
        line_num = 2;
        result = dal_reset_line(line_num);
    }
    else if(strcmp(gWebParams.voice_line_type,"registerLine1") == 0)
    {
        line_num = 1;
        result = dal_register_line(line_num);
    }
    else if(strcmp(gWebParams.voice_line_type,"registerLine2") == 0)
    {
        line_num = 2;
        result = dal_register_line(line_num);
    }
    
    gui_debug("cgiPostVoiceLine result=%d",result);

    dal_SaveConfig();
    return result;
}
