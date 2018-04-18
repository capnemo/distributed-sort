#include "dispatchIterations.h"
#include <iostream>
#include <map>

/***************************************************************
FUNCTION: dispatchIters()
IN ds Dispatch object 
IN iG Iterator generator
IN funcType  Function type
OUT outArgs  Failed tasks

Extracts iterations of a particular work load, dispatches them and 
blocks until all iterations are complete.

****************************************************************/

void dispatchIters(dispatch& ds, iterGen& iG, char funcType, strVec& outArgs)
{
    strVec iter;
    std::map<std::string, std::string> retTable;
    while (iG.getNextIter(iter) == true) {
        std::string tid;
        ds.dispatchTask(funcType, iter, tid);
        retTable.insert({tid, iter[iter.size() - 1]});
        iter.clear();
    }
    
    strVec fails;
    ds.waitForCompletion(fails);

    for (auto mem:fails) 
        retTable.erase(mem);

    for (auto mem:retTable)
        outArgs.push_back(mem.second);
}
