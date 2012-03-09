#include <assert.h>
#include <iostream>
#include "base.h"
#include "access.h"
#include "demux.h"
#include "decode.h"
#include "vout.h"
#include "player.h"

TeaPlayer::TeaPlayer(TeaAccess *a, TeaDemux *d, TeaDecodeTask *dec, TeaVideoOutput *vo):
        access(a),
        demux(d),
        decode(dec),
        vout(vo){
    // building player 
    access->Close();
    demux->Close(); 
    decode->Stop();
    vout->Stop();

    access->signalBeginofStream.connect(this, &TeaPlayer::onAccessBegin);
    access->signalEndOfStream.connect(this, &TeaPlayer::onAccessEnd);
    access->signalData.connect(this, &TeaPlayer::onAccessData);
    demux->signalMediaPacket.connect(this, &TeaPlayer::onMediaPacket);

    state = TP_STOPED;
}

TeaPlayer::~TeaPlayer() {
    
}

void TeaPlayer::Play() {
    state = TP_PLAYING;
    decode->Start();
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
    } else {
        vout->Start();
    }
}

void TeaPlayer::onAccessEnd() {
    access->Close();    
    demux->Close();

    vout->Stop();
}

void TeaPlayer::onAccessData(const unsigned char *p, size_t length) {
    if ( state == TP_STOPED)
       return;

    demux->PushNewData(p, length);    
}


void TeaPlayer::onMediaPacket(MediaPacket *p) {
    decode->PushMediaPacket(p);
}

