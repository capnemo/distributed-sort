#!/bin/bash

cd $1/run
echo $(uname -s)
./agent_$(uname -s)_$(hostname) $2

