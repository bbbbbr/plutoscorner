//
// SpriteKitchenDishes.c
//

#pragma bank 2
#include "main.h"
UINT8 bank_SPRITE_WALTER = 2;

#include "ZGBMain.h"
#include "Sound.h"
#include "SpriteManager.h"

#include "zgb_utils.h"
#include "StatekitchenCounter_3.h"

#include "SpriteWalter.h"
#include "SpriteCatKibble.h"
#include "SpritePlayer.h"

#include "AudioCommon.h"
#include "StatusWindow.h"

// {state count, start index, end index}
const UINT8 walter_run[]      = {4, 0, 1, 0, 2};

extern UINT8  frame_count;

extern struct Sprite * spr_catkibble;

struct WalterInfo* data;
struct CatKibbleInfo* kibble_data;


void Walter_HandleWalterWon(void) {

    // Deduct a life from the player
    Player_DeductLife();
}


void Walter_HandleWalterEatKibble(void) {
    // Eat the kibble then reset it
    PLAY_SOUND_WALTER_GOT_KIBBLE;

    kibble_data->state = CATKIBBLE_EAT_START;

    data->healthpoints++;
    StatusWindow_SetEnemyHealth(data->healthpoints,
                                WALTER_HEALTH_MAX);

    if (data->healthpoints == WALTER_HEALTH_MAX) {
        Walter_HandleWalterWon();
    }

}


void Start_SPRITE_WALTER() {
    // Hook up sprite sub-data structures
    data = (struct WalterInfo*)THIS->custom_data; // Walter

    kibble_data = (struct CatKibbleInfo*)spr_catkibble->custom_data; // Kibble target

    // Set sprite collision bounds
    THIS->coll_x = 2;
    THIS->coll_y = 7;
    THIS->coll_w = 12;
    THIS->coll_h = 6;

    THIS->lim_x = 1000;  // Not used right now. See: SPRITE_OFFSCREEN_AUTO_REMOVE_ENABLED
    THIS->lim_y = 1000;

    SPRITE_SET_VMIRROR(THIS); // Turn ON Flip/Mirror tile to face LEFT
    SpriteChangeDefaultTile(THIS, SPRITE_WALTER, 0); // Set Walter to standing still tile
    data->vx = 0;
    data->vy = 0;

    data->healthpoints = 0;
    StatusWindow_SetEnemyHealth(data->healthpoints,
                                WALTER_HEALTH_MAX);


    data->state = SPRITE_WALTER_WAITING;
    data->idlecount = 0;
}


