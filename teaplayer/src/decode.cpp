#include <assert.h>
#include <iterator>
#include "decode.h"
#include "demux.h"

TeaDecodeTask::TeaDecodeTask(TeaDemux *dm) {
    demux = dm;
    
    pthread_mutex_init(&avbuffer_locker, NULL);
    pthread_mutex_init(&control_mutex, NULL);
    pthread_cond_init(&control_cond, NULL); 
    
    videoBuffer.clear();
    state = ST_STOPPED;

    thread = new talk_base::Thread();
    thread->Start();
}

TeaDecodeTask::~TeaDecodeTask() {
    // TODO
}

void TeaDecodeTask::Start() {
    assert( state == ST_STOPPED);
    state = ST_DECODING;
    thread->Post(this, MSG_START_DECODE);
}

void TeaDecodeTask::Stop() {
    state = ST_STOPPED;
    pthread_cond_signal(&control_cond);
}

void TeaDecodeTask::Pause() {
    state = ST_PAUSED;
}

void TeaDecodeTask::Resume() {
    state = ST_DECODING;
    pthread_cond_signal(&control_cond);
}

void TeaDecodeTask::PushMediaPacket(MediaPacket *pkt) {
    assert( state != ST_STOPPED);

    pthread_mutex_lock(&avbuffer_locker);
    videoBuffer.push_back(pkt);
    pthread_mutex_unlock(&avbuffer_locker);
    
    pthread_cond_signal(&control_cond);
}

void TeaDecodeTask::OnMessage(talk_base::Message *msg) {
    switch( msg->message_id) {
        case MSG_START_DECODE:
            doDecode();
            break; 
    }
}

void TeaDecodeTask::doDecode() {
    
    while(1) {
        if ( state == ST_STOPPED)       //exiting from main loop
            break;        
        if ( state == ST_PAUSED ) {
            pthread_cond_wait(&control_cond, &control_mutex);    //idle in main loop
            continue;
        }
        
        MediaPacket *target = NULL;

        pthread_mutex_lock(&avbuffer_locker);
        if ( videoBuffer.size() > 0) {
            target = videoBuffer.front();
            videoBuffer.pop_front();
        }
        pthread_mutex_unlock(&avbuffer_locker);

        if ( target == NULL) {
            pthread_cond_wait(&control_cond, &control_mutex); 
            continue;
        }

        // TODO 
        printf("new package arrive....\n");
    }
    
    //cleaning avbuffer,
    for( std::list<MediaPacket *>::iterator i = videoBuffer.begin(); 
            i != videoBuffer.end(); i++) {
        MediaPacket *pkt = *i;
        delete pkt;
    }
    videoBuffer.clear();
}


