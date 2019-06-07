//
// StatusWindow.h
//

#include "ZGBMain.h"

#define HW_WINDOW_X_OFFSET 7

#define WIN_MAP_WIDTH 160/8
#define WIN_MAP_HEIGHT 144/8

#define STATUS_WIN_Y_OFFSCREEN 18*8
#define STATUS_WIN_WIDTH  20
#define STATUS_WIN_Y_START 17*8
#define STATUS_WIN_HEIGHT  1

#define STATUS_WIN_TILE_COUNT 15 // WARNING: Needs to be manually updated
#define FONT_TILE_COUNT 30

#define WIN_TILES  255 - STATUS_WIN_TILE_COUNT
#define FONT_TILES WIN_TILES - FONT_TILE_COUNT
#define FONT_TILE_SPACE FONT_TILES + 33 // Space tile is part of window tile set

#define STATUS_WIN_TILE_PLAYER_HEALTH_ON 5
#define STATUS_WIN_PLAYER_HEALTH_ON 4
#define STATUS_WIN_ENEMY_HEALTH_BLANK 5

// The -1 is due to the off by 7 Window X positioning
#define STATUS_WIN_LIVES_X_START  2 - 1
#define STATUS_WIN_LIVES_SIZE     1

#define STATUS_WIN_HEALTH_X_START 4 - 1

#define STATUS_WIN_ENEMY_X_START 15 - 1

void StatusWindow_InitWindowAndFontTiles(void);
void StatusWindow_Hide(void);
void StatusWindow_Init(void);

void StatusWindow_SetPlayerLives(void);
void StatusWindow_SetPlayerHealth(void);
void StatusWindow_SetPlayerFish(void);

void StatusWindow_SetEnemyHealth(UINT8 hp, UINT8 hp);
void StatusWindow_DrawMeter(UINT8 x, UINT8 hp, UINT8 hp_max, UINT8 tile_off, UINT8 tile_on);

void StatusWindow_Clear();
void StatusWindow_ScrollUpTo(UINT8 window_y_dest);
void StatusWindow_ScrollDownFrom(UINT8 window_y_start);

void StatusWindow_PopupDialog(int printat_x, int printat_y, unsigned char print_delay_time, UINT8 window_y_dest, const char* txt);
void StatusWindow_HandlePause(void);
