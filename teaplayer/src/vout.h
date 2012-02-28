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

    MediaTime PlayingTime();
    MediaTime BufferedTime();
    unsigned int BufferedPictures();
    MediaTime FirstPictureTime();
    MediaTime LastPictureTime();

    sigslot::signal0<> signalBufferOverflow;
    sigslot::signal0<> signalBufferUnderflow;

protected:
    virtual void OnMessage(talk_base::Message *msg);
    virtual void PopVideoPicture() = 0;

protected:
    enum {
        MSG_RENDER_TIMER,
        MSG_RENDER_STOP,
    };
    bool isPlaying; 
    talk_base::Thread *thread;

    MediaTime underFullness;
    MediaTime overFullness;
    
    std::list<VideoPicture *> videoPictureFIFO;
    MediaTime   systemTime;
    MediaTime   mediaTime;
};

#endif
