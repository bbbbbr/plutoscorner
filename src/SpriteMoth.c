// SpriteMoth.c

#pragma bank 2
#include "main.h"
UINT8 bank_SPRITE_MOTH = 2;

#include "ZGBMain.h"
#include "Sound.h"
#include "SpriteManager.h"

#include "AudioCommon.h"
#include "MatchyChecklist.h"

#include "SpriteMoth.h"

#define ENEMY_MOTH_DIST_TRAVEL 3*8 -4 // 40 pixels travel, or 5 tiles
// {state count, start index, end index}
const UINT8 moth_anim_fly[]  = {2, 0, 1};
const UINT8 moth_anim_die[]  = {3, 2, 3, 4};


const UINT8 * p_spawn_loc;
const UINT8 * p_spawn_max;

extern const UINT8 moth_spawn_living_room_1[];
extern const UINT8 moth_spawn_max_living_room_1;

extern const UINT8 moth_spawn_living_room_2[];
extern const UINT8 moth_spawn_max_living_room_2;

#define SPAWN_START 0 // index 0 of spawn table
// #define SPAWN_MAX  (UINT8) (sizeof(spawn_loc) / 3) -1


// from player sprite
extern UINT16 player_x_limit;


void Respawn_SPRITE_MOTH();
void HandleEaten_SPRITE_MOTH();
void Start_SPRITE_MOTH();
void Update_SPRITE_MOTH();
void Destroy_SPRITE_MOTH();


void HandleEaten_SPRITE_MOTH() {
    struct SprMothInfo* data = (struct SprMothInfo*)THIS->custom_data;

    if (data->spawn_count++ < *p_spawn_max) {
        Respawn_SPRITE_MOTH();
    }
    else {
        // No more respawning, this is the end
        SpriteManagerRemoveSprite(THIS);

        PlaySound_EndOfLevel();

        // Move to next level
        switch (current_state) {
            case STATE_LIVING_ROOM_1:
                Sound_Volume_Fadeout();
                SetState(STATE_LIVING_ROOM_2);
                break;

            case STATE_LIVING_ROOM_2:
                // Implied: SetState(STATE_PLUTO_WINDOW);
                matchy_checklist_status = CHECKLIST_MOTHS_DONE;
                Sound_Volume_Fadeout();
                SetState(STATE_MATCHY_CHECKLIST);
                break;
        }
    }
}


// Load params for next spawn from shared const array (specified in levels)
void Respawn_SPRITE_MOTH() {
    struct SprMothInfo* data = (struct SprMothInfo*)THIS->custom_data;

    data->vy = 1; // Set velocity Y to 1
    data->die  = 0;

    data->vtravelamount = p_spawn_loc[(data->spawn_count) * 4 + 3];
    data->vcount = data->vtravelamount;

    THIS->x = p_spawn_loc[(data->spawn_count) * 4] * 8;
    THIS->y = p_spawn_loc[(data->spawn_count) * 4 + 1] * 8;

    // Update map scrolling limit
    player_x_limit = p_spawn_loc[(data->spawn_count) * 4 + 2] * 8;

    SetSpriteAnim(THIS, moth_anim_fly, 8);
}


void Start_SPRITE_MOTH() {
    struct SprMothInfo* data = (struct SprMothInfo*)THIS->custom_data;


    // Load moth spawn info
    switch (current_state) {
        case STATE_LIVING_ROOM_1:
            p_spawn_loc = &moth_spawn_living_room_1[0];
            p_spawn_max = &moth_spawn_max_living_room_1;
            break;

        case STATE_LIVING_ROOM_2:
            p_spawn_loc = &moth_spawn_living_room_2[0];
            p_spawn_max = &moth_spawn_max_living_room_2;
            break;
    }

    data->spawn_count = SPAWN_START;

    THIS->coll_x = 0;
    THIS->coll_y = 0;
    THIS->coll_w = 8;
    THIS->coll_h = 6;

    THIS->lim_x = 1000;  // Not used right now. See: SPRITE_OFFSCREEN_AUTO_REMOVE_ENABLED
    THIS->lim_y = 1000;

    Respawn_SPRITE_MOTH();
}


void Update_SPRITE_MOTH() {
    struct SprMothInfo* data = (struct SprMothInfo*)THIS->custom_data;

    if (data->die) {
        SetSpriteAnim(THIS, moth_anim_die, 16);
        data->die--;

        if (data->die == 0)
            HandleEaten_SPRITE_MOTH();

    }
    else {
        // If optimizing is needed, do this every other frame

        // Move enemy sprite UP by velocity Y, reverse if bumping an object
        if (TranslateSprite(THIS, 0, data->vy) || ((data->vcount--) == 0)) {
            // If the sprite bumped into a wall, invert direction
            data->vy = -data->vy;
            data->vcount = data->vtravelamount;
        }
    }
}

void Destroy_SPRITE_MOTH() {
}