//
// SpritePlayer.h
//

typedef enum  {
    PLAYER_STATE_NORMAL,
    PLAYER_STATE_JUMPING,
    PLAYER_STATE_FALLING,
    PLAYER_STATE_OUCH,
}PLAYER_STATES;

#define PLAYER_WALK_SPEED 1
#define PLAYER_RUN_SPEED  2

#define PLAYER_JUMP_ACCEL_START -5
#define PLAYER_JUMP_ACCEL_CONT  -6
#define PLAYER_JUMP_ACCEL_NONE   0
#define PLAYER_JUMP_GRAVITY      1 // Only applied mod of PLAYER_JUMP_GRAVITY_MOD
#define PLAYER_JUMP_TERM_VELOC   2
//#define PLAYER_JUMP_GRAVITY_MOD  3
#define PLAYER_JUMP_GRAVITY_MOD  2  // this produces inconsistant jumping results...
#define PLAYER_JUMP_REPEAT_MAX   100
#define PLAYER_JUMP_REPEAT_ZERO  0
#define PLAYER_JUMP_PRESS_MAX    6
#define PLAYER_JUMP_PRESS_RESET  0

#define PLAYER_FELL_GROUND_LEVEL 16 * 8
#define PLAYER_FELL_MAX          1000 * 8

#define SPRITE_PLAYER_coll_x  2
#define SPRITE_PLAYER_coll_y  0
#define SPRITE_PLAYER_coll_w 14
#define SPRITE_PLAYER_coll_h 13

void Player_DeductLife(void);
void Player_HandleHitDeductHealth(void);
void Player_HandleBeatPluto(void);