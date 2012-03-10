#ifndef _TEADECODE_H_
#define _TEADECODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <list>

#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"

#include "base.h"

class TeaDemux;
class TeaDecodeTask : public sigslot::has_slots<> , talk_base::MessageHandler{
public:    
    TeaDecodeTask(TeaDemux *dm);
    ~TeaDecodeTask();

    void Start();
    void Pause();
    void Resume();
    void Stop();

    void PushMediaPacket(MediaPacket *pkt);

    MediaTime BufferedVideoLength() {
        talk_base::CritScope lock(&mutex_); 
        if ( videoBuffer.size() >= 2) {
            MediaTime delta = videoBuffer.back()->pts - videoBuffer.front()->pts;
            return delta;
        } 
        return 0;
    }
    unsigned int BufferedPictures() {
        talk_base::CritScope lock(&mutex_); 
        return videoBuffer.size();
    }
    MediaTime FirstPictureTime() {
        talk_base::CritScope lock(&mutex_); 
        return videoBuffer.front()->pts;
    }
    MediaTime LastPictureTime() {
        talk_base::CritScope lock(&mutex_); 
        return videoBuffer.back()->pts;
    }

    sigslot::signal1<VideoPicture *> signalVideoPicture;

protected:
    void OnMessage(talk_base::Message *msg);

private:
    enum {
        ST_STOPPED,
        ST_DECODING,
        ST_PAUSED,
    }state;

    enum {
        MSG_START_DECODE
    };
    void doDecode();

    std::list<MediaPacket *> videoBuffer;
    
    pthread_cond_t  control_cond;                                         
    pthread_mutex_t control_mutex;
    talk_base::CriticalSection mutex_;
    talk_base::Thread *thread; 

    TeaDemux *demux;
};

#endif
