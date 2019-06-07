// AudioCommon.c

#include "ZGBMain.h"
#include "Sound.h"

#include "AudioCommon.h"

// This is unbanked

void Sound_Init(void) {
    NR52_REG = 0x80; // Enables sound, always set this first
    NR51_REG = 0xFF; // Enables all channels (left and right)
    NR50_REG = 0x77; // Max volume
}


void Sound_Volume_Fadeout(void) {
    NR50_REG = 0x77; delay(10);
    NR50_REG = 0x66; delay(10);
    NR50_REG = 0x55; delay(10);
    NR50_REG = 0x44; delay(10);
    NR50_REG = 0x33; delay(10);
    NR50_REG = 0x22; delay(10);
    NR50_REG = 0x11;
}


void Sound_GBTStopAndRestoreAudio(void) {
    gbt_stop();

    // Need to turn sound back on after a gbt_stop()
    Sound_Init();
}


void PlaySound_BeatLevelBoss(void) {
    Sound_GBTStopAndRestoreAudio();

    PlayFx(CHANNEL_1, 30, 0x76, 0x8A, 0xA6, 0x7D, 0x86);
    delay(200);
}


void PlaySound_EndOfLevel(void) {
    Sound_GBTStopAndRestoreAudio();

    PlayFx(CHANNEL_1, 30, 0x76, 0x8A, 0xA6, 0x7D, 0x86);
    delay(400);
}


void PlaySound_TextMessage(void) {
    PlayFx(CHANNEL_2, 0, 0x81, 0x83, 0x91, 0x86);
    delay(100);
    PlayFx(CHANNEL_2, 0, 0x81, 0x82, 0xF7, 0x86);
}
