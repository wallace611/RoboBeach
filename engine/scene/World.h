#pragma once

#include "object/Camera.h"
#include "object/Object.h"
#include "utils/ObjContainer.h"

typedef struct _world {
	Camera* cam;
	ObjContainer* instances;
	ObjContainer* collisionList;
} World;

World* newWorld();
void worldUpdate(World* world, float deltatime);
void worldRender(World* world);
int worldSpawnObj(World* world, Object* obj);
int worldKillObj(World* world, Object* obj);
void worldSetCamera(World* world, Camera* cam);
void worldCollisionDetection(World* world);