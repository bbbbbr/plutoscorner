//
// SpriteHeart.c
//

#pragma bank 2
#include "main.h"
UINT8 bank_SPRITE_HEART = 2;

#include "ZGBMain.h"
#include "Sound.h"
#include "Scroll.h"
#include "SpriteManager.h"

#include "AudioCommon.h"
#include "StatusWindow.h"


extern UINT8 playerlives;

void Start_SPRITE_HEART() {
    // Set collision bounding box
    THIS->coll_x = 0;
    THIS->coll_y = 0;
    THIS->coll_w = 13;
    THIS->coll_h = 12;
}


void Update_SPRITE_HEART() {

    // Check collision with the player (scroll_target)
    if(CheckCollision(THIS, scroll_target)) {
        // Add a life
        playerlives++;

        // Update the status window
        StatusWindow_SetPlayerLives();

        PLAY_SOUND_EAT_KIBBLE;

        // Warp Player up to top of screen
        if (current_state == STATE_LIVING_ROOM_2) {
            scroll_target->y = 0;
        }

        // Remove heart now that it's been used
        SpriteManagerRemoveSprite(THIS);
    }


}

void Destroy_SPRITE_HEART() {
}