#ifndef _ACCESS_H_
#define _ACCESS_H_

#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"

class TeaAccess : public sigslot::has_slots<> {
public:
    typedef enum {
        AST_UNPREPARED,
        AST_OPENED,
        AST_STREAMING,
        AST_PAUSED,
    }AccessState;
public:    
    virtual ~TeaAccess(){};

    virtual AccessState State() = 0;
    virtual bool Open() = 0;
    virtual bool Start() = 0;
    virtual bool Stop() = 0;
    virtual void Close() = 0;

    // streaming access
    sigslot::signal1<bool> signalBeginofStream;
    sigslot::signal0<> signalEndOfStream;
    sigslot::signal2<const unsigned char*, size_t> signalData;
};

#endif
