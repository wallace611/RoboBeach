#pragma once

#include "object/component/Component.h"

typedef struct _collision CollisionShape;

struct _collision {
	comp_type_t comp_type;

	Component* comp;
};

CollisionShape* newCollisionShape();

void csReady(CollisionShape* cs);
void csUpdate(CollisionShape* cs, float deltatime);
void csRender(CollisionShape* cs);