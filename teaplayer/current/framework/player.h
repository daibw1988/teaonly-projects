#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <map>
#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"

class TeaAccess;
class TeaDemux;
class TeaDecodeTask;
class TeaVideoOutput;
class MediaPacket;
class TeaMedia;

class TeaPlayer : public sigslot::has_slots<>, public talk_base::MessageHandler{
public:
    typedef enum {
        TP_STOPED,
        TP_PLAYING,
        TP_PAUSED,
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
    MediaTime   mediaTime;
    MediaTime   localTime;

    talk_base::Thread *thread;
};

#endif
