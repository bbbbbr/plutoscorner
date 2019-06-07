#pragma bank 2
#include "main.h"
UINT8 bank_STATE_JOSIE_CALL = 2;

#include "../res/src/josie_call_tiles.h"
#include "../res/src/josie_call_map.h"
#include "AudioCommon.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Keys.h"

#include "StatusWindow.h"
#include "zgb_utils.h"

#include "MatchyChecklist.h"

// Intro text message sequence timing
#define INTRO_CALL_SEQ_MSG_1  5
#define INTRO_CALL_SEQ_MSG_2  5 + INTRO_CALL_SEQ_MSG_1
#define INTRO_CALL_SEQ_MSG_3  10 + INTRO_CALL_SEQ_MSG_2
#define INTRO_CALL_SEQ_MSG_4  12 + INTRO_CALL_SEQ_MSG_3
#define INTRO_CALL_SEQ_END    80


extern UINT8 shared_counter; // Re-used on multiple levels

void Start_STATE_JOSIE_CALL() {

    // Initialize Sound
    Sound_Init();


	// == BACKGROUND TILE MAP
	// first tile (loc in tile bank), n tiles,  tile_data, tile_bank
	InitScrollTiles(0, 93, josie_call_tiles, 3);
	InitScroll(josie_call_mapWidth, josie_call_mapHeight, josie_call_map, 0, 0, 3);
	SHOW_BKG;

	StatusWindow_Hide();

	shared_counter = 0;
}


void Update_STATE_JOSIE_CALL() {

    shared_counter++;
    delay(100);

    switch (shared_counter) {
        case INTRO_CALL_SEQ_MSG_1:
            PlaySound_TextMessage();
            SET_PRINT_TARGET(PRINT_BKG);
            PRINT(5, 1, "YO\n"
                        "MATCHY!", 30);
            break;

        case INTRO_CALL_SEQ_MSG_2:
            PRINT(5, 4, "HEARD\n"
                        "PLUTO'S\n"
                        "ON THE\n"
                        "PROWL.", 30);
            break;

        case INTRO_CALL_SEQ_MSG_3:
            PlaySound_TextMessage();
            PRINT(5, 10, "GONNA\n"
                         "CHECK\n"
                         "IT OUT?", 30);
            break;

        case INTRO_CALL_SEQ_MSG_4:
            PlaySound_TextMessage();
            PRINT(2, 14, "I'M\n"
                         "ON IT!", 30);
            break;

    }

	// Wait for player to press start
	if (KEY_TICKED(J_ANYKEY) || (shared_counter > INTRO_CALL_SEQ_END)) {
		// Go to first level
        // SetState(STATE_KITCHEN_COUNTER_1);
        Sound_Volume_Fadeout();
    	SetState(STATE_MATCHY_CHECKLIST);
    }
}

