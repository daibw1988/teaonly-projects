#include "media.h"
#include "access.h"
#include "demux.h"
#include "decode.h"
#include "vout.h"
#include "aout.h"
#include "player.h"

TeaPlayer::TeaPlayer(TeaAccess *a, TeaDemux *d):
        access(a),
        demux(d){
    // setup basic 
    decodes.clear();
    bool ret = d->open(a); 
    assert(ret == true);

    d->signalMediaData.connect(this, &TeaPlayer::onMediaData);
}

TeaPlayer::~TeaPlayer() {
    
}

TeaPlayer::setDecode(unsigned int n, TeaDecode *d) {
    decodes[n] = d;
}

TeaPlayer::onMediaData(unsigned int n, unsigned char *data, size_t length) {
    TeaDecode *decode = decodes[n];
    assert(decode != NULL);
}


