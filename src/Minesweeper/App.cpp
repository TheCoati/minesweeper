//
// Created by Kevin on 16 Dec 2025.
//

#include "StateManager.h"
#include "MainMenu.h"
#include "Game.h"

uint8_t globalSeed = 0;
AppState globalState = MENU;

void switchState(AppState newState) {
    globalState = newState;

    switch (globalState) {
        case MENU:
            drawMenu();
            break;
        case GAME:
            startGame(globalSeed);
            break;
    }
}

void tickStateManager() {
    globalSeed++;

    switch (globalState) {
        case MENU:
            tickMainMenu();
            break;
        case GAME:
            tickGame();
            break;
    }
}

void beginStateManager() {
    switchState(globalState);
}