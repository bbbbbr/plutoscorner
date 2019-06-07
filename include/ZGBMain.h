#ifndef ZGBMAIN_H
#define ZGBMAIN_H

#define J_ANYKEY 0xFF

#define STATES \
_STATE(STATE_INTRO_SCREEN)\
_STATE(STATE_INTRO_DREAM_SCREEN)\
_STATE(STATE_JOSIE_CALL)\
_STATE(STATE_MATCHY_CHECKLIST)\
_STATE(STATE_KITCHEN_COUNTER_1)\
_STATE(STATE_KITCHEN_COUNTER_2)\
_STATE(STATE_KITCHEN_COUNTER_3)\
_STATE(STATE_LIVING_ROOM_1)\
_STATE(STATE_LIVING_ROOM_2)\
_STATE(STATE_PLUTO_WINDOW)\
_STATE(STATE_GAMEOVER_SCREEN)\
_STATE(STATE_END_GAME_SCREEN)\
_STATE(STATE_CREDITS)\
STATE_DEF_END

// _SPRITE_DMG(label, var-name, bank, frame-size, num-frames)
//  "Right now only 8x16 and 16x16 sprites are supported and
//   no matter the size you cannot have more than 20 sprites
//   at a time or the rom will crash"
// _SPRITE_DMG(ID, resource_var_name, bank, size, frames)

#define SPRITES \
   _SPRITE_DMG(SPRITE_PLAYER,         player,     3, FRAME_16x16, 4)\
   _SPRITE_DMG(SPRITE_MOTH,           moth,       3, FRAME_16x16, 5)\
   _SPRITE_DMG(SPRITE_PLUTO_HEAD,     pluto_head, 3, FRAME_16x16, 7)\
   _SPRITE_DMG(SPRITE_PLUTO_CLAW,     pluto_claw, 3, FRAME_16x16, 2)\
   _SPRITE_DMG(SPRITE_KITCHEN_DISHES, kitchen_dishes,  3, FRAME_16x16, 9)\
   _SPRITE_DMG(SPRITE_KITCHEN_STOVE,  kitchen_stove,    3, FRAME_16x16, 1)\
   _SPRITE_DMG(SPRITE_CATKIBBLE,      catkibble_tiles,  3, FRAME_16x16, 6)\
   _SPRITE_DMG(SPRITE_WALTER,         walter_tiles,     3, FRAME_16x16, 4)\
   _SPRITE_DMG(SPRITE_HEART,          heart_tiles,      3, FRAME_16x16, 2)\
SPRITE_DEF_END

#include "ZGBMain_Init.h"

#endif