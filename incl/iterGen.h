#include "localTypes.h"

#ifndef ITERGEN_H
#define ITERGEN_H

class iterGen {
    public:
    virtual bool getNextIter(strVec& iter) = 0;
};

#endif /*ITERGEN_H */
