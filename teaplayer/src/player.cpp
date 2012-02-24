#include "media.h"
#include "access.h"
#include "demux.h"
#include "decode.h"
#include "vout.h"
#include "aout.h"
#include "player.h"

TeaPlayer::TeaPlayer():
        media(NULL),
        access(NULL),
        demux(NULL),
        vout(NULL),
        aout(NULL) {
    decodes.clear();
}

