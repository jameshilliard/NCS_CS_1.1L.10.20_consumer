/******************************************************************
Copyright (C) Actiontec 2013
Cisco GW HNAP protocol Simulater
fkong@actiontec.com
 ******************************************************************/
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <nanohttp/nanohttp-logging.h>
#include <nanohttp/nanohttp-server.h>
#include <libcsoap/soap-client.h>
#include "cisco_hnap.h"

char wecb_hnap_url[128]={0};
char wecb_hnap_url2[128]={0};

HNAP_Method_List hnap_methods_list[] = {
    {HNAP_GET_URL, hnap_url, "Get WECB HNAP URL."},    
    {HNAP_SET_URL, hnap_url, "Set WECB HNAP URL."},
    {HNAP_GET_REQ_TIMEOUT, hnap_reqtime, "cisco_hnap GetReqTimeout\n  Get HNAP reqest ack timeout period." },
    {HNAP_SET_REQ_TIMEOUT, hnap_reqtime, "cisco_hnap SetReqTimeout 20\n  Set HNAP reqest ack timeout period." },
    {METHOD_GET_DEVICESETTINGS, hnap_get, "Use this method to discover WECB device capabilities."},
    {METHOD_GET_EXTENDERSTATUS, hnap_get, "This method can be used to find the status of an attached WiFiExtender."},
    {METHOD_GET_WLANRADIOS, hnap_get, "This method can be used to find the status of an attached WiFiExtender."},
    {METHOD_GET_CLIENTINFO, hnap_get, "This method can be used to find the status of an attached WiFiExtender."},

    {METHOD_SET_WPS, hnap_set, "This method can be used to set WPS configuration in WECB"},
    {METHOD_SET_SSIDSETTINGS, hnap_set, "Use this method to set Multiple SSIDs"},
    {METHOD_SET_RADIOS, hnap_set, "Use this method to set each Radio’s PHY Settings. "},

    {METHOD_ISDEVICEREADY, hnap_get, "This method is used to determine if a given device is ready to process HNAP calls."},
    {METHOD_SETDORESTART, hnap_get,"If USG any setting to WECB is finish then use this method to notify the WiFi extender device"},

};

void hnap_method_usage()
{
    int i = 0;

    printf("usage:   ./cisco_hnap method <variable>\n");
    for (i=0; i<sizeof(hnap_methods_list)/sizeof(HNAP_Method_List); i++) {
        if (hnap_methods_list[i].description)
            printf("[%d]  method: %s\n     description: %s\n", i+1,
                hnap_methods_list[i].name, 
                hnap_methods_list[i].description);
    }
}

int hnap_reqtime(const char *method, int argc, char **argv)
{
	if (strcasecmp(method, HNAP_SET_REQ_TIMEOUT) ==0) {
		if (argc > 2) {
				if (atoi(argv[2])>0) {
					httpd_set_timeout(atoi(argv[2]));
					log_info2("WECB HNAL Reqest Ack Timeout set as %s secs.\n", argv[2]);
				} else {
					log_error2("Reqest Ack Timeout set %s failed!\n", argv[2]);
				}
		}
	}
	else {
		log_info2("WECB HNAL Reqest Ack Timeout set as %d secs.\n", httpd_get_timeout());
	}

	return 0;
}

int hnap_url(const char *method, int argc, char **argv)
{   
    FILE *fp = NULL;

    if (strcasecmp(method, HNAP_SET_URL) ==0) {
        if (argc > 2)
        {
            snprintf(wecb_hnap_url, sizeof(wecb_hnap_url), "%s", argv[2]);
            if ((fp=fopen(TMP_HNAP_URL_FILE,"w"))!=NULL)      {
                snprintf(wecb_hnap_url, sizeof(wecb_hnap_url), "%s%s", argv[2], HNAP_PROTOCOL);
                snprintf(wecb_hnap_url2, sizeof(wecb_hnap_url2), "%s%s", argv[2], HNAP_PROTOCOL2);


                fprintf(fp, "%s", argv[2]);
                fclose(fp);
            }         
            else 
                log_warn1("set wecb hnap url failed! \n");
        }
        else {
            log_info2("./cisco_hnap %s URL", HNAP_SET_URL);
            return -1;
        }   
    } 
    else if (strcasecmp(method, HNAP_GET_URL) ==0) {
        log_info2("WECB HNAL URL: %s\n", wecb_hnap_url);
        log_info2("WECB HNAL URL2: %s\n", wecb_hnap_url2);
    }

    return 0;
}

