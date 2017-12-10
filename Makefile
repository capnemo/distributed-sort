CC=clang++ -c -Wall -ggdb -std=c++11 -Wall  -Iincl
CCLD=clang++ -ggdb -std=c++11 -Wall  -Iincl

AGENT_OBJS := handlers/msgHandler.o handlers/sortHandler.o handlers/multiMergeHandler.o mergeSort/mergeSort.o threadUtil/worker.o threadUtil/threadPool.o io/bufferedReader.o io/bufferedWriter.o common/filePartition.o common/config.o common/logger.o main.o common/tcpUtil.o common/protocol.o mergeSort/multiMerge.o common/dispatchIterations.o common/globalLogger.o common/globalConfig.o

SERVER_OBJS := common/tcpUtil.o common/protocol.o common/filePartition.o io/nwDispatch.o common/logger.o common/dispatchIterations.o common/globalLogger.o lsort.cpp 

red_binary=agent_$(shell uname -s)

all: lsort agent

lsort:$(SERVER_OBJS)
	$(CCLD) $(SERVER_OBJS) -o lsort -lpthread

agent:$(AGENT_OBJS)
	$(CCLD) $(AGENT_OBJS) -o agent -lpthread

remote:$(AGENT_OBJS)
	$(CCLD) $(AGENT_OBJS) -o $(red_binary) -lpthread

%.o:%.cpp
	$(CC) $*.cpp  -o $*.o


clean:
	rm  -f initiators/*.o handlers/*.o agents/*.o io/*.o mergeSort/*.o threadUtil/*.o common/*.o main.o agent lsort
