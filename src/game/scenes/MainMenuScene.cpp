#include "MainMenuScene.h"
#include "engine/Controller.h"
#include "engine/SceneManager.h"

#include "GameScene.h"
#include "MPMenuScene.h"

void MainMenuScene::onBegin() {
    Screen.begin();

    tft.println("Z - Singleplayer");
    tft.println("C - Multiplayer");

    Controller.registerCallback(ControllerAction::PRIMARY, MainMenuScene::onPrimaryPress);
    Controller.registerCallback(ControllerAction::SECONDARY, MainMenuScene::onSecondaryPress);
}

void MainMenuScene::onTick() {

}

void MainMenuScene::onDestroy() {

}

void MainMenuScene::onPrimaryPress() {
    tft.println("TEST 1");
    SceneManager.switchScene(new GameScene());
}

void MainMenuScene::onSecondaryPress() {
    tft.println("TEST 2");
    SceneManager.switchScene(new MPMenuScene());
}