int hnap_get(const char *method, int argc, char **argv)
{
    herror_t err;
    SoapCtx *ctx, *ctx2;
    char action[128]={0};

    if (! method  || argc < 2)
        return -1;

    err = soap_ctx_new_with_method(HNAP_URN, method, &ctx);
    if (err != H_OK)
    {
        log_error4("%s():%s [%d]", herror_func(err), herror_message(err),
                herror_code(err));
        herror_release(err);
        return 1;
    }

    snprintf(action, sizeof(action), "%s%s", HNAP_URN, method);
    err = soap_client_invoke(ctx, &ctx2, wecb_hnap_url, action);

    if (err != H_OK)
    {
        log_error4("[%d] %s(): %s ", herror_code(err), herror_func(err),
                herror_message(err));
        herror_release(err);
        soap_ctx_free(ctx);
        return 1;
    }

    soap_xml_doc_print(ctx2->env->root->doc);

    soap_ctx_free(ctx2);
    soap_ctx_free(ctx);

    return 0;
}

int hnap_set(const char *method, int argc, char **argv)
{
    herror_t err;
    SoapCtx *ctx, *ctx2;
    char action[128]={0};

    if (! method  || argc < 2)
        return -1;

    err = soap_ctx_new_with_method(HNAP_URN2, method, &ctx);

    if (err != H_OK)
    {
        log_error4("%s():%s [%d]", herror_func(err), herror_message(err),
                herror_code(err));
        herror_release(err);
        return 1;
    }

    do {
        if (strcasecmp(method, METHOD_SET_WPS) ==0) {
            hnap_set_wps(ctx->env, argc, argv);
            break;
        }

        if (strcasecmp(method, METHOD_SET_SSIDSETTINGS) ==0) {
            hnap_set_ssid(ctx->env, argc, argv);
            break;
        }

        if (strcasecmp(method, METHOD_SET_RADIOS) ==0) {
            hnap_set_radios(ctx->env, argc, argv);
            break;
        }
        // TODO : other set method ...
    }
    while (0);

    snprintf(action, sizeof(action), "%s%s", HNAP_URN2, method);
    err = soap_client_invoke(ctx, &ctx2, wecb_hnap_url2, action);

    if (err != H_OK)
    {
        log_error4("[%d] %s(): %s ", herror_code(err), herror_func(err),
                herror_message(err));
        herror_release(err);
        soap_ctx_free(ctx);
        return 1;
    }

    soap_xml_doc_print(ctx2->env->root->doc);

    soap_ctx_free(ctx2);
    soap_ctx_free(ctx);

    return 0;
}

int hnap_set_wps(SoapEnv *env, int argc, char **argv)
{
    if (argc > 4)     {
        soap_env_add_item(env, NULL, "WPSEnable", argv[2]);
        soap_env_add_item(env, NULL, "WPSPINCode", argv[3]);
        soap_env_add_item(env, NULL, "SSIDIndex", argv[4]);
    }  else {
        soap_env_add_item(env, NULL, "WPSEnable", "1");
        soap_env_add_item(env, NULL, "WPSPINCode", "12345678");
        soap_env_add_item(env, NULL, "SSIDIndex", "20002000");
    }

    return 0;
}

