#!/bin/sh

header=$(grep -n "DO NOT DELETE" Makefile | awk 'BEGIN {FS=":"}{print $1}')
head -$header Makefile > make.stub

for i in $(find . -name "*.cpp")
do
    depLine=$(clang++ -std=c++11 -Iincl -MM $i | sed 's/\\//g')
    echo $(dirname $i)/$depLine >> depList
done

cat make.stub > Makefile
printf "\n\n" >> Makefile
cat depList >> Makefile

rm depList
rm make.stub
