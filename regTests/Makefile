CC=clang++ -Wall -ggdb -Ofast -std=c++11 -Wall  -I../incl
COMMON=../common/globalLogger.cpp ../common/logger.cpp
BR=../io/bufferedReader.cpp 
BW=../io/bufferedWriter.cpp 
MM=../handlers/multiMergeHandler.cpp
PT=../common/protocol.cpp
TU=../common/tcpUtil.cpp
TP=../threadUtil/threadPool.cpp
TW=../threadUtil/worker.cpp

all: brTest bwTest mergeTest protTest threadTest

brTest:brTest.cpp
	$(CC) $(COMMON) $(BR) brTest.cpp -o brTest -lpthread

bwTest:bwTest.cpp
	$(CC) $(COMMON) $(BW) bwTest.cpp -o bwTest -lpthread

mergeTest:mergeTest.cpp
	$(CC) $(COMMON) $(BW) $(BR) $(MM) mergeTest.cpp -o mergeTest -lpthread

protTest:protTest.cpp
	$(CC) $(PT) $(TU) protTest.cpp  -o protTest

threadTest:threadTest.cpp
	$(CC) $(COMMON) $(TP) $(TW) threadTest.cpp -o threadTest -lpthread

clean:
	rm -rf *.o brTest bwTest mergeTest protTest threadTest
