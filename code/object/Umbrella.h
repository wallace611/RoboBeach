#pragma once

#include "object/Object.h"
#include "object/CollisionShape.h"

typedef struct _umb Umbrella;

struct _umb {
	obj_type_t obj_type;

	CollisionShape* csCollide;
	CollisionShape* csPickup;
	CollisionShape* csFloor;

	Object* obj;
};

Umbrella* newUmbrella();

void umbUpdate(Object* obj, float deltatime);
void umbRender(Object* obj);

void botTouchFloor(Object* self, CollisionShape* selfcs, Object* other, CollisionShape* othercs);