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

    void setDecode(unsigned int n, TeaDecode *d);

private:
    void onMediaPackage(unsigned int n, const unsigned char *p, size_t length);
    void onMediaData(unsigned int n, void *m);

private:
    TeaAccess *access;
    TeaDemux *demux;

    std::map<unsigned int, TeaDecode *> decodes;
};

#endif
