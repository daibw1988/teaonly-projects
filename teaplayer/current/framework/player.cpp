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

    timing.fullNess = 1000;
    timing.beginNess = 500;
    timing.mediaTime = BAD_TIME;
    timing.localTime = BAD_TIME; 
    timing.controlTime = BAD_TIME;

    access->signalBeginofStream.connect(this, &TeaPlayer::onAccessBegin);
    access->signalEndOfStream.connect(this, &TeaPlayer::onAccessEnd);
    access->signalData.connect(this, &TeaPlayer::onAccessData);
    demux->signalMediaPacket.connect(this, &TeaPlayer::onMediaPacket);
    decode->signalVideoPicture.connect(this, &TeaPlayer::onVideoPicture);

    state = TP_STOPED;
}

TeaPlayer::~TeaPlayer() {
     
}

void TeaPlayer::Play() {
    state = TP_BUFFERING;
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

int TeaPlayer::totalBufferedVideoPictures() {
    return decode->BufferedPictures() + vout->BufferedPictures(); 
}

MediaTime TeaPlayer::totalBufferedVideoLength() {
    return decode->BufferedVideoLength() + vout->BufferedLength();  
}

void TeaPlayer::onVideoPicture(VideoPicture *newPic) {
    vout->PushVideoPicture(newPic);
}

void TeaPlayer::doControl() {
    // first upatetime
    if ( timing.localTime == BAD_TIME) {
        timing.localTime = CurrentTime();
        timing.controlTime = 0;
        timing.mediaTime = 0;
    }
    MediaTime cur = CurrentTime();
    MediaTime upd = cur - timing.localTime;
    timing.localTime = cur;

    timing.controlTime = timing.controlTime + upd;

    switch( state ) {
        case TP_PLAYING:
            timing.mediaTime = timing.mediaTime + upd;
            break;
        case TP_BUFFERING:
            break;
        case TP_CATCHUP:
            timing.mediaTime = timing.mediaTime + 2*upd;
            break;
        case TP_PAUSED:
            break;
    }

}