int hnap_set_ssid(SoapEnv *env, int argc, char **argv)
{
    soap_env_push_item(env, NULL, "SSIDList");

    {
        soap_env_push_item(env, NULL, "SSIDSettings");

        soap_env_add_item(env, NULL, "SSIDRadioID", "RADIO_2.4GHz");
        soap_env_add_item(env, NULL, "SSIDIndex", "1");
        soap_env_add_item(env, NULL, "SSID", "HOME_CA23_2.4");
        soap_env_add_item(env, NULL, "SSIDEnabled", "true");
        soap_env_add_item(env, NULL, "SSIDBroadcast", "true");
        soap_env_add_item(env, NULL, "SSIDVlanID", "100");
        soap_env_add_item(env, NULL, "SSIDLanBase", "10.0.0.1");

        {
            soap_env_push_item(env, NULL, "SSIDEncryption");
            soap_env_add_item(env, NULL, "ModeEnabled", "6");
            soap_env_add_item(env, NULL, "Encryption", "TKIPORAES");
            soap_env_add_item(env, NULL, "Passphrase", "Example Passprhase");
            soap_env_add_item(env, NULL, "RekeyInterval", "600");
            soap_env_add_item(env, NULL, "RadiusServerIP", "");
            soap_env_add_item(env, NULL, "RadiusServerPort", "");
            soap_env_add_item(env, NULL, "RadiusSecret", "");
            soap_env_pop_item(env);
        }  //SSIDEncryption


        {
            soap_env_push_item(env, NULL, "SSIDQoS");
            soap_env_add_item(env, NULL, "WMMEnable", "1");
            soap_env_add_item(env, NULL, "UAPSDEnable", "1");

            {
                soap_env_push_item(env, NULL, "QosSettings");
                soap_env_add_item(env, NULL, "AC", "0");
                soap_env_add_item(env, NULL, "ACM", "0");
                soap_env_add_item(env, NULL, "AIFSN", "3");
                soap_env_add_item(env, NULL, "CWMin", "5");
                soap_env_add_item(env, NULL, "CWMax", "11");
                soap_env_add_item(env, NULL, "TXOPLimit", "0");
                soap_env_add_item(env, NULL, "NoACK", "0");
                soap_env_pop_item(env);
            } //QosSettings 0

            {
                soap_env_push_item(env, NULL, "QosSettings");
                soap_env_add_item(env, NULL, "AC", "1");
                soap_env_add_item(env, NULL, "ACM", "0");
                soap_env_add_item(env, NULL, "AIFSN", "7");
                soap_env_add_item(env, NULL, "CWMin", "5");
                soap_env_add_item(env, NULL, "CWMax", "11");
                soap_env_add_item(env, NULL, "TXOPLimit", "0");
                soap_env_add_item(env, NULL, "NoACK", "0");
                soap_env_pop_item(env);
            }  //QosSettings 1

            {
                soap_env_push_item(env, NULL, "QosSettings");
                soap_env_add_item(env, NULL, "AC", "2");
                soap_env_add_item(env, NULL, "ACM", "0");
                soap_env_add_item(env, NULL, "AIFSN", "2");
                soap_env_add_item(env, NULL, "CWMin", "4");
                soap_env_add_item(env, NULL, "CWMax", "5");
                soap_env_add_item(env, NULL, "TXOPLimit", "94");
                soap_env_add_item(env, NULL, "NoACK", "0");
                soap_env_pop_item(env);
            }	// QosSettings 2

            {
                soap_env_push_item(env, NULL, "QosSettings");
                soap_env_add_item(env, NULL, "AC", "3");
                soap_env_add_item(env, NULL, "ACM", "0");
                soap_env_add_item(env, NULL, "AIFSN", "2");
                soap_env_add_item(env, NULL, "CWMin", "3");
                soap_env_add_item(env, NULL, "CWMax", "4");
                soap_env_add_item(env, NULL, "TXOPLimit", "47");
                soap_env_add_item(env, NULL, "NoACK", "0");
                soap_env_pop_item(env);
            } // QosSettings 3

            soap_env_pop_item(env);

        } 	//   SSIDQoS


        soap_env_add_item(env, NULL, "MaxClients", "20");
        soap_env_pop_item(env);
    }  // SSIDSettings


    {
        soap_env_push_item(env, NULL, "SSIDSettings");
        soap_env_add_item(env, NULL, "SSIDRadioID", "RADIO_5GHz");
        soap_env_add_item(env, NULL, "SSIDIndex", "0x000010000");
        soap_env_add_item(env, NULL, "SSID", "HOME_CA23_5");
        soap_env_add_item(env, NULL, "SSIDEnabled", "1");
        soap_env_add_item(env, NULL, "SSIDBroadcast", "1");
        soap_env_add_item(env, NULL, "SSIDVlanID", "500");
        soap_env_add_item(env, NULL, "SSIDLanBase", "10.0.0.1");

        {
            soap_env_push_item(env, NULL, "SSIDEncryption");
            soap_env_add_item(env, NULL, "ModeEnabled", "6");
            soap_env_add_item(env, NULL, "Encryption", " TKIPORAES");
            soap_env_add_item(env, NULL, "Passphrase", "Example Passprhase");
            soap_env_add_item(env, NULL, "RekeyInterval", "600");
            soap_env_add_item(env, NULL, "RadiusServerIP", "");
            soap_env_add_item(env, NULL, "RadiusServerPort", "");
            soap_env_add_item(env, NULL, "RadiusSecret", "");
            soap_env_pop_item(env);
        }   //  SSIDEncryption


        {
            soap_env_push_item(env, NULL, "SSIDQoS");
            soap_env_add_item(env, NULL, "WMMEnable", "1");
            soap_env_add_item(env, NULL, "UAPSDEnable", "1");

            {
                soap_env_push_item(env, NULL, "QosSettings");
                soap_env_add_item(env, NULL, "AC", "0");
                soap_env_add_item(env, NULL, "ACM", "0");
                soap_env_add_item(env, NULL, "AIFSN", "3");
                soap_env_add_item(env, NULL, "CWMin", "5");
                soap_env_add_item(env, NULL, "CWMax", "11");
                soap_env_add_item(env, NULL, "TXOPLimit", "0");
                soap_env_add_item(env, NULL, "NoACK", "0");
                soap_env_pop_item(env);
            }  //  QosSettings 0

            {
                soap_env_push_item(env, NULL, "QosSettings");
                soap_env_add_item(env, NULL, "AC", "1");
                soap_env_add_item(env, NULL, "ACM", "0");
                soap_env_add_item(env, NULL, "AIFSN", "7");
                soap_env_add_item(env, NULL, "CWMin", "5");
                soap_env_add_item(env, NULL, "CWMax", "11");
                soap_env_add_item(env, NULL, "TXOPLimit", "0");
                soap_env_add_item(env, NULL, "NoACK", "0");
                soap_env_pop_item(env);
            }  //  QosSettings 1

            {
                soap_env_push_item(env, NULL, "QosSettings");
                soap_env_add_item(env, NULL, "AC", "2");
                soap_env_add_item(env, NULL, "ACM", "0");
                soap_env_add_item(env, NULL, "AIFSN", "2");
                soap_env_add_item(env, NULL, "CWMin", "4");
                soap_env_add_item(env, NULL, "CWMax", "5");
                soap_env_add_item(env, NULL, "TXOPLimit", "94");
                soap_env_add_item(env, NULL, "NoACK", "0");
                soap_env_pop_item(env);
            }   // QosSettings 2

            {
                soap_env_push_item(env, NULL, "QosSettings");
                soap_env_add_item(env, NULL, "AC", "3");
                soap_env_add_item(env, NULL, "ACM", "0");
                soap_env_add_item(env, NULL, "AIFSN", "2");
                soap_env_add_item(env, NULL, "CWMin", "3");
                soap_env_add_item(env, NULL, "CWMax", "4");
                soap_env_add_item(env, NULL, "TXOPLimit", "47");
                soap_env_add_item(env, NULL, "NoACK", "0");
                soap_env_pop_item(env);
            }  //  QosSettings 3

            soap_env_pop_item(env);
        }  //  SSIDQoS


        soap_env_add_item(env, NULL, "MaxClients", "20");
        soap_env_pop_item(env);
    }   // SSIDSettings

    {
        soap_env_push_item(env, NULL, "SSIDSettings");
        soap_env_add_item(env, NULL, "SSIDRadioID", "RADIO_2.4GHz &RADIO_5GHz");
        soap_env_add_item(env, NULL, "SSIDIndex", "0x20002000");
        soap_env_add_item(env, NULL, "SSID", "XFinity_Hotspot");
        soap_env_add_item(env, NULL, "SSIDEnabled", "1");
        soap_env_add_item(env, NULL, "SSIDBroadcast", "1");
        soap_env_add_item(env, NULL, "SSIDVlanID", "600");
        soap_env_add_item(env, NULL, "SSIDLanBase", "10.0.0.1");

        {
            soap_env_push_item(env, NULL, "SSIDEncryption");
            soap_env_add_item(env, NULL, "ModeEnabled", "1");
            soap_env_add_item(env, NULL, "Encryption", "");
            soap_env_add_item(env, NULL, "Passphrase", "");
            soap_env_add_item(env, NULL, "RekeyInterval", "");
            soap_env_add_item(env, NULL, "RadiusServerIP", "");
            soap_env_add_item(env, NULL, "RadiusServerPort", "");
            soap_env_add_item(env, NULL, "RadiusSecret", "");
            soap_env_pop_item(env);
        }  //   SSIDEncryption



        {
            soap_env_push_item(env, NULL, "SSIDQoS");
            soap_env_add_item(env, NULL, "WMMEnable", "1");
            soap_env_add_item(env, NULL, "UAPSDEnable", "1");

            {
                soap_env_push_item(env, NULL, "QosSettings");
                soap_env_add_item(env, NULL, "AC", "0");
                soap_env_add_item(env, NULL, "ACM", "0");
                soap_env_add_item(env, NULL, "AIFSN", "3");
                soap_env_add_item(env, NULL, "CWMin", "5");
                soap_env_add_item(env, NULL, "CWMax", "11");
                soap_env_add_item(env, NULL, "TXOPLimit", "0");
                soap_env_add_item(env, NULL, "NoACK", "0");
                soap_env_pop_item(env);
            }   // QosSettings 0


            {
                soap_env_push_item(env, NULL, "QosSettings");
                soap_env_add_item(env, NULL, "AC", "1");
                soap_env_add_item(env, NULL, "ACM", "0");
                soap_env_add_item(env, NULL, "AIFSN", "7");
                soap_env_add_item(env, NULL, "CWMin", "5");
                soap_env_add_item(env, NULL, "CWMax", "11");
                soap_env_add_item(env, NULL, "TXOPLimit", "0");
                soap_env_add_item(env, NULL, "NoACK", "0");
                soap_env_pop_item(env);
            }   //   QosSettings 1


            {
                soap_env_push_item(env, NULL, "QosSettings");
                soap_env_add_item(env, NULL, "AC", "2");
                soap_env_add_item(env, NULL, "ACM", "0");
                soap_env_add_item(env, NULL, "AIFSN", "2");
                soap_env_add_item(env, NULL, "CWMin", "4");
                soap_env_add_item(env, NULL, "CWMax", "5");
                soap_env_add_item(env, NULL, "TXOPLimit", "94");
                soap_env_add_item(env, NULL, "NoACK", "0");
                soap_env_pop_item(env);
            }  //  QosSettings 2


            {
                soap_env_push_item(env, NULL, "QosSettings");
                soap_env_add_item(env, NULL, "AC", "3");
                soap_env_add_item(env, NULL, "ACM", "0");
                soap_env_add_item(env, NULL, "AIFSN", "2");
                soap_env_add_item(env, NULL, "CWMin", "3");
                soap_env_add_item(env, NULL, "CWMax", "4");
                soap_env_add_item(env, NULL, "TXOPLimit", "47");
                soap_env_add_item(env, NULL, "NoACK", "0");
                soap_env_pop_item(env);
            }   // QosSettings 3

            soap_env_pop_item(env);
        }  //  SSIDQoS

        soap_env_add_item(env, NULL, "MaxClients", "5");
        soap_env_pop_item(env);
    }   // SSIDSettings

    soap_env_pop_item(env);

    return 0;
}

