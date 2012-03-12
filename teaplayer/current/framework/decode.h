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
    
    void Reset();
    void PushMediaPacket(MediaPacket *pkt);
    void DecodeVideo(MediaTime target);
    sigslot::signal1<VideoPicture *> signalVideoPicture;

    MediaTime BufferedVideoLength() {
        talk_base::CritScope lock(&vb_mutex_); 
        if ( videoBuffer.size() >= 2) {
            MediaTime delta = videoBuffer.back()->pts - videoBuffer.front()->pts;
            return delta;
        } 
        return 0;
    }
    unsigned int BufferedPictures() {
        talk_base::CritScope lock(&vb_mutex_); 
        return videoBuffer.size();
    }
    MediaTime FirstPictureTime() {
        talk_base::CritScope lock(&vb_mutex_); 
        return videoBuffer.front()->pts;
    }
    MediaTime LastPictureTime() {
        talk_base::CritScope lock(&vb_mutex_); 
        return videoBuffer.back()->pts;
    }

protected:
    void OnMessage(talk_base::Message *msg);

private:
    enum {
        MSG_DECODE_VIDEO,
    };
    MediaTime targetVideoTime;
    void doDecodeVideo();

    std::list<MediaPacket *> videoBuffer;
    talk_base::CriticalSection vb_mutex_;
    talk_base::CriticalSection vd_mutex_;
    talk_base::Thread *thread; 
    TeaDemux *demux;
};

#endif
