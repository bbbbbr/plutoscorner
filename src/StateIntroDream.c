#pragma bank 2
#include "main.h"
UINT8 bank_STATE_INTRO_DREAM_SCREEN = 2;

#include "../res/src/end_game_tiles.h" // NOTE: Uses same tile set as end-game screen
#include "../res/src/intro_dream_map.h"
#include "AudioCommon.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Keys.h"

#include "StatusWindow.h"

#include "zgb_utils.h"

// IMPORTANT
#define INTRO_DREAM_RES_BANK 1

#define INTRO_DREAM_TILE_EYE_OPEN     130
#define INTRO_DREAM_TILE_EYE_HALFOPEN 131
#define INTRO_DREAM_TILE_EYE_CLOSED    96
#define INTRO_DREAM_TILE_Z             132

#define INTRO_DREAM_SEQ_HALF_1  8
#define INTRO_DREAM_SEQ_OPEN_1  6 + INTRO_DREAM_SEQ_HALF_1
#define INTRO_DREAM_SEQ_HALF_2 13 + INTRO_DREAM_SEQ_OPEN_1
#define INTRO_DREAM_SEQ_OPEN_2  6 + INTRO_DREAM_SEQ_HALF_2
#define INTRO_DREAM_SEQ_HALF_3 13 + INTRO_DREAM_SEQ_OPEN_2
#define INTRO_DREAM_SEQ_CLOSED 10 + INTRO_DREAM_SEQ_HALF_3
#define INTRO_DREAM_SEQ_Z_2     6 + INTRO_DREAM_SEQ_CLOSED
#define INTRO_DREAM_SEQ_Z_3     6 + INTRO_DREAM_SEQ_Z_2
#define INTRO_DREAM_SEQ_Z_4     6 + INTRO_DREAM_SEQ_Z_3
#define INTRO_DREAM_SEQ_END    24 + INTRO_DREAM_SEQ_Z_4


extern UINT8 shared_counter; // Re-used on multiple levels


void Start_STATE_INTRO_DREAM_SCREEN() {

	StatusWindow_Hide();

	// == BACKGROUND TILE MAP
	// first tile (loc in tile bank), n tiles,  tile_data, tile_bank
	InitScrollTiles(0, 133, end_game_tiles, INTRO_DREAM_RES_BANK);
	InitScroll(intro_dream_mapWidth, intro_dream_mapHeight, intro_dream_map, 0, 0, INTRO_DREAM_RES_BANK);
	SHOW_BKG;

	shared_counter = 0;

    PlayMusic(TRACK4_VAR, TRACK4_BANK, 0); // param3 loop = NO
    // NR50_REG = 0x44; // Lower the volume since the mod track can only get so quiet before it starts to make "clicking" sounds
}


void Update_STATE_INTRO_DREAM_SCREEN() {

	shared_counter++;
	delay(100);

	// Eyes blinking and going to sleep sequence
	switch (shared_counter) {
		case INTRO_DREAM_SEQ_HALF_1:
		case INTRO_DREAM_SEQ_HALF_2:
		case INTRO_DREAM_SEQ_HALF_3:
			UpdateMapTile(10,11, INTRO_DREAM_TILE_EYE_HALFOPEN, 0);
			UpdateMapTile(12,11, INTRO_DREAM_TILE_EYE_HALFOPEN, 0);
			break;

		case INTRO_DREAM_SEQ_OPEN_1:
		case INTRO_DREAM_SEQ_OPEN_2:
			UpdateMapTile(10,11, INTRO_DREAM_TILE_EYE_OPEN, 0);
			UpdateMapTile(12,11, INTRO_DREAM_TILE_EYE_OPEN, 0);
			break;

		case INTRO_DREAM_SEQ_CLOSED:
			UpdateMapTile(10,11, INTRO_DREAM_TILE_EYE_CLOSED, 0);
			UpdateMapTile(12,11, INTRO_DREAM_TILE_EYE_CLOSED, 0);
            UpdateMapTile(8,8, INTRO_DREAM_TILE_Z, 0);
			break;

        case INTRO_DREAM_SEQ_Z_2:
            UpdateMapTile(7,6, INTRO_DREAM_TILE_Z, 0);
            break;

        case INTRO_DREAM_SEQ_Z_3:
            UpdateMapTile(9,3, INTRO_DREAM_TILE_Z, 0);
            break;

        case INTRO_DREAM_SEQ_Z_4:
            UpdateMapTile(6,1, INTRO_DREAM_TILE_Z, 0);
            break;

		case INTRO_DREAM_SEQ_END:
            Sound_Volume_Fadeout();
			SetState(STATE_JOSIE_CALL);
			break;
	}


	// Wait for player to press start
	// Note: KEY_TICKED is less CPU intensive /
	//       non-blocking than waitpad(J_START);
	if (KEY_TICKED(J_ANYKEY)) {
		// Go to first level
        Sound_Volume_Fadeout();
    	SetState(STATE_JOSIE_CALL);
    }
}

