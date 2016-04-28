/* dhcpc.c
 *
 * udhcp DHCP client
 *
 * Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
 
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/file.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <errno.h>

#include "dhcpd.h"
#include "dhcpc.h"
#include "options.h"
#include "clientpacket.h"
#include "packet.h"
#include "script.h"
#include "socket.h"
#include "debug.h"
#include "pidfile.h"


#ifdef AEI_WECB
#include <dirent.h>
#include <sys/fcntl.h>

#include "ctl.h"
#include "ctl_log.h"
#include "ctl_validstrings.h"
int CTL_LOG_LEVEL = CTL_LOG_LEVEL_WARN;

#if (defined(AEI_ENABLE_DBUS) && defined(AEI_ENABLE_SSD))
//#include files for d-bus
#else
#include "libtr69_client.h"

#define DATACENTER_PID_FILE_NAME "/var/data_center.pid"
#endif
#endif

static int state;
static unsigned long requested_ip; /* = 0 */
static unsigned long server_addr;
static unsigned long timeout;
static int packet_num; /* = 0 */
#ifdef AEI_WECB
static int fd = -1;
#else
static int fd;
#endif
static int signal_pipe[2];

#define LISTEN_NONE 0
#define LISTEN_KERNEL 1
#define LISTEN_RAW 2
static int listen_mode;

#define DEFAULT_SCRIPT	"/usr/share/udhcpc/default.script"

#ifdef AEI_TR111
char oui_125[10]={0};
char sn_125[64]={0};
char prod_125[64]={0};
#endif

struct client_config_t client_config = {
	/* Default options. */
	abort_if_no_lease: 0,
	foreground: 0,
	quit_after_lease: 0,
	background_if_no_lease: 0,
	interface: "eth0",
	pidfile: NULL,
	script: DEFAULT_SCRIPT,
	clientid: NULL,
	hostname: NULL,
	ifindex: 0,
	arp: "\0\0\0\0\0\0",		/* appease gcc-3.0 */
};

#ifdef AEI_WECB
static char * strStatus[] = {
    CTLVS_SELECTING,     // INIT_SELECTING 0
    CTLVS_REQUESTING,    // REQUESTING 1
    CTLVS_BOUND,         // BOUND 2
    CTLVS_RENEWING,      // RENEWING 3
    CTLVS_REBINDING,     // REBINDING 4
    CTLVS_INIT,          // INIT_REBOOT 5

     CTLVS_RENEWING,      // RENEW_REQUESTED 6
     CTLVS_SELECTING,     // RELEASED 7
};

static void set_state( int new_state)
{
    if( state != new_state ) {
        state = new_state;
#if defined(AEI_TR181) || defined(AEI_TR106)
        if( (new_state >= INIT_SELECTING) && (new_state <= RELEASED )) { 
            char * pst = NULL;

            pst = strStatus[new_state];
            tr69_set_leaf_data( "Device.DHCPv4.Client.1.DHCPStatus",
                        (void **)pst,
                        TR69_NODE_LEAF_TYPE_STRING );
        }
#endif
    }
}

#ifdef AEI_CTL_BRIDGE
static int get_dhcp_server_mac(const char* pServIp, char* pServMac,int max_mac_len)
{
    int rv = 1;
    char cmd[255] = {0};
    char pipeline[255] = {0};
    FILE *pipe;
    
    do{
        //1. run command :  cat /proc/net/arp |grep "192.168.200.1 "
        snprintf(cmd,sizeof(cmd),"cat /proc/net/arp | grep '%s '",pServIp);
        if((pipe = popen(cmd, "r")) != NULL)
        {
            if(fgets(pipeline, sizeof(pipeline)-1, pipe))
            {
                ;
            }

            pclose(pipe);
        }

        if (0==strlen(pipeline) )
        {
            //not found arp rule
            ctllog_warn("not found arp rule for dhcp server (cmd : %s), (resp : %s)  !\n",cmd,pipeline);
            break;
        }
        // parse line like :
        // 192.168.200.1    0x1         0x2         00:10:18:1a:7d:21     *        br0
        // 

        char* m = "%[0-9.] %*x %*x %[0-9a-f:]";
        char ip[20] = {0};
        char mac[20] = {0};
        sscanf(pipeline,m,ip,mac);
        if (0==strlen(mac))
        {
            ctllog_error("sscan mac and port failed(%s) !\n",pipeline);
            break;
        }
        strncpy(pServMac,mac,max_mac_len);
        rv = 0;
    }while(0);
    return rv;
}

