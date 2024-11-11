#pragma once

#include "object/Object.h"
#include "object/CollisionShape.h"

typedef struct _rock Rock;

struct _rock {
	obj_type_t obj_type;

	CollisionShape* collision;

	Object* obj;
};

Rock* newRock();

void rockReady(Object* obj);
void rockUpdate(Object* obj, float deltatime);
void rockRender(Object* obj);