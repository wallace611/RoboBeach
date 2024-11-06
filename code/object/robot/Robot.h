#pragma once

#include "object/Camera.h"
#include "object/Object.h"
#include "object/CollisionShape.h"
#include "object/robot/components/BotBody.h"

typedef struct _bot Robot;

struct _bot {
	obj_type_t obj_type;

	Camera* cam;

	BotBody* bbody;

	CollisionShape* bodyCollision;

	Object* obj;
};

Robot* newRobot();

static void botReady(Object* obj);
static void botUpdate(Object* obj, float deltatime);
static void botRender(Object* obj);
void botCollided(Object* self, CollisionShape* selfcs, Object* other, CollisionShape* othercs);

void botCamUpdate(Object* obj, float deltatime);