int hnap_set_radios(SoapEnv *env, int argc, char **argv)
{

    int ret = 0;
    soap_env_push_item(env, NULL, "RadioList");

    soap_env_push_item(env, NULL, "RadioSettings");
    soap_env_add_item(env, NULL, "RadioID", "Radio_2.4GHz");
    soap_env_add_item(env, NULL, "Enable", "true");
    soap_env_add_item(env, NULL, "Mode", "802_11bgn");
    soap_env_add_item(env, NULL, "ChannelWidth", "3");
    soap_env_add_item(env, NULL, "Channel", "0");
    soap_env_add_item(env, NULL, "SecondaryChannel", "3");
    soap_env_add_item(env, NULL, "BeaconInterval", "100");
    soap_env_add_item(env, NULL, "DTIMInterval", "2");
    soap_env_add_item(env, NULL, "GuardInterval", "0");
    soap_env_add_item(env, NULL, "Coexistance", "1");
    soap_env_pop_item(env);

    soap_env_push_item(env, NULL, "RadioSettings");
    soap_env_add_item(env, NULL, "RadioID", "Radio_5GHz");
    soap_env_add_item(env, NULL, "Enable", "true");
    soap_env_add_item(env, NULL, "Mode", "802_11an");
    soap_env_add_item(env, NULL, "ChannelWidth", "2");
    soap_env_add_item(env, NULL, "Channel", "100");
    soap_env_add_item(env, NULL, "SecondaryChannel", "104");
    soap_env_add_item(env, NULL, "BeaconInterval", "100");
    soap_env_add_item(env, NULL, "DTIMInterval", "2");
    soap_env_add_item(env, NULL, "GuardInterval", "0");
    soap_env_add_item(env, NULL, "Coexistance", "0");
    soap_env_pop_item(env);

    soap_env_pop_item(env);

    return 0;
}

