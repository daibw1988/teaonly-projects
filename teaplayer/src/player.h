#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>
#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"

class TeaAccess;
class TeaDemux;
class TeaDecode;
class TeaVideoOut;
class TeaAudioOut;
class TeaMedia;

class TeaPlayer {
public:
    TeaPlayer();
    ~TeaPlayer();

private:
    TeaMedia *media;

    TeaAccess *access;
    TeaDecode *demux;
    std::vector<TeaDecode *> decodes;
    TeaVideoOut *vout;
    TeaAudioOut *aout;
};

#endif
