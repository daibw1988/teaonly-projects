#include <time.h>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <vector>
#include <iostream>
#include "fileaccess.h"
#include "httpaccess.h"
#include "ffdemux.h"
#include "bmp_vout.h"
#include "player.h"

#define  JNIDEFINE(fname) Java_teaonly_projects_teaplayer_TeaPlayer_##fname

static HttpAccess *access;
//static FileAccess *access;
static FFDemux *demux;
//static TeaVideoOutput *vout;
static BmpVideoOutput *vout;
static TeaPlayer *player;

extern "C" {
  JNIEXPORT jint JNICALL JNIDEFINE(startPlayer)(JNIEnv* env, jobject obj, jobject bmp_fb, jstring ipaddr);
  JNIEXPORT jint JNICALL JNIDEFINE(renderBMP)(JNIEnv* env, jobject obj, jobject bmp_fb);
  JNIEXPORT jint JNICALL JNIDEFINE(stopPlayer)(JNIEnv* env, jclass clz);
};

static std::string convert_jstring(JNIEnv *env, const jstring &js) {
    static char outbuf[1024];
    std::string str;

    int len = env->GetStringLength(js);
    env->GetStringUTFRegion(js, 0, len, outbuf);

    str = outbuf;
    return str;
}

JNIEXPORT jint JNICALL JNIDEFINE(startPlayer)( JNIEnv* env, jobject obj, jobject bmp, jstring ipaddr) {
    if (player != NULL)
        return -1;

    access = new HttpAccess( convert_jstring(env, ipaddr) );
    //access = new FileAccess("/sdcard/demo.flv");
    demux = new FFDemux("live.flv");
    //vout = new TeaVideoOutput();
    vout = new BmpVideoOutput(env, bmp);
    player = new TeaPlayer(access, demux, vout);
    
    player->Play();

    return 0;
}


JNIEXPORT jint JNICALL JNIDEFINE(renderBMP)(JNIEnv* env, jobject obj, jobject bmp) {
    if (player != NULL) {
        return vout->BitBlt(env, bmp);
    }
    return -1;
}

JNIEXPORT jint JNICALL JNIDEFINE(stopPlayer)( JNIEnv* env , jclass clz) {
    if ( player != NULL) {
        player->Stop();

        delete player;
        delete access;
        delete demux;
        delete vout;
    }

    player = NULL;

    return 0;
}

