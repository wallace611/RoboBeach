#pragma once

#include "object/Object.h"
#include "object/CollisionShape.h"

typedef struct _fan Fan;

struct _fan {
	obj_type_t obj_type;

	float angle;

	CollisionShape* csCollide;
	CollisionShape* csPickup;
	CollisionShape* csBlow;

	Object* obj;
};

Fan* newFan();

void fanUpdate(Object* obj, float deltatime);
void fanRender(Object* obj);

void fanTouchFloor(Object* self, CollisionShape* selfcs, Object* other, CollisionShape* othercs);
void fanBlowObj(Object* self, CollisionShape* selfcs, Object* other, CollisionShape* othercs);