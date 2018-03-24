#!/usr/bin/python

import random
import sys

def genRandomString(length):
    
    rec = ''
    for i in xrange(0, length):
        rec += chr(random.randint(ord('a'), ord('z')))
   
    return rec

if (len(sys.argv) != 4):
    print "usage ", sys.argv[0], "<record size> <size of output> <out file>"
    sys.exit()

recSz = int(sys.argv[1])
numRecs = int(sys.argv[2])
outFile = open(sys.argv[3], 'w')

buffer = []
for i in xrange(0, numRecs):
    outFile.write(genRandomString(recSz) + '\n')
