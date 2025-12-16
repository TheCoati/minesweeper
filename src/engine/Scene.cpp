#include "Scene.h"
#include "Controller.h"
#include "Networking.h"
#include "SceneManager.h"

void Scene::begin() {
    this->onBegin();
}

void Scene::tick() {
    this->onTick();
}

void Scene::destroy() {
    Controller.clearCallbacks();
    Networking.clearCallbacks();

    this->onDestroy();
}
