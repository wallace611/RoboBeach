#include "CollisionShape.h"

CollisionShape* newCollisionShape() {
	CollisionShape* cs = (CollisionShape*)malloc(sizeof(CollisionShape));
	if (cs == NULL) return NULL;

	cs->comp_type = COLLISION;
	cs->comp = inheriteComp(cs, COLLISION);

	cs->comp->ready = csReady;
	cs->comp->update = csUpdate;
	cs->comp->render = csRender;

	return cs;
}

void csReady(CollisionShape* cs) {
}

void csUpdate(CollisionShape* cs, float deltatime) {
}

void csRender(CollisionShape* cs) {
}
