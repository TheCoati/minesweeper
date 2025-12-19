#ifndef MINESWEEPER_SCENEMANAGER_H
#define MINESWEEPER_SCENEMANAGER_H

#include "Scene.h"

class SceneManager {
public:
     SceneManager() = default;
     ~SceneManager();

    void tick();
    void unloadScene();
    void switchScene(Scene *newScene);
private:
    Scene *currentScene = nullptr;
};

extern SceneManager SceneManager;

#endif //MINESWEEPER_SCENEMANAGER_H
