#! /bin/sh

SUCC=0
FAIL=1
SKIP=2

SKIP_5G=0

if [ "$1" != "" ] ; then 
    if [ "$1" == "--skip5g" ] ; then
        SKIP_5G=1
    fi
fi

check_init ()
{
    #echo "Check Boot Test: Initial ..."

    return 0
}

check_mem()
{
    result=$(cat /proc/meminfo | grep "MemTotal" | grep "12.... kB")
    if [ -z "${result}" ] ; then
        result=$FAIL;
    else
        result=$SUCC;
    fi
    return $result
}

check_fs()
{
    result=$(mount | grep "/mnt/rt_conf type jffs2 (rw,relatime)")
    if [ -z "${result}" ] ; then
        result=$FAIL;
    else
        result=$SUCC;
    fi
    return $result
}

check_eth()
{
    result=$(ifconfig eth0 | grep "UP")
    if [ -z "${result}" ] ; then
        result=$FAIL;
        return $result
    else
        result=$SUCC;
    fi

    result=$(ifconfig eth1 | grep "UP")
    if [ -z "${result}" ] ; then
        result=$FAIL;
    else
        result=$SUCC;
    fi
    return $result
}

check_wlan_2g()
{
    result=$(cli -g Device.WiFi.SSID.1.Status | grep "Up (String)")
    if [ -z "${result}" ] ; then
        result=$FAIL;
    else
        result=$SUCC;
    fi
    return $result
}

check_wlan_5g()
{
    if [ $SKIP_5G == 1 ] ;  then
        return $SKIP
    fi

    result=$(cli -g Device.WiFi.SSID.5.Status | grep "Up (String)")
    if [ -z "${result}" ] ; then
        result=$FAIL;
    else
        result=$SUCC;
    fi
    return $result
}

is_wcb()
{
    isWCB=$(cfecmd -g BoardID | grep "WCB")
    if [ -z "${isWCB}" ] ; then
        isWCB=0;
    else
        isWCB=1;
    fi
    return $isWCB
}


check_moca()
{
    is_wcb
    if [ $? == 0 ] ; then
        # For those board w/o MoCA, SKIP MoCA test
        return $SKIP
    fi

    result=$(moca show --status | grep "MoCA Status(General)")
    if [ -z "${result}" ] ; then
        result=$FAIL;
    else
        result=$SUCC;
    fi
    return $result
}

check_main()
{
    CheckStatus=$SUCC
    Result=$SUCC

    echo "Check Boot Test:"
    echo ""
    
    # 1. Check Memory Size
    check_mem
    CheckStatus=$?
    if [ $CheckStatus -eq $FAIL ] ; then
        echo "Mem Size Test:      FAIL"
        Result=$FAIL
    else
        echo "Mem Size Test:      PASS"
    fi

    # 2. Check File System
    check_fs
    CheckStatus=$?
    if [ $CheckStatus -eq $FAIL ] ; then
        echo "File System Test:   FAIL"
        Result=$FAIL
    else
        echo "File System Test:   PASS"
    fi

    # 3. Check Ethernet
    check_eth
    CheckStatus=$?
    if [ $CheckStatus -eq $FAIL ] ; then
        echo "Ethernet Test:      FAIL"
        Result=$FAIL
    else
        echo "Ethernet Test:      PASS"
    fi

    # 4. Check wlan 2G
    check_wlan_2g
    CheckStatus=$?
    if [ $CheckStatus -eq $FAIL ] ; then
        echo "Wlan 2G Test:       FAIL"
        Result=$FAIL
    else
        echo "Wlan 2G Test:       PASS"
    fi

    # 5. Check wlan 5G
    check_wlan_5g
    CheckStatus=$?
    if [ $CheckStatus -eq $FAIL ] ; then
        echo "Wlan 5G Test:       FAIL"
        Result=$FAIL
    elif [ $CheckStatus -eq $SKIP ] ; then
        echo "Wlan 5G Test:       SKIP"
    else
        echo "Wlan 5G Test:       PASS"
    fi

    # 6. Check MoCA
    check_moca
    CheckStatus=$?
    if [ $CheckStatus -eq $FAIL ] ; then
        echo "MoCA Test:          FAIL"
        Result=$FAIL
    elif [ $CheckStatus -eq $SKIP ] ; then
        echo "MoCA Test:          SKIP"
    else
        echo "MoCA Test:          PASS"
    fi

    return $Result
}

check_init
check_main

exit $?

