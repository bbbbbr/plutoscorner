#include "ZGBMain.h"
#include "Math.h"

#include "PlayerHealth.h"
#include "MatchyChecklist.h"

UINT8 next_state = STATE_INTRO_SCREEN;
// UINT8 next_state = STATE_INTRO_DREAM_SCREEN;
// UINT8 next_state = STATE_JOSIE_CALL;
// UINT8 next_state = STATE_MATCHY_CHECKLIST;
// UINT8 next_state = STATE_KITCHEN_COUNTER_1;
// UINT8 next_state = STATE_KITCHEN_COUNTER_2;
// UINT8 next_state = STATE_KITCHEN_COUNTER_3;
// UINT8 next_state = STATE_LIVING_ROOM_1;
// UINT8 next_state = STATE_LIVING_ROOM_2;
// UINT8 next_state = STATE_PLUTO_WINDOW;
// UINT8 next_state = STATE_GAMEOVER_SCREEN;
// UINT8 next_state = STATE_END_GAME_SCREEN;
// UINT8 next_state = STATE_CREDITS;

UINT8 playerhealth;
UINT8 playerlives;

UINT8 matchy_checklist_status;

UINT8 shared_counter; // Re-used on multiple levels

// This function is called for any tile that its placed in the background.
// * It receives a UINT8* pointing to the tile that will be placed
// * It must return an Sprite_Type or 255 to spawn nothing.
// * It must assign on tile the tile index that should go in
//   the background (usually the same that was passed on
//   tile_ptr except when spawning an enemy)
// * This default function will spawn and enemy of type 0
//   when the tile is 255, and enemy of type 1 when the
//   tile is 254, and enemy of type 2 when the tile is 253... and so on
/*
UINT8 GetTileReplacement(UINT8* tile_ptr, UINT8* tile) {


	if(current_state == STATE_LIVING_ROOM) {
		if(U_LESS_THAN(255 - (UINT16)*tile_ptr, N_SPRITE_TYPES)) {
			*tile = 0;
			return 255 - (UINT16)*tile_ptr;
		}

		*tile = *tile_ptr;
	}
	return 255u;
}
*/
