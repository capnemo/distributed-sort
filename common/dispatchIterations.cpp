#include "dispatchIterations.h"
#include <iostream>

void dispatchIters(dispatch& ds, iterGen& iG, char fTy, strVec& outArgs)
{
    strVec iter;
    std::map<std::string, std::string> retTable;
    while (iG.getNextIter(iter) == true) {
        std::string tid;
        ds.dispatchTask(fTy, iter, tid);
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
