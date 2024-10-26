#include "World.h"

World* newWorld() {
    World* world = (World*)malloc(sizeof(World));
    if (world == NULL) return NULL;

    world->instances = ocCreate();
    return world;
}

void worldUpdate(World* world, float deltatime) {
    world->camera->updateFunction(world->camera, deltatime);
    for (int i = 0; i < world->instances->end; i++) {
        world->instances->list[i]->updateFunction(world->instances->list[i], deltatime);
    }
}

void worldRender(World* world) {
    for (int i = 0; i < world->instances->end; i++) {
        world->instances->list[i]->renderFunction(world->instances->list[i]);
    }
}

int worldSpawnObj(World* world, Object* obj) {
    if (world == NULL || obj == NULL) return -1;

    int flag = ocPushBack(world->instances, obj);
    if (flag != 1) return -1;

    obj->readyFunction(obj);
    return 1;
}

int worldKillObj(World* world, Object* obj) {
    if (world == NULL || obj == NULL) return -1;

    int flag = ocRemove(world->instances, obj);
    if (flag != 1) return -1;

    free(obj);
    return 1;
}

void worldSetCamera(World* world, Object* cam) {
    if (world == NULL || cam == NULL) return;

    world->camera = cam;
}
