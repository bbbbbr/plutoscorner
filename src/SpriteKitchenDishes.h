//
// SpriteKitchenDishes.h
//

struct DishInfo {
    // This can be up to 8 bytes
    INT8 vy; // velocity Y
    INT8 die;
    UINT8 spawn_count;

};

// BUILD TOGGLE controls whether dishes can be destroyed with a jump
// #define ENABLE_DISH_JUMP_SMASH


#define SPRITE_DISHES_FALL_Y_MAX   124
#define SPRITE_DISHES_FALL_SPEED_Y   2 // Speed dishes fall to the ground at
#define SPRITE_DISHES_PUSH_BACK_X    1 // How much pushback the player gets from dishes (2 causes jitter)

void StartDie_SPRITE_KITCHEN_DISHES();