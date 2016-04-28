
#ifndef CISCO_HNAP_H
#define CISCO_HNAP_H

#ifdef HAVE_SSL
#define HNAP_WECB_URL           "https://localhost:8080/HNAP1"
#define HNAP_WECB_URL2          "https://localhost:8080/HNAPExt"
#else
#define HNAP_WECB_URL           "http://localhost:8080/HNAP1"
#define HNAP_WECB_URL2          "http://localhost:8080/HNAPExt"
#endif

#define HNAP_PROTOCOL               "/HNAP1"
#define HNAP_PROTOCOL2               "/HNAPExt"

#define HNAP_URN                "http://purenetworks.com/HNAP1/"
#define HNAP_URN2               "http://cisco.com/HNAPExt/"

#define HNAP_GET_URL                    "GetURL"
#define HNAP_SET_URL                    "SetURL"
#define HNAP_GET_REQ_TIMEOUT            "GetReqTimeout"
#define HNAP_SET_REQ_TIMEOUT            "SetReqTimeout"

#define METHOD_GET_DEVICESETTINGS           "GetDeviceSettings"
#define METHOD_GET_EXTENDERSTATUS           "GetExtenderStatus"
#define METHOD_GET_WLANRADIOS               "GetWLanRadios"
#define METHOD_GET_CLIENTINFO               "GetClientInfo"

#define METHOD_SET_WPS                      "SetWPS"
#define METHOD_SET_SSIDSETTINGS             "SetSSIDSettings"
#define METHOD_SET_RADIOS                   "SetRadios"

#define METHOD_ISDEVICEREADY                "IsDeviceReady"
#define METHOD_SETDORESTART                 "SetDoRestart"


#define TMP_HNAP_URL_FILE       "/tmp/wecb_hnap_url.txt"

typedef int (* method_func)(const char *method, int argc, char **argv);

typedef struct {
    char name[32];
    method_func func;
    char* description;
} HNAP_Method_List;

int hnap_url(const char *method, int argc, char **argv);
int hnap_reqtime(const char *method, int argc, char **argv);

int hnap_get(const char *method, int argc, char **argv);
int hnap_set(const char *method, int argc, char **argv);

int hnap_set_wps(SoapEnv *env, int argc, char **argv);
int hnap_set_ssid(SoapEnv *env, int argc, char **argv);

#endif

