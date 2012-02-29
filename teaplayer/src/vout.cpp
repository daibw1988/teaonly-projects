#include "video.h"
#include "vout.h"

TeaVideoOutput::TeaVideoOutput(MediaTime overFull, MediaTime beginFull) {
    overFullness = overFull;
    beginFullness = beginFull;

    state = VO_STATE_STOPEED;
    thread = new talk_base::Thread();
    thread->Start();
}

TeaVideoOutput::~TeaVideoOutput() {
        
}

TeaVideoOutput::Start() {
    state = VO_STATE_BUFFERING;
    mediaTime = 0;
    thread->PostDelayed(RENDER_DELAY, this, MSG_RENDER_TIMER);
}

TeaVideoOutput::Pause() {
    state = VO_STATE_PAUSED;
    //updateBuffer();
}

TeaVideoOutput::Stop() {
    state = VO_STATE_STOPEED;
    thread->Post(this, MSG_RENDER_STOP);
}

TeaVideoOutput::OnMessage(talk_base::Message *msg) {
    switch(msg->message_id) {
        case MSG_RENDER_TIMER:
            doRender();
        case MSG_RENDER_STOP:
            doStop();
            break;
    }
}

TeaVideoOutput::doRender() {
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
            thread->PostDelayed(RenderTimerDelay, this, MSG_RENDER_TIMER);
            break;
        case VO_STATE_PLAYING:
            mediaTime += delta;
            doPlaying();
            thread->PostDelayed(RENDER_DELAY, this, MSG_RENDER_TIMER);
            break;
    }
}


TeaVideoOutput::doBuffering() {
    asset( state == VO_STATE_BUFFERING);
    if ( BufferedLength() > beginFullness) {
        state = VO_STATE_PLAYING;
        signalBufferDone();
    }
}

TeaVideoOutput::doPlaying() {
    asset( state == VO_STATE_PLAYING);
    
    if ( mediaTime >= FirstPictureTime() ) {
    
    }

    if ( BufferedLength() > beginFullness) {
        state = VO_STATE_PLAYING;
        signalBufferDone();
    }
}

