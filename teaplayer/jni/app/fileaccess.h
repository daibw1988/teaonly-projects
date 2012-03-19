#ifndef _FILEACCESS_H_
#define _FILEACCESS_H_

#include "access.h"
#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"

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


