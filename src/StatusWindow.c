//
// StatusWindow.c
//

// Make it unbanked so code from any bank
// can call it without setting the bank
// (There is room in bank 0)
//
// #pragma bank 2


#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"
#include "gbt_player.h"

#include "StatusWindow.h"
// #include "../res/src/status_window_tiles.h"
#include "../res/src/font_and_status_window_tiles.h"

#include "PlayerHealth.h"

#include "zgb_utils.h"

extern UINT8 playerhealth;
extern UINT8 playerlives;

UINT8 win_count;
UINT8 win_y_pos;

UINT8 cur_tile;

// Status Win: +L-HHHH-------EEEEE+
//             12345678901234567890
const UINT8 status_window_map_default[] = {WIN_TILES + 0,
                                        // 2:
                                       WIN_TILES + 1, // Lives #6,7,8,9
                                       WIN_TILES + 1,
                                        // 4:
                                       WIN_TILES + 1, // Health #5/4
                                       WIN_TILES + 1,
                                       WIN_TILES + 1,
                                       WIN_TILES + 1,
                                       WIN_TILES + 1,
                                       // 9:
                                       WIN_TILES + 1, WIN_TILES + 1, WIN_TILES + 1,
                                       WIN_TILES + 1, WIN_TILES + 1, WIN_TILES + 1,
                                       // 15:
                                       WIN_TILES + 1, // Enemy #11/10
                                       WIN_TILES + 1,
                                       WIN_TILES + 1,
                                       WIN_TILES + 1,
                                       WIN_TILES + 1,
                                       // 20:
                                       WIN_TILES + 2};


extern INT8 healthpoints;

// Tile 6 = 0 lives ... 9 = 3 lives
UINT8 player_status_window_lives[4] = { WIN_TILES + 6,   // 0 Lives
                                        WIN_TILES + 7,
                                        WIN_TILES + 8,
                                        WIN_TILES + 9 }; // 3 lives


void StatusWindow_Hide(void) {

    HIDE_WIN;
}


void StatusWindow_SetYPos(UINT8 new_y) {
    win_y_pos = new_y;  // Save current window pos for later reference
    WY_REG = win_y_pos; // Set Window Y Coordinate
}


void StatusWindow_InitWindowAndFontTiles(void) {
    // Load tiles for Status Window
    // Note: tiles are loaded at END of tile RAM
    // --> first tile (loc in tile bank), n tiles,  tile_data, tile_bank
    InitScrollTiles(255 - FONT_TILE_COUNT - STATUS_WIN_TILE_COUNT, FONT_TILE_COUNT + STATUS_WIN_TILE_COUNT, font_and_win_tiles, 3);
    StatusWindow_SetYPos(STATUS_WIN_Y_OFFSCREEN);
}


void StatusWindow_Init(void) {

    StatusWindow_InitWindowAndFontTiles();

    // --> (start_tile_x, start_tile_y, tile_width, tile_height, tile_map_array, Bank);
    InitWindow(0, 0, (UINT8)sizeof(status_window_map_default), 1, &status_window_map_default[0], 3, 0);

    StatusWindow_SetYPos(STATUS_WIN_Y_START);
    WX_REG = 0 + HW_WINDOW_X_OFFSET;  // Start at 0, window starts offset 7 pixels to the right
    SHOW_WIN;
}


void StatusWindow_SetPlayerHealth() {

    StatusWindow_DrawMeter(STATUS_WIN_HEALTH_X_START,
      playerhealth, PLAYER_HEALTH_MAX,
      WIN_TILES + 4,  // Empty Heart
      WIN_TILES + 5); // Filled-in Heart
/*
    //  startx, starty, length, height, tile map, bank, cmap(null)
    InitWindow(STATUS_WIN_HEALTH_X_START, 0,
               (UINT8)sizeof(player_status_window_health[0]), 1,
               &player_status_window_health[playerhealth][0],
               3, 0);
*/
}


void StatusWindow_SetPlayerFish() {

    StatusWindow_DrawMeter(STATUS_WIN_HEALTH_X_START,
      playerhealth, PLAYER_HEALTH_FISH_MAX,
      WIN_TILES + 12,  // Empty Fish
      WIN_TILES + 13); // Filled-in Fish
}


void StatusWindow_SetPlayerLives(void) {

    //  startx, starty, length, height, tile map, bank, cmap(null)
    InitWindow(STATUS_WIN_LIVES_X_START, 0,
               STATUS_WIN_LIVES_SIZE,    1,
               &player_status_window_lives[playerlives],
               3, 0);
}


