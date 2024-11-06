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

	Object* pickedObj;
	CollisionShape* bodyCollision;
	CollisionShape* pickCollision;

	ObjContainer* pickupSpace;

	Object* obj;
};

Robot* newRobot();

static void botReady(Object* obj);
static void botUpdate(Object* obj, float deltatime);
static void botRender(Object* obj);
void botCollided(Object* self, CollisionShape* selfcs, Object* other, CollisionShape* othercs);
void botCamUpdate(Object* obj, float deltatime);
void botMoving(Robot* bot, float forward, float side);
void botPickup(Robot* bot);
void botDrop(Robot* bot);
void botObjEnterPickupSpace(Object* self, CollisionShape* selfcs, Object* other, CollisionShape* othercs);