#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <map>
#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"

class TeaAccess;
class TeaDemux;
class TeaDecode;

class TeaPlayer : public sigslot::has_slots<> {
public:
    TeaPlayer(TeaAccess *a, TeaDemux *d);
    ~TeaPlayer();

    setDecode(unsigned int n, TeaDecode *d);

private:
    onMediaData(unsigned int n, unsigned char *data, size_t);

private:
    TeaAccess *access;
    TeaDecode *demux;

    std::map<unsigned int, TeaDecode *> decodes;
};

#endif
