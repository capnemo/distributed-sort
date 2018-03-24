#!/bin/bash

fileStub=/tmp/out.
fileSize=100000
numFiles=12
./threadTest $fileStub $fileSize $numFiles 2>&1 > /dev/null

totalLines=0
for i in $fileStub*
    do
        nL=$(wc -l $i | awk '{print $1}')
        let totalLines="$totalLines + $nL"
    done

let expectedLines="$fileSize * $numFiles"

if [ $totalLines == $expectedLines ]
    then
        echo "Thread test succeeded"
    else
        echo "Thread test failed"
fi
