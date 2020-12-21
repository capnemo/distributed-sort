#include "localTypes.h"

#ifndef BLOCKINTERFACE_H
#define BLOCKINTERFACE_H

class blockInterface {
    public:
    virtual void operator () () = 0;
    virtual ~blockInterface() {}
};

#endif /*BLOCKINTERFACE_H*/
