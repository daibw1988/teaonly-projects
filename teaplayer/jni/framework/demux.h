#ifndef _TEADEMUX_H_
#define _TEADEMUX_H_

#include <map>
#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"
#include "base.h"

class TeaDecoder{
public:    
    virtual ~TeaDecoder(){};
    virtual int DecodeVideoPacket(MediaPacket *, VideoPicture*) { return 0; }
    virtual void *DecodeAudioSamples(MediaPacket *) {return NULL; }     
    TeaCodecType type;
};

class TeaDemux : public sigslot::has_slots<> {
public:    
    virtual ~TeaDemux(){};
    
    virtual bool Open() = 0;
    virtual void Close() = 0;
    virtual bool PushNewData(const unsigned char *data, size_t length) = 0;

    sigslot::signal1<bool> signalProbed;
    sigslot::signal1<MediaPacket *> signalMediaPacket;
    
    std::map<unsigned int, TeaDecoder *> decoders;
};

#endif
