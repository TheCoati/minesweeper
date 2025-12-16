#include "GameScene.h"
#include "engine/Networking.h"

void GameScene::onBegin() {
    tft.println("Game Screen");

    Networking.registerCallback(0x02, GameScene::onNetMoveCursor);
}

void GameScene::onTick() {

}

void GameScene::onDestroy() {

}

void GameScene::onNetMoveCursor(MinenetPacket packet) {

}

