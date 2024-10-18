#ifndef SONIC_ANYWHERE_MDAUDIO_H
#define SONIC_ANYWHERE_MDAUDIO_H

void md_audio__stop_sounds();

typedef enum MDSoundID {
    SND__SEGA
} MDSoundID;

void md_audio__play_sound_special(MDSoundID snd_id);

#endif // SONIC_ANYWHERE_MDAUDIO_H
