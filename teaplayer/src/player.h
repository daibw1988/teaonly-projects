#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>
#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"
#include "talk/base/scoped_ptr.h"

class TeaAccess;
class TeaDemux;
class TeaDecode;
class TeaVideoOut;
class TeaAudioOut;
class TeaMedia;

class TeaPlayer {

private:
    talk_base::scped_ptr<TeaMedia> meida;
};

#endif
