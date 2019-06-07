#pragma bank 2
#include "main.h"
UINT8 bank_STATE_LIVING_ROOM_2 = 2;

#include "../res/src/living_room_shared_tiles.h"
#include "../res/src/living_room_map_2.h"
#include "AudioCommon.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"

#include "StatusWindow.h"
#include "PlayerHealth.h"

extern const UINT8 living_room_collision_tiles[];

extern UINT8 playerlives;


// Next spawn locations in tile coord units, i.e.: x 8
// (x,y, player->x limit)
// -> "x limit" keeps player within scroll bounds until current moth has been caught
const UINT8 moth_spawn_living_room_2[] = {12, 10, 17, 25,
                                           3,  0, 17, 25,
                                          24,  3, 33, 32,
                                          32, 12, 40, 32,
                                          54,  8, 56, 32,
                                          52, 11, 56, 32,
                                          67,  0, 80, 32,
                                          76,  3, 80, 32 };


const UINT8 moth_spawn_max_living_room_2 = (UINT8) (sizeof(moth_spawn_living_room_2) / 4) -1;

void Start_STATE_LIVING_ROOM_2() {

    // Initialize Sound
    Sound_Init();


	// == BACKGROUND TILE MAP
	//  InitScrollTiles(first_tile, n_tiles, tile_data, tile_bank)
	InitScrollTiles(0, 82, living_room_tiles_2, 1);
	InitScroll(living_room_map_2Width, living_room_map_2Height, living_room_map_2, living_room_collision_tiles, 0, 1);
	SHOW_BKG;


	// == STATUS WINDOW
	// Note: This loads/uses BG tiles and the end of the Tile Ram
	StatusWindow_Init();


	// == SPRITES
	// TODO: should this be 16x16?
	SPRITES_8x16;
	SpriteManagerLoad(SPRITE_PLAYER);
	SpriteManagerLoad(SPRITE_MOTH);
    SpriteManagerLoad(SPRITE_HEART);
	SHOW_SPRITES;

	scroll_target = SpriteManagerAdd(SPRITE_PLAYER, 40, 30);

	// Spawn a moth
    SpriteManagerAdd(SPRITE_MOTH, 3*8, 3*8);



    // If the player is missing some lives, add a 1-up sprite
    if (playerlives < PLAYER_LIVES_MAX) {
        SpriteManagerAdd(SPRITE_HEART, 24*8, 13*8);
    }

    PlayMusic(TRACK3_VAR, TRACK3_BANK, 1); // param3 loop = yes
}

void Update_STATE_LIVING_ROOM_2() {

    if (KEY_TICKED(J_START))
        StatusWindow_HandlePause();

}
