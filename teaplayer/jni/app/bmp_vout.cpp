#include "helper.h"
#include "bmp_vout.h"

extern "C" {
#include "yuv2rgb.h"
};

BmpVideoOutput::BmpVideoOutput(JNIEnv* jenv, jobject bmp) {
    target_fb = NULL;
    isRendered = true;    
    if ((AndroidBitmap_getInfo(jenv, bmp, &info)) < 0) {  
        return;
    }
    
    target_fb = malloc( info.height * info.stride );             
    LOGD("Target frame buffer size = %dx%d  stride = %d", info.width, info.height, info.stride);
}

BmpVideoOutput::~BmpVideoOutput() {
    if ( target_fb != NULL)
        free(target_fb);   
}

void BmpVideoOutput::RenderVideoPicture(VideoPicture *target) {
    int minwidth;
    int minheight;

    minwidth = target->width < info.width ? target->width : info.width;
    minheight = target->height < info.height ? target->height : info.height;

    yuv420_2_rgb565( (unsigned char *)target_fb,
                     target->vplan[0],
                     target->vplan[1],
                     target->vplan[2],
                     minwidth,
                     minheight,
                     target->vplan_length[0],
                     target->vplan_length[1],
                     info.stride,
                     yuv2rgb565_table,
                     0);
    isRendered = false;
}

int BmpVideoOutput::BitBlt(JNIEnv* jenv, jobject bmp) {
    void *pixels;
    
    if ( isRendered )
        return -1;

    if ((AndroidBitmap_lockPixels(jenv, bmp, &pixels)) < 0) { 
        return -1;
    }
    memcpy( pixels, target_fb, info.height * info.stride);
    AndroidBitmap_unlockPixels(jenv, bmp);

    return 0;
}

