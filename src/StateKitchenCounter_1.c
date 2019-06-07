#pragma bank 2
#include "main.h"
UINT8 bank_STATE_KITCHEN_COUNTER_1 = 2;

#include "../res/src/kitchencounter_tiles.h"
#include "../res/src/kitchencounter_map_1.h"
#include "AudioCommon.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"

#include "StatusWindow.h"

#include "SpritePlayer.h"
#include "SpriteKitchenDishes.h"



// #include "zgb_utils.h"

// Initial spawn location for burner, but it cycles through many
#define STOVE_BURNER_SPAWN_Y (11 * 8) + 2
#define STOVE_BURNER_SPAWN_X 35 *8

#define KITCHEN_COUNTER_RES_BANK 1
#define KITCHEN_COUNTER_TILE_BANK 3

// These are shared with the other Kitchen Counter Levels
// Collision tile list terminates with Zero / 0
const UINT8 kitchencounter_collision_tiles[] = {14,13,15,48,52, // Counter left, top and side
                                                44,45,46, // Stove handle below burners
                                                0};


extern const UINT8 * dish_spawn_loc;
extern UINT8 dish_spawn_max;

// Next spawn locations in tile coord units, i.e.: x 8
// (x,y, player->x limit)
// -> "x limit" keeps player within scroll bounds until current sprite has been remove
static const UINT8 dish_spawn_array[] = {23,  5,  29,
    				                     37,  7,  47,
    				                     51,  11, 55,
    				                     73,  10, 79 };

#define DISH_SPAWN_KITCHEN_COUNTER_1 (sizeof(dish_spawn_array) / 3) -1

void Start_STATE_KITCHEN_COUNTER_1() {

    // Initialize Sound
    Sound_Init();


	// == BACKGROUND TILE MAP
	//  InitScrollTiles(first_tile, n_tiles, tile_data, tile_bank)
	InitScrollTiles(0, 69, kitchencounter_tiles, KITCHEN_COUNTER_TILE_BANK);
	InitScroll(kitchencounter_map_1Width, kitchencounter_map_1Height, kitchencounter_map_1, kitchencounter_collision_tiles, 0, KITCHEN_COUNTER_RES_BANK);
	SHOW_BKG;


	// == STATUS WINDOW
	// Note: This loads/uses BG tiles at the end of the BG Tile RAM
	StatusWindow_Init();


	// == SPRITES
	// TODO: should this be 16x16?
	SPRITES_8x16;
	SpriteManagerLoad(SPRITE_PLAYER);
	SpriteManagerLoad(SPRITE_KITCHEN_DISHES);
	// No Stove / Burner on this Kitchen level
	SHOW_SPRITES;

	scroll_target = SpriteManagerAdd(SPRITE_PLAYER, 32, 20);

	// Load dish spawn array and create a Dish Sprite
	dish_spawn_max = DISH_SPAWN_KITCHEN_COUNTER_1;
	dish_spawn_loc = &dish_spawn_array[0];
    SpriteManagerAdd(SPRITE_KITCHEN_DISHES, 3*8, 3*8);

    PlayMusic(TRACK2_VAR, TRACK2_BANK, 1); // param3 loop = yes
}

void Update_STATE_KITCHEN_COUNTER_1() {

    if (KEY_TICKED(J_START))
        StatusWindow_HandlePause();

}
