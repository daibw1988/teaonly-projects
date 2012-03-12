#ifndef _BASE_H_
#define _BASE_H_

#ifdef LINUX
#include <stdint.h>
#include <sys/time.h>

typedef uint64_t MediaTime;
const MediaTime BAD_TIME = (uint64_t)(-1);
inline MediaTime CurrentTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    MediaTime ret = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    return ret;    
}
#elif WIN32
// TODO
#endif

typedef enum {
    TEACODEC_TYPE_UNKNOW,
    TEACODEC_TYPE_VIDEO,
    TEACODEC_TYPE_AUDIO
}TeaCodecType;

class MediaPacket {
public:
    MediaPacket(unsigned int size) {
        data = new unsigned char[size + 8 - (size%8) ];
    }
    ~MediaPacket(){
        delete data;
    }
public:
    MediaTime pts;
    MediaTime dts;
    int       duration;
    unsigned int channel; 
    unsigned char *data;
    unsigned int size;
    void *priv;
};

typedef enum {
    YUV_420_PLAN = 1,
    YUV_420_PACK = 2,
    RGBA_8_PACK  = 3
}VType;

struct VideoPicture{
    VType video_type;
    unsigned int width;
    unsigned int height;
    unsigned char **vplan;
    unsigned int *vplan_length;
    MediaTime   mt;    
};

#endif