void StatusWindow_SetEnemyHealth(UINT8 hp, UINT8 hp_max) {

    StatusWindow_DrawMeter(STATUS_WIN_ENEMY_X_START,
      hp, hp_max,
      WIN_TILES + 10,  // Empty Cat Head
      WIN_TILES + 11); // Filled-in Cat Head

/*
    // Loop through all tiles to draw
    for (win_count = 0; win_count < hp_max; win_count++) {

        // Choose which tile to draw based on hp level
        if (win_count < hp)
            cur_tile = WIN_TILES + 11; // Filled-in Cat Head
        else
            cur_tile = WIN_TILES + 10; // Empty Cat Head

        // Draw tile
        //  startx, starty, length, height, tile map, bank, cmap(null)
        InitWindow(STATUS_WIN_ENEMY_X_START + win_count, 0, // Location
                   1, 1,      // 1 tile x 1 tile
                   &cur_tile, // tile to draw
                   3, 0);     // bank and cmap(NULL)
    }
*/
}


void StatusWindow_DrawMeter(UINT8 x, UINT8 hp, UINT8 hp_max, UINT8 tile_off, UINT8 tile_on) {

    // Loop through all tiles to draw
    for (win_count = 0; win_count < hp_max; win_count++) {

        // Choose which tile to draw based on hp level
        if (win_count < hp)
            cur_tile = tile_on; // Filled-in Cat Head
        else
            cur_tile = tile_off; // Empty Cat Head

        // Draw tile
        //  startx, starty, length, height, tile map, bank, cmap(null)
        InitWindow(x++, 0, // Location
                   1, 1,      // 1 tile x 1 tile
                   &cur_tile, // tile to draw
                   3, 0);     // bank and cmap(NULL)
    }

}

// NOTE: Does not clear status bar area (row 0)
void StatusWindow_Clear() {

    UINT8 window_y;
    UINT8 tile_id;
    tile_id = WIN_TILES + 3; // Blank window tile

    // Loop through all tiles to draw
    for (window_y = STATUS_WIN_HEIGHT; window_y <= WIN_MAP_HEIGHT; window_y++) {

        for (win_count = 0; win_count < WIN_MAP_WIDTH; win_count++) {

            // Draw tile
            //  startx, starty, length, height, tile map, bank, cmap(null)
            InitWindow(win_count, window_y, // Location
                       1, 1,      // 1 tile x 1 tile
                       &tile_id, // &cur_tile, // tile to draw
                       3, 0);     // bank and cmap(NULL)
        }
    }
}


void StatusWindow_ScrollUpTo(UINT8 window_y_dest) {

    // win_y_pos is a global of where the status window ought to be
    for (win_count = win_y_pos; win_count >= window_y_dest; win_count--) {
        WY_REG = win_count;
        delay(2);
    }
}

void StatusWindow_ScrollDownFrom(UINT8 window_y_start) {

    for (win_count = window_y_start; win_count <= win_y_pos; win_count++) {
        WY_REG = win_count;
        delay(2);
    }
}




void StatusWindow_PopupDialog(int printat_x, int printat_y, unsigned char print_delay_time, UINT8 window_y_dest, const char* txt) {

    HIDE_SPRITES;
    SET_PRINT_TARGET(PRINT_WIN);
    StatusWindow_Clear();

    StatusWindow_ScrollUpTo(window_y_dest);

    PRINT(printat_x, printat_y,
          txt,
          print_delay_time);

    // Wait for the player to press A, B or Start
    waitpadup();
    waitpad(J_A | J_B | J_START);
    waitpadup();

    // Now reset the key state to avoid passing a keypress into gameplay
    UPDATE_KEYS();

    StatusWindow_ScrollDownFrom(window_y_dest);

    SHOW_SPRITES;
}

void StatusWindow_HandlePause(void) {

    gbt_pause(0);
    // Mute output of all sound since gpt_pause just freezes the sound and holds the current note
    NR51_REG = 0x00; // Turn off all channels (left and right)
    NR50_REG = 0x00; // Max volume .[2..0] Right Main Vol, .[6..4] Left Main Volume


    StatusWindow_PopupDialog(3, 2,  // X,Y tile position in window
         5,     // print delay
         144-(4*8),  // Window scroll-up to position
         "... PAUSED ...");
    gbt_pause(1);
    NR51_REG = 0xFF; // Turn ON all channels (left and right)
    NR50_REG = 0x77; // Max volume .[2..0] Right Main Vol, .[6..4] Left Main Volume
}
