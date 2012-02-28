#ifndef _TEADEMUX_H_
#define _TEADEMUX_H_

#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"

class TeaAccess;
class TeaDemux : public sigslot::has_slots<> {
public:    
    virtual ~TeaDemux(){};
    
    virtual bool open(TeaAccess *) = 0;

    // streaming access
    sigslot::signal3<unsigned int, const unsigned char*, size_t> signalMediaPackage;
};


#endif
