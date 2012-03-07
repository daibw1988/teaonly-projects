#include <iostream>

#include "access.h"
#include "demux.h"
#include "vout.h"
#include "player.h"

int main(int argc, const char *argv[]) {
    FileAccess *access = new FileAccess( argv[1] );
    
    FFDemux *demux = new FFDemux("live.flv");
    TeaVideoOutput *vout = new TeaVideoOutput(1000,1000);
    TeaPlayer *player = new TeaPlayer(access, demux, vout);
    player->Play();

    for(;;) {
        sleep(1);
    }
}

