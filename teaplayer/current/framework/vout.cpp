#include <assert.h>
#include "vout.h"

void TeaVideoOutput::RenderVideoPicture(VideoPicture *target) {
//    delete target;
    signalPictureRendered();
}


