#!/bin/bash

function copySource {
    
    srcList=$(grep ^$1 ../Makefile | awk 'BEGIN{FS="="}{print $2}' | sed s/'\.o'/.cpp/g)
    headerList=$(grep ^$1 ../Makefile | awk 'BEGIN{FS="="}{print $2}' | sed s/'\.o'/.h/g)
    
    currDir=$(pwd)
    cd ..
    for i in $srcList
        do
            cp --parent $i $2
       done

    cd $currDir
    for i in $headerList
        do
            #cp ../$i $2
            cp ../incl/$(basename ../$i) $2/incl
        done
}

if [ $# -ne 1 ]
    then
        echo "Usage: $0 <destination directory>"
        exit
fi

if [ -e $1 ]
    then
    echo "$1 exists. Please delete"
    exit
fi

parentDir=$1
mkdir $parentDir
chmod 777 $parentDir

mkdir -p $parentDir/code/incl
chmod 777 $parentDir/code
chmod 777 $parentDir/code/incl

mkdir $parentDir/run
chmod 777 $parentDir/run

mkdir $parentDir/scripts
chmod 777 $parentDir/scripts

mkdir $parentDir/logs
chmod 777 $parentDir/logs
logPref="logFilePrefix $parentDir/logs/"

copySource "AGENT_OBJS" $parentDir/code
for i in serverTypes dispatch localTypes sortMerge
    do
        cp ../incl/$i.h $parentDir/code/incl
    done

#cp ../incl/dispatch.h ../incl/serverTypes.h ../incl/localTypes.h $parentDir/code/incl
#copySource "COMMON_OBJS" $parentDir/code
cp ../incl/* $parentDir/code/incl

chmod 777 $parentDir/code/*
chmod 777 $parentDir/code/*/*

cp ../Makefile $parentDir/code
cp ../lsort $parentDir/run #Change from server to something else.
cp ../config.txt $parentDir/run
cp config-check.sh remoteCompile.sh localMake.sh $parentDir/scripts
cp node-list remoteStart.sh runLocal.sh $parentDir/scripts
echo $logPref >> $parentDir/run/config.txt
