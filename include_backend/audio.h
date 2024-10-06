#ifndef SONIC_ANYWHERE_AUDIO_H
#define SONIC_ANYWHERE_AUDIO_H

void audio__stop_sounds();

typedef enum SoundID {
    SND__SEGA
} SoundID;

void audio__play_sound_special(SoundID snd_id);

#endif // SONIC_ANYWHERE_AUDIO_H
