#pragma bank 3
#include "main.h"
UINT8 bank_STATE_CREDITS = 3;

#include "../res/src/end_game_tiles.h"
#include "AudioCommon.h"


#include "../res/src/end_game_tiles.h"
#include "../res/src/end_game_map_bg.h"


#include "StatusWindow.h"

#include "ZGBMain.h"
#include "Scroll.h"
#include "SpriteManager.h"
#include "Keys.h"

#include "zgb_utils.h"
#include "PlayerHealth.h"


#define CREDITS_WINDOW_Y_SCROLL_START 18*8
#define CREDITS_WINDOW_Y_SCROLL_END    0*8
#define CREDITS_SEQ_STARTWIN            30
#define CREDITS_SEQ_END                250

// These tiles are in BANK 1 with CODE
#define CREDITS_RES_BANK 1


extern UINT8 shared_counter;

static UINT8 window_y;
static UINT8 x;
static UINT8 y;
static UINT8 cur_tile;
#define STAR_ANIM_LEN 5
const UINT8 star_anim[] = {1,2,2,1,0};

void Credits_Render_Star(void);
void Credits_Render_Background(void);
void Credits_Window_Render(void);


void Start_STATE_CREDITS() {

	// Initialize Sound
    Sound_Init();

    // == BACKGROUND TILE MAP
    // first tile (loc in tile bank), n tiles,  tile_data, tile_bank
    InitScrollTiles(0, 133, end_game_tiles, CREDITS_RES_BANK);
    //InitScroll(end_game_map_bgWidth, end_game_map_bgHeight, end_game_map_bg, 0, 0, CREDITS_RES_BANK);
    // Misuse this to initialize the background instead of InitScroll()
    ScrollSetMapColor(20, 20, end_game_tiles, CREDITS_RES_BANK, 0);
    SHOW_BKG;


    StatusWindow_Hide();
    StatusWindow_InitWindowAndFontTiles();

    window_y = CREDITS_WINDOW_Y_SCROLL_START;
    WY_REG = window_y; // Window Y Coordinate
    WX_REG = 0 + HW_WINDOW_X_OFFSET;  // Start at 0, window starts offset 7 pixels to the right
    Credits_Window_Render();
    SHOW_WIN;

    Credits_Render_Background();
    shared_counter = 0;
    cur_tile = 0;
}

void Credits_Render_Background(void) {

    // Clear background to blank tiles
    for (x=0; x < 20; x++)
        for (y=0; y < 20; y++)
            UpdateMapTile(x,y, 0, 0);
}


void Credits_Render_Star(void) {

    // increment to next tile
    cur_tile++;

    // If it's the end then choose a new location
    if (cur_tile == STAR_ANIM_LEN) {
        // Pseudo-random location for next star)
        x = ((UINT8)DIV_REG & 0x0F);
        y = ((UINT8)DIV_REG & 0xF0) >> 4;
        // Reset to start of star animation
        cur_tile = 0;
    }

    // Draw current star tile
    UpdateMapTile(x,y, star_anim[cur_tile], 0);
}


void Credits_Window_Render(void) {

    UINT8 cur_tile;
    StatusWindow_Clear();

    // Print a border along the top
    cur_tile =  WIN_TILES + 1; // WIN_TILES + 10;
    for (shared_counter=0; shared_counter < STATUS_WIN_WIDTH; shared_counter++) {
        // cur_tile = WIN_TILES + 10 + (shared_counter % 2);
        InitWindow(shared_counter, 0, // Location
                   1, 1,      // 1 tile x 1 tile
                   &cur_tile, // tile to draw
                   3, 0);     // bank and cmap(NULL)
    }
        // Left status border cap
        cur_tile = WIN_TILES + 14;
        InitWindow(0, 0, // Location
                   1, 1,      // 1 tile x 1 tile
                   &cur_tile, // tile to draw
                   3, 0);     // bank and cmap(NULL)

        // Right status border cap
        cur_tile = WIN_TILES + 2;
        InitWindow(19, 0, // Location
                   1, 1,      // 1 tile x 1 tile
                   &cur_tile, // tile to draw
                   3, 0);     // bank and cmap(NULL)



    SET_PRINT_TARGET(PRINT_WIN);
    PRINT(1,1,"CREDITS...", 0);
    PRINT(10,1,"Matchy\n"
              "Josie\n"
              "Pluto\n"
              "Walter\n"
              "Stove\n"
              "  Clock\n"
              "Couches",0);

    PRINT(1, 10,"TOOLS...", 0);
    PRINT(10,10,"GBDK  ZGB\n"
                "GBTD GBMB", 0);

    PRINT(1, 14,"MUSIC....", 0);
    PRINT(10,14,"AVGVST", 0);

    PRINT(1, 16,"GAME....", 0);
    PRINT(10,16,"B", 0);
}


void Update_STATE_CREDITS() {

    shared_counter++;
    delay(130);

    Credits_Render_Star();

    if (shared_counter > CREDITS_SEQ_STARTWIN) {
        if (window_y > CREDITS_WINDOW_Y_SCROLL_END) {
            window_y--;
            WY_REG = window_y;
            }
    }

    // Wait for player to press start
    if (KEY_TICKED(J_ANYKEY) || (shared_counter > CREDITS_SEQ_END)) {
        if (window_y > CREDITS_WINDOW_Y_SCROLL_END)
            // If the window is still scrolling up, skip to the end of scrolling
            window_y = CREDITS_WINDOW_Y_SCROLL_END + 1;
        else {
            // Implied: SetState(STATE_INTRO_DREAM_SCREEN);
            Sound_Volume_Fadeout();
            SetState(STATE_INTRO_SCREEN);
        }
    }

}
