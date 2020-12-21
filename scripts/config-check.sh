#!/bin/bash

if [ $# != 2 ]; then
    echo "$0 <runList> <common mount point>"
    exit
fi

m_pnt=$2
a_file=$m_pnt/agents
touch $a_file
nodeList=$(cat $1)
status=0
for i in $nodeList
do
    echo $i
    ssh $i ls -l $m_pnt > /dev/null 2>&1
    if [ $? != 0 ]; then
        echo "$i is not configured for ssh or NFS or both."
        status=1
    fi
    ssh $i echo $i >> $a_file 
    if [ $? != 0 ]; then
        echo "$i is not configured for ssh or NFS or both."
        status=1
    fi
done
rm $a_file
if [ $status == 0 ]; then
    echo "ssh and nfs appear to be configured correctly"
else
    echo "ssh and nfs need to be configured correctly"
fi

