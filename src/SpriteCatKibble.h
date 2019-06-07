//
// SpriteCatKibble.h
//


#define SPRITE_CATKIBBLE_START_X  9*8 +3
#define SPRITE_CATKIBBLE_START_Y  12*8 // 3*8 -1

// 6*8, 9*8 -4);


#define SPRITE_CATKIBBLE_GRAVITY     1
#define SPRITE_CATKIBBLE_TERM_VELOC  6


#define SPRITE_CATKIBBLE_EAT_COUNT 15

typedef enum {
    CATKIBBLE_HIDDEN = 1,
    CATKIBBLE_IN_BOX,
    CATKIBBLE_TO_LAUNCH,
    CATKIBBLE_LAUNCHED,
    CATKIBBLE_READY_TO_EAT,
    CATKIBBLE_EAT_START,
    CATKIBBLE_EAT_COUNTDOWN,
    CATKIBBLE_RESET,
} CATKIBBLE_STATES;

#define CATKIBBLE_AVAILABLE CATKIBBLE_LAUNCHED | CATKIBBLE_READY_TO_EAT

struct CatKibbleInfo {
    // This can be up to 8 bytes
    INT8  state;
    INT8  eatpoints;
    INT8 vy; // velocity Y
    INT8 vx; // velocity X
    INT8 count;
};


