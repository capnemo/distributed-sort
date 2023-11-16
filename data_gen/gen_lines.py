#!/usr/bin/python3
import sys
import random
import threading
import time

from os import cpu_count
from multiprocessing import Process, Lock

def gen_lines(serial, line_len, num_lines, file_ob, lck):
    bf=[]
    for nl in range(0, num_lines):
        ln=""
        for rc in range(0, line_len):
            ln += chr(random.randrange(ord('a'), ord('z') + 1))
        bf.append(ln)
        if (len(bf) == 1000):
            write_to_file(file_ob, bf, lck)
            bf = []

    write_to_file(file_ob, bf, lck)
    print("thread ends ", serial)

def write_to_file(file_object, lines, lck):
    lck.acquire();
    for l in lines:
        file_object.write(l + '\n');
    file_object.flush()
    lck.release();

    
if __name__ == "__main__":
    if (len(sys.argv) != 4):
        print('Generates random strings')
        arg_str = '<line length> <number of lines> <output file>';
        print('usage:', sys.argv[0], arg_str)
        sys.exit()

    line_len = int(sys.argv[1])
    num_recs = int(sys.argv[2])
    file_name = sys.argv[3]

    num_threads = int(cpu_count()/2)
    recs_per_thread = int(num_recs/num_threads)
    outfile = open(file_name, 'w')
    lk = threading.Lock();
    for t in range(0, num_threads):
        Process(target=gen_lines, args=(t, line_len, recs_per_thread, 
                outfile, lk)).start()
        print("starting thread " + str(t))

    outfile.close()
