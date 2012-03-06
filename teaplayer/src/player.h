#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <map>
#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"

class TeaAccess;
class TeaDemux;
class TeaDecode;
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

    void setDecode(unsigned int n, TeaDecode *d);
   
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
    void onMediaPacket(unsigned int n, MediaPacket *p);
    void onMediaData(unsigned int n, void *m);
    

private:
    TeaAccess *access;
    TeaDemux *demux;
    TeaVideoOutput *vout;

    PlayerState state;
};

#endif
