#pragma once

#include "object/Object.h"

typedef struct _tri Triangle;

struct _tri {
	obj_type_t obj_type;
	Object* obj;
};

Triangle* newTriangle();

static void triUpdate(Object* tri, float deltatime);
static void triRender(Object* tri);