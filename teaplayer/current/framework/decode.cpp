#include <assert.h>
#include <iterator>
#include "decode.h"
#include "demux.h"

TeaDecodeTask::TeaDecodeTask(TeaDemux *dm) {
    demux = dm;
    
    videoBuffer.clear();

    thread = new talk_base::Thread();
    thread->Start();
}

TeaDecodeTask::~TeaDecodeTask() {
    // TODO
}

void TeaDecodeTask::Reset() {
    //cleaning avbuffer,
    talk_base::CritScope lock(&vb_mutex_); 
    for( std::list<MediaPacket *>::iterator i = videoBuffer.begin(); 
            i != videoBuffer.end(); i++) {
        MediaPacket *pkt = *i;
        delete pkt;
    }
    videoBuffer.clear();
}

void TeaDecodeTask::PushMediaPacket(MediaPacket *pkt) {
    if ( pkt->type != TEACODEC_TYPE_VIDEO )
        return;
    talk_base::CritScope lock(&vb_mutex_); 
    videoBuffer.push_back(pkt);
}

void TeaDecodeTask::DecodeVideo(MediaTime target) {
    if ( vd_mutex_.TryEnter() ) {
        targetVideoTime = target;
        thread->Post(this, MSG_DECODE_VIDEO);
    }
}

void TeaDecodeTask::OnMessage(talk_base::Message *msg) {
    switch( msg->message_id) {
        case MSG_DECODE_VIDEO:
            doDecodeVideo();
            break; 
    }
}

void TeaDecodeTask::doDecodeVideo() {
    
    while(1) {
        MediaPacket *target = NULL;
        {
            talk_base::CritScope lock(&vb_mutex_); 
            if ( videoBuffer.size() > 0) {
                target = videoBuffer.front();
                if ( target->pts > targetVideoTime)
                    break;
                videoBuffer.pop_front();
            } else {
                break;
            } 
        }


        TeaDecoder *dec = demux->decoders[ target->channel];
        assert(dec != NULL);
        assert(dec->type == TEACODEC_TYPE_VIDEO);

        VideoPicture *pic = dec->DecodeVideoPacket( target );
        printf("Decoding one frame!\n");
        if ( pic != NULL) 
            signalVideoPicture(pic);
    }

    vd_mutex_.Leave();
}
