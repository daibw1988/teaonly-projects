#ifndef _TEADEMUX_H_
#define _TEADEMUX_H_

#include <map>
extern "C" {
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
}
#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"
#include "codec.h"

class TeaDemux : public sigslot::has_slots<> {
public:    
    virtual ~TeaDemux(){};
    
    virtual bool Open() = 0;
    virtual void Close() = 0;
    virtual bool PushNewData(const unsigned char *data, size_t length) = 0;

    // streaming access
    sigslot::signal1<bool> signalProbed;
    sigslot::signal0<> signalOverflow;
    sigslot::signal1<MediaPacket *> signalMediaPacket;
    
    std::map<unsigned int, TeaDecoder *> decoders;
};

class FFDecoder: public TeaDecoder {
public:
    FFDecoder(AVCodecContext *pCC, AVCodec *pC);
    virtual ~FFDecoder() {
        avcodec_close(pCodecCtx);
    }
    virtual VideoPicture* DecodeVideoPacket(MediaPacket *pkt);
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
    AVIOContext   *pIO;
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
