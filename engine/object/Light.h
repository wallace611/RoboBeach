#pragma once

#include "object/Object.h"

typedef struct _light Light;

struct _light {
	obj_type_t obj_type;

	int light_numb;
	int type;
	float diffuse[4];
	float specular[4];
	float ambient[4];

	unsigned char isOn;
	unsigned char isRenderSphere;

	Object* obj;
};

Light* newLight(int numb);

void lightRender(Object* obj);