static int get_bridge_port_by_mac(const char* mac, int *brPort)
{
    int rv = 1;
    char cmd[255] = {0};
    char pipeline[255] = {0};
    FILE *pipe;


    do{
        //1. run command :  brctl  showmacs br0 | grep "00:10:18:1a:7d:21"
        snprintf(cmd,sizeof(cmd),"brctl  showmacs br0 | grep '%s'",mac);
        if((pipe = popen(cmd, "r")) != NULL)
        {
            if(fgets(pipeline, sizeof(pipeline)-1, pipe))
            {
                ;
            }

            pclose(pipe);
        }

        if (0==strlen(pipeline) )
        {
            ctllog_warn("not found bridge port (cmd : %s), (resp : %s)  !\n",cmd,pipeline);
            break;
        }
        // parse line like :
        //   5     00:10:18:1a:7d:21       no                25.74
        // 

        char* m = "%*[^0-9]%d";
        int port = -1;
        sscanf(pipeline,m,&port);
        if (port==-1)
        {
            ctllog_error("sscan  port failed(%s) !\n",pipeline);
            break;
        }
        *brPort=port;
        rv = 0;
    }while(0);
    return rv;
}

static int get_ifname_by_bridge_port(int brPort, char *ifName,int size)
{
#define MAX_UP_LAYER_PORT_NMU 3

    char path[256]="";
    DIR *d;
    int port=-1;
    int rv = 1;
    char cmd[255] = {0};
    char pipeline[255] = {0};
    FILE *pipe;
	int i=0;
	char *ifaces[MAX_UP_LAYER_PORT_NMU]={"eth0","eth1","moca0"};
	for(i;i<MAX_UP_LAYER_PORT_NMU;i++)
	{
		printf("ifaces[%d]=%s\n",i,ifaces[i]);

            snprintf(path, sizeof(path),  "/sys/class/net/%s/brport", ifaces[i]);

            d = opendir(path);
            if (d)
            {
                //1. run command :  cat /sys/class/net/eth0/brport/port_no
                snprintf(cmd,sizeof(cmd),"cat %s/port_no",path);

                if((pipe = popen(cmd, "r")) != NULL)
                {
                    if(fgets(pipeline, sizeof(pipeline)-1, pipe))
                    {
                        ;
                    }

                    pclose(pipe);
                }

                if (0==strlen(pipeline) )
                {
                    ctllog_warn("not found bridge port (cmd : %s), (resp : %s)  !\n",cmd,pipeline);
                    closedir(d);
                    break;
                }
                sscanf(pipeline,"%x",&port);
                if (port==-1)
                {
                    ctllog_error("sscan  port failed(%s) !\n",pipeline);
                    closedir(d);
                    break;
                }
                if(port==brPort)
                {
                    //found match
                    snprintf(ifName,size,"%s",ifaces[i]);
                    rv=0;
                    closedir(d);
                    break;
                    
                }
                closedir(d);
            }
       }
    return rv;
}

static int get_switch_port_by_ifname(const char* ifName, int *swPort)
{
#define SWITCH_PORT_MAX_NUM 9
    int rv = 1;
    if(!ifName || strlen(ifName)==0)
        return rv;

    char cmd[255] = {0};
    char pipeline[255] = {0};
    FILE *pipe;
    int i=0;
    for(i;i < SWITCH_PORT_MAX_NUM;i++)
    {
        memset(pipeline,'\0',sizeof(pipeline));
        do{
            //1. run command :  ethswctl getifname 3 | grep "eth0 "
            snprintf(cmd,sizeof(cmd),"ethswctl getifname %d | grep '%s'",i,ifName);
            if((pipe = popen(cmd, "r")) != NULL)
            {
                if(fgets(pipeline, sizeof(pipeline)-1, pipe))
                {
                   //ctllog_warn("pipeline=(%s)",pipeline);
                   ;
                }

                pclose(pipe);
            }

            if (0==strlen(pipeline) )
            {
                //ctllog_warn("not match port %d with %s (cmd : %s), (resp : %s)  !\n",i,ifName,cmd,pipeline);
                break;
            }
            else
            {
                *swPort=i;
                //ctllog_warn("found port %d by %s",*swPort,ifName);
                rv = 0;
                return rv;
            }
        }while(0);
    }
    return rv;
        
}
static int get_dhcp_server_of_switch_port(const char* pServIp, int *swPort)
{
    char mac[32] = {0};
    int brPort=-1;
    char ifName[32]="";
    if(get_dhcp_server_mac(pServIp, mac,sizeof(mac))==0)
    {
        if(get_bridge_port_by_mac(mac, &brPort)==0)
        {
            if(get_ifname_by_bridge_port(brPort, ifName,sizeof(ifName))==0)
            {
                return get_switch_port_by_ifname(ifName,swPort);
            }
        }
    }
    return 1;
}


#endif

