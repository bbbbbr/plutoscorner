//
// SpritePlayer.c
//


#pragma bank 2
#include "main.h"

#include "ZGBMain.h"
#include "Sound.h"
#include "Keys.h"
#include "Fade.h"
#include "SpriteManager.h"

#include "SpritePlayer.h"

#include "SpritePlutoHead.h"
#include "SpriteKitchenDishes.h"
#include "SpriteCatKibble.h"
#include "SpriteWalter.h"
#include "SpriteMoth.h"

#include "AudioCommon.h"
#include "StatusWindow.h"
#include "PlayerHealth.h"
#include "MatchyChecklist.h"

#include "Scroll.h"

UINT8 bank_SPRITE_PLAYER = 2;



extern UINT8 playerhealth;
extern UINT8 playerlives;

// = {num frames, ..frame(s)..}
const UINT8 anim_idle[] = {1, 0};
const UINT8 anim_walk[] = {2, 1, 2};
const UINT8 anim_ouch[] = {2, 3, 0};

// External sprites to interact with and control
extern struct Sprite * spr_pluto_head;
extern struct Sprite * spr_catkibble;
extern struct Sprite * spr_walter;



PLAYER_STATES player_state;


INT8   player_accel_y;
UINT8  tile_collision;
UINT8  player_jump_press;
UINT8  frame_count;
UINT16 player_x_limit;
UINT16 player_y_fall_limit;


// Called (via SPRITE_PLUTO_HEAD) after Pluto Defeated
// animation sequence has finished
void Player_HandleBeatPluto(void) {

    PlaySound_BeatLevelBoss();

    StatusWindow_PopupDialog(1, 2,  // X,Y tile position in window
             50,     // print delay
             160/2,  // Window scroll-up to position
             "YOU BEAT THE CAT\n"
             "KING OF THE BLOCK!");

    FadeOut();
    delay(150);

    // PLAYER WON THE GAME- GO TO WON GAME SEQUENCE
    // Implied: SetState(STATE_END_GAME_SCREEN);
    matchy_checklist_status = CHECKLIST_PLUTO_DONE;
    Sound_Volume_Fadeout();
    SetState(STATE_MATCHY_CHECKLIST);
}


void Player_DeductLife(void) {

    PLAY_SOUND_LOST_A_LIFE;

    Sound_Volume_Fadeout();

    if (current_state == STATE_PLUTO_WINDOW) {
        StatusWindow_PopupDialog(3, 2,  // X,Y tile position in window
                                0,     // print delay
                                160/2,  // Window scroll-up to position
                                "PLUTO GOTCHA!\n\n"
                                "BETTER LUCK\n"
                                "NEXT TIME!\n");

    } else if ( (current_state == STATE_KITCHEN_COUNTER_3) &&
              ( ((struct WalterInfo*)spr_walter->custom_data)->state != SPRITE_WALTER_WAITING)) {
        StatusWindow_PopupDialog(3, 2,  // X,Y tile position in window
                                0,     // print delay
                                160/2,  // Window scroll-up to position
                                "TOUGH LUCK!\n\n"
                                "WALTER GOT ALL\n"
                                "THE FOOD BEFORE\n"
                                "YOU DID!\n");
    }


    FadeOut();
    delay(150);

    // Deduct a life
    playerlives--;

    // Update the status window
    StatusWindow_SetPlayerLives();

    if (playerlives == PLAYER_LIVES_NONE) {
        // Game Over!
        // Display releveant screen
        Sound_Volume_Fadeout();
        SetState(STATE_GAMEOVER_SCREEN);

    } else {
        // If lives remain, restart current level
        Sound_Volume_Fadeout();
        SetState(current_state);
    }
}


