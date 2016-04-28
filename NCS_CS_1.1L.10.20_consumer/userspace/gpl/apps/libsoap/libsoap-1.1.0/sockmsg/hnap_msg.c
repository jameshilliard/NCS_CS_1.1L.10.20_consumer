
#include <stdio.h>
#include "unix_sock.h"

int main(int argc, char **argv)
{
    char cmd[32] = {0};

    if (argc < 2){
        return 0;
    }

    snprintf(cmd, sizeof(cmd)-1, "%s", argv[1]);

    switch(cmd[1]){
        case 'l':
            unix_sock_client_send(HNAP_MSG_SOCK_FILE, hnap_msg_event_log_start);
            printf("start hnap log\n");
            break;

        case 'n':
            unix_sock_client_send(HNAP_MSG_SOCK_FILE, hnap_msg_event_log_stop);
            printf("stop hnap log\n");
            break;

        case 'c':
            unix_sock_client_send(HNAP_MSG_SOCK_FILE, hnap_msg_event_log_clean);
            printf("clean hnap log\n");
            break;
    }

    return 0;
}
