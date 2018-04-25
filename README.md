
Description
-----------

Distributed Sort is a tool to sort large files ( > 1GB). The solution 
is to break up the input file into multiple segments, sort each one of 
the segments in parallel and merge all the sorted segments. Essentially 
a distributed merge sort. The software works on most unix like OSes 
and has been tested on linux, freeBSD and Mac OS X. The cluster consists 
of one initiator and multiple agents.  The initiator starts sorts on 
different agents and completes the merge once the sorts are done.
It is possible to run the software with a single agent on the same machine 
as the initiator.

Prerequisites and configuration
-------------------------------
None of the following are needed if you want to run the software locally.

NFS
---
Setup NFS so all the agents and the initiator have read and write 
access to the same disk area. The mount point on all machines in the 
cluster should be the same and should point to the same directory on 
the NFS server. Any recent NFS version should do.

SSH
---
Setup passwordless ssh from the initiator to all the agents. The userid 
to connect should be the same for all the agents.

Compiler
--------
Clang++ ver. >= 3.8.

Testing and Running the software on a single machine.
-----------------------------------------------------
a) Extract the code from github and compile. 
   cd <location of extract>;make release
b) Regression tests are in regTests. The script runTests.sh will run all 
   the tests.
   cd regTests;./runTests.sh
c) Open two terminals. Run ./lsort <input file> <output file> in one terminal and
   ./agent 127.0.0.1 in the other.

Running the software in a cluster
----------------------------------
a) Complete the configuration section and the section above.
b) Deploy to NFS.
   Create a new directory. It should be a child of the NFS mount point. 
   Lets call it deploy_dir.
   cd deploy_dir/scripts;./deploy.sh deploy_dir
   deploy.sh will complain about a few missing header files. This is normal.
c) Compile the agent code on remote agents.
   cd deploy_dir/scripts
   Create a file with one agent's name for each OS in the cluster. One 
   agent name per line.
   Run ./remoteCompile.sh compList This will create an agent binary 
   for every OS.
   Create a scratch directory on each machine's local file system. 
   The name should be the same on all the machines. Add this name 
   to the config file, config.txt. It should be the value to the key 
   scratchLocation. 
d) To sort the file. 
   cd deploy_dir/scripts
   Create a file with all the agent names. One agent name on each line. 
   Lets call this file runList.
   Run ./remoteStart <input file> <output file> runList
   Both output file and input file should be on NFS and be accessible to 
   all the agents and the initiator. Logs can be found in deploy_dir/logs.

Performance Comparision (1 agent and initiator on the same machine).
------------------------------------------------------------------
Distributed sort was run on 3 different machines locally on a 
file containing 100 million records. For comparision, the same 
file was sorted using the platform's command line sort. 

The 3 machines are
a) Ubuntu 16.04 2x Xeon E5405 2.0 GHz 8GB 8 Cores total
b) Ubuntu 16.04 AMD FX 8320 1.4 - 3.5 GHz 16GB
c) Mac OS x High Sierra Core i7 2.5 GHz 16 GB

The following are the elapsed times in seconds
------------------------------------------
Machine | Distributed Sort | Native sort |
------------------------------------------
|a      |      80          |   235       |
------------------------------------------
|b      |      49          |   122       |
------------------------------------------
|c      |      53          |   863       |
------------------------------------------

Distributed sort is multithreaded which explains to a certain 
extent the differences in performance between it and the 
native sort on Linux. On Mac OS, the most likely exlpanation is that
native sort is IO constrained.

Performance for different input sizes 
-------------------------------------
Initiator on a linux machine and 1 agent each on a, b and c.
-------------------------------------------
|No. of records in billion | Time to sort |
-------------------------------------------
| 1                        | 402          |
-------------------------------------------
| 2                        | 863          |
-------------------------------------------
| 3                        | 1225         |
-------------------------------------------

Future direction.
-----------------
a) A higher abstraction with a type algebra to handle a bigger subset 
   of parallel algorithms.
b) A facility to run or bind code in different languages. 
c) Merge seems to be inefficient at larger file sizes. 
   To be investigated.
