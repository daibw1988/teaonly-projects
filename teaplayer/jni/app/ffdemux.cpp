#include "ffdemux.h"

int ReadFunc(void *opaque, uint8_t *buf, int buf_size) {
    FFDemux *demux = (FFDemux *)opaque;
    return demux->ReadFunc(buf, buf_size);    
}

FFDecoder::FFDecoder(AVCodecContext *pCC, AVCodec *pC) {
    pCodecCtx = pCC;
    pCodec = pC;
    pFrame = NULL;

    if ( pCodecCtx->codec_type == AVMEDIA_TYPE_VIDEO) {
        type = TEACODEC_TYPE_VIDEO;
        pFrame = avcodec_alloc_frame();
    }else if ( pCodecCtx->codec_type == AVMEDIA_TYPE_AUDIO)
        type = TEACODEC_TYPE_AUDIO;
    else
        type = TEACODEC_TYPE_UNKNOW;
}

FFDecoder::~FFDecoder(){
    avcodec_close(pCodecCtx);
    if ( pFrame != NULL)
        av_free(pFrame);
}

int FFDecoder::DecodeVideoPacket(MediaPacket *pkt, VideoPicture* vp) {
    if ( type != TEACODEC_TYPE_VIDEO)
        return 0;
    
    
    //convert MediaPacket to FFMPEG's AVPacket
    AVPacket ffpkt;
    ffpkt.data = pkt->data;
    ffpkt.size = pkt->size;
    ffpkt.pts = pkt->pts;
    ffpkt.dts = pkt->dts;
    
    int isFinished;
    avcodec_decode_video2(pCodecCtx, pFrame, &isFinished, &ffpkt);
    delete pkt;

    if ( !isFinished ) 
        return 0;
    vp->video_type = YUV_420_PLAN;
    vp->width = pCodecCtx->width;
    vp->height = pCodecCtx->height;
   
    for(int i = 0; i < 3; i++) {
        if( (vp->vplan_length[i] != (unsigned int)pFrame->linesize[i]) && (pFrame->linesize[i] > 0) ) {
            vp->vplan_length[i] = pFrame->linesize[i];
            if ( vp->vplan[i] != NULL ) {
                free(vp->vplan[i]);           
            }
            vp->vplan[i] = (unsigned char *)malloc( vp->height * vp->vplan_length[i]);            
        }
        if ( vp->vplan_length[i] > 0) {
            assert(vp->vplan[i] != NULL);       
            if ( i > 1)
                memcpy( vp->vplan[i], pFrame->data[i], vp->height * pFrame->linesize[i] / 2);
            else
                memcpy( vp->vplan[i], pFrame->data[i], vp->height * pFrame->linesize[i]);
        }
    }

    return 1;
}

FFDemux::FFDemux(const std::string &file) {
    targetFile = file;
    probeFailed = false;
    pFormatCtx = NULL;
    pFormat = NULL;
    pIO = NULL;
    buffer_io = NULL;
    decoders.clear();

    buffer_stream_size = 1024*1024*4;
    buffer_stream = new unsigned char[buffer_stream_size];  
    buffer_stream_length = 0;
    buffer_io = NULL;
    buffer_io_size = 1024*2;
    buffer_probe_size = 1024*8;
    pthread_mutex_init(&data_locker, NULL);
    pthread_cond_init(&data_arrive_cond, NULL); 

    av_register_all();
    thread = new talk_base::Thread();
    thread->Start();
}

FFDemux::~FFDemux() {
    delete thread;
    
    for(std::map<unsigned int, TeaDecoder *>::iterator i=decoders.begin(); i != decoders.end(); i++) {
        delete (*i).second;
    } 
    decoders.clear();

    if ( pFormatCtx != 0) {
        av_close_input_stream(pFormatCtx);
        pFormatCtx = 0;
    }
    av_free(pIO);
    pIO = NULL;
    delete buffer_io;
    buffer_io = NULL;
    delete buffer_stream;
    buffer_stream = NULL;
}

int FFDemux::ReadFunc(unsigned char *buf, int buf_size) {
   
    int ret = buf_size;
    pthread_mutex_lock(&data_locker); 
    while(1) {
        if ( buffer_stream_length == -1) {          //streaming is end
            ret = -1;
            break;
        }
        if ( buffer_stream_length >= buf_size) {
            memcpy(buf, buffer_stream, buf_size);
            buffer_stream_length -= buf_size;
            memmove(buffer_stream, &buffer_stream[buf_size], buffer_stream_length);
            break;
        } else {
            memcpy(buf, buffer_stream, buffer_stream_length);
            buf_size -= buffer_stream_length; 
            buf += buffer_stream_length;
            buffer_stream_length = 0;
        }
        pthread_cond_wait(&data_arrive_cond, &data_locker);
    }
    pthread_mutex_unlock(&data_locker);

    return ret;
}

