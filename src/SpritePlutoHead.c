//
// SpritePlutoHead.c
//

#pragma bank 2
#include "main.h"
#include "SpriteManager.h"
#include "ZGBMain.h"

#include "zgb_utils.h"
#include "StatusWindow.h"
#include "AudioCommon.h"

#include "SpritePlayer.h"
#include "SpritePlutoHead.h"




UINT8 bank_SPRITE_PLUTO_HEAD = 2;


const UINT8 anim_pluto_taunt[] = {2, 0, 1};
const UINT8 anim_pluto_haha[]  = {2, 5, 6};
const UINT8 anim_pluto_tail[]  = {1, 2};
const UINT8 anim_pluto_ouch[]  = {2, 3, 4};
const UINT8 anim_pluto_claw_ouch[]  = {2, 0, 1}; // For claw sprite

const UINT8 anim_pluto_head_defeated[]  = {1, 3};
const UINT8 anim_pluto_claw_defeated[]  = {1, 1}; // For claw sprite



void Start_SPRITE_PLUTO_HEAD() {
    struct PlutoInfo* data = (struct PlutoInfo*)THIS->custom_data;

    THIS->coll_x = 0;
    THIS->coll_y = 0;
    THIS->coll_w = 16;
    THIS->coll_h = 16;

    THIS->lim_x = 1000;  // Not used right now. See: SPRITE_OFFSCREEN_AUTO_REMOVE_ENABLED
    THIS->lim_y = 1000;

    // Use OAM background priorty flag to draw sprite behind background
    THIS->flags |= S_PRIORITY;

    data->healthpoints = PLUTO_HEALTH_MAX;
    StatusWindow_SetEnemyHealth(data->healthpoints, PLUTO_HEALTH_MAX);

    data->dir_x = -1; // FACING LEFT
    data->claw_offset = 0;
    data->claw_dir = PLUTO_CLAW_SPEED;

    data->state = PLUTO_LEVEL_INIT;
}


