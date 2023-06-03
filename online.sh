#!/bin/bash

while :
do
    lastfile1=`ls -tr data/*.bld | tail -n 1`
    echo $lastfile1
    
    ./ana $lastfile1 online.root -online -last -web 5920 &
    pid=$!
    echo $pid
    
    while :
    do
	lastfile2=`ls -tr data/*.bld | tail -n 1`    
	if [ $lastfile2 = $lastfile1 ]; then
	    sleep 1
	else
	    echo "New file detected."
	    kill -2 $pid
	    break
	fi
    done
done
