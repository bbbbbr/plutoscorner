#pragma bank 2
#include "main.h"
UINT8 bank_STATE_PLUTO_WINDOW = 2;

#include "../res/src/living_room_shared_tiles.h"
#include "../res/src/living_room_map_2.h"
#include "AudioCommon.h"

#include "StatusWindow.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"

#include "zgb_utils.h"
#include "PlayerHealth.h"

const UINT8 pluto_window_collision_tiles[] = {2, // Dither Tile
                                              50, 51, 52, 53, // Window Bottom Edge
                                              77, 78, 79, 80, 81, // Desk
                                              0}; // Terminates with Zero/0


struct Sprite * spr_pluto_head;
struct Sprite * spr_pluto_claw_left;
struct Sprite * spr_pluto_claw_right;

void Start_STATE_PLUTO_WINDOW() {

    // Initialize Sound
    Sound_Init();


	// == BACKGROUND TILE MAP
	// first tile (loc in tile bank), n tiles,  tile_data, tile_bank
    InitScrollTiles(0, 82, living_room_tiles_2, 1);
    InitScroll(living_room_map_2Width, living_room_map_2Height, living_room_map_2, pluto_window_collision_tiles, 0, 1);
	SHOW_BKG;


	// == STATUS WINDOW
	// Note: This loads/uses BG tiles and the end of the Tile Ram
	StatusWindow_Init();


	// == SPRITES
	// TODO: should this be 16x16?
	SPRITES_8x16;
		SpriteManagerLoad(SPRITE_PLAYER);
		SpriteManagerLoad(SPRITE_PLUTO_HEAD);
		SpriteManagerLoad(SPRITE_PLUTO_CLAW);
	SHOW_SPRITES;


#define PLAYER_START_X  (80-9)*8
#define PLAYER_START_Y  6*8

#define PLUTO_START_X  70 * 8 // (80-8)*8
#define PLUTO_START_Y  112 - 12
#define PLUTO_CLAW_OFFSET 2*8

// Relocate the map down to the bottom of the level
// --> Should roughly match PLAYER_START_(X/Y)
#define MAP_START_X     (80 -20) * 8
#define MAP_START_Y            0 * 8


    ScrollRelocateMapTo(MAP_START_X, MAP_START_Y);
    scroll_target = SpriteManagerAdd(SPRITE_PLAYER, PLAYER_START_X, PLAYER_START_Y);

	spr_pluto_claw_left  = 0;
	spr_pluto_claw_right = 0;

	spr_pluto_claw_left  = SpriteManagerAdd(SPRITE_PLUTO_CLAW, PLUTO_START_X - PLUTO_CLAW_OFFSET +3, PLUTO_START_Y + 4); // Left Claw
	spr_pluto_claw_right = SpriteManagerAdd(SPRITE_PLUTO_CLAW, PLUTO_START_X + PLUTO_CLAW_OFFSET -2, PLUTO_START_Y + 4); // Right Claw
	// NOTE: Does adding the head after the claws change the detection order and make it harder?
	spr_pluto_head       = SpriteManagerAdd(SPRITE_PLUTO_HEAD, PLUTO_START_X,  PLUTO_START_Y);

    PlayMusic(TRACK1_VAR, TRACK1_BANK, 1); // param3 loop = yes
}


void Update_STATE_PLUTO_WINDOW() {

    if (KEY_TICKED(J_START))
        StatusWindow_HandlePause();

}
