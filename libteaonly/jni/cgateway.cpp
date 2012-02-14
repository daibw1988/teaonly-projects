#include "cgateway.h"

GatewayInterface *GatewayInterface::Gateway = NULL;

GatewayInterface::GatewayInterface() {
}

GatewayInterface::~GatewayInterface() {
}

int GatewayInterface::RegisterInterface(std::string &file, Interface *itf) {
    int ret = 1;
    if ( interfaces[file] == NULL)
        ret = 0;                            //0:new  1:replace 
    interfaces[file] = (void *)itf; 
    return ret;
}

int GatewayInterface::RunInterface(std::string &file, std::string &uri, std::string &result) {
    int ret;
    Interface *itf = (Interface *) interfaces[file];
    //-1:error, >=0: return code
    if (itf == NULL)
        ret = -1;
    else 
        ret = itf->Run(file, uri, result);
    return ret;
}

