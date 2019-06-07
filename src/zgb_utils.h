//
// zgb_utils.h
//

#include <gb/gb.h>

#include "Sprite.h"
#include "SpriteManager.h"
#include "Scroll.h"
#include "BankManager.h"
#include "Sound.h"

void SpriteChangeDefaultTile(struct Sprite* sprite, UINT8 sprite_type, UINT8 tile_index);
void UpdateMapTile(INT16 x, INT16 y, UINT8 tile_id, UINT8 c);
void ScrollRelocateMapTo(UINT16 new_x, UINT16 new_y);
void SetColList(const UINT8* coll_list, const UINT8* coll_list_down);
void AddToColList(const UINT8* coll_list, const UINT8* coll_list_down);


extern UINT8 print_x, print_y, font_idx, print_target;

typedef enum {
    PRINT_BKG,
    PRINT_WIN
} PRINT_TARGET;

void print_text(const char* txt, unsigned char delay_time);
#define PRINT_POS(X, Y) print_x = X; print_y  = Y
#define PRINT(X, Y, TXT, DELAY) PRINT_POS(X,Y); print_text(TXT, DELAY)
#define SET_PRINT_TARGET(TARGET) print_target = TARGET
#define INIT_FONT(FONT_TILE_START, TARGET) font_idx = FONT_TILE_START; print_target = TARGET

