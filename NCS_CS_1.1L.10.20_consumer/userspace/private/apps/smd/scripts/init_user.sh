#! /bin/sh

#TARGET_DIR="./"

PASSWD_FILE=$TARGET_DIR"/var/passwd"
PASSWD_MANU=$TARGET_DIR"/etc/passwd.manu"
PASSWD_FIX=$TARGET_DIR"/etc/passwd.fix"
PASSWD_RUNTIME=$TARGET_DIR"/tmp/passwd.runtime"


GROUP_FILE="/var/group"
GROUP_FIX="/etc/group.fix"


user_init ()
{
    if [ "$1" == "NO_WAIT_DATACENTER" ]; then
        return
    fi

    # Waiting for passwd.dyn created by datacenter
    DATA_CENTER_INITED="/tmp/.datacenter_inited"
    while [ ! -f "$DATA_CENTER_INITED" ]
    do
        sleep 5
    done

    return 0
}

file_append()
{
    if [ -f $1 ];then
        cat $1 >> $2
        echo "cat $1 >> $2"
    else 
        echo DEBUG file $1 not found 
    fi
}

user_main()
{
    # initial group and passwd
    if [ ! -f "$PASSWD_FILE" ];then
        $TARGET_DIR/bin/mp_is_manu_mode.sh
        ManuMode=$?
        if [ "$ManuMode" = "1" ] ; then
            file_append $PASSWD_MANU $PASSWD_FILE
        fi
        
        file_append $PASSWD_FIX $PASSWD_FILE

        file_append $GROUP_FIX $GROUP_FILE
    fi

    file_append $PASSWD_RUNTIME $PASSWD_FILE
    rm -f $PASSWD_RUNTIME

    return 0
}


user_init $1
user_main
if [ $? -eq 0 ] ; then
    exit 0
else
    exit 1
fi

