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

    access->signalBeginofStream.connect(this, &TeaPlayer::onAccessBegin);
    access->signalEndOfStream.connect(this, &TeaPlayer::onAccessEnd);
    access->signalData.connect(this, &TeaPlayer::onAccessData);
    demux->signalMediaPacket.connect(this, &TeaPlayer::onMediaPacket);

    state = TP_STOPED;
    mediaTime = BAD_TIME;
}

TeaPlayer::~TeaPlayer() {
    
}

void TeaPlayer::Play() {
    state = TP_PLAYING;
    mediaTime = 0;
    thread->Post(this, MSG_CONTROL_TIMER);

    decode->Start();
    demux->Open();
    access->Open();
}

void TeaPlayer::Pause() {
    
}

void TeaPlayer::Stop() {
        
}

void TeaPlayer::OnMessage(talk_base::Message *msg) {
    switch(msg->message_id) {
        case MSG_CONTROL_TIMER:
            doControl();
            break;
    }
}

void TeaPlayer::onAccessBegin(bool isOK) {
    if ( isOK == false) {
        access->Close();    
        demux->Close();
    } 
}

void TeaPlayer::onAccessEnd() {
    access->Close();    
    demux->Close();

}

void TeaPlayer::onAccessData(const unsigned char *p, size_t length) {
    if ( state == TP_STOPED)
       return;

    demux->PushNewData(p, length);    
}

void TeaPlayer::onMediaPacket(MediaPacket *p) {
    decode->PushMediaPacket(p);
}

void TeaPlayer::doControl() {
    
}


