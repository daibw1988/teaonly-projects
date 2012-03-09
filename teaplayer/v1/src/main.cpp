#include <iostream>

#include "access.h"
#include "demux.h"
#include "decode.h"
#include "vout.h"
#include "player.h"

int main(int argc, const char *argv[]) {
    FileAccess *access = new FileAccess( argv[1] );
    
    FFDemux *demux = new FFDemux("live.flv");
    TeaDecodeTask *decode = new TeaDecodeTask(demux);
    TeaVideoOutput *vout = new TeaVideoOutput(1000,1000);
    TeaPlayer *player = new TeaPlayer(access, demux, decode, vout);
    player->Play();

    for(;;) {
        sleep(1);
    }
}

