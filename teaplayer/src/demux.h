#ifndef _TEADEMUX_H_
#define _TEADEMUX_H_

#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"

class TeaAccess;
class TeaDemux : public sigslot::has_slots<> {
public:    
    virtual ~TeaDemux(){};
    
    virtual bool Open() = 0;
    virtual void Close() = 0;
    virtual void PushNewData(const unsigned char *data, size_t length) = 0;

    // streaming access
    sigslot::signal3<unsigned int, const unsigned char*, size_t> signalMediaPackage;
};


#endif
