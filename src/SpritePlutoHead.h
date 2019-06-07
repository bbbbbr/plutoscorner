//
// SpritePlutoHead.h
//

extern struct Sprite * spr_pluto_claw_left;
extern struct Sprite * spr_pluto_claw_right;


typedef enum {
    PLUTO_LEVEL_INIT = 1,
    PLUTO_HIDE_START,
    PLUTO_HIDE_WAIT,
    PLUTO_TAIL_START,
    PLUTO_TAIL_RAISE,
    PLUTO_TAIL_PROWL,
    PLUTO_TAIL_PROWL_START,
    PLUTO_TAIL_LOWER,
    PLUTO_HEAD_START,
    PLUTO_HEAD_HECKLE_WAIT,
    PLUTO_HEAD_RAISE,
    PLUTO_HEAD_LOWER,
    PLUTO_HEAD_OUCH_START,
    PLUTO_HEAD_OUCH,
    PLUTO_HEAD_HAHA_START,
    PLUTO_HEAD_HAHA,
    PLUTO_HEAD_DEFEATED_START,
    PLUTO_HEAD_DEFEATED
} PLUTO_STATES;

struct PlutoInfo {
    // This can be up to 8 bytes
    INT8  state;
    INT8  dir_x;
    UINT8 dest_x;
    UINT8 countdown;
    UINT8 prowlcount;
    INT8  healthpoints;
    INT8  claw_offset;
    INT8  claw_dir;
};

// Start hidden
// -> Into Hide Time
//
//    A) -> Sprite: Tail
//          -> [ Tail Up  -> Prowl  -> Tail Down ]
//
//    B) -> Sprite: Head + Body, Random: Left / Right Arms
//          -> [ Pluto up -> Heckle -> Pluto Down ]
//
//    C) -> Repeat: goto A

#define PLUTO_DELAY_HIDE_INTRO   60 * 1  // 2 seconds
#define PLUTO_DELAY_HECKLE       60 * 2  // 2 seconds
#define PLUTO_DELAY_OUCH         30      // 0.5 second
#define PLUTO_DELAY_HAHA         60 * 2  // 1.5 second
#define PLUTO_DELAY_DEFEATED     179     // sequence of N bumps down / sounds (see PLUTO_DEFEATED_INTERVAL)
#define PLUTO_DEFEATED_INTERVAL  30      // 0.5 seconds

#define PLUTO_PROWL_COUNT_START   7
#define PLUTO_PROWL_COUNT_NORM    4


#define PLUTO_LOC_Y_START_HIDING 112 - 16           // Showing in bottom of window
#define PLUTO_LOC_X_TAIL_START (60 * 8) + 160 / 2   // Centered
//#define PLUTO_LOC_Y_TAIL_START 120                // Hiding
#define PLUTO_LOC_Y_TAIL_START 116 -16              // Bottom of screen - 1 - window
#define PLUTO_LOC_Y_TAIL_STOP  136 - 16 -(5*8)      // Showing in bottom of window
#define PLUTO_AXIS_Y_SPEED     1
#define PLUTO_AXIS_X_SPEED     1
#define PLUTO_AXIS_Y_SPEED_DEFEATED  3

// DIFFICULTY->Higher (use below)
// #define PLUTO_AXIS_Y_SPEED     4
// #define PLUTO_AXIS_X_SPEED     2
#define PLUTO_CLAW_SPEED       1

#define PLUTO_LOC_X_TAIL_PROWL_MIN   (60 +  6) * 8 // value in tiles
#define PLUTO_LOC_X_TAIL_PROWL_MAX   (60 + 14) * 8 // value in tiles
#define PLUTO_PROWL_RANGE            PLUTO_LOC_X_TAIL_PROWL_MAX - PLUTO_LOC_X_TAIL_PROWL_MIN
// Create a range of offsets to pop up that are : 0, 21, 42, 63 : RND(MASK) * MULTIPLY
#define PLUTO_PROWL_MASK             0x07   // Range of 0-7 // OPTIONAL: Could also do 0x0F -> 64 / 16 = increments of 4
#define PLUTO_PROWL_MULTIPLY         8      // = 64 / PLUTO_PROWL_MASK
// DIFFICULTY->Higher (use below)
// #define PLUTO_PROWL_MASK             0x0F   // Range of 0-15
// #define PLUTO_PROWL_MULTIPLY         64 / (PLUTO_PROWL_MASK + 1)  // = 64 / PLUTO_PROWL_MASK


#define PLUTO_CLAW_OFFSET 8 * 2 // 2 tiles


#define PLUTO_HEALTH_MAX                    5
#define PLUTO_HEALTH_DAMAGE_HIT_BY_PLAYER   1
#define PLUTO_HEALTH_DAMAGE_CLAWS_PLAYER    1

void Sprite_Pluto_Defeated_PushDown(void);