#pragma once

#include "object/Object.h"
#include "object/CollisionShape.h"

typedef struct _floor Floor;

struct _floor {
	obj_type_t obj_type;

	CollisionShape* csCollide;

	float color[3];

	Object* obj;
};

Floor* newFloor();
void floorRender(Object* obj);