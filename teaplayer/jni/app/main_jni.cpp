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

//static HttpAccess *access;
static FileAccess *access;
static FFDemux *demux;
//static TeaVideoOutput *vout;
static BmpVideoOutput *vout;
static TeaPlayer *player;

extern "C" {
  JNIEXPORT jint JNICALL JNIDEFINE(startPlayer)(JNIEnv* env, jobject obj, jobject bmp_fb);
  JNIEXPORT jint JNICALL JNIDEFINE(renderBMP)(JNIEnv* env, jobject obj, jobject bmp_fb);
  JNIEXPORT jint JNICALL JNIDEFINE(stopPlayer)(JNIEnv* env, jclass clz);
};


JNIEXPORT jint JNICALL JNIDEFINE(startPlayer)( JNIEnv* env, jobject obj, jobject bmp) {
    if (player != NULL)
        return -1;

    //access = new HttpAccess();
    access = new FileAccess("/sdcard/demo.flv");
    demux = new FFDemux("live.flv");
    //vout = new TeaVideoOutput();
    vout = new BmpVideoOutput(env, bmp);
    player = new TeaPlayer(access, demux, vout);
    
    player->Play();

    return 0;
}


JNIEXPORT jint JNICALL JNIDEFINE(renderBMP)(JNIEnv* env, jobject obj, jobject bmp) {
    if (player != NULL) {
        vout->BitBlt(env, bmp);
        return 0;        
    }
    return -1;
}

JNIEXPORT jint JNICALL JNIDEFINE(stopPlayer)( JNIEnv* env , jclass clz) {
    if ( player == NULL) {
        player->Stop();

        delete player;
        delete access;
        delete demux;
        delete vout;
    }

    player = NULL;

    return 0;
}