int main(int argc, char *argv[])
{
    herror_t err;
    int i=0;
    FILE *fp=NULL;
    char wecb_host_url[128]={0};

#ifdef HAVE_SSL
    int hnap_argc = 5;
#define HNAP_CAFILE   "/etc/hnapca.pem"
    char *hnap_argv[] = { argv[0], "-NHTTPS", "-NHTTPSClient", "-NHTTPcert", HNAP_CAFILE };
#endif

    hlog_set_level(HLOG_VERBOSE); 
    // set wecb HNAP URL default value
    snprintf(wecb_hnap_url, sizeof(wecb_hnap_url2), "%s", HNAP_WECB_URL);
    snprintf(wecb_hnap_url2, sizeof(wecb_hnap_url2), "%s", HNAP_WECB_URL2);

    if ((fp=fopen(TMP_HNAP_URL_FILE,"r"))!=NULL)      {
        if (fgets(wecb_host_url, sizeof(wecb_host_url)-1, fp) != NULL) {
            snprintf(wecb_hnap_url, sizeof(wecb_hnap_url), "%s%s", wecb_host_url, HNAP_PROTOCOL);
            snprintf(wecb_hnap_url2, sizeof(wecb_hnap_url2), "%s%s", wecb_host_url, HNAP_PROTOCOL2);
        }
        fclose(fp);
    }
    
#ifdef HAVE_SSL
    err = soap_client_init_args(hnap_argc, hnap_argv);
#else
    err = soap_client_init_args(argc, argv);
#endif

    if (err != H_OK)
    {
        log_error4("%s():%s [%d]", herror_func(err), herror_message(err),
                herror_code(err));
        herror_release(err);
        return 1;
    }

    if (argc < 2) {
        hnap_method_usage();
        soap_client_destroy();
        return -1;
    }
           
    for (i=0; i<sizeof(hnap_methods_list)/sizeof(HNAP_Method_List); i++) {
        if ( strncasecmp(hnap_methods_list[i].name, argv[1], 5)==0 ) {
            hnap_methods_list[i].func(hnap_methods_list[i].name, argc, argv);
            break;
        }
    }

    if (i>=sizeof(hnap_methods_list)/sizeof(HNAP_Method_List))
        hnap_method_usage();

    soap_client_destroy();
    return 0;
}

