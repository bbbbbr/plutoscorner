//
// zgb_utils.c
//

// #include "Sprite.h"
// #include "SpriteManager.h"
// #include "Scroll.h"
// #include "BankManager.h"

#include "asm/types.h"
#include "StatusWindow.h"

#include "zgb_utils.h"


extern UINT8 spriteIdxs[];

void SpriteChangeDefaultTile(struct Sprite* sprite, UINT8 sprite_type, UINT8 tile_index) {

    // Change default tile for sprite without using an animation
    // (can be used to assign a different tile set)
    sprite->first_tile = spriteIdxs[sprite_type] + (tile_index << sprite->size);
}


// Pass through to replace BG tile at map_x, map_y with new tile_id
void UpdateMapTile(INT16 map_x, INT16 map_y, UINT8 tile_id, UINT8 c) {
       UPDATE_TILE(map_x, map_y, &tile_id, &c);
}


// Copied from Scroll.h
#define SCREEN_TILES_W       20 // 160 >> 3 = 20
#define SCREEN_TILES_H       18 // 144 >> 3 = 18
#define SCREEN_PAD_LEFT   1
#define SCREEN_PAD_RIGHT  2
#define SCREEN_PAD_TOP    1
#define SCREEN_PAD_BOTTOM 2

#define SCREEN_TILE_REFRES_W (SCREEN_TILES_W + SCREEN_PAD_LEFT + SCREEN_PAD_RIGHT)
#define SCREEN_TILE_REFRES_H (SCREEN_TILES_H + SCREEN_PAD_TOP  + SCREEN_PAD_BOTTOM)



// Jumps the map to a new location and repaints the entire screen
//   You can't use MoveScroll() since it's meant for small panning movements
//   and glitches with large jumps
void ScrollRelocateMapTo(UINT16 new_x, UINT16 new_y) {
    UINT8 i;
    INT16 y;

    // These are externs from scroll.h
    // Update the
    scroll_x = new_x;
    scroll_y = new_y;

    PUSH_BANK(scroll_bank);
    y = new_y >> 3;
    for(i = 0u; i != (SCREEN_TILE_REFRES_H) && y != scroll_h; ++i, y ++) {
        ScrollUpdateRow((scroll_x >> 3) - SCREEN_PAD_LEFT,  y - SCREEN_PAD_TOP);
    }
    POP_BANK;
}



// Loads a new collision list without resetting the scroll tiles/map
void SetColList(const UINT8* coll_list, const UINT8* coll_list_down) {
    UINT8 i;

    // Erase previous collision list entries
    for(i = 0u; i != 128; ++i) {
        scroll_collisions[i] = 0u;
        scroll_collisions_down[i] = 0u;
    }

    AddToColList(coll_list, coll_list_down);
}


// Loads a new collision list without resetting the scroll tiles/map
void AddToColList(const UINT8* coll_list, const UINT8* coll_list_down) {
    UINT8 i;

    if(coll_list) {

        // Set general collision entries: tile index = 1 (true)
        for(i = 0u; coll_list[i] != 0u; ++i) {
            scroll_collisions[coll_list[i]] = 1u;
        }
    }

    if(coll_list_down) {

        // Set downward collision entries: tile index = 1 (true)
        for(i = 0u; coll_list_down[i] != 0u; ++i) {
            scroll_collisions_down[coll_list_down[i]] = 1u;
        }
    }
}


// Copied from ZGB Print
// Removed some code, removed some characters

UINT8 print_x  = 0;
UINT8 print_y  = 0;
UINT8 print_target = PRINT_BKG;

void print_text(const char* txt, unsigned char delay_time){
    UINT8 idx = 0;
    unsigned char c;
    unsigned char start_x;

    start_x = print_x; // Save start X for newline return

    while(*txt) {
        if (joypad())
            delay_time = 0;

        if(*txt == ' ') {
            c = FONT_TILE_SPACE;
        } else if(*txt >= 'A' && *txt <= 'Z'){
            c = FONT_TILES + *txt - 'A';
        } else if(*txt >= 'a' && *txt <= 'z') {
            c = FONT_TILES + *txt - 'a';
        } /* else if(*txt >= '0' && *txt <= '9') {
            c = font_idx + 27 + *txt - '0';
        } */ else {
            switch(*txt) {
                case  '?': c = FONT_TILES + 26; break;
                case  '!': c = FONT_TILES + 27; break;
                case  '.': c = FONT_TILES + 28; break;
                case '\'': c = FONT_TILES + 29; break;
                case '\n':
                    // Do a carriage return, no printing and skip to top of loop
                    print_x = start_x;
                    print_y++;
                    txt++;
                    if (delay_time)
                        delay(delay_time);
                    continue;
            }
        }

        if(print_target == PRINT_BKG)
            set_bkg_tiles(0x1F & (print_x), 0x1F & (print_y), 1, 1, &c);
        else
            set_win_tiles(print_x, print_y, 1, 1, &c);

        print_x++;
        txt++;
        if (delay_time)
            delay(delay_time);
            // OPTIONAL: enable or remove sounds while printing text
            // PlayFx(CHANNEL_1, 0,  0x20, 0x81, 0x43, 0x59, 0x86);
    }
}