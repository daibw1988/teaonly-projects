#include <assert.h>
#include "media.h"
#include "access.h"
#include "demux.h"
#include "decode.h"
#include "vout.h"
#include "aout.h"
#include "video.h"
#include "player.h"

TeaPlayer::TeaPlayer(TeaAccess *a, TeaDemux *d, TeaVideoOutput *vo):
        access(a),
        demux(d),
        vout(vo){
    // building player 
    access->Close();
    demux->Close(); 
    decodes.clear();
    vout->Stop();
    
    access->signalBeginofStream.connect(this, &TeaPlayer::onAccessBegin);
    access->signalEndOfStream.connect(this, &TeaPlayer::onAccessEnd);
    demux->signalMediaPackage.connect(this, &TeaPlayer::onMediaPackage);

    state = TP_STOPED;
}

TeaPlayer::~TeaPlayer() {
    
}

void TeaPlayer::setDecode(unsigned int n, TeaDecode *d) {
    decodes[n] = d;
    d->signalMediaData.connect(this, &TeaPlayer::onMediaData);
    d->Close();
}

void TeaPlayer::Play() {
    for(std::map<unsigned int, TeaDecode *>::iterator i=decodes.begin(); i != decodes.end(); i++) {
        (*i).second->Open();
    } 
    demux->Open();
    access->Open();
}

void TeaPlayer::Pause() {

}

void TeaPlayer::Stop() {

}

void TeaPlayer::onAccessBegin(bool isOK) {
    if ( isOK == false) {
        access->Close();    
        demux->Close();

        for(std::map<unsigned int, TeaDecode *>::iterator i=decodes.begin(); i != decodes.end(); i++) {
            (*i).second->Close();
        } 
    } else {
        vout->Start();
    }
}

void TeaPlayer::onAccessEnd() {
    access->Close();    
    demux->Close();

    for(std::map<unsigned int, TeaDecode *>::iterator i=decodes.begin(); i != decodes.end(); i++) {
        (*i).second->Close();
    } 
    
    vout->Stop();
}

void TeaPlayer::onAccessData(const unsigned char *p, size_t length) {
    if ( state == TP_STOPED)
       return;

    demux->PushNewData(p, length);    
}


void TeaPlayer::onMediaPackage(unsigned int n, const unsigned char *p, size_t length) {
    if ( state == TP_STOPED)
        return;

    TeaDecode *decode = decodes[n];
    assert(decode != NULL);
    
    decode->PushNewPacket(p, length);
}

void TeaPlayer::onMediaData(unsigned int n, void *m) {
    TeaDecode *decode = decodes[n];
    assert(decode != NULL);

    if ( decode->type == CODEC_TYPE_VIDEO) {
        VideoPicture *p = (VideoPicture *)m;
        if ( ! vout->PushVideoPicture(p) )
            delete p;
    }
}

