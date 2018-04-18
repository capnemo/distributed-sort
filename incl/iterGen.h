/***************************************************************************************
FILENAME: iterGen.h

DESCRIPTION:
    Abstract class to divide a workload into smaller units.

NOTES:

***************************************************************************************/


#ifndef ITERGEN_H
#define ITERGEN_H

#include "localTypes.h"

class iterGen {
    public:
    virtual bool getNextIter(strVec& iter) = 0;
};

#endif /*ITERGEN_H */
