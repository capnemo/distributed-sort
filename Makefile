CC=clang++ -c -Wall -ggdb -std=c++11 -Wall  -Iincl
CCLD=clang++ -ggdb -std=c++11 -Wall  -Iincl

AGENT_OBJS := handlers/msgHandler.o handlers/sortHandler.o handlers/multiMergeHandler.o mergeSort/mergeSort.o threadUtil/worker.o threadUtil/threadPool.o io/bufferedReader.o io/bufferedWriter.o common/fileUtil.o common/config.o common/logger.o main.o common/tcpUtil.o common/protocol.o

SERVER_OBJS := common/tcpUtil.o common/protocol.o common/fileUtil.o io/dispatch.o common/logger.o lsort.cpp -lpthread
red_binary=agent_$(shell uname -s)

all: lsort agent

lsort:$(SERVER_OBJS)
	$(CCLD) $(SERVER_OBJS) -o lsort

agent:$(AGENT_OBJS)
	$(CCLD) $(AGENT_OBJS) -o agent -lpthread

remote:$(AGENT_OBJS)
	$(CCLD) $(AGENT_OBJS) -o $(red_binary) -lpthread

%.o:%.cpp
	$(CC) $*.cpp  -o $*.o


clean:
	rm  -f initiators/*.o handlers/*.o agents/*.o io/*.o mergeSort/*.o threadUtil/*.o common/*.o main.o agent lsort
