echo "start telnetd"
iptables -A INPUT -p tcp --syn --dport 23 -j ACCEPT
/usr/sbin/telnetd &
