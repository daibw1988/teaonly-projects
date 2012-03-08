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

class TeaPlayer : public sigslot::has_slots<> {
public:
    typedef enum {
        TP_STOPED,
        TP_PLAYING,
        TP_PAUSED,
    }PlayerState;

public:
    TeaPlayer(TeaAccess *a, TeaDemux *d, TeaVideoOutput *vo);
    ~TeaPlayer();

    PlayerState State() {
        return state;
    };
    void Play();
    void Pause();
    void Stop();

private:
    void onAccessBegin(bool isOK);
    void onAccessData(const unsigned char *p, size_t length);
    void onAccessEnd();
    void onMediaPacket(MediaPacket *p);

private:
    TeaAccess *access;
    TeaDemux *demux;
    TeaDecodeTask *decode;
    TeaVideoOutput *vout;

    PlayerState state;
};

#endif