void Update_SPRITE_WALTER() {

    // Uncomment for extra fast Walter
    if ((frame_count % 2) == 0) {

        if(CheckCollision(THIS, spr_catkibble)) {

            if (kibble_data->state == CATKIBBLE_READY_TO_EAT) {
                Walter_HandleWalterEatKibble();
            }
        }

        // Handle sprite orientation
        if (data->vx > 0)
            SPRITE_UNSET_VMIRROR(THIS); // Turn OFF Flip/Mirror tile to face RIGHT
        else
            SPRITE_SET_VMIRROR(THIS); // Turn ON Flip/Mirror tile to face LEFT

        // Keep walter within the Left and Right edges
        // of the room. Could be fixed by adding *walls*
        if (THIS->x < WALTER_X_MIN)
            THIS->x = WALTER_X_MIN;
        else if (THIS->x > WALTER_X_MAX)
            THIS->x = WALTER_X_MAX;

        // NOTE: A lot of Walter's state is controlled
        //       and driven in SpritePlayer.c when
        //       interacting with the CatKibble Sprite
        switch (data->state) {

            case SPRITE_WALTER_WAITING:
                // Do nothing
                break;


            case SPRITE_WALTER_START_MOVING:
                // Start boss area sequence
                KitchenCounter3_seal_off_boss_zone();
                SetSpriteAnim(THIS, walter_run, 12);

                // Display instructions for the level
                StatusWindow_PopupDialog(3, 2,  // X,Y tile position in window
                         50,     // print delay
                         160/2,  // Window scroll-up to position
                         "KNOCK FISH\n"
                         "OUT OF THE BOX.\n\n"
                         "GET THEM BEFORE\n"
                         "WALTER DOES!"); // DOES
                         //"BUMP THE BOX\nAND GOBBLE UP FISH!\n\nGET THEM\n BEFORE WALTER!");



                data->state = SPRITE_WALTER_IDLE;
                break;


            case SPRITE_WALTER_JUMPING:

                // == Horizontal / X movement while jumping ==

                // Only move toward kibble on X axis when it's available
                // if (kibble_data->state != CATKIBBLE_IN_BOX) {   // Use this instead for higher difficulty - chase food before it even lands
                if (kibble_data->state == CATKIBBLE_READY_TO_EAT) {

                    if ((THIS->x - WLT_MARGIN) > spr_catkibble->x)
                        data->vx = -SPRITE_WALTER_SPEED_X;
                    else if ((THIS->x + WLT_MARGIN) < spr_catkibble->x)
                        data->vx = SPRITE_WALTER_SPEED_X;
                    else {
                        // If on same Y and same X, then don't move
                        data->vx = 0;
                    }
                }

                // OK to continue any existing X movement
                if (data->vx) {
                    // If Walter hit a wall,
                    // reverse direction when not following kibble
                    if (TranslateSprite(THIS, data->vx, 0) &&
                        (kibble_data->state == CATKIBBLE_IN_BOX)) {
                        data->vx *= -1;
                    }
                }

                // == Vertical / Y movement while jumping ==

                // Apply Gravity and Vertical movement
                data->vy += WALTER_GRAVITY;
                if (data->vy > WALTER_TERM_VELOC)
                    data->vy = WALTER_TERM_VELOC;

                // Apply movement and Check floor
                if (TranslateSprite(THIS, 0, data->vy)) {
                    // (Ceiling handling removed: vy<0)
                    // Did Walter land after jumping?
                    // Resume normal chasing if food present, otherwise idle
                    // if (kibble_data->state == CATKIBBLE_IN_BOX) // Use this instead for higher difficulty - chase food before it even lands
                    if (kibble_data->state != CATKIBBLE_READY_TO_EAT)
                        data->state = SPRITE_WALTER_IDLE;
                    else
                        data->state = SPRITE_WALTER_CHASE_FOOD;

                    // Zero out vertical movement (stop falling)
                    data->vy = 0;
                }

                break;



            case SPRITE_WALTER_CHASE_FOOD:

                // == HORIZONTAL ==

                if ((THIS->x - WLT_MARGIN) > spr_catkibble->x)
                    data->vx = -SPRITE_WALTER_SPEED_X;
                else if ((THIS->x + WLT_MARGIN) < spr_catkibble->x)
                    data->vx = SPRITE_WALTER_SPEED_X;
                else {

                    // If on same Y and same X, then don't move
                    data->vx = 0;
                }

                // Apply Horizontal movement
                if (data->vx) {
                    if (TranslateSprite(THIS, data->vx, 0)) {
                        // If walter hit a wall and wasn't jumping,
                        // try to jump to get over
                        data->vy = (spr_catkibble->y - THIS->y) >> 1;

                    if (data->vy < WALTER_BOUNCE_Y_MAX)
                        data->vy = WALTER_BOUNCE_Y_MAX;

                        data->state = SPRITE_WALTER_JUMPING;
                    }
                }


                // == Vertical ==

                // If not falling, try to follow or jump toward target
                if ((THIS->y > spr_catkibble->y) &&
                    (data->state != SPRITE_WALTER_JUMPING)) {
                    // Below target
                    // Need to jump to get up?
                    data->vy = (spr_catkibble->y - THIS->y) >> 1;

                    if (data->vy < WALTER_BOUNCE_Y_MAX)
                        data->vy = WALTER_BOUNCE_Y_MAX;

                    data->state = SPRITE_WALTER_JUMPING;
                }

                // Apply Downward movement and Check floor
                if (!TranslateSprite(THIS, 0, 1)) {
                    // If no floor below, start falling/jumping
                    data->state = SPRITE_WALTER_JUMPING;
                }

                break;



            case SPRITE_WALTER_RESUME_IDLE:
                // Apply Downward movement and Check floor
                // If there is a floor, go to idle state
                if (TranslateSprite(THIS, 0, 1)) {
                    data->state = SPRITE_WALTER_IDLE;
                } else {
                    // If no floor below, start falling/jumping
                    data->state = SPRITE_WALTER_JUMPING;
                }

                break;



            case SPRITE_WALTER_IDLE:

                if (kibble_data->state == CATKIBBLE_READY_TO_EAT) {

                    // If the kibble is ready, break out of idle
                    data->state = SPRITE_WALTER_CHASE_FOOD;
                }
                else if (data->idlecount-- == 0) {

                    // Otherwise random jumps while idle (not chasing food)
                    if ((UINT8)DIV_REG & 0x01)
                        data->vx = WALTER_BOUNCE_X_IDLE;
                    else
                        data->vx = -WALTER_BOUNCE_X_IDLE;

                    if ((UINT8)DIV_REG & 0x01)
                        data->vy = WALTER_BOUNCE_Y_IDLE;
                    data->state = SPRITE_WALTER_JUMPING;
                    data->idlecount = WALTER_IDLE_MAX;

                }

                break;

        }
    }
}

void Destroy_SPRITE_WALTER() {
}

