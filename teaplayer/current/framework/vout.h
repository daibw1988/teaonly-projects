#ifndef _VOUT_H_
#define _VOUT_H_

#include <list>
#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"
#include "base.h"

class TeaVideoOutput: public sigslot::has_slots<>  {
public:    
    TeaVideoOutput();
    virtual ~TeaVideoOutput();

    virtual void RenderVideoPicture(VideoPicture *target);
    sigslot::signal0<> signalPictureRendered;
};

#endif
