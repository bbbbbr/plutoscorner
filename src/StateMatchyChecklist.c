#pragma bank 2
#include "main.h"
UINT8 bank_STATE_MATCHY_CHECKLIST = 2;

#include "../res/src/josie_call_tiles.h"
#include "../res/src/matchy_checklist_map.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Keys.h"

#include "AudioCommon.h"
#include "StatusWindow.h"
#include "zgb_utils.h"

#include "MatchyChecklist.h"

#define CHECKLIST_TIMEOUT  70
#define CHECKLIST_BOX_X  2 // Checkboxes X location in tiles
#define CHECKLIST_TILE_ID_CHECKED 92

const UINT8 checklist_box_y[] = {5,8,10,12,15}; // Checkbox Y location in Tiles

extern UINT8 shared_counter; // Re-used on multiple levels

void Start_STATE_MATCHY_CHECKLIST() {

    // Initialize Sound
    Sound_Init();


	// == BACKGROUND TILE MAP
	// first tile (loc in tile bank), n tiles,  tile_data, tile_bank
    // OPTIONAL: switch to load RLE compressed map since there is no scrolling
	InitScrollTiles(0, 93, josie_call_tiles, 3);
	InitScroll(matchy_checklist_mapWidth, matchy_checklist_mapHeight, matchy_checklist_map, 0, 0, 3);
	SHOW_BKG;

    StatusWindow_InitWindowAndFontTiles();
    StatusWindow_Hide();

    SET_PRINT_TARGET(PRINT_BKG);
    PRINT(2, 2, "TO DO\n", 0);
    PRINT(4, 5, "KNOCK STUFF\n"
                "OFF COUNTER\n\n"
                "EAT KIBBLE\n\n"
                "CHASE MOTHS\n\n"
                "DEFEAT\n"
                "PLUTO\n\n"
                "NAP\n"
                ,0);

    shared_counter = 0;
}


void Update_STATE_MATCHY_CHECKLIST() {

    shared_counter++;

    if (shared_counter <= matchy_checklist_status) {

        // Add a delay if it's the most recently completed item
        if (shared_counter == matchy_checklist_status) {
            delay(800);

            PLAY_SOUND_CHECKLIST_ITEM;
        }

        // Draw the checked-box via tile update
        UpdateMapTile(CHECKLIST_BOX_X,
                      checklist_box_y[shared_counter - 1],
                      CHECKLIST_TILE_ID_CHECKED, 0);

    }
    else
        delay(100);


	if (KEY_TICKED(J_ANYKEY) || (shared_counter > CHECKLIST_TIMEOUT)) {
		// Go to next
        switch (matchy_checklist_status) {
            case CHECKLIST_INIT:
                SetState(STATE_KITCHEN_COUNTER_1);
                break;

            case CHECKLIST_DISHES_DONE:
                SetState(STATE_KITCHEN_COUNTER_3);
                break;

            case CHECKLIST_KIBBLE_DONE:
                SetState(STATE_LIVING_ROOM_1);
                break;

            case CHECKLIST_MOTHS_DONE:
                SetState(STATE_PLUTO_WINDOW);
                break;

            case CHECKLIST_PLUTO_DONE:
                SetState(STATE_END_GAME_SCREEN);
                break;

            case CHECKLIST_NAP_DONE:
                SetState(STATE_CREDITS);
                break;
        }
    }
}

