#pragma bank 2
#include "main.h"
UINT8 bank_STATE_LIVING_ROOM_1 = 2;

#include "../res/src/living_room_shared_tiles.h"
#include "../res/src/living_room_map_1.h"
#include "AudioCommon.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"

#include "StatusWindow.h"

const UINT8 living_room_collision_tiles[] = {13, 14, 15,  // Lamp top
                                             21, 24, 25, // Lamp middle

                                            // 43, // lamp post knob
											35, 36, 37, // Ledge
											39, 40, 41, 42,  // Fancy Ledge
											46, 47, 48, 49,  // Couch
                                            62, 71, // couch cushion
//                                            60, // Couch left diagonal
//											45, // Veritcal Left
											70, 74, // Vertical islands
											77, 78, 79, 80, 81, // Desk
										    0}; // Terminates with Zero/0

// Next spawn locations in tile coord units, i.e.: x 8
// (x,y, player->x limit, y travel (in pixels))
// -> "x limit" keeps player within scroll bounds until current moth has been caught
const UINT8 moth_spawn_living_room_1[] = { 6,  0,  9, 15,
                                          10,  0, 14, 21,
                                          19,  2, 18, 30,
                                          21, 11, 26, 30,
                                          32,  0, 39, 24,
                                          42, 13, 49, 10,
                                          52,  0, 57, 20,
                                          65,  7, 71, 48,
                                          65,  0, 71, 32,
                                          77,  9, 79, 32};

const UINT8 moth_spawn_max_living_room_1 = (UINT8) (sizeof(moth_spawn_living_room_1) / 4) -1;

void Start_STATE_LIVING_ROOM_1() {

    // Initialize Sound
    Sound_Init();


	// == BACKGROUND TILE MAP
	//  InitScrollTiles(first_tile, n_tiles, tile_data, tile_bank)
	InitScrollTiles(0, 82, living_room_tiles_2, 1);
	InitScroll(living_room_map_1Width, living_room_map_1Height, living_room_map_1, living_room_collision_tiles, 0, 1);
	SHOW_BKG;


	// == STATUS WINDOW
	// Note: This loads/uses BG tiles and the end of the Tile Ram
	StatusWindow_Init();


	// == SPRITES
	// TODO: should this be 16x16?
	SPRITES_8x16;
	SpriteManagerLoad(SPRITE_PLAYER);
	SpriteManagerLoad(SPRITE_MOTH);
	SHOW_SPRITES;

	scroll_target = SpriteManagerAdd(SPRITE_PLAYER, 4*8, 2*8);

	// Spawn a moth
    SpriteManagerAdd(SPRITE_MOTH, 4*8, 3*8);

    PlayMusic(TRACK3_VAR, TRACK3_BANK, 1); // param3 loop = yes
}

void Update_STATE_LIVING_ROOM_1() {

    if (KEY_TICKED(J_START))
        StatusWindow_HandlePause();

}
