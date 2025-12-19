#include "Scene.h"
#include "Controller.h"
#include "engine/Timer.h"

void Scene::begin() {
    this->onBegin();
}

void Scene::tick() {
    this->onTick();
}

void Scene::destroy() {
    this->onDestroy();
}
