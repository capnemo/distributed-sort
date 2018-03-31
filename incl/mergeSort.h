#ifndef MERGESORT_H
#define MERGESORT_H

#include "localTypes.h"

namespace mergeSort {
    int sortBlock(const strVec& args);
    int mergeBlocks(const strVec& args);
    int mergeFiles(const strVec& args);
}

#endif /* MERGESORT_H */
