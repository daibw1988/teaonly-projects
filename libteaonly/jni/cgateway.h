#ifndef _CGATEWAY_H_
#define _CGATEWAY_H_

#include <jni.h>
#include <android/log.h>
#include <vector>
#include <string>
#include <map>

#define  LOG_TAG    "TEAONLY"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)  

class Interface {
public:
    virtual int Run(std::string &file, std::string &uri, std::string &result) = 0;
};

class GatewayInterface {
public:
    GatewayInterface();
    ~GatewayInterface();

public:
    int RegisterInterface(std::string &file, Interface *itf);
    int RunInterface(std::string &file, std::string &uri, std::string &result);

private:
    std::map<std::string, void *> interfaces;

public:
    static GatewayInterface *Gateway;
};

#endif
