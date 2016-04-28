#!/bin/sh

case "$1" in
	start)
		echo "Mounting filesystems..."
		/bin/mount -a
		mknod /var/fuse c 10 229
		chmod a+rw /var/fuse
		mkdir -p /var/log /var/run /var/state/dhcp /var/ppp /var/udhcpd /var/zebra /var/siproxd /var/cache /var/tmp /var/samba /var/samba/share /var/samba/homes /var/samba/private /var/samba/locks /var/udhcpc /var/share/udhcpc
		mkdir -p /var/tmp/run
		mkdir -p /mnt/rt_conf
		/bin/mount /data /mnt/rt_conf
		cp  /etc/smb.conf /var/samba/ 2>/dev/null
		cp  /etc/udhcpc/* /var/share/udhcpc/ 2>/dev/null
		exit 0
		;;

	stop)
		sync
		echo "Unmounting filesystems..."
		/bin/umount /mnt/rt_conf
		/bin/umount -a
		exit 0
		;;

	*)
		echo "$0: unrecognized option $1"
		;;

esac

