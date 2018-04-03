#!/bin/sh

git checkout Makefile

rm depList
for i in $(find . -name "*.cpp")
    do
        depLine=$(clang++ -std=c++11 -Iincl -MM $i | sed 's/\\//g')
        echo $(dirname $i)/$depLine >> depList
    done

printf "\n\n" >> Makefile
cat depList >> Makefile
rm depList
