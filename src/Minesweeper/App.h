//
// Created by Kevin on 16 Dec 2025.
//

#ifndef MINESWEEPER_STATEMANAGER_H
#define MINESWEEPER_STATEMANAGER_H

enum AppState {
    MENU,
    GAME,
};

void beginStateManager();
void tickStateManager();
void switchState(AppState newState);

#endif //MINESWEEPER_STATEMANAGER_H
