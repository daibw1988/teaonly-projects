#include "cgateway.h"

#define  JNIDEFINE(fname) Java_Teaonly_NativeAgent_##fname

extern "C" {
    JNIEXPORT jint JNICALL JNIDEFINE(nativeCallGatway)(JNIEnv* env, jclass clz, jstring file, jstring uri);
};

static std::string convert_jstring(JNIEnv *env, const jstring &js) {
    static char outbuf[1024];
    std::string str;

    int len = env->GetStringLength(js);
    env->GetStringUTFRegion(js, 0, len, outbuf);

    str = outbuf;
    return str;
}

JNIEXPORT jint JNICALL JNIDEFINE(nativeCallGatway)(JNIEnv* env, jclass clz, jstring jstr_file, jstring jstr_uri) {
   	std::string file, uri;
	file = convert_jstring(env, jstr_file);
	uri = convert_jstring(env, jstr_uri);
	int ret = GatwayInterface(file, uri);
	return ret;
}


