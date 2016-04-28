#! /bin/sh

cli -r default
cfecmd -s ManuStatus 0

rm -rf /mnt/rt_conf/FactoryNumber
rm -rf /mnt/rt_conf/RebootNumber
rm -rf /mnt/rt_conf/.ver_info

