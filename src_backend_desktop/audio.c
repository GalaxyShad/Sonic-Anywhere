#include "include_backend/audio.h"
#include <raylib.h>

#include "include_backend/debug.h"


static Sound snd__ = {};

void audio__play_sound_special(SoundID snd_id) {
    if (snd__.stream.buffer == 0) {
        snd__ = LoadSound("../src_backend_desktop/audio/SEGA.wav");
    }


    PlaySound(snd__);

    LOG("Playing sound id: %02X", snd_id)
    RAISE_NOT_IMPLEMENTED
}