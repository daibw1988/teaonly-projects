#ifndef _VIDEO_H_
#define _VIDEO_H_

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
};

#endif
