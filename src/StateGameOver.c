#pragma bank 2
#include "main.h"
UINT8 bank_STATE_GAMEOVER_SCREEN = 2;

#include "../res/src/game_over_tiles.h"
#include "../res/src/game_over_map.h"
#include "AudioCommon.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Keys.h"

#include "StatusWindow.h"
#include "zgb_utils.h"

#define GAME_OVER_PLUTO_TOUNGE_IN  79
#define GAME_OVER_PLUTO_TOUNGE_OUT 41
#define GAME_OVER_PLUTO_SEQ_TOUNGE_IN  8
#define GAME_OVER_PLUTO_SEQ_TOUNGE_OUT GAME_OVER_PLUTO_SEQ_TOUNGE_IN * 2

extern UINT8 shared_counter; // Re-used on multiple levels

void Start_STATE_GAMEOVER_SCREEN() {

    // Initialize Sound
    Sound_Init();


	StatusWindow_Hide();

	// == BACKGROUND TILE MAP
	// first tile (loc in tile bank), n tiles,  tile_data, tile_bank
	InitScrollTiles(0, 80, game_over_tiles, 3);
	InitScroll(game_over_mapWidth, game_over_mapHeight, game_over_map, 0, 0, 3);
	SHOW_BKG;

    shared_counter = 0;
    PlayMusic(TRACK6_VAR, TRACK6_BANK, 0); // param3 loop = NO
}


void Update_STATE_GAMEOVER_SCREEN() {

    shared_counter++;
    delay(100);

    switch (shared_counter) {
        case GAME_OVER_PLUTO_SEQ_TOUNGE_IN:
            UpdateMapTile(10,9, GAME_OVER_PLUTO_TOUNGE_IN,  0);
            break;

        case GAME_OVER_PLUTO_SEQ_TOUNGE_OUT:
            UpdateMapTile(10,9, GAME_OVER_PLUTO_TOUNGE_OUT, 0);
            shared_counter = 0;
            break;
    }

	// Wait for player to press start
	// Note: KEY_TICKED is less CPU intensive /
	//       non-blocking than waitpad(J_START);
	if (KEY_TICKED(J_ANYKEY)) {
		// Go to first level
        Sound_Volume_Fadeout();
    	SetState(STATE_INTRO_SCREEN);
    }
}

