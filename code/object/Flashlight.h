#pragma once

#include "object/Object.h"

typedef struct _flash Flashlight;

struct _flash {
	obj_type_t obj_type;

	Object* obj;
};

Flashlight* newFlashlight();