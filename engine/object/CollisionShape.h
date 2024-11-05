#pragma once

#include "object/Object.h"

typedef struct _collision CollisionShape;

struct _collision {
	comp_type_t obj_type;

	float width, height, depth;

	float color[3];
	unsigned char bInFront;
	unsigned char bIsVisible;

	Object* obj;

	void (*collided) (Object* self, CollisionShape* selfcs, Object* other, CollisionShape* othercs);
};

CollisionShape* newCollisionShape();

void csReady(Object* obj);
void csUpdate(Object* obj, float deltatime);
void csRender(Object* obj);
unsigned char csIsCollideWith(CollisionShape* self, CollisionShape* target);
void csCollided(Object* self, CollisionShape* selfcs, Object* other, CollisionShape* othercs);