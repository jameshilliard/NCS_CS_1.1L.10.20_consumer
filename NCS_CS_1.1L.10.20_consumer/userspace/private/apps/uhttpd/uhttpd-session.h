/*
 * deal with session
 *
 */

//only check limited user
typedef enum {
    FILEEXT_ALL = 15,
    FILEEXT_HTML = 1,
    FILEEXT_CGI = 2,
    FILEEXT_ACT = 4,
    FILEEXT_CONF = 8,
    
    FILEEXT_NONE = 0
} FILEEXT_TYPE;

typedef struct {
    char *filename;
    //FILEEXT_TYPE file_ext;
    int attr;
    int modeltype;  //1:web can visit;    2:wcb can visit   3:all can visit
} CGI_REJECTITEM;

typedef enum {
    USERLEVEL_ROOT = 3,
    USERLEVEL_ADMIN = 2,
    USERLEVEL_USER = 1
} USERLEVEL_TYPE;

typedef struct {
    unsigned int session_id;
    char username[32];
    char userpass[64];
#ifdef AEI_UHTTPD_REALM_AUTH
    char *client_realm;
#endif
    
    int user_level;
    unsigned int session_start;
    
    int is_login;
} HTTP_SESSION;
extern HTTP_SESSION gSession[99];


void session_init();
int session_is_needcheck(char *url);
unsigned int session_gen_session_id();
void session_has_logined(unsigned int session_id);

int session_is_logined(unsigned int session_id);
int session_has_empty();
void session_timeout_check();
void session_clear_single(unsigned int session_id);

void session_update_starttime(unsigned int session_id);
void session_logout(unsigned int session_id);

#ifdef AEI_UHTTPD_REALM_AUTH
//for access map
int isAllowAccess(char *url, int level);
int set_session_realm2id(unsigned int session_id, char *realm, int level);
int get_session_realm2id(unsigned int session_id, char *realm, int *level);
#else
//for access map
int isAllowAccess(char *url, char *username);
#endif
