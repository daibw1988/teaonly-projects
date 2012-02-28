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

    virtual AccessState state() = 0;
    virtual bool open(const std::string &mrl) = 0;
    virtual bool start() = 0;
    virtual bool stop() = 0;
    virtual void close() = 0;

    // streaming access
    sigslot::signal0<> signalEndOfStream;
    sigslot::signal2<const unsigned char*, size_t> signalData;
};

#endif
