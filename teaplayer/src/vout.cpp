#include "video.h"
#include "vout.h"

TeaVideoOutput::TeaVideoOutput(MediaTime overFull, MediaTime underFull) {
    overFullness = overFull;
    underFullness = underFull;

    systemTime = (MediaTime)(-1);
    mediaTime = (MediaTime)(-1);
    
    isPlaying = false;
    thread = new talk_base::Thread();
    thread->Start();
}

TeaVideoOutput::~TeaVideoOutput() {
        
}

TeaVideoOutput::Start() {
    isPlaying = true;
    thread->PostDelayed(10, this, MSG_RENDER_TIMER);
}

TeaVideoOutput::Pause() {
    isPlaying = false;
}

TeaVideoOutput::Stop() {
    isPlaying = false;
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


