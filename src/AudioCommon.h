// AudioCommon.h

#ifndef AUDIO_COMMON_H
#define AUDIO_COMMON_H

#include "gbt_player.h"

#define TRACK1_VAR track1_mod_Data // File Name: track1.b2.mod
#define TRACK2_VAR track2_mod_Data
#define TRACK3_VAR track3_mod_Data
#define TRACK4_VAR track4_mod_Data
#define TRACK5_VAR track5_mod_Data
#define TRACK6_VAR track6_mod_Data
#define TRACK7_VAR track7_mod_Data

extern const unsigned char * TRACK1_VAR[]; // Boss
extern const unsigned char * TRACK2_VAR[]; // Level-Kitchen
extern const unsigned char * TRACK3_VAR[]; // Level-Living Room
extern const unsigned char * TRACK4_VAR[]; // Intro Dream
extern const unsigned char * TRACK5_VAR[]; // End Game
extern const unsigned char * TRACK6_VAR[]; // Game Over
extern const unsigned char * TRACK7_VAR[]; // Intro Screen


#define TRACK1_BANK 2
#define TRACK2_BANK 3
#define TRACK3_BANK 3
#define TRACK4_BANK 3
#define TRACK5_BANK 3
#define TRACK6_BANK 2
#define TRACK7_BANK 3


#define PLAY_SOUND_JUMP                 PlayFx(CHANNEL_1, 5, 0x66, 0xC4, 0x63, 0x40, 0x86);
#define PLAY_SOUND_BREAK_DISHES         PlayFx(CHANNEL_1, 30, 0x55, 0x83, 0x56, 0x86, 0x86); PlayFx(CHANNEL_4, 30, 0x01, 0x4F, 0x7A, 0xC0);
#define PLAY_SOUND_EAT_MOTH             PLAY_SOUND_BREAK_DISHES // duplicate sound
#define PLAY_SOUND_LAUNCH_KIBBLE        PlayFx(CHANNEL_2, 30, 0x81, 0x82, 0xF7, 0x86); delay(90); PlayFx(CHANNEL_2, 30, 0x85, 0xA2, 0xA0, 0x86);
#define PLAY_SOUND_JUMP_ON_PLUTO_HEAD   PlayFx(CHANNEL_1, 30, 0x4A, 0xc4, 0x94, 0x19, 0x86); delay(20); PlayFx(CHANNEL_1, 30, 0x4A, 0xc4, 0x94, 0x19, 0x86);

// #define PLAY_SOUND_LEVEL_COMPLETE PlayFx(CHANNEL_1, 30, 0x76, 0x8A, 0xA6, 0x7D, 0x86); delay(150);
#define PLAY_SOUND_LOST_A_LIFE          gbt_pause(0); PlayFx(CHANNEL_1, 30,  0x4D, 0x94, 0x96, 0x26, 0x87); PlayFx(CHANNEL_4, 30, 0x26, 0x35, 0x32, 0xC0); delay(700);
#define PLAY_SOUND_OUCH_DEDUCT_HEALTH   PlayFx(CHANNEL_1, 30, 0x3F, 0x4A, 0x67, 0x46, 0xC7); // PlayFx(CHANNEL_4, 30, 0x3F, 0xAB, 0x62, 0xC0);
#define PLAY_SOUND_EAT_KIBBLE           PlayFx(CHANNEL_2, 30, 0x00, 0x82, 0x06, 0xC7); delay(150); PlayFx(CHANNEL_2, 30, 0x00, 0x82, 0x06, 0xC7); delay(80); PlayFx(CHANNEL_2, 30, 0x41, 0x94, 0x22, 0x87); // PlayFx(CHANNEL_2, 30, 0x85, 0xA3, 0xAF, 0xC6);
#define PLAY_SOUND_WALTER_GOT_KIBBLE    PlayFx(CHANNEL_2, 30, 0x00, 0x92, 0xA4, 0xB6); delay(125); PlayFx(CHANNEL_2, 30, 0x00, 0x92, 0x5D, 0x86);


#define PLAY_SOUND_WALTER_SEAL_OFF      PlayFx(CHANNEL_1, 30, 0x4A, 0xc4, 0x74, 0x19, 0x86); delay(90);

#define PLAY_SOUND_CHECKLIST_ITEM       PlayFx(CHANNEL_2, 0, 0x81, 0x62, 0xF0, 0x86); delay(80); PlayFx(CHANNEL_2, 0, 0x81, 0x82, 0x30, 0x87);


// Centralized Sound Effects
void Sound_Init(void);
void Sound_Volume_Fadeout(void);
void Sound_GBTStopAndRestoreAudio(void);

void PlaySound_EndOfLevel(void);
void PlaySound_BeatLevelBoss(void);
void PlaySound_TextMessage(void);


#endif // AUDIO_COMMON_H