#include "cgateway.h"

#define  JNIDEFINE(fname) Java_Teaonly_NativeAgent_##fname

extern "C" {
    JNIEXPORT jint JNICALL JNIDEFINE(nativeInitial)(JNIEnv* env, jclass clz);
    JNIEXPORT jint JNICALL JNIDEFINE(nativeRelease)(JNIEnv* env, jclass clz);
    JNIEXPORT jstring JNICALL JNIDEFINE(nativeCallGatway)(JNIEnv* env, jclass clz, jstring file, jstring uri);
};

static std::string convert_jstring(JNIEnv *env, const jstring &js) {
    static char outbuf[1024];
    std::string str;

    int len = env->GetStringLength(js);
    env->GetStringUTFRegion(js, 0, len, outbuf);

    str = outbuf;
    return str;
}

JNIEXPORT jint JNICALL JNIDEFINE(nativeInitial)(JNIEnv* env, jclass clz) {
    GatewayInterface::Gateway = new GatewayInterface();
    return 0;
}

JNIEXPORT jint JNICALL JNIDEFINE(nativeRelease)(JNIEnv* env, jclass clz) {
    if (GatewayInterface::Gateway != NULL)
        delete GatewayInterface::Gateway;
    return 0;
}
 
JNIEXPORT jstring JNICALL JNIDEFINE(nativeCallGatway)(JNIEnv* env, jclass clz, jstring jstr_file, jstring jstr_uri) {
   	std::string file, uri, result;
	file = convert_jstring(env, jstr_file);
	uri = convert_jstring(env, jstr_uri);

    result = "";
    if (GatewayInterface::Gateway != NULL)
        GatewayInterface::Gateway->RunInterface(file, uri, result);
    
    return env->NewStringUTF(result.c_str()); 
}


