#include <assert.h>
#include "vout.h"

TeaVideoOutput::TeaVideoOutput() {
}

TeaVideoOutput::~TeaVideoOutput() {
        
}

void TeaVideoOutput::RenderVideoPicture(VideoPicture *target) {
    delete target;
    signalPictureRendered();
}



