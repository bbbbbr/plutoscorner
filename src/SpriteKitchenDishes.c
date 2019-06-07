//
// SpriteKitchenDishes.c
//

#pragma bank 2
#include "main.h"
UINT8 bank_SPRITE_KITCHEN_DISHES = 2;

#include "ZGBMain.h"
#include "Sound.h"
#include "SpriteManager.h"

#include "AudioCommon.h"
#include "MatchyChecklist.h"

#include "SpriteKitchenDishes.h"


// {state count, start index, end index}
const UINT8 kitchen_anim_bowl_warm[]   = {2, 0, 1};
const UINT8 kitchen_anim_bowl_break[]  = {3, 2, 3, 8};
const UINT8 kitchen_anim_cup_warm[]    = {2, 4, 5};
const UINT8 kitchen_anim_cup_break[]   = {3, 6, 7, 8};


// Next spawn locations in tile coord units, i.e.: x 8
// (x,y, player->x limit)
// -> "x limit" keeps player within scroll bounds until current sprite has been remove
//UINT8 dish_spawn_loc[12]; // Array gets initialized at the start of the KitchenCounter levels

UINT8 * dish_spawn_loc; // Array gets initialized at the start of the KitchenCounter levels

#define DISH_SPAWN_START       0 // index 0 of spawn table
UINT8   dish_spawn_max;
// #define DISH_SPAWN_MAX         (UINT8) (sizeof(dish_spawn_loc) / 3) -1

struct DishInfo* dish_data;

// from player sprite
extern UINT16 player_x_limit;


void StartDie_SPRITE_KITCHEN_DISHES() {
    PLAY_SOUND_BREAK_DISHES;

    dish_data->die = 40;
    dish_data->vy  = 0;
}


void Respawn_SPRITE_KITCHEN_DISHES() {

    dish_data->vy = 0; // Set velocity Y to 0
    dish_data->die  = 0;

    THIS->x = dish_spawn_loc[(dish_data->spawn_count) * 3] * 8;
    THIS->y = dish_spawn_loc[(dish_data->spawn_count) * 3 + 1] * 8;

    // Update map scrolling limit
    player_x_limit = dish_spawn_loc[(dish_data->spawn_count) * 3 + 2] * 8;


    if (((dish_data->spawn_count) % 2) == 0) {
        SetSpriteAnim(THIS, kitchen_anim_bowl_warm, 4);
        THIS->coll_x = 3;
        THIS->coll_w = 10;
    } else {
        SetSpriteAnim(THIS, kitchen_anim_cup_warm, 4);
        THIS->coll_x = 6;
        THIS->coll_w = 6;
    }
}


void Start_SPRITE_KITCHEN_DISHES() {
    // Assign the global sprite custom data pointer
    dish_data = (struct DishInfo*)THIS->custom_data;

    dish_data->spawn_count = DISH_SPAWN_START;

    THIS->coll_y = 8;
    THIS->coll_h = 8;

    THIS->lim_x = 1000;  // Not used right now. See: SPRITE_OFFSCREEN_AUTO_REMOVE_ENABLED
    THIS->lim_y = 1000;

    Respawn_SPRITE_KITCHEN_DISHES();
}


void Update_SPRITE_KITCHEN_DISHES() {

    if (dish_data->die) {
        if ((dish_data->spawn_count) % 1)
            SetSpriteAnim(THIS, kitchen_anim_bowl_break, 8);
        else
            SetSpriteAnim(THIS, kitchen_anim_cup_break, 16);

        dish_data->die--;

        if (dish_data->die == 0) {

            if (dish_data->spawn_count++ < dish_spawn_max) {
                Respawn_SPRITE_KITCHEN_DISHES();
            }
            else {
                // No more respawning, this is the end
                SpriteManagerRemoveSprite(THIS);

                // Play end of level sound
                PlaySound_EndOfLevel();

                // Move to next level
                if (current_state == STATE_KITCHEN_COUNTER_1) {
                    Sound_Volume_Fadeout();
                    SetState(STATE_KITCHEN_COUNTER_2);
                }
                else if (current_state == STATE_KITCHEN_COUNTER_2) {
                    // Implied: SetState(STATE_KITCHEN_COUNTER_3);
                    matchy_checklist_status = CHECKLIST_DISHES_DONE;
                    Sound_Volume_Fadeout();
                    SetState(STATE_MATCHY_CHECKLIST);
                }
            }
        }

    }

    else if (dish_data->vy) {
        // If dish is falling then move it down until it hits something
        if (TranslateSprite(THIS, 0, dish_data->vy) || THIS->y >= SPRITE_DISHES_FALL_Y_MAX) {
            StartDie_SPRITE_KITCHEN_DISHES();
        }
    }
}

void Destroy_SPRITE_KITCHEN_DISHES() {
}