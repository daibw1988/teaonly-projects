#ifndef _ACCESS_H_
#define _ACCESS_H_

#include <string>
#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"

class TeaAccess :public MessageHandler, public sigslot::has_slots<> {
public:
    enum {
        BLOCKED_ACCESS = 0,
        STREAMED_ACCESS
    }AccessType;

public:    
    virtual ~TeaAccess();

    virtual bool open(const std::string &mrl) = 0;
    virtual void close() = 0;
    
    // block based access
    virtual int read(unsigned char *, size_t ) = 0;
    virtual int seek(uint64_t ) = 0;
    
    // streaming access
    talk_base::sigslot0<> signalEndOfStream;
    talk_base::sigslot2<const unsigned char*, size_t length> signalData;

public: 
    AccessType type;
};

