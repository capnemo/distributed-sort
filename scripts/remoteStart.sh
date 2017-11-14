#!/bin/bash

if [ $# -le 2 ]
    then
        echo "Usage: $0 <input args> <node file>"
        exit
fi

argArray=( "$@" )
let lastArg="$# - 1"
nodeFile=${argArray[$lastArg]}
numAgents=$(cat $nodeFile | wc -l)
list=$(cat $nodeFile)
argArray[$lastArg]=$numAgents

ip=$(ifconfig | grep inet | grep -v inet6 | grep -v 127.0.0.1 | awk '{print $2}'  | awk 'BEGIN {FS=":"}{print $2}')

cd ..
binDir=$(pwd)
cd run; ./lsort "${argArray[@]}" &
cd ..
for i in $list
    do
        echo $i
        ssh $i $binDir/scripts/runLocal.sh $binDir $ip &
    done




