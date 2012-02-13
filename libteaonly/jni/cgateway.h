#ifndef _CGATWAY_H_
#define _CGATWAY_H_

#include <vector>
#include <string>
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "TEAONLY"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)  

int GatwayInterface(std::string &file, std::string &uri);

#endif
