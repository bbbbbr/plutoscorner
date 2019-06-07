#pragma bank 2
#include "main.h"
UINT8 bank_STATE_KITCHEN_COUNTER_2 = 2;

#include "../res/src/kitchencounter_tiles.h"
#include "../res/src/kitchencounter_map_2.h"
#include "AudioCommon.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"

#include "StatusWindow.h"
#include "SpritePlayer.h"
#include "SpriteKitchenDishes.h"

// Initial spawn location for burner, but it cycles through many
#define STOVE_BURNER_SPAWN_Y (11 * 8) + 2
#define STOVE_BURNER_SPAWN_X 35 *8

#define KITCHEN_COUNTER_RES_BANK 1
#define KITCHEN_COUNTER_TILE_BANK 3


extern const UINT8 kitchencounter_collision_tiles[]; // Reuse collision tiles from other level

extern const UINT8 * dish_spawn_loc;
extern UINT8 dish_spawn_max;

// Next spawn locations in tile coord units, i.e.: x 8
// (x,y, player->x limit)
// -> "x limit" keeps player within scroll bounds until current sprite has been remove
static const UINT8 dish_spawn_array[] = {10, 11, 25,
                                         17,  1, 32,
                                         45,  1, 55,
                                         77,  4, 79 };

#define DISH_SPAWN_KITCHEN_COUNTER_2 (sizeof(dish_spawn_array) / 3) -1

void Start_STATE_KITCHEN_COUNTER_2() {

    // Initialize Sound
    Sound_Init();


	// == BACKGROUND TILE MAP
	//  InitScrollTiles(first_tile, n_tiles, tile_data, tile_bank)
	InitScrollTiles(0, 69, kitchencounter_tiles, KITCHEN_COUNTER_TILE_BANK);
	InitScroll(kitchencounter_map_2Width, kitchencounter_map_2Height, kitchencounter_map_2, kitchencounter_collision_tiles, 0, KITCHEN_COUNTER_RES_BANK);
	SHOW_BKG;


	// == STATUS WINDOW
	// Note: This loads/uses BG tiles and the end of the Tile Ram
	StatusWindow_Init();


	// == SPRITES
	// TODO: should this be 16x16?
	SPRITES_8x16;
	SpriteManagerLoad(SPRITE_PLAYER);
	SpriteManagerLoad(SPRITE_KITCHEN_DISHES);
	SpriteManagerLoad(SPRITE_KITCHEN_STOVE);
	SHOW_SPRITES;

	scroll_target = SpriteManagerAdd(SPRITE_PLAYER, 32, 20);

	SpriteManagerAdd(SPRITE_KITCHEN_STOVE,  STOVE_BURNER_SPAWN_X,  STOVE_BURNER_SPAWN_Y);


	// Load dish spawn array and create a Dish Sprite
	dish_spawn_max = DISH_SPAWN_KITCHEN_COUNTER_2;
	dish_spawn_loc = &dish_spawn_array[0];
    SpriteManagerAdd(SPRITE_KITCHEN_DISHES, 3*8, 3*8);

    PlayMusic(TRACK2_VAR, TRACK2_BANK, 1); // param3 loop = yes
}

void Update_STATE_KITCHEN_COUNTER_2() {

    if (KEY_TICKED(J_START))
        StatusWindow_HandlePause();

}
