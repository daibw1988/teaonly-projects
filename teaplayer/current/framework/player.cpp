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
    decode->Reset();

    state = TP_STOPED;
    timing.jitterDelay = 2000;
    timing.mediaTime = BAD_TIME;
    timing.localTime = BAD_TIME; 
    thread = new talk_base::Thread();
    thread->Start();


    access->signalBeginofStream.connect(this, &TeaPlayer::onAccessBegin);
    access->signalEndOfStream.connect(this, &TeaPlayer::onAccessEnd);
    access->signalData.connect(this, &TeaPlayer::onAccessData);
    demux->signalProbed.connect(this, &TeaPlayer::onDemuxProbed);
    demux->signalMediaPacket.connect(this, &TeaPlayer::onMediaPacket);
    decode->signalVideoPicture.connect(this, &TeaPlayer::onVideoPicture);
    vout->signalPictureRendered.connect(this, &TeaPlayer::onPictureRendered);
    
}

TeaPlayer::~TeaPlayer() {
     
}

void TeaPlayer::Play() {
    state = TP_OPENIGN;

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

void TeaPlayer::onDemuxProbed(bool isOK) {
    if ( isOK) {
        state = TP_BUFFERING;
        thread->Post(this, MSG_CONTROL_TIMER);
    }
}

void TeaPlayer::onMediaPacket(MediaPacket *p) {    
    decode->PushMediaPacket(p);
}

void TeaPlayer::onVideoPicture(VideoPicture *newPic) {
    vout->RenderVideoPicture(newPic);
}

void TeaPlayer::onPictureRendered() {
    // syncing for audio
}

void TeaPlayer::doControl() {
    if ( timing.localTime == BAD_TIME) {
        timing.localTime = CurrentTime();
        timing.mediaTime = 0;
    }
    MediaTime cur = CurrentTime();
    MediaTime upd = cur - timing.localTime;
    timing.localTime = cur;

    switch( state ) {
        case TP_PLAYING:
            timing.mediaTime = timing.mediaTime + upd;
            doPlay();
            break;
        case TP_BUFFERING:
            doBuffering();
            break;
        case TP_PAUSED:
            doPause();
            break;
    }

    printf("MediaTime = %lld\n", timing.mediaTime);
    if ( state != TP_STOPED)
        thread->PostDelayed(5, this, MSG_CONTROL_TIMER);
}

void TeaPlayer::doBuffering() {
    if ( decode->BufferedVideoLength() >= timing.jitterDelay ) {
        state = TP_PLAYING;
    }
}

void TeaPlayer::doPlay() {
    if ( decode->BufferedPictures() >= 1) {
        if ( decode->LastPictureTime() > timing.mediaTime ) {
            decode->DecodeVideo( timing.mediaTime );
        }
    }
}

void TeaPlayer::doPause() {

}
