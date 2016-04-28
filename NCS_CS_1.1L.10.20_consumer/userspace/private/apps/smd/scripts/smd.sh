#!/bin/sh

case "$1" in
	start)
		/bin/init_user.sh NO_WAIT_DATACENTER
        #cp /etc/group.manu /etc/group
        #cp /etc/passwd.manu /etc/passwd
		# echo "Starting CMS smd..."
		# /bin/smd
		#echo "Starting CMS smd..."
		#sleep 20
		#killall acsd nas eapd lld2d wlevt wlmngr dsldiagd mcpd dnsproxy ssk smd
		echo "add ebtables rule for QTN"
		ebtables -A INPUT -p ARP --arp-ip-src 169.254.1.2 --in-if ! host0 -j DROP
		ebtables -A INPUT -p IPv4 --ip-src 169.254.1.2 --in-if ! host0 -j DROP
		ebtables -A OUTPUT -p ARP --arp-ip-src 169.254.1.1 --out-if ! host0 -j DROP
		ebtables -A OUTPUT -p IPv4 --ip-src 169.254.1.1 --out-if ! host0 -j DROP
        echo "set 169.254.1.1 to interface bcmsw, up interface host0 and add route"
        ifconfig bcmsw 169.254.1.1 netmask 255.255.255.248
        ifconfig bcmsw up
        ifconfig host0 up
        route del -net 169.254.1.0 netmask 255.255.255.248
        route add -host 169.254.1.2 host0
        echo "sb 10700067 c0"
        sb 10700067 c0
        echo "start tftpd"
        cp -rf /etc/firmware/* /var/tmp
        /usr/bin/tftpd -p 69 -a 169.254.1.1 -s
		echo "Starting Actiotnec control layer..."
		/bin/ctl_start.sh
		/bin/mp_burnin.sh &
		exit 0
		;;

	stop)
		echo "Stopping Actiontec control layer..."
#		echo "Stopping CMS smd..."
#		/bin/send_cms_msg -r 0x1000080D 20
		exit 0
		;;

	*)
		echo "$0: unrecognized option $1"
		exit 1
		;;

esac

