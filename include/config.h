//
// Created by Kevin on 19 Dec 2025.
//

#ifndef MINESWEEPER_CONFIG_H
#define MINESWEEPER_CONFIG_H

// Uncomment to allow running without an SD card (debugging)
// #define ALLOW_NO_SD_CARD 1

// Screen Dimensions (Adafruit ILI9341)
#define SCREEN_WIDTH 320  //  320px
#define SCREEN_HEIGHT 240 //  240px

// Generic Menu
#define MENU_BACKGROUND_COLOR 0x8BCB //  #8BCBFF

// Menu Buttons
#define BUTTON_WIDTH 192              //  Button image width (192px)
#define BUTTON_HEIGHT 48              //  Button image height (48px)
#define BUTTON_CURSOR_BORDER 3        //  3px
#define BUTTON_CURSOR_COLOR 0x4A07    //  #49423D
#define BUTTON_CURSOR_WIDTH (192 - 6) //  6px = Button shadow
#define BUTTON_CURSOR_HEIGHT (48 - 6) //  6px = Button shadow

// Main Menu
#define MAIN_MENU_SCREEN_PADDING 60 //  60px
#define MAIN_MENU_BUTTON_SPACE 24   //  24px

// Multiplayer Menu
#define MP_MENU_SCREEN_PADDING 48 //  48px

// Game Grid
#define GRID_MARGIN 5             //  5px
#define GRID_BORDER_PADDING 4     //  4px
#define GRID_COL_SIZE 24          //  24px
#define GRID_CURSOR_WIDTH 3       //  3px
#define GRID_CURSOR_COLOR 0x4A07  // #49423D

#endif //MINESWEEPER_CONFIG_H
