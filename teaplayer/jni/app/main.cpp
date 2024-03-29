#include <iostream>

#include "fileaccess.h"
#include "httpaccess.h"
#include "ffdemux.h"
#include "vout.h"
#include "player.h"

int main(int argc, const char *argv[]) {

    FileAccess *access = new FileAccess( argv[1] );    
    //HttpAccess *access = new HttpAccess();

    FFDemux *demux = new FFDemux("live.flv");
    TeaVideoOutput *vout = new TeaVideoOutput();
    TeaPlayer *player = new TeaPlayer(access, demux, vout);
    
    player->Play();
    for (int i = 0; i < 10; i++) {
        sleep(1);
    }
    player->Stop();
    
    delete player;
    delete vout;
    delete demux;
    delete access;
}

