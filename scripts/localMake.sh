#!/bin/bash

cd ../code
if [ $(uname -s) == "FreeBSD" ]
    then
    gmake clean; gmake remote
    else
    make clean; make remote
fi
cp agent_$(uname -s) ../run
