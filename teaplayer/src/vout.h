#ifndef _VOUT_H_
#define _VOUT_H_

#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"
#include "video.h"

#include "timing.h"

class TeaVideoOutput: public sigslot::has_slots<>{
public:    
    virtual ~TeaVideoOutput() = 0;

    virtual void Start() = 0;
    virtual void Pause() = 0;
    virtual void Stop() = 0;
    virtual void PushNewVideoPicture(VideoPicture *) = 0;

    virtual MediaTime PlayingTime();
    virtual MediaTime BufferedTime();
    virtual unsigned int BufferedPictures();
    virtual MediaTime FirstPictureTime();
    virtual MediaTime LastPictureTime();

    sigslot::signal0<> signalBufferOverflow;
    sigslot::signal0<> signalBufferUnderflow;
};

#endif
