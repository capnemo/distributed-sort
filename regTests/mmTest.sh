#!/bin/bash

fileSize=100000
numFiles=5
recLen=10

mergeArgs=()
for i in 1 2 3 4 5
    do
    ./genData.sh $recLen $fileSize mm.$i.u
    sort mm.$i.u > mm.$i.s
    rm mm.$i.u
    mergeArgs[$i]=mm.$i.s
    done

./mergeTest "${mergeArgs[@]}" merge.out

inputLength=$(echo $fileSize" * "$numFiles | bc)
outputLength=$(wc -l merge.out | awk '{print $1}')
if [ $inputLength -eq $outputLength ]
    then
    echo "Merge test passed"
    rm "${mergeArgs[@]}" merge.out
    else
    echo "Merge test failed"
fi

