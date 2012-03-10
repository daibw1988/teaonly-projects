#ifndef _VOUT_H_
#define _VOUT_H_

#include <list>
#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"
#include "base.h"

class TeaVideoOutput: public sigslot::has_slots<>, public talk_base::MessageHandler{
public:    
    TeaVideoOutput();
    virtual ~TeaVideoOutput();

    void PushVideoPicture(VideoPicture *);
    VideoPicture *PullVideoPicture();

    MediaTime BufferedLength() {
        talk_base::CritScope lock(&mutex_); 
        if ( videoPictureFIFO.size() >= 2) {
            MediaTime delta = videoPictureFIFO.back()->mt - videoPictureFIFO.front()->mt;
            return delta;
        } 
        return 0;
    }
    unsigned int BufferedPictures() {
        talk_base::CritScope lock(&mutex_); 
        return videoPictureFIFO.size();
    }
    MediaTime FirstPictureTime() {
        talk_base::CritScope lock(&mutex_); 
        return videoPictureFIFO.front()->mt;
    }
    MediaTime LastPictureTime() {
        talk_base::CritScope lock(&mutex_); 
        return videoPictureFIFO.back()->mt;
    }

    virtual void RenderVideoPicture();

protected:
    virtual void OnMessage(talk_base::Message *msg);

private:    
    void doRender();

private:
    enum {
        MSG_DO_RENDER,
    };
    talk_base::Thread *thread;
    std::list<VideoPicture *> videoPictureFIFO;
    talk_base::CriticalSection mutex_;
};

#endif
