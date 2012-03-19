#ifndef _FFDEMUX_H_
#define _FFDEMUX_H_

#include <map>
#include <iostream>
#include <assert.h>


#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
};

#include "demux.h"

//#define OLD_FFMPEG 1

class FFDecoder: public TeaDecoder {
public:
    FFDecoder(AVCodecContext *pCC, AVCodec *pC);
    virtual ~FFDecoder();
    virtual int DecodeVideoPacket(MediaPacket *, VideoPicture*);

private:
    AVCodecContext *pCodecCtx;
    AVCodec        *pCodec;
    AVFrame        *pFrame;
};

class FFDemux:public TeaDemux, talk_base::MessageHandler {
public:
    FFDemux(const std::string &file);
    virtual ~FFDemux();
    
    virtual bool Open();
    virtual void Close();
    virtual bool PushNewData(const unsigned char *data, size_t length);

    int ReadFunc(unsigned char *buf, int buf_size); 

protected:     
    virtual void OnMessage(talk_base::Message *msg);

private:
    enum {
        MSG_DEMUX_START,
    };
    void prepareProbe();
    void decodeInit();
    void doDemux();

    bool probeFailed;
    std::string targetFile;
    talk_base::Thread *thread;              //demuxing thread 
    AVFormatContext *pFormatCtx;
    AVInputFormat *pFormat;     
#ifdef OLD_FFMPEG
    ByteIOContext *pIO;    
#else
    AVIOContext   *pIO;
#endif
    unsigned char *buffer_io;
    unsigned int buffer_io_size;
    unsigned int buffer_probe_size;
    unsigned char *buffer_stream;
    volatile int buffer_stream_length;
    unsigned int buffer_stream_size;

    AVProbeData probe_data;

    pthread_cond_t  data_arrive_cond;
    pthread_mutex_t data_locker;      
};

#endif