void sendEventMessage(unsigned char assigned, const char *ip, const char *mask, const char *gateway, const char *nameserver)
{
#ifdef AEI_TR69
    char connreq_url[128]={0};
#endif
    /*
    Sync data to those TR-106 objects:
        Device.LAN.IPAddress string xxx.xxx.xxx.xxx
        Device.LAN.SubnetMask string xxx.xxx.xxx.xxx
        Device.LAN.DefaultGateway string xxx.xxx.xxx.xxx
    */
#if (defined(AEI_ENABLE_DBUS) && defined(AEI_ENABLE_SSD))
    // Send EVENT to SSD by D-Bus message
#else

    if( assigned ) {
        // TR-106
        ctllog_debug( "AEI UDHCPC > ip: %s, mask: %s, gw: %s, dns: %s",
                ip, mask, gateway, nameserver );
#if defined(AEI_TR181) || defined(AEI_TR106)
        tr69_commit_set_leaf_data( "Device.IP.Interface.1.IPv4Address.1.IPAddress", (void **)ip, TR69_NODE_LEAF_TYPE_STRING);
        tr69_commit_set_leaf_data( "Device.IP.Interface.1.IPv4Address.1.SubnetMask", (void **)mask, TR69_NODE_LEAF_TYPE_STRING);
        tr69_commit_set_leaf_data_end( "Device.IP.Interface.1.IPv4Address.1");

        tr69_commit_set_leaf_data( "Device.DHCPv4.Client.1.IPAddress", (void **)ip, TR69_NODE_LEAF_TYPE_STRING);
        tr69_commit_set_leaf_data( "Device.DHCPv4.Client.1.SubnetMask", (void **)mask, TR69_NODE_LEAF_TYPE_STRING);
        tr69_commit_set_leaf_data( "Device.DHCPv4.Client.1.IPRouters", (void **)gateway, TR69_NODE_LEAF_TYPE_STRING);
        tr69_commit_set_leaf_data( "Device.DHCPv4.Client.1.DNSServers", (void **)nameserver, TR69_NODE_LEAF_TYPE_STRING);
#ifdef AEI_CTL_BRIDGE
#ifdef AEI_WECB_CUSTOMER_BA
         do{
             char mac[32] = {0};
             char macAddr[32] = {0};
             char ArpEntry[64]={0};
             char *pToken = NULL;
             int i,j=0;
            if(get_dhcp_server_mac(gateway, mac,sizeof(mac))==0){
                 ctllog_debug("gateway=%s,mac=%s\n",gateway,mac);
                 /* Mac address has the following format xx:xx:xx:xx:xx:xx where x is hex number */
                for ( i = 0;mac[i]!='\0';i ++ ) {
                if(mac[i] !=':')
                macAddr[j++] = mac[i];
                }
                ctllog_debug("macaddr=%s\n",macAddr);
                 snprintf(ArpEntry,sizeof(ArpEntry),"%s:%s",gateway,macAddr);
                 tr69_commit_set_leaf_data( "Device.GatewayInfo.X_ACTIONTEC_COM_IPAddress", (void **)gateway, TR69_NODE_LEAF_TYPE_STRING);
                 tr69_commit_set_leaf_data( "Device.GatewayInfo.X_ACTIONTEC_COM_MACAddress", (void **)mac, TR69_NODE_LEAF_TYPE_STRING);
                 tr69_commit_set_leaf_data( "Device.GatewayInfo.X_ACTIONTEC_COM_ArpEntry", (void **)ArpEntry, TR69_NODE_LEAF_TYPE_STRING);
                 tr69_commit_set_leaf_data_end( "Device.GatewayInfo");
                }
            else{
                ctllog_error("get gateway and mac fail\n");
                }
        }while(0);
#endif
        do{
            int rv = 0;
            int wan_port = -1;
            rv = get_dhcp_server_of_switch_port(gateway,&wan_port);
            if (rv)
            {
                ctllog_warn("get wan port failed!\n");
            }
            else
            {
                ctllog_warn("get wan port : %d\n",wan_port);
            }
            tr69_commit_set_leaf_data("Device.DHCPv4.Client.1.X_ACTIONTEC_COM_WanPort", (void*)&wan_port, TR69_NODE_LEAF_TYPE_INT);

        }while(0);
#endif
        tr69_commit_set_leaf_data_end( "Device.DHCPv4.Client.1");

    #ifdef AEI_SYSLOG
        tr69_commit_set_leaf_data( "Device.X_ACTIONTEC_COM_SYSLOG.ServerIP", (void **)gateway, TR69_NODE_LEAF_TYPE_STRING);
        tr69_commit_set_leaf_data_end( "Device.X_ACTIONTEC_COM_SYSLOG");
    #endif
#endif

#ifdef AEI_TR181

#ifdef AEI_NCS_CFG_SYNC
    int zero=0;
    tr69_commit_set_leaf_data("Device.DHCPv4.Server.Enable", (void*)&zero, TR69_NODE_LEAF_TYPE_INT);
    tr69_commit_set_leaf_data_end( "Device.DHCPv4.Server");
    tr69_commit_set_leaf_data("Device.X_ACTIONTEC_COM_DNSServer.Enable", (void*)&zero, TR69_NODE_LEAF_TYPE_INT);
    tr69_commit_set_leaf_data_end( "Device.X_ACTIONTEC_COM_DNSServer");

#endif

#ifdef AEI_TR69
    #ifdef AEI_WECB_CUSTOMER_VERIZON
        char *p_str = NULL;
        int b_setconnrequrl = 0;
        int type;

        tr69_commit_set_leaf_data( "Device.LAN.IPAddress", (void **)ip, TR69_NODE_LEAF_TYPE_STRING);
        tr69_commit_set_leaf_data( "Device.LAN.X_ACTIONTEC_COM_IPAddress", (void **)ip, TR69_NODE_LEAF_TYPE_STRING);
        tr69_commit_set_leaf_data( "Device.LAN.SubnetMask", (void **)mask, TR69_NODE_LEAF_TYPE_STRING);
        tr69_commit_set_leaf_data( "Device.LAN.DefaultGateway", (void **)gateway, TR69_NODE_LEAF_TYPE_STRING);
        tr69_commit_set_leaf_data_end( "Device.LAN");

        tr69_get_unfresh_leaf_data("Device.ManagementServer.X_ACTIONTEC_COM_SetConnReqURL", (void *)&b_setconnrequrl, &type);

        if (b_setconnrequrl) {

            snprintf(connreq_url, sizeof(connreq_url), "http://%s:%d", ip, AEI_TR69_PASSIVE_PORT);

            if (tr69_get_unfresh_leaf_data("Device.ManagementServer.X_ACTIONTEC_COM_UUID", (void *)&p_str, &type)==0) {
                if (type==TR69_NODE_LEAF_TYPE_STRING && p_str) {
                    snprintf(connreq_url, sizeof(connreq_url), "http://%s:%d/%s", ip, AEI_TR69_PASSIVE_PORT, p_str);
                    CTLMEM_FREE_BUF_AND_NULL_PTR(p_str);
                }
            }
            tr69_commit_set_leaf_data("Device.ManagementServer.ConnectionRequestURL", (void**)connreq_url, TR69_NODE_LEAF_TYPE_STRING);
            tr69_commit_set_leaf_data_end("Device.ManagementServer");
        }
    #else
        snprintf(connreq_url, sizeof(connreq_url), "http://%s:%d", ip, AEI_TR69_PASSIVE_PORT);
 
        tr69_commit_set_leaf_data("Device.ManagementServer.ConnectionRequestURL", (void**)connreq_url, TR69_NODE_LEAF_TYPE_STRING);
        tr69_commit_set_leaf_data_end("Device.ManagementServer");

    #endif
#endif

#elif AEI_TR106

#ifdef AEI_TR69
    #ifdef AEI_WECB_CUSTOMER_VERIZON
        char *p_str = NULL;
        int b_setconnrequrl = 0;
        int type;

        tr69_commit_set_leaf_data( "Device.LAN.IPAddress", (void **)ip, TR69_NODE_LEAF_TYPE_STRING);
        tr69_commit_set_leaf_data( "Device.LAN.SubnetMask", (void **)mask, TR69_NODE_LEAF_TYPE_STRING);
        tr69_commit_set_leaf_data( "Device.LAN.DefaultGateway", (void **)gateway, TR69_NODE_LEAF_TYPE_STRING);
        //tr69_commit_set_leaf_data( "Device.LAN.DNSServers", nameserver, TR69_NODE_LEAF_TYPE_STRING);
        tr69_commit_set_leaf_data_end( "Device.LAN");


        tr69_get_unfresh_leaf_data("Device.ManagementServer.X_ACTIONTEC_COM_SetConnReqURL", (void *)&b_setconnrequrl, &type);

        if (b_setconnrequrl) {

            snprintf(connreq_url, sizeof(connreq_url), "http://%s:%d", ip, AEI_TR69_PASSIVE_PORT);

            if (tr69_get_unfresh_leaf_data("Device.ManagementServer.X_ACTIONTEC_COM_UUID", (void *)&p_str, &type)==0) {
                if (type==TR69_NODE_LEAF_TYPE_STRING && p_str) {
                    snprintf(connreq_url, sizeof(connreq_url), "http://%s:%d/%s", ip, AEI_TR69_PASSIVE_PORT, p_str);
                    CTLMEM_FREE_BUF_AND_NULL_PTR(p_str);
                }
            }
            tr69_commit_set_leaf_data("Device.ManagementServer.ConnectionRequestURL", (void**)connreq_url, TR69_NODE_LEAF_TYPE_STRING);
            tr69_commit_set_leaf_data_end("Device.ManagementServer");
        }

    #endif
#endif

#endif
    } else {


    }
#endif
   return;
}

