#include "localTypes.h"
#include "blockInterface.h"

#ifndef BLOCKSORT_H
#define BLOCKSORT_H

class blockSort:public blockInterface {
    public:
    blockSort(const strVec& argsVec, int tid):args(argsVec),id(tid) {}
    virtual void operator () ();
    void getRecords(charPtrVec& rv);
    void freeBuffer();
    virtual ~blockSort();

    private:
    int sort();

    private:
    strVec args;
    int id;
    charPtrVec recVec;
    char *buffer = nullptr;
};

#endif /*BLOCKSORT_H*/
