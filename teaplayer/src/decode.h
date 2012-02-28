#ifndef _TEADECODE_H_
#define _TEADECODE_H_

#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"
#include "demux.h"

typedef enum {
    CODEC_TYPE_VIDEO,
    CODEC_TYPE_AUDIO
}CodecType;

class TeaDecode : public sigslot::has_slots<> {
public:    
    virtual ~TeaDecode(){};
    
    virtual bool Open() = 0;
    virtual void Close() = 0;
    virtual void PushNewPacket(const unsigned char *data, size_t length);

    // streaming access
    sigslot::signal0<> signalBufferOverflow;
    sigslot::signal2<unsigned int, void *> signalMediaData;
    CodecType type;
};


#endif
