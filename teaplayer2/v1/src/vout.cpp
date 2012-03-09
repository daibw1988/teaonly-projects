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
}

void TeaVideoOutput::Stop() {
    state = VO_STATE_STOPEED;
    
    thread->Clear(this);
    
    if ( BufferedPictures() > 0) {
        talk_base::CritScope lock(&mutex_); 
        while( videoPictureFIFO.size() > 0) {
            VideoPicture * pRender = videoPictureFIFO.front();
            videoPictureFIFO.pop_front();        
            delete pRender;
        }         
    }

}

bool TeaVideoOutput::PushVideoPicture(VideoPicture *newPic) {
    if ( state == VO_STATE_STOPEED ) {
        return false;
    } else {
        talk_base::CritScope lock(&mutex_); 
        videoPictureFIFO.push_back(newPic);
    }
    
    if ( videoPictureFIFO.size() > overFullness ) {
        signalBufferOverflow();
    } 
    return true;
}

void TeaVideoOutput::RenderVideoPicture(VideoPicture *target) {
    delete target;
}

void TeaVideoOutput::OnMessage(talk_base::Message *msg) {
    switch(msg->message_id) {
        case MSG_RENDER_TIMER:
            doRender();
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
        signalBufferUnderflow();
    }
}



