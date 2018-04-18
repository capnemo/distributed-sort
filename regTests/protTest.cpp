/***************************************************************************************
FILENAME: protTest.cpp

DESCRIPTION:
    Test of the protocol layer. No network code involved.

NOTES:

***************************************************************************************/


#include <iostream>
#include "protocol.h"

int main(int argc, char *argv[])
{
    std::string s1 = "T0000 s /tmp/xx/100m 0 1100000000 /tmp/xx/100m.x";
    struct task t;

    protocol::decodeTask(s1, t);
#if 0
    std::cout << t.id << " " << t.type;
    for (auto mem:t.args)
        std::cout << mem << " ";
    std::cout << std::endl;
#endif

 
    std::string s2;
    protocol::encodeTask(t, s2);
    if (s1 != s2)
        std::cout << "Encode test failed" << std::endl;
    else
        std::cout << "Encode test Succeeded" << std::endl;

    
    std::string s3 = "T0001 0";
    struct result r;

    protocol::decodeResult(s3, r);
    //std::cout << r.id << " " << r.rc << std::endl;
    std::string s4;
    protocol::encodeResult(r, s4);
    if (s3 != s4)
        std::cout << "Decode test failed" << std::endl;
    else
        std::cout << "Decode test Succeeded" << std::endl;

}
