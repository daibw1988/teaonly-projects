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

void BmpVideoOutput::RenderVideoPicture(VideoPicture *vp) {
    int minwidth = vp->width < info.width ? vp->width : info.width;
    int minheight = vp->height < info.height ? vp->height : info.height;

    int src_offset_x = (vp->width - minwidth) / 2;
    int src_offset_y = (vp->height - minheight) / 2;
    src_offset_x = src_offset_x + src_offset_x % 2;
    src_offset_y = src_offset_y + src_offset_y % 2;
    
    int dst_offset_x = (info.width - minwidth) / 2;
    int dst_offset_y = (info.height - minheight) / 2; 

    yuv420_2_rgb565( 
                     (unsigned char *)target_fb + dst_offset_x * 2 + dst_offset_y * info.stride, 
                     vp->vplan[0] + src_offset_x + src_offset_y * vp->vplan_length[0],
                     vp->vplan[1] + src_offset_x/2 + src_offset_y * vp->vplan_length[1]/2,
                     vp->vplan[2] + src_offset_x/2 + src_offset_y * vp->vplan_length[2]/2,
                     minwidth,
                     minheight,
                     vp->vplan_length[0],
                     vp->vplan_length[1],
                     info.stride,
                     yuv2rgb565_table,
                     1);

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

