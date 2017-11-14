
SRCS := ../io/bufferedReader.cpp ../io/bufferedWriter.cpp multipleMerge.cpp

mmerge:$(SRCS)
	clang++ -std=c++11 -ggdb -I../incl $(SRCS) -o mmerge -lpthread
