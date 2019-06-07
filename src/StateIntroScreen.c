#pragma bank 2
#include "main.h"
UINT8 bank_STATE_INTRO_SCREEN = 2;

#include "../res/src/intro_screen_tiles.h"
#include "../res/src/intro_screen_map.h"
#include "AudioCommon.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Keys.h"

#include "zgb_utils.h"
#include "StatusWindow.h"

#include "PlayerHealth.h"
#include "MatchyChecklist.h"

void Start_STATE_INTRO_SCREEN() {

    // Initialize Sound
    Sound_Init();


	// == BACKGROUND TILE MAP
	// first tile (loc in tile bank), n tiles,  tile_data, tile_bank
	InitScrollTiles(0, 172, intro_screen_tiles, 3);
	InitScroll(intro_screen_mapWidth, intro_screen_mapHeight, intro_screen_map, 0, 0, 3);
	SHOW_BKG;

    StatusWindow_InitWindowAndFontTiles();
	StatusWindow_Hide();


    SET_PRINT_TARGET(PRINT_BKG);
    PRINT(4, 15, "PRESS  START", 0);


    // Do some game Init
    PlayerHealth_Reset(PLAYER_HEALTH_MAX);
    PlayerLives_Reset();
    matchy_checklist_status = CHECKLIST_INIT;

    PlayMusic(TRACK7_VAR, TRACK7_BANK, 1); // param3 loop = yes
}


void Update_STATE_INTRO_SCREEN() {

    // Wait for player to press start
    if (KEY_TICKED(J_START)) {
        // Go to Game Intro Sequence
        Sound_Volume_Fadeout();
        SetState(STATE_INTRO_DREAM_SCREEN);
    }
    else if (KEY_PRESSED(J_SELECT)) {

        if (KEY_PRESSED(J_A))
            PlayMusic(TRACK7_VAR, TRACK7_BANK, 1); // param3 loop = yes
            //PlayMusic(TRACK1_VAR, TRACK1_BANK, 1); // param3 loop = yes
        if (KEY_PRESSED(J_B))
            PlayMusic(TRACK2_VAR, TRACK2_BANK, 1); // param3 loop = yes
        if (KEY_PRESSED(J_UP))
            PlayMusic(TRACK3_VAR, TRACK3_BANK, 1); // param3 loop = yes
        if (KEY_PRESSED(J_DOWN))
            PlayMusic(TRACK4_VAR, TRACK4_BANK, 1); // param3 loop = yes
        if (KEY_PRESSED(J_LEFT))
            PlayMusic(TRACK5_VAR, TRACK5_BANK, 1); // param3 loop = yes
        if (KEY_PRESSED(J_RIGHT))
            PlayMusic(TRACK6_VAR, TRACK6_BANK, 0); // param3 loop = NOS
    }


}

