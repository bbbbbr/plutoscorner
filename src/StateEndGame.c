#pragma bank 2
#include "main.h"
UINT8 bank_STATE_END_GAME_SCREEN = 2;

#include "../res/src/end_game_tiles.h"
#include "../res/src/end_game_map_bg.h"
#include "../res/src/end_game_map_window.h"
#include "AudioCommon.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "Keys.h"

#include "StatusWindow.h"
#include "MatchyChecklist.h"

#define END_GAME_WINDOW_Y_SCROLL_START 18*8
#define END_GAME_WINDOW_Y_SCROLL_END   7*8
#define ENDGAME_SEQ_END                62 // OPTIONAL: use 200 for longer delay (another full loop of music)

// RAN OUT OF SPACE IN BANK 3, so put these tiles into BANK 1 with CODE
#define END_GAME_RES_BANK 1


static UINT8 window_y;
extern UINT8 shared_counter; // Re-used on multiple levels


void Start_STATE_END_GAME_SCREEN() {

	// Initialize Sound
    Sound_Init();


	StatusWindow_Hide();

	// == BACKGROUND TILE MAP
	// first tile (loc in tile bank), n tiles,  tile_data, tile_bank
	InitScrollTiles(0, 133, end_game_tiles, END_GAME_RES_BANK);
	InitScroll(end_game_map_bgWidth, end_game_map_bgHeight, end_game_map_bg, 0, 0, END_GAME_RES_BANK);
	SHOW_BKG;


// void StatusWindow_EndGameInit(void) {

    // --> (start_tile_x, start_tile_y, tile_width, tile_height, tile_map_array, Bank);
    InitWindow(0, 0, end_game_map_bgWidth, end_game_map_bgHeight, end_game_map_window, END_GAME_RES_BANK, 0);


	window_y = END_GAME_WINDOW_Y_SCROLL_START;
    WY_REG = window_y; // Window Y Coordinate
    WX_REG = 0 + HW_WINDOW_X_OFFSET;  // Start at 0, window starts offset 7 pixels to the right

    SHOW_WIN;


	PlayMusic(TRACK5_VAR, TRACK5_BANK, 1); // param3 loop = yes

    shared_counter = 0;
}


void Update_STATE_END_GAME_SCREEN() {

	// Wait, then scroll status window with matchy dreaming image up
	if (window_y == END_GAME_WINDOW_Y_SCROLL_START) {
		delay(4000);
		window_y--;
		WY_REG = window_y;
	}
	else if (window_y > END_GAME_WINDOW_Y_SCROLL_END) {
		window_y--;
		WY_REG = window_y;
		    delay(40);
	} else {
        // Scrolling complete, wait start a countdown until next screen
        shared_counter++;
        delay(100);
    }



	// Wait for player to press start
	if (KEY_TICKED(J_ANYKEY)  || (shared_counter > ENDGAME_SEQ_END)) {
        if (window_y > END_GAME_WINDOW_Y_SCROLL_END)
            // If the window is still scrolling up, skip to the end of scrolling
            window_y = END_GAME_WINDOW_Y_SCROLL_END + 1;
        else {
            // Implied: SetState(STATE_INTRO_DREAM_SCREEN);
            matchy_checklist_status = CHECKLIST_NAP_DONE;
            Sound_Volume_Fadeout();
            SetState(STATE_MATCHY_CHECKLIST);
        }
    }
}

