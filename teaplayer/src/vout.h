#ifndef _VOUT_H_
#define _VOUT_H_

#include <list>
#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"
#include "timing.h"

struct VideoPicture;

class TeaVideoOutput: public sigslot::has_slots<>, public talk_base::MessageHandler{
public:    
    TeaVideoOutput(MediaTime, MediaTime);
    virtual ~TeaVideoOutput();

    void Start();
    void Pause();
    void Stop();
    void PushVideoPicture(VideoPicture *);

    MediaTime PlayedTime() {
        return mediaTime;
    };
    MediaTime BufferedLength();
    unsigned int BufferedPictures();
    MediaTime FirstPictureTime();
    MediaTime LastPictureTime();

    sigslot::signal0<> signalBufferOverflow;
    sigslot::signal0<> signalBufferDone;
    sigslot::signal0<> signalBufferUnderflow;

protected:
    virtual void PopVideoPicture() = 0;
    virtual void OnMessage(talk_base::Message *msg);

private:    
    void doRender();
    void doStop();
    void resetTiming();
    void resetBuffer();
    

private:
    enum {
        MSG_RENDER_TIMER,
        MSG_RENDER_STOP,
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

    const int RENDER_DELAY = 10;
};

#endif
