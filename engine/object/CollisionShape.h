#pragma once

#include "object/Object.h"

typedef int collision_channel_t;
#define CC_DEFAULT		0
#define CC_COLLISION	1
#define CC_PICKUP		2
#define CC_FLOOR		3
#define CC_WATER		4

typedef struct _collision CollisionShape;

struct _collision {
	comp_type_t obj_type;

	collision_channel_t channel;
	float width, height, depth;

	
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