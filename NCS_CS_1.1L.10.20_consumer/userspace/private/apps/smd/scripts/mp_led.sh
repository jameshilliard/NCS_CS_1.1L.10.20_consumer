#! /bin/sh

if [ $# -eq 0 ]; then
	exit
else
	case "$1" in
		"on")
		if [ "-$2" = "-" ];then
                # 2G
        		wl down
        		echo "0501" >/proc/led
			echo "0401" >/proc/led
                # 5G
        		call_qcsapi enable_interface wifi0 0
				echo "0700" >/proc/led
        		call_qcsapi set_LED 1 1
                # POWER
        		echo "0601" >/proc/led
                # WPS
        		echo "0301" >/proc/led
                # GPHY1
                echo "0800" >/proc/led
                # GPHY2
                echo "0900" >/proc/led
		else
			case "$2" in
				"2G") 
        				wl down
                        		echo "0401" >/proc/led
        				echo "0501" >/proc/led;;
				"5G")
        				call_qcsapi enable_interface wifi0 0
					echo "0700" >/proc/led
        				call_qcsapi set_LED 1 1;;
				"POWER")
        				echo "0601" >/proc/led;;
				"WPS")
        				echo "0301" >/proc/led;; 
				*)
					echo "error $2 $1";; 
			esac
		fi;;
		"off") 
        	if [ "-$2" = "-" ];then
			echo "0600" >/proc/led
			echo "0300" >/proc/led
			echo "0500" >/proc/led
			echo "0700" >/proc/led
			call_qcsapi set_LED 1 0
			echo "0800" >/proc/led
			echo "0900" >/proc/led
		else
			case "$2" in
				"2G")
                        		echo "0400" >/proc/led
        				echo "0500" >/proc/led;;
				"5G")
					echo "0700" >/proc/led
        				call_qcsapi set_LED 1 0;;
				"POWER")
        			    	echo "0600" >/proc/led;;
				"WPS")
        			    	echo "0300" >/proc/led;;
                    "GPHY1")
                        echo "0800" >/proc/led;;
                    "GPHY2")
                        echo "0900" >/proc/led;;
				*)
        				echo "error $2 $1";;
			esac
		fi ;;
		"onnext")
        	if [ "-$2" = "-" ];then
			echo "0602" >/proc/led
			echo "0302" >/proc/led
			echo "0502" >/proc/led
			echo "0702" >/proc/led
			call_qcsapi set_LED 1 0
			echo "0802" >/proc/led
			echo "0902" >/proc/led
		else
        		case "$2" in
				"2G")
        				echo "0502" >/proc/led;;
				"5G")
					echo "0702" >/proc/led
					call_qcsapi set_LED 1 0;;
        			"POWER")
					echo "0602" >/proc/led;;
       				"WPS")
        				echo "0302" >/proc/led;;
				"GPHY1")
					echo "0802" >/proc/led;;
				"GPHY2")
					echo "0902" >/proc/led;;
        			*)
        				echo "error $2 $1";;
			esac
		fi;;
		"both")
		if [ "-$2" = "-" ];then
                        echo "0608" >/proc/led
                        echo "0308" >/proc/led
        		echo "0508" >/proc/led
			echo "0702" >/proc/led
			call_qcsapi set_LED 1 1
			echo "0802" >/proc/led
			echo "0902" >/proc/led
                else
                       case "$2" in
				"2G")
        				echo "0508" >/proc/led;;
				"5G")
					echo "0702" >/proc/led
					call_qcsapi set_LED 1 1;;
                                "POWER")
                                        echo "0608" >/proc/led;;
                                "WPS")
                                        echo "0308" >/proc/led;;
				"GPHY1")
					echo "0802" >/proc/led;;
				"GPHY2")
					echo "0902" >/proc/led;;
                                *)
                                        echo "error $2 $1";;
			esac
		fi;;
		*)
		echo "error $1";;
	esac
fi  
