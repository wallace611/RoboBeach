#pragma once

#include "object/Object.h"
#include "texture/Texture.h"

typedef struct _bill Billboard;

struct _bill {
	obj_type_t obj_type;

	int texture;

	float x, y;
	float scale;
	float timer;

	Object* obj;
};

Billboard* newBillboard();

void billRender(Object* obj);