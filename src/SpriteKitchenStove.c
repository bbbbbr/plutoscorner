#pragma bank 2
#include "main.h"
#include "SpriteManager.h"

UINT8 bank_SPRITE_KITCHEN_STOVE = 2;

struct StoveInfo {
    // This can be up to 8 bytes
    INT8 disable_time;
    INT8 spawn_count;
    INT8 spawn_index;
    INT8 spawn_min;
    INT8 spawn_max;

};

struct StoveInfo* stove_data;

// = {num frames, ..frame(s) 0 based index..}
// const UINT8 anim_kitchen_stove[] = {2, 0, 1};

static const UINT16 stove_spawn_loc[] = {35 * 8,
                                         67 * 8,
                                         38 * 8,
                                         74 * 8}; // OPTIONAL 71 * 8 is middle burner on last stove

#define STOVE_SPAWN_START       0 // index 0 of spawn table
#define STOVE_SPAWN_MAX         3 // (UINT8) (sizeof(stove_spawn_loc)) -1 <- Compiler handles this as U16, so set it manually

#define STOVE_SPAWN_ROTATE_DELAY       6

void Start_SPRITE_KITCHEN_STOVE() {

    // Assign the global sprite custom data pointer
    stove_data = (struct StoveInfo*)THIS->custom_data;

    stove_data->spawn_count = 0;
    stove_data->spawn_index = STOVE_SPAWN_START;

    THIS->coll_x = 2;
    THIS->coll_y = 1;
    THIS->coll_w = 11;
    THIS->coll_h = 3;

    THIS->lim_x = 1000;  // Not used right now. See: SPRITE_OFFSCREEN_AUTO_REMOVE_ENABLED
    THIS->lim_y = 1000;

    // This was less efficient
    // If you want to re-enable then change SPRITE_KITCHEN_STOVE needs to have 2 frames and load both
    // SetSpriteAnim(THIS, anim_kitchen_stove, 4);
}

void Update_SPRITE_KITCHEN_STOVE() {

    // Rotate the sprite through all burner locations
    // to simulate blinking them on and off
    // Doesn't handle more than 4 locations
    stove_data->spawn_index++;
    THIS->x = stove_spawn_loc[ (stove_data->spawn_index >> 3) & 0x03 ];

/*
    // Less efficient implementation of above
    // Rotate the sprite through all burner locations
    if (stove_data->spawn_count++ > STOVE_SPAWN_ROTATE_DELAY) {
        stove_data->spawn_count = 0;

        stove_data->spawn_index++;
        if (stove_data->spawn_index > STOVE_SPAWN_MAX)
            stove_data->spawn_index = STOVE_SPAWN_START;

        THIS->x = stove_spawn_loc[ stove_data->spawn_index ];
    }
*/

}

void Destroy_SPRITE_KITCHEN_STOVE() {
}