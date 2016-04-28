#! /bin/sh

# unit in minutes
# 180 = 3*60
WAIT_SYSTEM_UP_TIME=180
# 2880 = 3*60 + 45*60
BURN_IN_TEST_TIME=2880

LED=/proc/led

led_green_on ()
{
    # POWER LED
    echo "0601" > $LED
    # WPS LED
    echo "0301" > $LED
}

led_red_on ()
{
    # POWER LED
    echo "0602" > $LED
    # WPS LED
    echo "0302" > $LED
}

led_off ()
{
    # POWER LED
    echo "0600" > $LED
    # WPS LED
    echo "0300" > $LED
}

burn_in_succ()
{
    echo "Burn-in Test: PASS"
    led_green_on
    cfecmd -s BurnInStatus 1

    return 0
}

burn_in_fail ()
{
    echo "Burn-in Test: FAIL"
    led_off
    led_red_on

    exit -1
}

burn_in_init ()
{
    # 1. Check Manu
    /bin/mp_is_manu_mode.sh
    ManuMode=$?
    if [ "$ManuMode" != "1" ] ; then
        # "Not in manufactory mode"
        echo "Not in manufactory mode, skip Burn-In Test."
        exit 1
    fi

    # 2. Waiting for DUT bringup,
    #    at least 3 minutes since DUT poweron.
    #UpTime=$(cat /proc/uptime | awk -F. '{print $1}')
    DATA_CENTER_INITED="/tmp/.datacenter_inited"
    while [ ! -f "$DATA_CENTER_INITED" ]
    do
        sleep 5
        #UpTime=$(cat /proc/uptime | awk -F. '{print $1}')
    done

    # 3. Manufactory mode Initial, ONLY run once since DUT bringup
    MANU_INIT_FLAG="/tmp/mp_manu_inited.lock"
    if [ ! -f "$MANU_INIT_FLAG" ] ; then
        echo "Manufactory mode: Initial"
        # 3.1. Enable telnet
        /bin/start_telnetd.sh
        # 3.1.1 Enable ICMP. After 3 continus success PING response,
        #       test utility on PC will try to telnet to DUT.
        iptables -A INPUT -p icmp --icmp-type echo-request -j ACCEPT        

        # 3.2. Disable WPS
        cli -s Device.WiFi.AccessPoint.1.WPS.Enable int 0
        cli -s Device.WiFi.AccessPoint.5.WPS.Enable int 0

        # 3.3 Set Manufactory mode init flag
        echo 1 > "$MANU_INIT_FLAG"
    fi

    # 4. Check Burn-In Status
    BurnInStatus=$(cfecmd -g BurnInStatus | grep "0")
    if [ -z "${BurnInStatus}" ] ; then
        BurnInStatus=1
    else
        BurnInStatus=0
    fi

    if [ "$BurnInStatus" == "1" ] ; then
        # "No need to do Burn-in Test"
        echo "No need to do Burn-in Test"
        exit 1
    fi

    # 5. Burn-In Test Initial
    echo "Burn-in Test: Initial"
    # 5.1. Off LEDs
    led_off

    return 0
}


burn_in_main()
{
    echo "Burn-in Test: Start"

    TestTime=$(cat /proc/uptime | awk -F. '{print $1}')
    while [ $TestTime -lt $BURN_IN_TEST_TIME ]
    do
        /bin/mp_check_boot_status.sh --skip5g
        CheckBoot=$?
        if [ $CheckBoot -eq 1 ] ; then
            return 1
        fi

        sec=0
        while [ $sec -lt 30 ]
        do
            led_green_on
            sleep 1

            led_off
            sleep 1

            let "sec+=1"
        done
        TestTime=$(cat /proc/uptime | awk -F. '{print $1}')
    done

    return 0
}


burn_in_init
burn_in_main
if [ $? -eq 0 ] ; then
    burn_in_succ
else
    burn_in_fail
fi

