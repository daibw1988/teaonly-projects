#ifndef _TEADECODE_H_
#define _TEADECODE_H_

#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"

class TeaDecode : public sigslot::has_slots<> {
public:    
    virtual ~TeaDecode(){};
    
    virtual bool Open() = 0;
    virtual void Close() = 0;
    virtual void PushPacket(unsigned char *data, size_t length);

    // streaming access
    sigslot::signal3<unsigned int channel, const unsigned char*, size_t> signalMediaData;
};


#endif
