#include <assert.h>
#include "vout.h"

TeaVideoOutput::TeaVideoOutput() {
    thread = new talk_base::Thread();
    thread->Start();
}

TeaVideoOutput::~TeaVideoOutput() {
        
}

void TeaVideoOutput::PushVideoPicture(VideoPicture *newPic) {
    talk_base::CritScope lock(&mutex_); 
    videoPictureFIFO.push_back(newPic);
}

void TeaVideoOutput::RenderVideoPicture() {
    thread->Post(this, MSG_DO_RENDER);
}

void TeaVideoOutput::OnMessage(talk_base::Message *msg) {
    switch(msg->message_id) {
        case MSG_DO_RENDER:
            doRender();
    }
}

void TeaVideoOutput::doRender() {
    //  Just do render
    
}


