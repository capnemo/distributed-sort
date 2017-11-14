#!/bin/bash

if [ $# -ne 1 ]
    then
        echo "Usage: $0 <file containing list of remote nodes>"
        exit
fi

list=$(cat $1)
binDir=$(pwd)
for i in $list
    do
        #ssh $i "cd $binDir/code; make clean;make remote"
        ssh $i "cd $binDir; ./localMake.sh"
    done