#endif

#ifndef BB_VER
static void show_usage(void)
{
	printf(
"Usage: udhcpc [OPTIONS]\n\n"
"  -c, --clientid=CLIENTID         Client identifier\n"
"  -H, --hostname=HOSTNAME         Client hostname\n"
"  -h                              Alias for -H\n"
"  -f, --foreground                Do not fork after getting lease\n"
"  -b, --background                Fork to background if lease cannot be\n"
"                                  immediately negotiated.\n"
"  -i, --interface=INTERFACE       Interface to use (default: eth0)\n"
"  -n, --now                       Exit with failure if lease cannot be\n"
"                                  immediately negotiated.\n"
"  -p, --pidfile=file              Store process ID of daemon in file\n"
"  -q, --quit                      Quit after obtaining lease\n"
"  -r, --request=IP                IP address to request (default: none)\n"
"  -s, --script=file               Run file at dhcp events (default:\n"
"                                  " DEFAULT_SCRIPT ")\n"
"  -v, --version                   Display version\n"
	);
	exit(0);
}
#endif


/* just a little helper */
static void change_mode(int new_mode)
{
	DEBUG(LOG_INFO, "entering %s listen mode",
		new_mode ? (new_mode == 1 ? "kernel" : "raw") : "none");
#ifdef AEI_WECB
	if (fd >= 0) close(fd);
#else
	close(fd);
#endif
	fd = -1;
	listen_mode = new_mode;
}


