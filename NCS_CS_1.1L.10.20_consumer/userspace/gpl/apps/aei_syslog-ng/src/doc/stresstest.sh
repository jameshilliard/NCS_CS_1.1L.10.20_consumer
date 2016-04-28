#!/bin/bash
i=0
while [ "$i" -lt "10000" ]
do
        #echo $i
        logger $@ "$i"
        i=$[$i+1]
done
