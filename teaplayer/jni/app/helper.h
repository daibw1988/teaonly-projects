#ifndef _H_HELPER_H_
#define _H_HELPER_H_

#ifdef ANDROID
#include <jni.h>
#include <android/log.h>
#define  LOG_TAG    "TEAONLY"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)  
#endif

#endif
