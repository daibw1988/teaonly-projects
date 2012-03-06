#ifndef _TEADEMUX_H_
#define _TEADEMUX_H_

#include <map>
extern "C" {
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
}
#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"
#include "decode.h"

class TeaDemux : public sigslot::has_slots<> {
public:    
    virtual ~TeaDemux(){};
    
    virtual bool Open(const std::string &file) = 0;
    virtual void Close() = 0;
    virtual bool PushNewData(const unsigned char *data, size_t length) = 0;

    // streaming access
    sigslot::signal1<bool> signalProbed;
    sigslot::signal2<unsigned int, MediaPacket *> signalMediaPackage;

    std::map<unsigned int, TeaDecode *> decodes;
};

class FFDecode: public TeaDecode {
public:
    FFDecode(AVCodecContext *pCC, AVCodec *pC) {
        pCodecCtx = pCC;
        pCodec = pC;
        
        if ( pCodecCtx->codec_type == AVMEDIA_TYPE_AUDIO)
            type = TEACODEC_TYPE_VIDEO;
        else if ( pCodecCtx->codec_type == AVMEDIA_TYPE_AUDIO)
            type = TEACODEC_TYPE_AUDIO;
        else
            type = TEACODEC_TYPE_UNKNOW;
    }

    virtual ~FFDecode() {
        avcodec_close(pCodecCtx);
    }
    
    virtual VideoPicture* DecodeVideoPacket(const unsigned char *data, size_t legth);

private:
    AVCodecContext *pCodecCtx;
    AVCodec        *pCodec;
};

class FFDemux:public TeaDemux, talk_base::MessageHandler {
public:
    FFDemux();
    virtual ~FFDemux();
    
    virtual bool Open(const std::string &file);
    virtual void Close();
    virtual bool PushNewData(const unsigned char *data, size_t length);

protected:     
    virtual void OnMessage(talk_base::Message *msg);

private:
    enum {
        MSG_DEMUX_START,
    };
    void doProbe();
    void decodeInit();
    void doDemux();

    bool probeFailed;
    std::string targetFile;
    talk_base::Thread *thread;              //demuxing thread 
    AVFormatContext *pFormatCtx;
    AVIOContext   *pIO;
    unsigned char *buffer_io;
    unsigned int buffer_io_size;
    unsigned char *buffer_stream;
    unsigned int buffer_stream_length;
    unsigned int buffer_stream_size;
    AVProbeData probe_data;

    talk_base::CriticalSection mutex_;
};

#endif