/* perform a renew */
static void perform_renew(void)
{
	LOG(LOG_INFO, "Performing a DHCP renew");
	switch (state) {
	case BOUND:
		change_mode(LISTEN_KERNEL);
	case RENEWING:
	case REBINDING:
#ifdef AEI_WECB
        set_state( RENEW_REQUESTED );
#else
		state = RENEW_REQUESTED;
#endif
		break;
	case RENEW_REQUESTED: /* impatient are we? fine, square 1 */
		run_script(NULL, "deconfig");
	case REQUESTING:
	case RELEASED:
		change_mode(LISTEN_RAW);
#ifdef AEI_WECB
        set_state( INIT_SELECTING );
#else
		state = INIT_SELECTING;
#endif
		break;
	case INIT_SELECTING:
		break;
	}

	/* start things over */
	packet_num = 0;

	/* Kill any timeouts because the user wants this to hurry along */
	timeout = 0;
}


/* perform a release */
static void perform_release(void)
{
	char buffer[16];
	struct in_addr temp_addr;

	/* send release packet */
	if (state == BOUND || state == RENEWING || state == REBINDING) {
		temp_addr.s_addr = server_addr;
		sprintf(buffer, "%s", inet_ntoa(temp_addr));
		temp_addr.s_addr = requested_ip;
		LOG(LOG_INFO, "Unicasting a release of %s to %s", 
				inet_ntoa(temp_addr), buffer);
		send_release(server_addr, requested_ip); /* unicast */
		run_script(NULL, "deconfig");
#ifdef AEI_WECB
        sendEventMessage(TRUE,"0.0.0.0","0.0.0.0","0.0.0.0","0.0.0.0");
#endif
	}
	LOG(LOG_INFO, "Entering released state");

	change_mode(LISTEN_NONE);
#ifdef AEI_WECB
    set_state( RELEASED );
#else
	state = RELEASED;
#endif
	timeout = 0x7fffffff;
}


/* Exit and cleanup */
static void exit_client(int retval)
{
	pidfile_delete(client_config.pidfile);
	CLOSE_LOG();
	exit(retval);
}


/* Signal handler */
static void signal_handler(int sig)
{
	if (send(signal_pipe[1], &sig, sizeof(sig), MSG_DONTWAIT) < 0) {
		LOG(LOG_ERR, "Could not send signal: %s",
			strerror(errno));
	}
}


static void background(void)
{
	int pid_fd;

	pid_fd = pidfile_acquire(client_config.pidfile); /* hold lock during fork. */
	while (pid_fd >= 0 && pid_fd < 3) pid_fd = dup(pid_fd); /* don't let daemon close it */
	if (daemon(0, 0) == -1) {
		perror("fork");
		exit_client(1);
	}
	client_config.foreground = 1; /* Do not fork again. */
	pidfile_write_release(pid_fd);
}


#ifdef COMBINED_BINARY
int udhcpc_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	unsigned char *temp, *message;
	unsigned long t1 = 0, t2 = 0, xid = 0;
	unsigned long start = 0, lease;
	fd_set rfds;
	int retval;
	struct timeval tv;
	int c, len;
	struct dhcpMessage packet;
	struct in_addr temp_addr;
	int pid_fd;
	time_t now;
	int max_fd;
	int sig;
#ifdef AEI_TR111
    char *p_str;
    int type;