void Player_HandleHitDeductHealth(void) {

    // Make the player flip upside down and play the was-hit sound

    // OPTIONAL: maybe remove upside down if optimizing is needed
    THIS->flags |= S_FLIPY;             // Turn off Flip/Mirror tile to face upside down
    SetSpriteAnim(THIS, anim_ouch, 30); // Set animation to hit
    PLAY_SOUND_OUCH_DEDUCT_HEALTH;

    // Deduct Health and update status window
    playerhealth--;
    StatusWindow_SetPlayerHealth();

    // If the player ran out of hearts
    if (playerhealth == PLAYER_HEALTH_NONE) {

        // Deduct a life, will replay "current_state" if lives remain
        Player_DeductLife();
    }

    // Make player bounce off what it hit
    player_jump_press = PLAYER_JUMP_PRESS_RESET;
    player_state      = PLAYER_STATE_OUCH;
    player_accel_y    = PLAYER_JUMP_ACCEL_CONT;

    // Special handling for Kitchen counter stage
    // Bounce player in direction they are pressing button
    if ((current_state == STATE_KITCHEN_COUNTER_1) || (current_state == STATE_KITCHEN_COUNTER_2)) {
        TranslateSprite(THIS,
                       (KEY_PRESSED(J_LEFT) * -10) + (KEY_PRESSED(J_RIGHT) * -10),
                       player_accel_y);
    }
    else {
        TranslateSprite(THIS, 0, player_accel_y);
    }
}


void Player_HandleEatKibble(void) {

    PLAY_SOUND_EAT_KIBBLE;

    // Deduct Health and update status window
    playerhealth++;
    StatusWindow_SetPlayerFish();

    // If the player ate all of the kibble, then they beat walter
    if (playerhealth == PLAYER_HEALTH_FISH_MAX) {

        PlaySound_BeatLevelBoss();

        StatusWindow_PopupDialog(4, 2,  // X,Y tile position in window
                 50,     // print delay
                 160/2,  // Window scroll-up to position
                 "YOU GOT A FULL\n"
                 "BREAKFAST!\n\n"
                 "LEVEL UP!");

        FadeOut();
        delay(150);

        // Go to next level
        // Implied: SetState(STATE_LIVING_ROOM_1);
        matchy_checklist_status = CHECKLIST_KIBBLE_DONE;
        Sound_Volume_Fadeout();
        SetState(STATE_MATCHY_CHECKLIST);
    }

}


void Start_SPRITE_PLAYER() {
    // Collision detection box for sprite
    THIS->coll_x = SPRITE_PLAYER_coll_x;
    THIS->coll_y = SPRITE_PLAYER_coll_y;
    THIS->coll_w = SPRITE_PLAYER_coll_w;
    THIS->coll_h = SPRITE_PLAYER_coll_h;

    player_accel_y = 0;

    switch (current_state) {

        case STATE_LIVING_ROOM_1:
        case STATE_LIVING_ROOM_2:
            PlayerHealth_Reset(PLAYER_HEALTH_MAX);
            StatusWindow_SetPlayerHealth();
            player_y_fall_limit = (17 * 8) - 10;
            // player_x_limit = 0;
            player_x_limit = 1000;
            break;

        case STATE_PLUTO_WINDOW:
            player_y_fall_limit = 17 * 8;
            player_x_limit = 1000;
            PlayerHealth_Reset(PLAYER_HEALTH_MAX);
            StatusWindow_SetPlayerHealth();
            break;

        case STATE_KITCHEN_COUNTER_1:
        case STATE_KITCHEN_COUNTER_2:
            PlayerHealth_Reset(PLAYER_HEALTH_MAX);
            StatusWindow_SetPlayerHealth();
            player_y_fall_limit = 17 * 8;
            player_x_limit = 1000;
            break;

        case STATE_KITCHEN_COUNTER_3:
            player_y_fall_limit = 80 * 8;
            player_x_limit = 18*8;
            PlayerHealth_Reset(PLAYER_HEALTH_FISH_NONE);
            StatusWindow_SetPlayerFish();
            break;
    }

    StatusWindow_SetPlayerLives();

	frame_count = 0;
    // Drop in to ground at the start, if above
    player_state = PLAYER_STATE_FALLING;

    player_jump_press = PLAYER_JUMP_PRESS_RESET;
}



