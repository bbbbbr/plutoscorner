//
// SpriteKitchenDishes.c
//

#pragma bank 2
#include "main.h"
UINT8 bank_SPRITE_CATKIBBLE = 2;

#include "ZGBMain.h"
#include "Sound.h"
#include "SpriteManager.h"

#include "zgb_utils.h"
#include "StatusWindow.h"

#include "SpriteCatKibble.h"


// {state count, start index, end index}
const UINT8 kibble_anim_eat[]        = {3, 1, 2, 3};
const UINT8 kibble_anim_readytoeat[] = {2, 1, 4};

const INT8 kibble_launch[] ={ 3, -9,  // Mid Upper Right
                             -3, -9,  // Mid Upper Left
                             -3, -14, // Upper Left
                              3, -14, // Upper Right
                              };

#define KIBBLE_RANDOM_MASK 0x03


extern UINT8  frame_count;
static UINT8  sprite_catkibble_destination;

void Start_SPRITE_CATKIBBLE() {
    struct CatKibbleInfo* data = (struct CatKibbleInfo*)THIS->custom_data;

    THIS->coll_x = 0;
    THIS->coll_y = 2;
    THIS->coll_w = 9;
    THIS->coll_h = 6;

    THIS->lim_x = 1000;  // Not used right now. See: SPRITE_OFFSCREEN_AUTO_REMOVE_ENABLED
    THIS->lim_y = 1000;

    data->vx = 0;
    data->vy = 0;

    data->state = CATKIBBLE_HIDDEN;
    // data->state = CATKIBBLE_RESET;
}


void Update_SPRITE_CATKIBBLE() {

    if ((frame_count % 2) == 1) {
        struct CatKibbleInfo* data = (struct CatKibbleInfo*)THIS->custom_data;

        switch (data->state) {

            case CATKIBBLE_HIDDEN:
                SpriteChangeDefaultTile(THIS, SPRITE_CATKIBBLE, 5); // Set sprite to hidden
                break;

            case CATKIBBLE_IN_BOX:
                // Ready to launch, do nothing
                break;

            case CATKIBBLE_TO_LAUNCH:
                // Set the sprite to visible and start moving it
                SetSpriteAnim(THIS, kibble_anim_readytoeat, 4);
                SpriteChangeDefaultTile(THIS, SPRITE_CATKIBBLE, 0); // Set sprite to visible (non-animated)

                // Select a semi-random destination for kibble
                // mask ensures it's within array bounds
                sprite_catkibble_destination = ((UINT8)DIV_REG & KIBBLE_RANDOM_MASK) * 2;

                data->vx = kibble_launch[sprite_catkibble_destination];
                data->vy = kibble_launch[sprite_catkibble_destination + 1];
                data->state = CATKIBBLE_LAUNCHED;
                break;

            case CATKIBBLE_LAUNCHED:
                if (TranslateSprite(THIS, data->vx, data->vy)) {
                    // Stop sprite from moving and change to next state
                    data->state = CATKIBBLE_READY_TO_EAT;
                    data->vx = 0;
                    data->vy = 0;
                } else {
                    data->vy += SPRITE_CATKIBBLE_GRAVITY;
                    if (data->vy > SPRITE_CATKIBBLE_TERM_VELOC)
                        data->vy = SPRITE_CATKIBBLE_TERM_VELOC;
                }
                break;

            case CATKIBBLE_READY_TO_EAT:
                break;

            case CATKIBBLE_EAT_START:
                SetSpriteAnim(THIS, kibble_anim_eat, 8);
                data->state = CATKIBBLE_EAT_COUNTDOWN;
                data->count = SPRITE_CATKIBBLE_EAT_COUNT;
                break;

            case CATKIBBLE_EAT_COUNTDOWN:
                data->count--;
                if (data->count == 0)
                    data->state = CATKIBBLE_RESET;
                break;

            case CATKIBBLE_RESET:
                SetSpriteAnim(THIS, 0, 0); // Clear animation
                SpriteChangeDefaultTile(THIS, SPRITE_CATKIBBLE, 0); // Reset default tile
                data->state = CATKIBBLE_IN_BOX;
                THIS->x = SPRITE_CATKIBBLE_START_X;
                THIS->y = SPRITE_CATKIBBLE_START_Y;
                break;
        }
    }

}

void Destroy_SPRITE_CATKIBBLE() {
}