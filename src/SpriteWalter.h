//
// SpriteWalter.h
//

// More difficult combo:
/*
#define WALTER_GRAVITY          2
#define WALTER_BOUNCE_Y_MAX    -16    // Larger max jumps decrease difficulty (longer air time), 12 = a little tough
#define WALTER_BOUNCE_Y_IDLE   -16
#define SPRITE_WALTER_SPEED_X   2
*/

/* Also consider;
#define WALTER_BOUNCE_Y_MAX   -16
#define WALTER_GRAVITY          2
or
#define WALTER_BOUNCE_Y_MAX   -10
#define WALTER_GRAVITY          1
*/

#define SPRITE_WALTER_SPRINT_X  4
#define SPRITE_WALTER_SPEED_X   4 // Faster speed increase difficulty, 2 = maybe too easy
#define WALTER_BOUNCE_Y_MAX   -11    // Larger max jumps decrease difficulty (longer air time), 12 = a little tough

#define WLT_MARGIN 3 // 4 // 2

#define WALTER_GRAVITY         1
#define WALTER_TERM_VELOC      6
#define WALTER_BOUNCE_X        2

#define WALTER_BOUNCE_Y_IDLE  -11
#define WALTER_BOUNCE_X_IDLE   2

#define WALTER_X_MIN 4
#define WALTER_X_MAX ((18*8)-4)

#define WALTER_HEALTH_MAX 5

#define WALTER_IDLE_MAX  60;

typedef enum {
    SPRITE_WALTER_WAITING = 1,
    SPRITE_WALTER_START_MOVING,
    SPRITE_WALTER_JUMPING,
    SPRITE_WALTER_CHASE_FOOD,
    SPRITE_WALTER_RESUME_IDLE,
    SPRITE_WALTER_IDLE
} WALTER_STATES;


struct WalterInfo {
    // This can be up to 8 bytes
    UINT8 state;
    UINT8 healthpoints;
    INT8  vy; // velocity Y
    INT8  vx; // velocity X
    UINT8 idlecount;
};

void KitchenCounter3_seal_off_boss_zone();