
CC=clang++ -Wall -std=c++11 -Iincl 

AGENT_OBJS := handlers/msgHandler.o handlers/sortHandler.o handlers/multiMergeHandler.o mergeSort/mergeSort.o threadUtil/worker.o threadUtil/threadPool.o io/bufferedReader.o io/bufferedWriter.o common/filePartition.o common/config.o common/logger.o agent.o common/tcpUtil.o common/protocol.o mergeSort/multiMerge.o common/dispatchIterations.o common/globalLogger.o common/globalConfig.o

SERVER_OBJS := common/tcpUtil.o common/protocol.o common/filePartition.o io/nwDispatch.o common/logger.o common/dispatchIterations.o common/config.o common/globalConfig.o common/globalLogger.o lsort.o

red_binary=agent_$(shell uname -s)

#thsan: CC += -fsanitize=thread
#thsan: CCLD += -fsanitize=thread
#thsan: all

all: lsort agent

debug: CC += -ggdb
debug: all

release: CC += -Ofast
release: all

depend:
	makedepend -- -Iincl -- -Y *.cpp */*.cpp 2>/dev/null  #Spurious warnings about missing system headers.


lsort:$(SERVER_OBJS)
	$(CC) $(SERVER_OBJS) -o lsort -lpthread

agent:$(AGENT_OBJS)
	$(CC) $(AGENT_OBJS) -o agent -lpthread

remote:$(AGENT_OBJS)
	$(CC) $(AGENT_OBJS) -o $(red_binary) -lpthread

%.o:%.cpp
	$(CC) -c $*.cpp  -o $*.o


sanity:
	$(CC)  --analyze -Xanalyzer -analyzer-output=text *.cpp */*.cpp

lint:
	clang-tidy *.cpp */*.cpp -- -std=c++11 -Iincl 

clean:
	rm  -f initiators/*.o handlers/*.o agents/*.o io/*.o mergeSort/*.o threadUtil/*.o common/*.o agent.o lsort.o agent lsort
# DO NOT DELETE

agent.o: incl/localTypes.h incl/serverTypes.h incl/protocol.h
agent.o: incl/globalConfig.h incl/config.h incl/msgHandler.h incl/logger.h
agent.o: incl/tcpUtil.h incl/globalLogger.h
lsort.o: incl/tcpUtil.h incl/serverTypes.h incl/localTypes.h
lsort.o: incl/filePartition.h incl/iterGen.h incl/nwDispatch.h
lsort.o: incl/dispatch.h incl/dispatchIterations.h incl/globalLogger.h
lsort.o: incl/globalConfig.h incl/config.h
common/config.o: incl/config.h
common/dispatchIterations.o: incl/dispatchIterations.h incl/dispatch.h
common/dispatchIterations.o: incl/localTypes.h incl/serverTypes.h
common/dispatchIterations.o: incl/iterGen.h
common/filePartition.o: incl/filePartition.h incl/iterGen.h incl/localTypes.h
common/fileUtil.o: incl/localTypes.h incl/fileUtil.h
common/globalConfig.o: incl/config.h incl/globalConfig.h
common/globalLogger.o: incl/logger.h incl/globalLogger.h
common/logger.o: incl/logger.h
common/protocol.o: incl/protocol.h incl/localTypes.h incl/serverTypes.h
common/protocol.o: incl/tcpUtil.h
common/tcpUtil.o: incl/tcpUtil.h
handlers/msgHandler.o: incl/msgHandler.h incl/logger.h incl/localTypes.h
handlers/msgHandler.o: incl/sortHandler.h incl/threadPool.h incl/dispatch.h
handlers/msgHandler.o: incl/serverTypes.h incl/worker.h
handlers/msgHandler.o: incl/multiMergeHandler.h
handlers/multiMergeHandler.o: incl/localTypes.h incl/bufferedWriter.h
handlers/multiMergeHandler.o: incl/bufferedReader.h incl/multiMergeHandler.h
handlers/multiMergeHandler.o: incl/msgHandler.h incl/logger.h
handlers/multiMergeHandler.o: incl/globalLogger.h
handlers/sortHandler.o: incl/threadPool.h incl/dispatch.h incl/localTypes.h
handlers/sortHandler.o: incl/serverTypes.h incl/worker.h incl/config.h
handlers/sortHandler.o: incl/mergeSort.h incl/sortMerge.h
handlers/sortHandler.o: incl/filePartition.h incl/iterGen.h
handlers/sortHandler.o: incl/sortHandler.h incl/msgHandler.h incl/logger.h
handlers/sortHandler.o: incl/dispatchIterations.h incl/globalLogger.h
handlers/sortHandler.o: incl/globalConfig.h
io/bufferedReader.o: incl/globalLogger.h incl/bufferedReader.h
io/bufferedWriter.o: incl/bufferedWriter.h incl/globalLogger.h
io/nwDispatch.o: incl/nwDispatch.h incl/dispatch.h incl/localTypes.h
io/nwDispatch.o: incl/serverTypes.h incl/tcpUtil.h incl/protocol.h
io/nwDispatch.o: incl/globalLogger.h
mergeSort/merge.o: incl/bufferedReader.h incl/bufferedWriter.h
mergeSort/mergeSort.o: incl/mergeSort.h incl/localTypes.h
mergeSort/mergeSort.o: incl/bufferedReader.h incl/bufferedWriter.h
mergeSort/multiMerge.o: incl/localTypes.h incl/bufferedWriter.h
mergeSort/multiMerge.o: incl/bufferedReader.h incl/multiMergeHandler.h
mergeSort/multiMerge.o: incl/msgHandler.h incl/logger.h incl/globalLogger.h
mergeSort/multipleMerge.o: incl/localTypes.h incl/bufferedWriter.h
mergeSort/multipleMerge.o: incl/bufferedReader.h
regTests/brTest.o: incl/bufferedReader.h
regTests/bwTest.o: incl/bufferedWriter.h
regTests/mergeTest.o: incl/multiMergeHandler.h incl/localTypes.h
regTests/mergeTest.o: incl/msgHandler.h incl/logger.h
regTests/protTest.o: incl/protocol.h incl/localTypes.h incl/serverTypes.h
regTests/threadTest.o: incl/threadPool.h incl/dispatch.h incl/localTypes.h
regTests/threadTest.o: incl/serverTypes.h incl/worker.h
threadUtil/threadPool.o: incl/threadPool.h incl/dispatch.h incl/localTypes.h
threadUtil/threadPool.o: incl/serverTypes.h incl/worker.h incl/mergeSort.h
threadUtil/threadPool.o: incl/globalLogger.h
threadUtil/worker.o: incl/worker.h incl/localTypes.h
