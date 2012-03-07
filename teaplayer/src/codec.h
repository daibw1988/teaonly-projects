#ifndef _TEADECODE_H_
#define _TEADECODE_H_

#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"
#include "timing.h"
#include "video.h"

typedef enum {
    TEACODEC_TYPE_UNKNOW,
    TEACODEC_TYPE_VIDEO,
    TEACODEC_TYPE_AUDIO
}TeaCodecType;

class MediaPacket {
public:
    MediaTime pts;
    MediaTime dts;
    unsigned char *data;
    unsigned int length;
    void *priv;
};

class TeaDecoder{
public:    
    virtual ~TeaDecoder(){};
    virtual VideoPicture* DecodeVideoPacket(MediaPacket *) { return NULL; }
    virtual void *DecodeAudioSamples(MediaPacket *) {return NULL; }     
    TeaCodecType type;
};

#endif
