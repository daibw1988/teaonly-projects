#include <assert.h>
#include <iostream>
#include "base.h"
#include "access.h"
#include "demux.h"
#include "decode.h"
#include "vout.h"
#include "player.h"

TeaPlayer::TeaPlayer(TeaAccess *a, TeaDemux *d, TeaVideoOutput *vo):
        access(a),
        demux(d),
        vout(vo){
    
    decode = new TeaDecodeTask(demux);

    state = TP_STOPED;
    timing.jitterDelay = 2000;
    timing.mediaTime = BAD_TIME;
    timing.localTime = BAD_TIME; 
    thread = new talk_base::Thread();
    thread->Start();

    access->signalBeginOfStream.connect(this, &TeaPlayer::onAccessBegin);
    access->signalEndOfStream.connect(this, &TeaPlayer::onAccessEnd);
    access->signalData.connect(this, &TeaPlayer::onAccessData);
    demux->signalProbed.connect(this, &TeaPlayer::onDemuxProbed);
    demux->signalMediaPacket.connect(this, &TeaPlayer::onMediaPacket);
    decode->signalVideoPicture.connect(this, &TeaPlayer::onVideoPicture);
    vout->signalPictureRendered.connect(this, &TeaPlayer::onPictureRendered);    
}

TeaPlayer::~TeaPlayer() {
    thread->Stop();
    
    delete thread;
    delete decode;
}

void TeaPlayer::Play() {
    state = TP_OPENIGN;

    demux->Open();
    access->Open();
}   

void TeaPlayer::Stop() {
    thread->Post(this, MSG_STOP_PLAY);
}

void TeaPlayer::OnMessage(talk_base::Message *msg) {
    switch(msg->message_id) {
        case MSG_CONTROL_TIMER:
            doControl();
            break;
        case MSG_STOP_PLAY:
            doStop();
            break;
    }
}

void TeaPlayer::onAccessBegin(bool isOK) {
    if ( isOK == false) {
        Stop();
    }
}

void TeaPlayer::onAccessEnd() {
    Stop();
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
    } else {
        printf("onProbe failed!\n");
        Stop();
    }
}

void TeaPlayer::onMediaPacket(MediaPacket *p) {    
    decode->PushMediaPacket(p);
}

void TeaPlayer::onVideoPicture(VideoPicture *newPic) {
    printf("KAKA\n");
    vout->RenderVideoPicture(newPic);
}

void TeaPlayer::onPictureRendered() {
    // syncing for audio
}

void TeaPlayer::doStop() {
    access->Close();
    demux->Close();

    decode->Reset();

    state = TP_STOPED;    
    thread->Clear(this);
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
    }

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