#endif

	static struct option arg_options[] = {
		{"clientid",	required_argument,	0, 'c'},
		{"foreground",	no_argument,		0, 'f'},
		{"background",	no_argument,		0, 'b'},
		{"hostname",	required_argument,	0, 'H'},
		{"hostname",    required_argument,      0, 'h'},
		{"interface",	required_argument,	0, 'i'},
		{"now", 	no_argument,		0, 'n'},
		{"pidfile",	required_argument,	0, 'p'},
		{"quit",	no_argument,		0, 'q'},
		{"request",	required_argument,	0, 'r'},
		{"script",	required_argument,	0, 's'},
		{"version",	no_argument,		0, 'v'},
		{"help",	no_argument,		0, '?'},
		{0, 0, 0, 0}
	};

	/* get options */
	while (1) {
		int option_index = 0;
		c = getopt_long(argc, argv, "c:fbH:h:i:np:qr:s:v", arg_options, &option_index);
		if (c == -1) break;
		
		switch (c) {
		case 'c':
			len = strlen(optarg) > 255 ? 255 : strlen(optarg);
			if (client_config.clientid) free(client_config.clientid);
			client_config.clientid = xmalloc(len + 2);
			client_config.clientid[OPT_CODE] = DHCP_CLIENT_ID;
			client_config.clientid[OPT_LEN] = len;
			client_config.clientid[OPT_DATA] = '\0';
			strncpy(client_config.clientid + OPT_DATA, optarg, len);
			break;
		case 'f':
			client_config.foreground = 1;
			break;
		case 'b':
			client_config.background_if_no_lease = 1;
			break;
		case 'h':
		case 'H':
			len = strlen(optarg) > 255 ? 255 : strlen(optarg);
			if (client_config.hostname) free(client_config.hostname);
			client_config.hostname = xmalloc(len + 2);
			client_config.hostname[OPT_CODE] = DHCP_HOST_NAME;
			client_config.hostname[OPT_LEN] = len;
			strncpy(client_config.hostname + 2, optarg, len);
			break;
		case 'i':
			client_config.interface =  optarg;
			break;
		case 'n':
			client_config.abort_if_no_lease = 1;
			break;
		case 'p':
			client_config.pidfile = optarg;
			break;
		case 'q':
			client_config.quit_after_lease = 1;
			break;
		case 'r':
			requested_ip = inet_addr(optarg);
			break;
		case 's':
			client_config.script = optarg;
			break;
		case 'v':
			printf("udhcpcd, version %s\n\n", VERSION);
			exit_client(0);
			break;
		default:
			show_usage();
		}
	}

	OPEN_LOG("udhcpc");
	LOG(LOG_INFO, "udhcp client (v%s) started", VERSION);

	pid_fd = pidfile_acquire(client_config.pidfile);
	pidfile_write_release(pid_fd);

	if (read_interface(client_config.interface, &client_config.ifindex, 
			   NULL, client_config.arp) < 0)
		exit_client(1);
		
	if (!client_config.clientid) {
		client_config.clientid = xmalloc(6 + 3);
		client_config.clientid[OPT_CODE] = DHCP_CLIENT_ID;
		client_config.clientid[OPT_LEN] = 7;
		client_config.clientid[OPT_DATA] = 1;
		memcpy(client_config.clientid + 3, client_config.arp, 6);
	}

	/* setup signal handlers */
	socketpair(AF_UNIX, SOCK_STREAM, 0, signal_pipe);
	signal(SIGUSR1, signal_handler);
	signal(SIGUSR2, signal_handler);
	signal(SIGTERM, signal_handler);
	
#ifdef AEI_WECB
#if !(defined(AEI_ENABLE_DBUS) && defined(AEI_ENABLE_SSD))
    // Send info to data-center directly
#if 0
    while(1) {
        // Wait here until data_center ready
        if( 0 == access(DATACENTER_PID_FILE_NAME, F_OK ) ) {
            LOG(LOG_INFO, "data_center is ready." );
            break;
        }
        sleep(2);
    }
#endif
    if( waitDataCenterReady( DEFAULT_WAIT_DATA_CENTER_READY_TIMEOUT )) {
            LOG(LOG_INFO, "data_center is ready." );
    } else {
            LOG(LOG_ERR, "data_center NOT ready!" );
    }
#endif

#endif

#ifdef AEI_WECB
    set_state( INIT_SELECTING );
#else
	state = INIT_SELECTING;
#endif
	run_script(NULL, "deconfig");
	change_mode(LISTEN_RAW);

#ifdef AEI_TR111
    // read ManufacturerOUI, SerialNumber, ProductClass from data_center
    if (tr69_get_unfresh_leaf_data("Device.DeviceInfo.ManufacturerOUI", (void **)&p_str, &type)==0) {
        if ((type==TR69_NODE_LEAF_TYPE_STRING) && p_str) {
            snprintf(oui_125, sizeof(oui_125), "%s", p_str);
            CTLMEM_FREE_BUF_AND_NULL_PTR(p_str);
        }
    }

    if (tr69_get_unfresh_leaf_data("Device.DeviceInfo.SerialNumber", (void **)&p_str, &type)==0) {
        if ((type==TR69_NODE_LEAF_TYPE_STRING) && p_str) {
            snprintf(sn_125, sizeof(sn_125), "%s", p_str);
            CTLMEM_FREE_BUF_AND_NULL_PTR(p_str);
        }
    }

    if (tr69_get_unfresh_leaf_data("Device.DeviceInfo.ProductClass", (void **)&p_str, &type)==0) {
        if ((type==TR69_NODE_LEAF_TYPE_STRING) && p_str) {
            snprintf(prod_125, sizeof(prod_125), "%s", p_str);
            CTLMEM_FREE_BUF_AND_NULL_PTR(p_str);
        }

    }

    DEBUG(LOG_ERR,"OUI=%s, SN=%s, PC=%s", oui_125, sn_125, prod_125);

