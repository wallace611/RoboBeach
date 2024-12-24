#pragma once

#include "object/Object.h"
#include "object/CollisionShape.h"
#include "object/Light.h"

typedef struct _flash Flashlight;

struct _flash {
	obj_type_t obj_type;

	Light* light;
	unsigned char isOn;
	Object* obj;
};

Flashlight* newFlashlight(int numb);

void flashUpdate(Object* obj, float deltatime);

void flashLightRender(Object* obj);