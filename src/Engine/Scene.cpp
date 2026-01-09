#include "Scene.h"

void Scene::begin() {
    onBegin();
}

void Scene::tick() {
    onTick();
}

void Scene::destroy() {
    onDestroy();
}
