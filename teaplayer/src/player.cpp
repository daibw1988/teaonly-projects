#include <assert.h>
#include "media.h"
#include "access.h"
#include "demux.h"
#include "decode.h"
#include "vout.h"
#include "aout.h"
#include "video.h"
#include "player.h"

TeaPlayer::TeaPlayer(TeaAccess *a, TeaDemux *d):
        access(a),
        demux(d){
    // setup basic 
    decodes.clear();
    bool ret = d->open(a); 
    assert(ret == true);

    d->signalMediaPackage.connect(this, &TeaPlayer::onMediaPackage);
}

TeaPlayer::~TeaPlayer() {
    
}

void TeaPlayer::setDecode(unsigned int n, TeaDecode *d) {
    decodes[n] = d;
    d->signalMediaData.connect(this, &TeaPlayer::onMediaData);
}

void TeaPlayer::onMediaPackage(unsigned int n, const unsigned char *p, size_t length) {
    TeaDecode *decode = decodes[n];
    assert(decode != NULL);
    
    decode->PushNewPacket(p, length);
}

void TeaPlayer::onMediaData(unsigned int n, void *m) {
    TeaDecode *decode = decodes[n];
    assert(decode != NULL);

    if ( decode->type == CODEC_TYPE_VIDEO) {
        VideoPicture *p = (VideoPicture *)m;
        //vout->pushNewPicture();
    }
}


