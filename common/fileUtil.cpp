
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "localTypes.h"
#include "fileUtil.h"

namespace fileUtil {

/***************************************************************
FUNCTION: splitFileBySize
IN: fileName Input file name
IN: start Start of the file segment to be split
IN: end End of the file segment to be split
IN: size Split size.
IN: segments Number of segments.

****************************************************************/

bool splitFileBySize(const std::string& fileName, uint64_t start, 
                     uint64_t end, uint64_t size, offPairVec& segments)
{
    std::ifstream inStr(fileName);
    if (inStr.is_open() == false) 
        return false;

    uint64_t begin = start;
    while (begin < end)  {
        uint64_t last = begin + size;
        if (last > end)
            last = end;
        inStr.seekg(last);
        if (last < end) {
            int c;
            while ((c = inStr.get()) != '\n') 
                last = inStr.tellg();
            last++;
         }
         segments.push_back({begin, last});
         begin = last;
    }

    return true;
}

}