void Update_SPRITE_PLAYER() {
    UINT8 i;
    struct Sprite* spr;

	frame_count++;

    // OPTIONAL: maybe remove Accelerate button (turn into animation?)
    // OPTIONAL: Convert to KEY_TICKED() ? -> speed goes into a var -> added each frame, could diminish to show decel
    // OPTIONAL: This could be simplified and optimized

    // Handle Horizontal Movement
    if (KEY_PRESSED(J_LEFT)) {
        if (THIS->x > 2) {
    		if (KEY_PRESSED(J_B) || ((frame_count % 2) == 0))
    	        TranslateSprite(THIS, -PLAYER_RUN_SPEED, 0);
    		else
    	        TranslateSprite(THIS, -PLAYER_WALK_SPEED, 0);
        }

        if (player_state != PLAYER_STATE_OUCH)
            SetSpriteAnim(THIS, anim_walk, 15);

        // TODO: Optimize upside down matchy away if CPU too limited?
        // Use?: else if (KEY_TICKED(J_LEFT)) {
        SPRITE_SET_VMIRROR(THIS); // Turn OFF Flip/Mirror tile to face LEFT
    }
    else if(KEY_PRESSED(J_RIGHT)) {
        if (THIS->x < player_x_limit) {
    		if (KEY_PRESSED(J_B) || ((frame_count % 2) == 0))
    	        TranslateSprite(THIS, PLAYER_RUN_SPEED, 0);
    		else
    	        TranslateSprite(THIS, PLAYER_WALK_SPEED, 0);
        }

        if (player_state != PLAYER_STATE_OUCH)
            SetSpriteAnim(THIS, anim_walk, 15);

        // TODO: Consider using (more efficient?) SPRITE_SET_VMIRROR(THIS); // Flip X
        // SetSpriteAnim(THIS, anim_walk_right, 15);
        SPRITE_UNSET_VMIRROR(THIS); // Turn OFF Flip/Mirror tile to face RIGHT
    }
    else if (KEY_RELEASED(J_LEFT)) {
        if (player_state != PLAYER_STATE_OUCH)
            SetSpriteAnim(THIS, anim_idle, 1);

        SPRITE_SET_VMIRROR(THIS); // Turn OFF Flip/Mirror tile to face LEFT

    }
    else if (KEY_RELEASED(J_RIGHT)) {
        if (player_state != PLAYER_STATE_OUCH)
            SetSpriteAnim(THIS, anim_idle, 1);

        SPRITE_UNSET_VMIRROR(THIS); // Turn OFF Flip/Mirror tile to face RIGHT
    }



    // Handle Veritcal Jumping / Falling
    switch(player_state) {
        case PLAYER_STATE_NORMAL:
            //Check if jumping is requested
            if (KEY_TICKED(J_A)) {

                // Play Sound: Jump
                PLAY_SOUND_JUMP;

                player_accel_y = PLAYER_JUMP_ACCEL_CONT;

                // Check for collision above player
                if (TranslateSprite(THIS, 0, player_accel_y)) {
                    // If so: Set acceleration to zero
                    player_accel_y = PLAYER_JUMP_ACCEL_NONE;
                    player_state   = PLAYER_STATE_FALLING;
                }
                else
                {
                    player_state      = PLAYER_STATE_JUMPING;
                    player_jump_press = PLAYER_JUMP_PRESS_RESET;
                }
            }
            else {
                // Test floor/gravity by trying to
                // Move player DOWN when they are standing still
                if (TranslateSprite(THIS, 0, 1)) {
                    // If so: Set acceleration to zero and reset state to NORMAL
                    player_accel_y = PLAYER_JUMP_ACCEL_NONE;
                    player_state = PLAYER_STATE_NORMAL;
                }
                else
                    player_state = PLAYER_STATE_FALLING;
            }

            break;

        case PLAYER_STATE_JUMPING:

            // Check to see if the player has remaining jump capacity
            if (player_jump_press < PLAYER_JUMP_PRESS_MAX)
            {
                // Check if jump key is still held down
                if (KEY_PRESSED(J_A)) {
                    // If it is then continue accelerating UPWARD
                    player_accel_y = PLAYER_JUMP_ACCEL_START;
                }
                else
                {
                    // If user let off key during jump then
                    // switch over to falling
                    // player_jump_press = PLAYER_JUMP_PRESS_MAX; // redundant
                    player_state   = PLAYER_STATE_FALLING;
                }
                // Deplete the jump capacity
                player_jump_press++;
            }
            else {
                // If the jump capacity is used up, then switch to falling
                player_state   = PLAYER_STATE_FALLING;
            } // Fall through to next stage

        case PLAYER_STATE_OUCH:
                // Pass through to FALLING state BELOW

        case PLAYER_STATE_FALLING:
                // If player is falling then add gravity
                // to accelerate DOWNWARD
                // But only if they are below max speed
                if (player_accel_y <= PLAYER_JUMP_TERM_VELOC) {
					if ((frame_count % PLAYER_JUMP_GRAVITY_MOD) == 0)
                    	player_accel_y += PLAYER_JUMP_GRAVITY;
                }

                // Check if player landed on GROUND *OR* hit CEILING
                if(TranslateSprite(THIS, 0, player_accel_y)) {

                    // If so: Set acceleration to zero and reset state to NORMAL
                    if (player_accel_y < 0) {
                        // Player was moving upward, hit CEILING
                        // so keep in FALLING state
                        player_accel_y = PLAYER_JUMP_ACCEL_NONE;
                        player_state = PLAYER_STATE_FALLING;
                    } else {
                        // Player was moving down, hit GROUND
                        // Return to NORMAL state
                        player_state = PLAYER_STATE_NORMAL;

                        // HANDLE: If the player had received a hit then / damage
                        // Reset the animation and sprite inversion
                        THIS->flags &= ~S_FLIPY;// Turn off Flip/Mirror tile to face upside down
                        SetSpriteAnim(THIS, anim_idle, 1); // Set animation to hit
                    }

                    player_accel_y = PLAYER_JUMP_ACCEL_NONE;
                }
                else if (THIS->y > player_y_fall_limit) {
                    // y is unsigned, so top of screen is negative
                    // To verify falling, make sure it's between
                    if (THIS->y < PLAYER_FELL_MAX) {

                        // Matchie Fell / Player Died!
                        // Deduct a life, will replay "current_state" if lives remain
                        Player_DeductLife();
                    }
                }
/*                else if (THIS->y > PLAYER_FELL_GROUND_LEVEL) {
                    // y is unsigned, so top of screen is negative
                    // To verify falling, make sure it's between
                    if (THIS->y < PLAYER_FELL_MAX) {

                        // Matchie Fell / Player Died!
                        // Deduct a life, will replay "current_state" if lives remain
                        Player_DeductLife();
                    }
                }
*/
            break;

    }



    // Iterate through all sprites
    SPRITEMANAGER_ITERATE(i, spr) {

        // If the sprite is an enemy
        if(spr->type == SPRITE_MOTH) {

            // Check collision with the player (THIS)
            if(CheckCollision(THIS, spr)) {
               //SpriteManagerRemove(i);
                struct SprMothInfo* data = (struct SprMothInfo*)spr->custom_data;
                if (!data->die) {
                    data->die = 40;

                PLAY_SOUND_EAT_MOTH;
                }

            }
        }

        // If the sprite is an enemy
        else if(spr->type == SPRITE_KITCHEN_DISHES) {

            // Check collision with the player (THIS)
            if(CheckCollision(THIS, spr)) {
                struct DishInfo* data = (struct DishInfo*)spr->custom_data;

                if (player_state == PLAYER_STATE_NORMAL) {
                    // Try to push the dish sprite RIGHT or LEFT
                    // * Bump the player back from the dish
                    // * Try to translate/move the dish
                    // * Check if it fell off an edge, if so, start it falling
                    if (THIS->x < spr->x) {
                        // Push RIGHT
                        THIS->x -= SPRITE_DISHES_PUSH_BACK_X;
                        TranslateSprite(spr, 1, 0);
                        if (!TranslateSprite(spr, 0, 1))
                            data->vy = SPRITE_DISHES_FALL_SPEED_Y;
                    } else {
                        // Push LEFT
                        THIS->x += SPRITE_DISHES_PUSH_BACK_X;
                        TranslateSprite(spr, -1, 0);
                         if (!TranslateSprite(spr, 0, 1))
                            data->vy = SPRITE_DISHES_FALL_SPEED_Y;
                    }
                }
            #ifdef ENABLE_DISH_JUMP_SMASH
                else if (player_state == PLAYER_STATE_FALLING) {

                    if (!data->die) {
                        StartDie_SPRITE_KITCHEN_DISHES();

                        // Make player bounce off the dish
                        // No need to do anything with collision data
                        player_accel_y = PLAYER_JUMP_ACCEL_CONT;
                        TranslateSprite(THIS, 0, player_accel_y);
                        player_state      = PLAYER_STATE_JUMPING;
                        player_jump_press = PLAYER_JUMP_PRESS_RESET;
                    }
                }
            #endif // ENABLE_DISH_JUMP_SMASH
            }
        }


        else if(spr->type == SPRITE_CATKIBBLE) {

            // Check collision with the player (THIS)
            if(CheckCollision(THIS, spr)) {

                struct CatKibbleInfo* data = (struct CatKibbleInfo*)spr->custom_data;

                // If kibble is ready in box
                if ((player_state == PLAYER_STATE_FALLING) &&
                    (data->state == CATKIBBLE_IN_BOX)) {

                    // Launch the kibble
                    PLAY_SOUND_LAUNCH_KIBBLE;
                    data->state = CATKIBBLE_TO_LAUNCH;

                    // TODO: centralize and de-duplicate player bounce off enemy
                    // Make player bounce off kibble box
                    player_accel_y = PLAYER_JUMP_ACCEL_CONT;
                    // No need to do anything with collision data
                    TranslateSprite(THIS, 0, player_accel_y);
                    player_state      = PLAYER_STATE_JUMPING;
                    player_jump_press = PLAYER_JUMP_PRESS_RESET;

                } else if (data->state == CATKIBBLE_READY_TO_EAT) {

                    // Eat the kibble then reset it
                    data->state = CATKIBBLE_EAT_START;

                    // Set Walter to Idle state
                    ((struct WalterInfo*)spr_walter->custom_data)->state = SPRITE_WALTER_RESUME_IDLE;

                    Player_HandleEatKibble();
                }
            }
        }



        // If the sprite is an enemy
        else if(spr->type == SPRITE_WALTER) {

            // If Walter is waiting at start of boss area
            if (((struct WalterInfo*)spr_walter->custom_data)->state == SPRITE_WALTER_WAITING) {

                if(CheckCollision(THIS, spr)) {

                    // It's the start of the boss level
                    // so trigger walter to start moving
                    // and make the kibble available
                    ((struct WalterInfo*)spr_walter->custom_data)->state = SPRITE_WALTER_START_MOVING;
                    ((struct CatKibbleInfo*)spr_catkibble->custom_data)->state = CATKIBBLE_RESET;
                }
            }
        }


       // Is the sprite a Kitchen Stove Burner?
        else if(spr->type == SPRITE_KITCHEN_STOVE) {

            // Check collision with the player (THIS)
            if(CheckCollision(THIS, spr)) {

                Player_HandleHitDeductHealth();
            }
        }


        // Is the sprite Pluto's Head?
        else if(spr->type == SPRITE_PLUTO_HEAD) {

            // Check collision with the player (THIS)
            if(CheckCollision(THIS, spr) && (player_state == PLAYER_STATE_FALLING)) {

                // Only allow hits if Pluto is in a heckling state
                if (PLUTO_HEAD_HECKLE_WAIT == ((struct PlutoInfo *)spr->custom_data)->state) {

                    ((struct PlutoInfo *)spr->custom_data)->state = PLUTO_HEAD_OUCH_START;

                    // Play sound for jumping on plutos head
                    PLAY_SOUND_JUMP_ON_PLUTO_HEAD;


                    // TODO: centralize and de-duplicate player bounce off enemy
                    // Make player bounce off Pluto's head
                    player_accel_y = PLAYER_JUMP_ACCEL_CONT;
                    // No need to do anything with collision data
                    TranslateSprite(THIS, 0, player_accel_y);
                    player_state      = PLAYER_STATE_JUMPING;
                    player_jump_press = PLAYER_JUMP_PRESS_RESET;

                    ((struct PlutoInfo *)spr->custom_data)->healthpoints -= PLUTO_HEALTH_DAMAGE_HIT_BY_PLAYER;

                    StatusWindow_SetEnemyHealth(((struct PlutoInfo *)spr->custom_data)->healthpoints,
                                                PLUTO_HEALTH_MAX);

                    if (((struct PlutoInfo *)spr->custom_data)->healthpoints <= 0) {

                        // Pluto was defeated! Start player-won sequence
                        ((struct PlutoInfo *)spr->custom_data)->state = PLUTO_HEAD_DEFEATED_START;
                    }

                }

            }
        }


        // Is the sprite Pluto's Claw?
        else if(spr->type == SPRITE_PLUTO_CLAW) {

            // Check collision with the player (THIS)
            if(CheckCollision(THIS, spr)) {

                // OPTIONAL: To increase difficulty also allow collision when claw is being raised or lowered (states)
                // Only attack when Pluto is fully revealed in the heckle state
                if (PLUTO_HEAD_HECKLE_WAIT == ((struct PlutoInfo *)spr_pluto_head->custom_data)->state) {

                    // Make pluto retract
                    ((struct PlutoInfo *)spr_pluto_head->custom_data)->state = PLUTO_HEAD_HAHA_START;

                    Player_HandleHitDeductHealth();
                }
            }
        }

    }
}

void Destroy_SPRITE_PLAYER() {
}
