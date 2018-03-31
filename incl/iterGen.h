#ifndef ITERGEN_H
#define ITERGEN_H

#include "localTypes.h"

class iterGen {
    public:
    virtual bool getNextIter(strVec& iter) = 0;
};

#endif /*ITERGEN_H */
