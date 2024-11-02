#include "World.h"

#include "utils/Shapes.h"

World* newWorld() {
    World* world = (World*)malloc(sizeof(World));
    if (world == NULL) return NULL;

    world->instances = newObjContainer();
    return world;
}

void worldUpdate(World* world, float deltatime) {
    world->cam->obj->update(world->cam->obj, deltatime);
    for (int i = 0; i < world->instances->end; i++) {
        Object* obj = cast(world->instances->list[i], OBJECT);
        obj->update(world->instances->list[i], deltatime);
    }
}

void worldRender(World* world) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    world->cam->obj->render(world->cam->obj);

    for (int i = 0; i < world->instances->end; i++) {
        Object* obj = cast(world->instances->list[i], OBJECT);
        obj->render(world->instances->list[i]);
    }
}

int worldSpawnObj(World* world, Object* obj) {
    if (world == NULL || obj == NULL) return -1;

    int flag = ocPushBack(world->instances, obj);
    if (flag != 1) return -1;

    obj->ready(obj);
    return 1;
}

int worldKillObj(World* world, Object* obj) {
    if (world == NULL || obj == NULL) return -1;

    int flag = ocRemove(world->instances, obj);
    if (flag != 1) return -1;

    free(obj);
    return 1;
}

void worldSetCamera(World* world, Camera* cam) {
    if (world == NULL || cam == NULL) return;

    world->cam = cam;
    cam->obj->ready(cam->obj);
}
