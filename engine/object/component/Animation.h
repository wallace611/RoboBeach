#pragma once

#include "object/Object.h"
#include "object/component/Component.h"

typedef struct _anim Animation;

struct _anim {
	comp_type_t obj_type;

	Object* parent;
	obj_type_t parent_type;

	float timer;
	unsigned char start;
	float magn;

	Component* comp;
};

Animation* newAnimation(Object* parent, obj_type_t parentType);

void animReady(Component* comp);
void animUpdate(Component* comp, float deltatime);