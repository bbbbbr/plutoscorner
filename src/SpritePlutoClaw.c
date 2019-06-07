#pragma bank 2
#include "main.h"
#include "SpriteManager.h"

UINT8 bank_SPRITE_PLUTO_CLAW = 2;

extern struct Sprite * spr_pluto_claw_left;
extern struct Sprite * spr_pluto_claw_right;


void Start_SPRITE_PLUTO_CLAW() {
    struct EnemyInfo* data = (struct EnemyInfo*)THIS->custom_data;

    THIS->coll_x = 4;
    THIS->coll_y = 0;
    THIS->coll_w = 9;
    THIS->coll_h = 16;

    // Use OAM background priorty flag to draw sprite behind background
    THIS->flags |= S_PRIORITY;
}

void Update_SPRITE_PLUTO_CLAW() {

}

void Destroy_SPRITE_PLUTO_CLAW() {
}