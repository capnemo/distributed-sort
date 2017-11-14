
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "localTypes.h"

namespace fileUtil 
{
bool splitFileBySize(const std::string& fileName, uint64_t start, uint64_t end,
                     uint64_t size, offPairVec& segments);
}
