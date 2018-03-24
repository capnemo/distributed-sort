#!/bin/bash

for i in $(cat testList.txt)
    do
        ./$i
    done
