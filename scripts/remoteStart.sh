#!/bin/bash

if [ $# -le 2 ]
    then
        echo "Usage: $0 <input file> <output file> <node file>"
        exit
fi

argArray=( "$@" )
let lastArg="$# - 1"
nodeFile=${argArray[$lastArg]}
numAgents=$(cat $nodeFile | wc -l)
list=$(cat $nodeFile)
argArray[$lastArg]=$numAgents
ip=$(hostname -I)

cd ..
binDir=$(pwd)
cd run; ./lsort "${argArray[@]}" &
cd ..

for i in $list
do
    echo $i
    ssh $i $binDir/scripts/runLocal.sh $binDir $ip &
done