bool FFDemux::Open() {

    // create io for libavformat
	buffer_io = new unsigned char[buffer_io_size];
#ifdef OLD_FFMPEG
    pIO = (ByteIOContext *) av_mallocz(sizeof(ByteIOContext));
    assert(pIO != NULL);
    int ret = init_put_byte(pIO,
                            buffer_io,
                            buffer_io_size,
                            0,
                            this,
                            ::ReadFunc,
                            0,
                            0);
    if ( ret < 0)
        assert(0);
    pIO->is_streamed = 1;
#else
	pIO = avio_alloc_context(buffer_io,
							buffer_io_size/2,
							0,
							this,
							::ReadFunc,
							0,
							0); 
    assert(pIO != NULL);
    pIO->seekable = 0;
#endif
    probeFailed = false;
    decoders.clear();
    pFormatCtx = NULL;

    probe_data.filename = targetFile.c_str(); 
    probe_data.buf_size = buffer_probe_size;
    probe_data.buf = (unsigned char *)malloc(probe_data.buf_size);
    
    buffer_stream_length = 0;
    return true;
}

void FFDemux::Close() {
    buffer_stream_length = -1;
    pthread_cond_signal(&data_arrive_cond);
    thread->Stop();
}

bool FFDemux::PushNewData(const unsigned char *data, size_t length) {
    if ( probeFailed )
        return false;

    if ( pFormat == NULL) {
        memcpy( &buffer_stream[buffer_stream_length], data, length);
        buffer_stream_length += length;
        if ( (unsigned int)buffer_stream_length >= buffer_probe_size ) { 
            memcpy(probe_data.buf, buffer_stream, buffer_probe_size);
            prepareProbe(); 
        }
    } else {
        if ( (buffer_stream_length + length )>= buffer_stream_size) {
            // internal buffer is full
            return false;
        } 
        pthread_mutex_lock(&data_locker);
        memcpy( &buffer_stream[buffer_stream_length], data, length);
        buffer_stream_length += length;
        pthread_mutex_unlock(&data_locker);
        pthread_cond_signal(&data_arrive_cond);    
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

void FFDemux::prepareProbe() {
    pFormat = av_probe_input_format(&probe_data, 1);
    if ( pFormat == NULL) {
        goto probe_failed;
    }
#ifdef OLD_FFMPEG
    pFormatCtx = NULL;
#else
    pFormatCtx = avformat_alloc_context();
    pFormatCtx->pb = pIO;
#endif
    
    thread->Post(this, MSG_DEMUX_START);
    return;

probe_failed:
    probeFailed = true;
    signalProbed(false);  
}

void FFDemux::decodeInit() {
    // Find the first video stream
    for(unsigned i = 0; i < pFormatCtx->nb_streams; i++){

        AVCodecContext *pCC = pFormatCtx->streams[i]->codec;
        AVCodec *pC  = avcodec_find_decoder(pCC->codec_id); 
        assert(pCC != NULL);
        assert(pC != NULL);
#ifdef OLD_FFMPEG
        int ret = avcodec_open(pCC, pC);
#else
        int ret = avcodec_open2(pCC, pC, NULL);
#endif
        if ( ret < 0)
            assert(0);
        TeaDecoder *dec = new FFDecoder(pCC, pC);
        decoders[i] = dec;    
    }
}

void FFDemux::doDemux() { 
    
    // probe meida stream format
#ifdef OLD_FFMPEG
    if( av_open_input_stream(&pFormatCtx, pIO, targetFile.c_str(), pFormat, 0)<0){
#else
    if( avformat_open_input(&pFormatCtx, targetFile.c_str(), pFormat, 0) < 0) {
#endif
        pFormatCtx = 0;
        goto probe_failed;
    }  

    pFormatCtx->probesize = 8000;
    if(av_find_stream_info(pFormatCtx)<0){
        goto probe_failed;        
    }
#ifdef OLD_FFMPEG   
    dump_format(pFormatCtx, 0, "null", 0);
#else
    av_dump_format(pFormatCtx, 0, "null", 0);
#endif
    decodeInit(); 
    signalProbed(true);
    
    // main loop of demux
    AVPacket newPacket;
    while(1) {
        if ( av_read_frame(pFormatCtx, &newPacket) < 0){
            goto stream_end;
        }   
        
        if ( decoders[newPacket.stream_index] == NULL)
            continue;

        //std::cout << "Get AV packet =  " << newPacket.stream_index << " PTS = " << newPacket.pts << "  DTS = " << newPacket.dts << std::endl;
        MediaPacket *pkt = new MediaPacket( newPacket.size );
        pkt->pts = newPacket.pts;
        pkt->dts = newPacket.dts;
        pkt->duration = newPacket.duration;
        pkt->size = newPacket.size;
        pkt->channel = newPacket.stream_index;
        pkt->type = decoders[newPacket.stream_index]->type;
        memcpy(pkt->data, newPacket.data, newPacket.size); 
        signalMediaPacket(pkt);
    }

probe_failed:
    probeFailed = true;
    signalProbed(false);  
    return;

stream_end:
    return;
}

