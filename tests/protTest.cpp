#include <iostream>
#include "protocol.h"

int main(int argc, char *argv[])
{
    std::string s1 = "T0000 s /tmp/xx/100m 0 1100000000 /tmp/xx/100m.x";
    struct task t;

    decodeTask(s1, t);
    std::cout << t.id << " " << t.type;
    for (auto mem:t.args)
        std::cout << mem << " ";
    std::cout << std::endl;
 
    std::string s2;
    encodeTask(t, s2);
    if (s1 != s2)
        std::cout << "False" << std::endl;
    else
        std::cout << "Succeeded" << std::endl;

    
    std::string s3 = "T0001 0";
    struct result r;

    decodeResult(s3, r);
    std::cout << r.id << " " << r.rc << std::endl;
    std::string s4;
    encodeResult(r, s4);
    if (s3 != s4)
        std::cout << "False" << std::endl;
    else
        std::cout << "Succeeded" << std::endl;

}
