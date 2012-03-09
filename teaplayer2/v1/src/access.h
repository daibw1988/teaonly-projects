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
    sigslot::signal1<bool> signalBeginofStream;
    sigslot::signal0<> signalEndOfStream;
    sigslot::signal2<const unsigned char*, size_t> signalData;
};

class FileAccess: public TeaAccess, talk_base::MessageHandler {
public:
    FileAccess(const std::string);
    virtual ~FileAccess();
    
    virtual bool Open();
    virtual void Close();

protected:
    virtual void OnMessage(talk_base::Message *msg);

private:
    enum {
        MSG_ACCESS_START,
        MSG_ACCESS_TIMER,
        MSG_ACCESS_STOP,
    };
    void doAccess();

    FILE *mediaFile;
    std::string mediaPath;
    talk_base::Thread *thread;
};

#endif
