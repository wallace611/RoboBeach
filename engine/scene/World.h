#pragma once

#include "object/Object.h"
#include "utils/ObjContainer.h"

typedef struct _world {
	Object* camera;
	ObjContainer* instances;
} World;

World* newWorld();
void worldUpdate(World* world, float deltatime);
void worldRender(World* world);
int worldSpawnObj(World* world, Object* obj);
int worldKillObj(World* world, Object* obj);
void worldSetCamera(World* world, Object* cam);