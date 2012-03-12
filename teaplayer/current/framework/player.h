#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <map>
#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"
#include "base.h"

class TeaAccess;
class TeaDemux;
class TeaDecodeTask;
class TeaVideoOutput;
class MediaPacket;
class TeaMedia;


class TeaPlayer : public sigslot::has_slots<>, talk_base::MessageHandler{
public:
    typedef enum {
        TP_STOPED,
        TP_PLAYING,
        TP_PAUSED,
        TP_BUFFERING,
        TP_CATCHUP,
    }PlayerState;

public:
    TeaPlayer(TeaAccess *a, TeaDemux *d, TeaDecodeTask *dec, TeaVideoOutput *vo);
    ~TeaPlayer();

    PlayerState State() {
        return state;
    };
    void Play();
    void Pause();
    void Stop();

protected:
    void OnMessage(talk_base::Message *msg);    

private:
    void onAccessBegin(bool isOK);
    void onAccessData(const unsigned char *p, size_t length);
    void onAccessEnd();
    void onMediaPacket(MediaPacket *p);
    void onVideoPicture(VideoPicture *p);
    int totalBufferedVideoPictures();
    MediaTime totalBufferedVideoLength();
    void doControl();


private:
    enum {
        MSG_CONTROL_TIMER,
    };
    TeaAccess *access;
    TeaDemux *demux;
    TeaDecodeTask *decode;
    TeaVideoOutput *vout;
    PlayerState state;
    
    struct{
        unsigned int overNess;
        unsigned int fullNess;
        unsigned int beginNess;
        MediaTime controlTime;
        MediaTime lastDelay;
        MediaTime mediaTime;
        MediaTime localTime;
    }timing;

    talk_base::Thread *thread;
};

#endif
