#!/bin/bash

if [ $# -ne 3 ]
    then
    echo "usage: $0 <record Length> <output size> <output file>" 
    exit
    fi

numCores=$(grep processor /proc/cpuinfo | wc -l)
#let "numThreads = ($numCores/2)"
let "numThreads = ($numCores/1)"
let "numLines = ($2/$numThreads)"
#echo $numThreads

i=0
while [ $i -lt $numThreads ]
    do
        ./genStrings.py $1 $numLines $3.$i &
        let "i = $i + 1"
    done
wait

i=0
while [ $i -lt $numThreads ]
    do
        cat $3.$i >> $3
        rm $3.$i
        let "i = $i + 1"
    done
