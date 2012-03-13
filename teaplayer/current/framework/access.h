#ifndef _ACCESS_H_
#define _ACCESS_H_

#include <stdio.h>
#include <stdlib.h>

#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"

class TeaAccess : public sigslot::has_slots<> {
public:    
    virtual ~TeaAccess(){};

    virtual bool Open() = 0;
    virtual void Close() = 0;

    // streaming access
    sigslot::signal1<bool> signalBeginOfStream;
    sigslot::signal0<> signalEndOfStream;
    sigslot::signal2<const unsigned char*, size_t> signalData;
};

#endif
