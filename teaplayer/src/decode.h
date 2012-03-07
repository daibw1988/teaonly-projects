#ifndef _DECODE_H_
#define _DECODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <list>

#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"
#include "codec.h"

class TeaDecodeTask : public sigslot::has_slots<> {
public:    
    virtual ~TeaDecodeTask(){};

    void Reset();
    void PushMediaPackage();
    
    sigslot::signal0<> signalBufferFull;
private:
    std::list<MediaPacket *> videoBuffer;
    std::list<MediaPacket *> audioBuffer;
};

#endif
