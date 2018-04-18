/***************************************************************************************
FILENAME: dispatchIterations.h

DESCRIPTION:
    Header for functions that divides an unit workload into smaller workloads and manages
    the dispatch and returns.

NOTES:
    Should be a part of the dispatch abstract class.

***************************************************************************************/ 


#ifndef DISPATCHITERATIONS_H
#define DISPATCHITERATIONS_H

#include "dispatch.h"
#include "iterGen.h"

void dispatchIters(dispatch& ds, iterGen& iG, char fTy, strVec& outArgs);

#endif /*DISPATCHITERATIONS_H*/