void Update_SPRITE_PLUTO_HEAD() {
    struct PlutoInfo* data = (struct PlutoInfo*)THIS->custom_data;

    switch (data->state) {

        case PLUTO_LEVEL_INIT:
            // Display instructions for the level
            StatusWindow_PopupDialog(1, 2,  // X,Y tile position in window
                     50,     // print delay
                     70,  // Window scroll-up to position
                     "OH NO!\n"
                     "RUMBLE WITH PLUTO!\n\n"
                     "POUNCE ON HIS HEAD\n"
                     "BUT WATCH OUT FOR\n"
                     "THOSE CLAWS!");

            data->state = PLUTO_HIDE_START;
            break;


        case PLUTO_HIDE_START:
            data->countdown  = PLUTO_DELAY_HIDE_INTRO;
            //data->prowlcount = PLUTO_PROWL_COUNT_START;

            THIS->y = PLUTO_LOC_Y_START_HIDING;    // Hide sprite
            THIS->x = PLUTO_LOC_X_TAIL_START;      // Start Centered

            data->state = PLUTO_HIDE_WAIT; // Next State
            break;


        case PLUTO_HIDE_WAIT:
            data->countdown--;
            if (!data->countdown)
                data->state =  PLUTO_TAIL_START; // Next State

            data->prowlcount = PLUTO_PROWL_COUNT_START;
            break;


        case PLUTO_TAIL_START:
            SetSpriteAnim(THIS, anim_pluto_tail, 1);
            data->prowlcount = PLUTO_PROWL_COUNT_START;
            THIS->y = PLUTO_LOC_Y_TAIL_START;
            data->state = PLUTO_TAIL_RAISE; // Next State
            break;


        case PLUTO_TAIL_RAISE:
            if (THIS->y > PLUTO_LOC_Y_TAIL_STOP) {
                THIS->y += -PLUTO_AXIS_Y_SPEED; // Move sprite UP on Y axis
            } else {
                data->state = PLUTO_TAIL_PROWL_START;
            }
            break;


        case PLUTO_TAIL_LOWER:
            if (THIS->y < PLUTO_LOC_Y_TAIL_START) {
                THIS->y += PLUTO_AXIS_Y_SPEED; // Move sprite DOWN on Y axis
            } else {
                data->state = PLUTO_HEAD_START;
            }
            break;


        case PLUTO_TAIL_PROWL_START:
            // Choose next prowl destination
            // Use not-so-random number from the Div register (faster than rand)
            // Range of ~0-63 increments/total slots of PLUTO_PROWL_MASK
            data->dest_x = PLUTO_LOC_X_TAIL_PROWL_MIN + (((UINT8)DIV_REG & PLUTO_PROWL_MASK) * PLUTO_PROWL_MULTIPLY);
            data->state  = PLUTO_TAIL_PROWL; // Next State

            if (data->dest_x > (UINT8)THIS->x) {
                // Move to the left
                data->dir_x = PLUTO_AXIS_X_SPEED; // Move sprite RIGHT on X axis
                SPRITE_SET_VMIRROR(THIS); // Turn off Flip/Mirror tile to face right
            }
            else {
                // Move to the right
                data->dir_x = -PLUTO_AXIS_X_SPEED; // Move sprite LEFT on X axis
                SPRITE_UNSET_VMIRROR(THIS); // Flip/Mirror tile to face right
            }
            break;


        case PLUTO_TAIL_PROWL:
            // Was the destination reached?
            if ((UINT8)THIS->x == data->dest_x) {
                // Reduce number of prowls remaining
                data->prowlcount--;

                // Check if there is more prowling to do
                if (data->prowlcount > 0)
                    // Down another prowl loop
                    data->state  = PLUTO_TAIL_PROWL_START; // Next State
                else
                    data->state  = PLUTO_TAIL_LOWER; // Next State
            } else {
                THIS->x += data->dir_x; // Move sprite on x axis
            }
            break;


        case PLUTO_HEAD_START:
            SetSpriteAnim(THIS, anim_pluto_taunt, 10);
            data->countdown = PLUTO_DELAY_HECKLE;
            data->state = PLUTO_HEAD_RAISE; // Next State

            spr_pluto_claw_left->x   = THIS->x - PLUTO_CLAW_OFFSET;
            spr_pluto_claw_right->x  = THIS->x + PLUTO_CLAW_OFFSET;
            break;


        case PLUTO_HEAD_RAISE:
            if (THIS->y > PLUTO_LOC_Y_TAIL_STOP) {
                THIS->y += -PLUTO_AXIS_Y_SPEED; // Move sprite UP on Y axis

                // Left & Right Claws
                data->claw_offset += data->claw_dir;
                if ((data->claw_offset > 7) | (data->claw_offset < -7))
                    data->claw_dir = data->claw_dir * -1;
                // Claw offset is inverted for L/R
                spr_pluto_claw_left->y  = THIS->y + data->claw_offset + 8;
                spr_pluto_claw_right->y = THIS->y - data->claw_offset + 8;
            }
            else
                data->state = PLUTO_HEAD_HECKLE_WAIT;
            break;


        case PLUTO_HEAD_HECKLE_WAIT:
            data->countdown--;
                // Left & Right Claws
                data->claw_offset += data->claw_dir;
                if ((data->claw_offset > 7) | (data->claw_offset < -7))
                    data->claw_dir = data->claw_dir * -1;
                // Claw offset is inverted for L/R
                spr_pluto_claw_left->y  = THIS->y + data->claw_offset + 8;
                spr_pluto_claw_right->y = THIS->y - data->claw_offset + 8;

            if (!data->countdown)
                data->state = PLUTO_HEAD_LOWER; // Next State
            break;


        case PLUTO_HEAD_LOWER:
            if (THIS->y < PLUTO_LOC_Y_TAIL_START) {
                THIS->y += PLUTO_AXIS_Y_SPEED; // Move sprite DOWN on Y axis

                // Left & Right Claws
                data->claw_offset += data->claw_dir;
                if ((data->claw_offset > 7) | (data->claw_offset < -7))
                    data->claw_dir = data->claw_dir * -1;
                // Claw offset is inverted for L/R
                spr_pluto_claw_left->y  = THIS->y + data->claw_offset + 8;
                spr_pluto_claw_right->y = THIS->y - data->claw_offset + 8;
            } else {
                data->state = PLUTO_TAIL_START;// PLUTO_HIDE_WAITING;
            }
            break;


        case PLUTO_HEAD_OUCH_START:
            data->countdown = PLUTO_DELAY_HECKLE;
            data->state     = PLUTO_HEAD_OUCH;

            // Change animation/tiles to semi-inverted state
            SetSpriteAnim(THIS, anim_pluto_ouch, 20);
            SetSpriteAnim(spr_pluto_claw_left, anim_pluto_claw_ouch, 20);
            SetSpriteAnim(spr_pluto_claw_right, anim_pluto_claw_ouch, 20);
           break;


        case PLUTO_HEAD_OUCH:
            data->countdown--;
            if (!data->countdown) {

                // Ouch period is over, reset to default animations and tiles
                SetSpriteAnim(THIS, anim_pluto_taunt, 10);
                SetSpriteAnim(spr_pluto_claw_left, 0, 0);  // disable animation
                SetSpriteAnim(spr_pluto_claw_right, 0, 0); // disable animation

                data->state = PLUTO_HEAD_LOWER; // Next State
            }
            break;


        case PLUTO_HEAD_HAHA_START:
            data->countdown = PLUTO_DELAY_HAHA;
            data->state     = PLUTO_HEAD_HAHA;
            SetSpriteAnim(THIS, anim_pluto_haha, 5);
           break;


        case PLUTO_HEAD_HAHA:
            data->countdown--;
            if (!data->countdown) {
                SetSpriteAnim(THIS, anim_pluto_taunt, 10);
                data->state = PLUTO_HEAD_LOWER; // Next State
            }
            break;


        case PLUTO_HEAD_DEFEATED_START:

            Sound_GBTStopAndRestoreAudio();
            PLAY_SOUND_JUMP_ON_PLUTO_HEAD;

            // Reset animation to Xed out eyes
            SetSpriteAnim(THIS,                 anim_pluto_head_defeated, 0); // disable animation
            SetSpriteAnim(spr_pluto_claw_left,  anim_pluto_claw_defeated, 0); // disable animation
            SetSpriteAnim(spr_pluto_claw_right, anim_pluto_claw_defeated, 0); // disable animation

            // Set counter and move to next state
            data->countdown = PLUTO_DELAY_DEFEATED;
            data->state     = PLUTO_HEAD_DEFEATED;
            break;


        case PLUTO_HEAD_DEFEATED:

            // Pluto defeated animation sequence
            data->countdown--;

            if ((data->countdown % PLUTO_DEFEATED_INTERVAL) == 0) {
                // Move pluto down and play sound every N frames
                THIS->y                 += PLUTO_AXIS_Y_SPEED_DEFEATED;
                spr_pluto_claw_left->y  += PLUTO_AXIS_Y_SPEED_DEFEATED;
                spr_pluto_claw_right->y += PLUTO_AXIS_Y_SPEED_DEFEATED;
                PLAY_SOUND_JUMP_ON_PLUTO_HEAD;
            }

            if (!data->countdown) {
                // Done animating, finish up the won-level sequence
                Player_HandleBeatPluto();
            }
            break;






    }
    //SetSpriteAnim(THIS, anim_pluto_scowl, 4);
}

void Destroy_SPRITE_PLUTO_HEAD() {
}