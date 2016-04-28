
#ifndef _UNIX_SOCK_H
#define _UNIX_SOCK_H

#define HNAP_MSG_SOCK_FILE  "/tmp/hnap_sock"

typedef int (*sock_msg_func)(int event);

typedef struct {
        char sock_file[32];
        sock_msg_func cb_func;
        int master_id;
        int quit;
} sock_msg_t;

typedef enum {
    hnap_msg_event_log_start = 1,
    hnap_msg_event_log_stop,
    hnap_msg_event_log_clean,

} hnap_event_t;

int unix_sock_client_send(char *p_sock_file, int event);
sock_msg_t *unix_sock_server_create(char *p_sock_file, sock_msg_func cb_func);
void unix_sock_server_cleanup(sock_msg_t *p_msg);
int unix_sock_server_service(sock_msg_t *p_msg);

#endif
