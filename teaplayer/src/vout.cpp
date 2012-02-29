#include <assert.h>
#include "video.h"
#include "vout.h"

const int RENDER_DELAY = 10;

TeaVideoOutput::TeaVideoOutput(MediaTime overFull, MediaTime beginFull) {
    overFullness = overFull;
    beginFullness = beginFull;

    state = VO_STATE_STOPEED;
    thread = new talk_base::Thread();
    thread->Start();
}

TeaVideoOutput::~TeaVideoOutput() {
        
}

void TeaVideoOutput::Start() {
    state = VO_STATE_BUFFERING;
    mediaTime = 0;
    thread->PostDelayed(RENDER_DELAY, this, MSG_RENDER_TIMER);
}

void TeaVideoOutput::Pause() {
    state = VO_STATE_PAUSED;
    //updateBuffer();
}

void TeaVideoOutput::Stop() {
    state = VO_STATE_STOPEED;
    thread->Post(this, MSG_RENDER_STOP);
}

bool TeaVideoOutput::PushVideoPicture(VideoPicture *) {
    return true;
}

void TeaVideoOutput::OnMessage(talk_base::Message *msg) {
    switch(msg->message_id) {
        case MSG_RENDER_TIMER:
            doRender();
        case MSG_RENDER_STOP:
            doStop();
            break;
    }
}

void TeaVideoOutput::doRender() {
    static MediaTime last= BAD_TIME;
    MediaTime current = currentTime();
    MediaTime delta = current - last;
    last = current;

    switch( state ) {
        case VO_STATE_STOPEED:
            break;
        case VO_STATE_PAUSED:
            thread->PostDelayed(RENDER_DELAY, this, MSG_RENDER_TIMER);
            break;
        case VO_STATE_BUFFERING:
            doBuffering();
            thread->PostDelayed(RENDER_DELAY, this, MSG_RENDER_TIMER);
            break;
        case VO_STATE_PLAYING:
            mediaTime += delta;
            doPlaying();
            thread->PostDelayed(RENDER_DELAY, this, MSG_RENDER_TIMER);
            break;
    }
}

void TeaVideoOutput::doBuffering() {
    assert( state == VO_STATE_BUFFERING);
    if ( BufferedLength() > beginFullness) {
        state = VO_STATE_PLAYING;
        signalBufferDone();
    }
}

void TeaVideoOutput::doPlaying() {
    assert( state == VO_STATE_PLAYING);
    
    VideoPicture *pRender = NULL;
    if ( mediaTime >= FirstPictureTime() ) {
        talk_base::CritScope lock(&mutex_); 
        pRender = videoPictureFIFO.front();
        videoPictureFIFO.pop_front();        
    }
    if ( pRender != NULL) {
        RenderVideoPicture(pRender);       
    }
    
    if ( BufferedLength() <= 0) {
        state = VO_STATE_BUFFERING;
        signalBufferOverflow();
    }
}

void TeaVideoOutput::doStop() {
    
}

