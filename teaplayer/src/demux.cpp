#define UINT64_C(c)   c ## ULL
#include <assert.h>
#include "demux.h"

int ReadFunc(void *opaque, uint8_t *buf, int buf_size) {
    return -1;
}


VideoPicture * FFDecode::DecodeVideoPacket(const unsigned char *data, size_t length) {
    if ( type != TEACODEC_TYPE_VIDEO)
        return NULL;

    int isFinished;
    avcodec_decode_video2(pCodecCtx, NULL, &isFinished, NULL); 
    return NULL;
}

FFDemux::FFDemux(const std::string &file) {
    targetFile = file;
    av_register_all();

    pFormatCtx = NULL;
    pIO = NULL;
    buffer_io = NULL;

    thread = new talk_base::Thread();
    thread->Start();

    probeFailed = false;
    decodes.clear();

    buffer_stream = new unsigned char[1024*1024];  
    buffer_stream_length = 0;
    buffer_stream_size = 1024*1024*4;
    buffer_io = NULL;
    buffer_io_size = 1024*32;
}

FFDemux::~FFDemux() {
    thread->Stop();
    delete thread;
    
    /*
    for(std::map<unsigned int, TeaDecode *>::iterator i=decodes.begin(); i != decodes.end(); i++) {
        (*i).second->Open();
    } 
    */

    if ( pIO == 0) {
        av_close_input_stream(pFormatCtx);
    }
    av_free(pIO);
    pIO = NULL;
    delete buffer_io;
    buffer_io = NULL;
}

bool FFDemux::Open() {

    // create io for libavformat
	buffer_io = new unsigned char[buffer_io_size];
	pIO = avio_alloc_context(buffer_io,
							buffer_io_size/2,
							0,
							this,
							ReadFunc,
							0,
							0); 
    pIO->seekable = 0;

    probeFailed = false;
    decodes.clear();
    pFormatCtx = NULL;

    probe_data.filename = targetFile.c_str(); 
    probe_data.buf_size = 1024*8;
    probe_data.buf = (unsigned char *)malloc(probe_data.buf_size);

    return true;
}

void FFDemux::Close() {
    // TODO
}

bool FFDemux::PushNewData(const unsigned char *data, size_t length) {
    if ( probeFailed )
        return false;

    if ( pFormatCtx == NULL) {
        memcpy( &buffer_stream[buffer_stream_length], data, length);
        buffer_stream_length += length;
        if ( length >= buffer_io_size ) { 
            memcpy( &buffer_io, buffer_stream, buffer_io_size);
            doProbe(); 
        }
    } else {
        
    }

    return true;    
}

void FFDemux::OnMessage(talk_base::Message *msg) {
    switch( msg->message_id) {
        case MSG_DEMUX_START:
            doDemux();
            break;
        default:
            break;
    }
}

void FFDemux::doProbe() {
    AVInputFormat *pAV = av_probe_input_format(&probe_data, 1);
    if ( pAV == NULL) {
        goto probe_failed;
    }
    pFormatCtx = avformat_alloc_context();
    pFormatCtx->pb = pIO;

    if( avformat_open_input(&pFormatCtx, targetFile.c_str(), pAV, 0) < 0){ 
        pFormatCtx = 0;
        goto probe_failed;
    }   
    pFormatCtx->probesize = 8000;
    if(av_find_stream_info(pFormatCtx)<0){
        goto probe_failed;        
    }   
    
    //av_dump_format(pFormatCtx, 0, "null", 0);
    decodeInit(); 
    signalProbed(true);
    thread->Post(this, MSG_DEMUX_START);
    return;

probe_failed:
    probeFailed = true;
    signalProbed(false);  
}

void FFDemux::decodeInit() {
    // Find the first video stream
    for(unsigned i = 0; i < pFormatCtx->nb_streams; i++){
        if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
            AVCodecContext *pCC = pFormatCtx->streams[i]->codec;
            AVCodec *pC  = avcodec_find_decoder(pCC->codec_id); 
            assert(pCC != NULL);
            assert(pC != NULL);
            int ret = avcodec_open2(pCC, pC, NULL);
            assert(ret >= 0);
            TeaDecode *dec = new FFDecode(pCC, pC);
            decodes[i] = dec;
        } else if ( pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            // TODO                    
        }
    }
}

void FFDemux::doDemux() {

}