#endif

	for (;;) {

		tv.tv_sec = timeout - time(0);
		tv.tv_usec = 0;
		FD_ZERO(&rfds);

		if (listen_mode != LISTEN_NONE && fd < 0) {
			if (listen_mode == LISTEN_KERNEL)
				fd = listen_socket(INADDR_ANY, CLIENT_PORT, client_config.interface);
			else
				fd = raw_socket(client_config.ifindex);
			if (fd < 0) {
				LOG(LOG_ERR, "FATAL: couldn't listen on socket, %s", strerror(errno));
				exit_client(0);
			}
		}
		if (fd >= 0) FD_SET(fd, &rfds);
		FD_SET(signal_pipe[0], &rfds);		

		if (tv.tv_sec > 0) {
			DEBUG(LOG_INFO, "Waiting on select...\n");
			max_fd = signal_pipe[0] > fd ? signal_pipe[0] : fd;
			retval = select(max_fd + 1, &rfds, NULL, NULL, &tv);
		} else retval = 0; /* If we already timed out, fall through */

		now = time(0);
		if (retval == 0) {
			/* timeout dropped to zero */
			switch (state) {
			case INIT_SELECTING:
				if (packet_num < 3) {
					if (packet_num == 0)
						xid = random_xid();

					/* send discover packet */
					send_discover(xid, requested_ip); /* broadcast */
					
					timeout = now + ((packet_num == 2) ? 4 : 2);
					packet_num++;
				} else {
					if (client_config.background_if_no_lease) {
						LOG(LOG_INFO, "No lease, forking to background.");
						background();
					} else if (client_config.abort_if_no_lease) {
						LOG(LOG_INFO, "No lease, failing.");
						exit_client(1);
				  	}
					/* wait to try again */
					packet_num = 0;
#ifdef AEI_WECB
                    /* make more constructive to send dhcp discover */
					timeout = now + 10;
#else
					timeout = now + 60;
#endif
				}
				break;
			case RENEW_REQUESTED:
			case REQUESTING:
				if (packet_num < 3) {
					/* send request packet */
					if (state == RENEW_REQUESTED)
						send_renew(xid, server_addr, requested_ip); /* unicast */
					else send_selecting(xid, server_addr, requested_ip); /* broadcast */
					
					timeout = now + ((packet_num == 2) ? 10 : 2);
					packet_num++;
				} else {
					/* timed out, go back to init state */
					if (state == RENEW_REQUESTED) run_script(NULL, "deconfig");
#ifdef AEI_WECB
                    set_state( INIT_SELECTING );
#else
					state = INIT_SELECTING;
#endif
					timeout = now;
					packet_num = 0;
					change_mode(LISTEN_RAW);
				}
				break;
			case BOUND:
				/* Lease is starting to run out, time to enter renewing state */
#ifdef AEI_WECB
                set_state( RENEWING );
#else
				state = RENEWING;
#endif
				change_mode(LISTEN_KERNEL);
				DEBUG(LOG_INFO, "Entering renew state");
				/* fall right through */
			case RENEWING:
				/* Either set a new T1, or enter REBINDING state */
				if ((t2 - t1) <= (lease / 14400 + 1)) {
					/* timed out, enter rebinding state */
#ifdef AEI_WECB
                    set_state( REBINDING );
#else
					state = REBINDING;
#endif
					timeout = now + (t2 - t1);
					DEBUG(LOG_INFO, "Entering rebinding state");
				} else {
					/* send a request packet */
					send_renew(xid, server_addr, requested_ip); /* unicast */
					
					t1 = (t2 - t1) / 2 + t1;
					timeout = t1 + start;
				}
				break;
			case REBINDING:
				/* Either set a new T2, or enter INIT state */
				if ((lease - t2) <= (lease / 14400 + 1)) {
					/* timed out, enter init state */
#ifdef AEI_WECB
                    set_state( INIT_SELECTING );
#else
					state = INIT_SELECTING;
#endif
					LOG(LOG_INFO, "Lease lost, entering init state");
					run_script(NULL, "deconfig");
					timeout = now;
					packet_num = 0;
					change_mode(LISTEN_RAW);
				} else {
					/* send a request packet */
#ifdef AEI_WECB
					change_mode(LISTEN_RAW);
#endif

					send_renew(xid, 0, requested_ip); /* broadcast */

					t2 = (lease - t2) / 2 + t2;
					timeout = t2 + start;
				}
				break;
			case RELEASED:
				/* yah, I know, *you* say it would never happen */
				timeout = 0x7fffffff;
				break;
			}
		} else if (retval > 0 && listen_mode != LISTEN_NONE && FD_ISSET(fd, &rfds)) {
			/* a packet is ready, read it */
			
			if (listen_mode == LISTEN_KERNEL)
				len = get_packet(&packet, fd);
			else len = get_raw_packet(&packet, fd);
			
			if (len == -1 && errno != EINTR) {
				DEBUG(LOG_INFO, "error on read, %s, reopening socket", strerror(errno));
				change_mode(listen_mode); /* just close and reopen */
			}
			if (len < 0) continue;
			
			if (packet.xid != xid) {
				DEBUG(LOG_INFO, "Ignoring XID %lx (our xid is %lx)",
					(unsigned long) packet.xid, xid);
				continue;
			}
			
			if ((message = get_option(&packet, DHCP_MESSAGE_TYPE)) == NULL) {
				DEBUG(LOG_ERR, "couldnt get option from packet -- ignoring");
				continue;
			}
			
			switch (state) {
			case INIT_SELECTING:
				/* Must be a DHCPOFFER to one of our xid's */
				if (*message == DHCPOFFER) {
					if ((temp = get_option(&packet, DHCP_SERVER_ID))) {
						memcpy(&server_addr, temp, 4);
						xid = packet.xid;
						requested_ip = packet.yiaddr;
						
						/* enter requesting state */
#ifdef AEI_WECB
                        set_state( REQUESTING );
#else
						state = REQUESTING;
#endif
						timeout = now;
						packet_num = 0;
#ifdef AEI_TR111  // DHCP Offer
                    } else if ((temp = get_option(&packet, VENDOR_IDENTIFYING_OPTION_CODE))) {
                        GetSvrVIoption(temp);
#endif
					} else {
						DEBUG(LOG_ERR, "No server ID in message");
					}
				}
				break;
			case RENEW_REQUESTED:
			case REQUESTING:
			case RENEWING:
			case REBINDING:
				if (*message == DHCPACK) {
					if (!(temp = get_option(&packet, DHCP_LEASE_TIME))) {
						LOG(LOG_ERR, "No lease time with ACK, using 1 hour lease");
						lease = 60 * 60;
					} else {
						memcpy(&lease, temp, 4);
						lease = ntohl(lease);
					}
						
#ifdef AEI_TR111          // DHCP ACK
    if ((temp = get_option(&packet, VENDOR_IDENTIFYING_OPTION_CODE))) {
        GetSvrVIoption(temp);
     }
#endif
					/* enter bound state */
					t1 = lease / 2;
					
					/* little fixed point for n * .875 */
					t2 = (lease * 0x7) >> 3;
					temp_addr.s_addr = packet.yiaddr;
					LOG(LOG_INFO, "Lease of %s obtained, lease time %ld", 
						inet_ntoa(temp_addr), lease);
					start = now;
					timeout = t1 + start;
					requested_ip = packet.yiaddr;
					run_script(&packet,
						   ((state == RENEWING || state == REBINDING) ? "renew" : "bound"));

#ifdef AEI_WECB
                    set_state( BOUND );
#else
					state = BOUND;
#endif
					change_mode(LISTEN_NONE);
					if (client_config.quit_after_lease) 
						exit_client(0);
					if (!client_config.foreground)
						background();

				} else if (*message == DHCPNAK) {
					/* return to init state */
					LOG(LOG_INFO, "Received DHCP NAK");
					run_script(&packet, "nak");
					if (state != REQUESTING)
						run_script(NULL, "deconfig");
#ifdef AEI_WECB
                    set_state( INIT_SELECTING );
#else
					state = INIT_SELECTING;
#endif
					timeout = now;
					requested_ip = 0;
					packet_num = 0;
					change_mode(LISTEN_RAW);
					sleep(3); /* avoid excessive network traffic */
				}
				break;
			/* case BOUND, RELEASED: - ignore all packets */
			}	
		} else if (retval > 0 && FD_ISSET(signal_pipe[0], &rfds)) {
			if (read(signal_pipe[0], &sig, sizeof(signal)) < 0) {
				DEBUG(LOG_ERR, "Could not read signal: %s", 
					strerror(errno));
				continue; /* probably just EINTR */
			}
			switch (sig) {
			case SIGUSR1: 
				perform_renew();
				break;
			case SIGUSR2:
				perform_release();
				break;
			case SIGTERM:
				LOG(LOG_INFO, "Received SIGTERM");
				exit_client(0);
			}
		} else if (retval == -1 && errno == EINTR) {
			/* a signal was caught */		
		} else {
			/* An error occured */
			DEBUG(LOG_ERR, "Error on select");
		}
		
	}
	return 0;
}

