
CC=clang++ -Wall -std=c++11 -Iincl 

AGENT_OBJS := handlers/msgHandler.o handlers/sortHandler.o handlers/blockSort.o handlers/multiMergeHandler.o handlers/blockSortHandler.o mergeSort/mergeSort.o threadUtil/worker.o threadUtil/threadPool.o io/bufferedReader.o io/bufferedWriter.o common/filePartition.o common/config.o common/logger.o mains/agent.o common/tcpUtil.o common/protocol.o mergeSort/multiMerge.o common/dispatchIterations.o common/globalLogger.o common/globalConfig.o

SERVER_OBJS := common/tcpUtil.o common/protocol.o common/filePartition.o io/nwDispatch.o common/logger.o common/dispatchIterations.o common/config.o common/globalConfig.o common/globalLogger.o mains/lsort.o

SSORT_OBJS := common/filePartition.o common/logger.o common/dispatchIterations.o common/globalLogger.o handlers/blockSortHandler.o handlers/blockSort.o io/bufferedWriter.o mains/ssort.o

red_binary=agent_$(shell uname -s)_$(shell hostname)

#thsan: CC += -fsanitize=thread
#thsan: CCLD += -fsanitize=thread
#thsan: all

all: lsort agent ssort dist

debug: CC += -ggdb
debug: all

release: CC += -Ofast
release: all

remote: CC += -Ofast
remote: rem_x

lsort:$(SERVER_OBJS)
	$(CC) $(SERVER_OBJS) -o lsort -lpthread

agent:$(AGENT_OBJS)
	$(CC) $(AGENT_OBJS) -o agent -lpthread

ssort:$(SSORT_OBJS)
	$(CC) $(SSORT_OBJS) -o ssort -lpthread

dist:lsort agent ssort
	mkdir -p bin
	cp lsort agent ssort bin

rem_x:$(AGENT_OBJS)
	$(CC) $(AGENT_OBJS) -Ofast -o $(red_binary) -lpthread

%.o:%.cpp
	$(CC) -c $*.cpp  -o $*.o


sanity:
	$(CC)  --analyze -Xanalyzer -analyzer-output=text *.cpp */*.cpp

lint:
	#clang-tidy *.cpp */*.cpp -- -std=c++11 -Iincl 
	clang-tidy */*.cpp -- -std=c++11 -Iincl */*.cpp

clean:
	rm  -rf initiators/*.o handlers/*.o agents/*.o io/*.o mergeSort/*.o threadUtil/*.o common/*.o mains/*.o mains/lsort mains/agent mains/ssort bin
# DO NOT DELETE


./mergeSort/mergeSort.o: mergeSort/mergeSort.cpp incl/mergeSort.h incl/localTypes.h incl/bufferedReader.h incl/bufferedWriter.h
./mergeSort/multiMerge.o: mergeSort/multiMerge.cpp incl/localTypes.h incl/bufferedWriter.h incl/bufferedReader.h incl/multiMergeHandler.h incl/msgHandler.h incl/logger.h incl/globalLogger.h
./mains/agent.o: mains/agent.cpp incl/localTypes.h incl/serverTypes.h incl/protocol.h incl/globalConfig.h incl/config.h incl/msgHandler.h incl/logger.h incl/tcpUtil.h incl/globalLogger.h
./mains/lsort.o: mains/lsort.cpp incl/tcpUtil.h incl/serverTypes.h incl/localTypes.h incl/filePartition.h incl/iterGen.h incl/nwDispatch.h incl/dispatch.h incl/dispatchIterations.h incl/globalLogger.h incl/globalConfig.h incl/config.h
./mains/ssort.o: mains/ssort.cpp incl/globalLogger.h incl/blockSortHandler.h incl/localTypes.h incl/msgHandler.h incl/logger.h
./threadUtil/worker.o: threadUtil/worker.cpp incl/worker.h incl/localTypes.h
./threadUtil/threadPool.o: threadUtil/threadPool.cpp incl/threadPool.h incl/dispatch.h incl/localTypes.h incl/serverTypes.h incl/worker.h incl/mergeSort.h incl/globalLogger.h
./regTests/brTest.o: regTests/brTest.cpp incl/bufferedReader.h
./regTests/bwTest.o: regTests/bwTest.cpp incl/bufferedWriter.h
./regTests/threadTest.o: regTests/threadTest.cpp regTests/../incl/threadPool.h incl/dispatch.h incl/localTypes.h incl/serverTypes.h incl/worker.h
./regTests/mergeTest.o: regTests/mergeTest.cpp incl/multiMergeHandler.h incl/localTypes.h incl/msgHandler.h incl/logger.h
./regTests/protTest.o: regTests/protTest.cpp incl/protocol.h incl/localTypes.h incl/serverTypes.h
./io/bufferedReader.o: io/bufferedReader.cpp incl/globalLogger.h incl/bufferedReader.h
./io/nwDispatch.o: io/nwDispatch.cpp incl/nwDispatch.h incl/dispatch.h incl/localTypes.h incl/serverTypes.h incl/tcpUtil.h incl/protocol.h incl/globalLogger.h
./io/bufferedWriter.o: io/bufferedWriter.cpp incl/bufferedWriter.h incl/globalLogger.h
./handlers/blockSort.o: handlers/blockSort.cpp incl/blockSort.h incl/localTypes.h incl/blockInterface.h incl/globalLogger.h
./handlers/multiMergeHandler.o: handlers/multiMergeHandler.cpp incl/localTypes.h incl/bufferedWriter.h incl/bufferedReader.h incl/multiMergeHandler.h incl/msgHandler.h incl/logger.h incl/globalLogger.h
./handlers/msgHandler.o: handlers/msgHandler.cpp incl/msgHandler.h incl/logger.h incl/localTypes.h incl/sortHandler.h incl/threadPool.h incl/dispatch.h incl/serverTypes.h incl/worker.h incl/blockSortHandler.h incl/multiMergeHandler.h
./handlers/blockSortHandler.o: handlers/blockSortHandler.cpp incl/filePartition.h incl/iterGen.h incl/localTypes.h incl/blockSortHandler.h incl/msgHandler.h incl/logger.h incl/globalLogger.h incl/blockSort.h incl/blockInterface.h incl/bufferedWriter.h
./handlers/sortHandler.o: handlers/sortHandler.cpp incl/threadPool.h incl/dispatch.h incl/localTypes.h incl/serverTypes.h incl/worker.h incl/config.h incl/mergeSort.h incl/sortMerge.h incl/filePartition.h incl/iterGen.h incl/sortHandler.h incl/msgHandler.h incl/logger.h incl/dispatchIterations.h incl/globalLogger.h incl/globalConfig.h
./common/fileUtil.o: common/fileUtil.cpp incl/localTypes.h incl/fileUtil.h
./common/dispatchIterations.o: common/dispatchIterations.cpp incl/dispatchIterations.h incl/dispatch.h incl/localTypes.h incl/serverTypes.h incl/iterGen.h
./common/config.o: common/config.cpp incl/config.h
./common/globalConfig.o: common/globalConfig.cpp incl/config.h incl/globalConfig.h
./common/filePartition.o: common/filePartition.cpp incl/filePartition.h incl/iterGen.h incl/localTypes.h
./common/globalLogger.o: common/globalLogger.cpp incl/logger.h incl/globalLogger.h
./common/tcpUtil.o: common/tcpUtil.cpp incl/tcpUtil.h
./common/protocol.o: common/protocol.cpp incl/protocol.h incl/localTypes.h incl/serverTypes.h incl/tcpUtil.h
./common/logger.o: common/logger.cpp incl/logger.h
