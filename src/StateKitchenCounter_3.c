#pragma bank 2
#include "main.h"
UINT8 bank_STATE_KITCHEN_COUNTER_3 = 2;

#include "../res/src/kitchencounter_tiles.h"
#include "../res/src/kitchencounter_map_3.h"
#include "AudioCommon.h"

#include "ZGBMain.h"
#include "Keys.h"
#include "SpriteManager.h"

#include "StatusWindow.h"
#include "SpritePlayer.h"
#include "SpriteKitchenDishes.h"

#include "zgb_utils.h"

#include "StatekitchenCounter_3.h"

#include "SpriteCatKibble.h"


#define KITCHEN_COUNTER_RES_BANK 1
#define KITCHEN_COUNTER_TILE_BANK 3

extern UINT8 kitchencounter_collision_tiles[]; // Reuse collision tiles from other level


// OPTIONAL: convert to AddToColList()
const UINT8 kitchencounter_collision_tiles_boss_stage[] = {14,13,15,48,52, // Counter left, top and side
                                                           44,45,46, // Stove handle below burners
                                                           26, 34,   // ADD IN: clock dial and light checker to make a wall beneath boss
                                                           // 75, 76,   // Kibble box // Kibble box got converted to a sprite
                                                           0};

#define KITCHEN_COUNTER_3_REPLACE_Y 14
#define KITCHEN_COUNTER_3_REPLACE_DELAY 60 // Pause between tiles getting replaced when sealing off boss area


#define PLAYER_START_X  3*8
#define PLAYER_START_Y  (79-5)*8 // Should roughly match map relocation in: MAP_START_Y

// Relocate the map down to the bottom of the level
#define MAP_START_X     0*8
#define MAP_START_Y     (80-18)*8 // Should roughly match: PLAYER_START_Y

extern UINT8 shared_counter;


struct Sprite * spr_catkibble;
struct Sprite * spr_walter;

void Start_STATE_KITCHEN_COUNTER_3() {

    // Initialize Sound
    Sound_Init();


    // == BACKGROUND TILE MAP
    //  InitScrollTiles(first_tile, n_tiles, tile_data, tile_bank, [CGB Palette entries])
    InitScrollTiles(0, 77, kitchencounter_tiles, KITCHEN_COUNTER_TILE_BANK);
    // ZInitScrollTilesColor_RLE(0, 77, kitchencounter_tiles, KITCHEN_COUNTER_TILE_BANK, 0);

// scroll_y = (79-18) * 8;
    InitScroll(kitchencounter_map_3Width, kitchencounter_map_3Height, kitchencounter_map_3, kitchencounter_collision_tiles, 0, KITCHEN_COUNTER_RES_BANK);
    SHOW_BKG;


    // == STATUS WINDOW
    // Note: This loads/uses BG tiles and the end of the Tile Ram
    StatusWindow_Init();


    // == SPRITES
    // TODO: should this be 16x16?
    SPRITES_8x16;
    SpriteManagerLoad(SPRITE_PLAYER);
    SpriteManagerLoad(SPRITE_CATKIBBLE);
    SpriteManagerLoad(SPRITE_WALTER);
    SHOW_SPRITES;

    ScrollRelocateMapTo(MAP_START_X, MAP_START_Y);
    scroll_target = SpriteManagerAdd(SPRITE_PLAYER, PLAYER_START_X, PLAYER_START_Y);

    spr_catkibble = SpriteManagerAdd(SPRITE_CATKIBBLE, SPRITE_CATKIBBLE_START_X, SPRITE_CATKIBBLE_START_Y);
    spr_walter    = SpriteManagerAdd(SPRITE_WALTER,   7*8-5, 14*8 -5); // 18*8-5, 2*8 -5);

    // SetColList(kitchencounter_collision_tiles_boss_stage, 0);

    // This music will get swapped for the boss music when the player reaches Walter
    PlayMusic(TRACK2_VAR, TRACK2_BANK, 1); // param3 loop = yes
}

void Update_STATE_KITCHEN_COUNTER_3() {

    if (KEY_TICKED(J_START))
        StatusWindow_HandlePause();

}


void KitchenCounter3_seal_off_boss_zone() {
    // Create a row of tiles at the bottom of the
    // boss area to seal off the rest of the level

    // The actual tiles in the map cannot change since they are in ROM
    // So instead change these:
    // * Tile Collision list
    // * Update BG tiles that are currently rendered on screen
    //   (would get overwritten if scrolled offscreen/edge of window reloaded

    // Swap to the boss stage collision list to seal off the level
    SetColList(kitchencounter_collision_tiles_boss_stage, 0);

    // Replace tiles for center

    UpdateMapTile(12,KITCHEN_COUNTER_3_REPLACE_Y,     9, 0);
    UpdateMapTile(13,KITCHEN_COUNTER_3_REPLACE_Y,     9, 0);
    UpdateMapTile(14,KITCHEN_COUNTER_3_REPLACE_Y,     43, 0);

    for (shared_counter = 0; shared_counter <= 19; shared_counter++) {
        UpdateMapTile(shared_counter,
                      KITCHEN_COUNTER_3_REPLACE_Y + 1,
                      14, 0);
        delay(KITCHEN_COUNTER_3_REPLACE_DELAY);
        PLAY_SOUND_WALTER_SEAL_OFF;
    }

    PlayMusic(TRACK1_VAR, TRACK1_BANK, 1); // param3 loop = yes
}