#ifndef _VOUT_H_
#define _VOUT_H_

#include <list>
#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"
#include "base.h"

class TeaVideoOutput: public sigslot::has_slots<>, public talk_base::MessageHandler{
public:    
    TeaVideoOutput(MediaTime, MediaTime);
    virtual ~TeaVideoOutput();

    void Start();
    void Pause();
    void Stop();
    bool PushVideoPicture(VideoPicture *);

    MediaTime PlayedTime() {
        return mediaTime;
    };
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


    sigslot::signal0<> signalBufferOverflow;
    sigslot::signal0<> signalBufferDone;
    sigslot::signal0<> signalBufferUnderflow;

protected:
    virtual void RenderVideoPicture( VideoPicture *target);
    virtual void OnMessage(talk_base::Message *msg);

private:    
    void doRender();
    void doBuffering();
    void doPlaying(); 

private:
    enum {
        MSG_RENDER_TIMER,
    };
    enum {
        VO_STATE_STOPEED,
        VO_STATE_PLAYING,
        VO_STATE_BUFFERING,
        VO_STATE_PAUSED,
    }state;
    
    talk_base::Thread *thread;

    MediaTime beginFullness;
    MediaTime overFullness;
    
    std::list<VideoPicture *> videoPictureFIFO;
    MediaTime   mediaTime;
    talk_base::CriticalSection mutex_;

};

#endif
