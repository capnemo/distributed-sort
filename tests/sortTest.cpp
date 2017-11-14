#include "mergeSort.h"
#include "localTypes.h"
#include "worker.h"
#include "threadPool.h"

int main(int argc, char *argv[])
{
    strVec x1 = {"/work/data/1m", "0", "11000000", "/tmp/1m.out.1"};
    strVec x2 = {"/work/data/1m", "0", "11000000", "/tmp/1m.out.2"};
    worker w;
    subTask t1 = {10, sortBlock, x1, -1};
    subTask t2 = {10, sortBlock, x2, -1};
    threadPool tp(1);
    tp.addToPool(t1);
    tp.addToPool(t2);
    tp.stopAllThreads();
}
