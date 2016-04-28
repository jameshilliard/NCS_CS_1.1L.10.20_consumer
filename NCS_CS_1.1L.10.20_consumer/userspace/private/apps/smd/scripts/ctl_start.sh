#! /bin/sh

echo 0 > /proc/sys/kernel/sysrq

insmod /lib/modules/3.4.11-rt19/extra/bcm96368wdt.ko

/sbin/ssd &

/sbin/tr69 &

/sbin/data_center &

cat /proc/nvram/BootImage &

/sbin/uhttpd -h /sbin/www -T 120 -I main.html -D 0 -p 80 -p 8080 -p [::]:80 -s 443 -s 8443 -C /etc/CA.crt -K /etc/CA.key

/sbin/mynetwork &

/bin/wifid &

./bin/record.sh /mnt/rt_conf/RebootNumber
