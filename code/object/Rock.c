#include "Rock.h"

#include "Engine.h"

Rock* newRock() {
	Rock* rock = (Rock*)malloc(sizeof(Rock));
	if (rock == NULL) return NULL;

	rock->obj_type = ROCK;
	rock->obj = inheriteObj(rock, ROCK);
	if (rock->obj == NULL) return NULL;

	rock->collision = newCollisionShape();
	if (rock->collision == NULL) return NULL;
	rock->collision->channel = CC_COLLISION;
	worldSpawnObj(world, rock->collision->obj);
	objAttachmentTo(rock->obj, rock->collision->obj);

	rock->obj->ready = rockReady;
	rock->obj->update = rockUpdate;
	rock->obj->render = rockRender;

	return rock;
}

void rockReady(Object* obj) {
	objReadyChild(obj);
}

void rockUpdate(Object* obj, float deltatime) {
	objUpdateChild(obj, deltatime);
}

void rockRender(Object* obj) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glMultMatrixf(obj->transform);

	glPushMatrix();
    glColor3f(0.5f, 0.35f, 0.05f); 
	glutSolidSphere(1.0f, 16, 16);

	glPopMatrix();

	objRenderChild(obj);

	glPopMatrix();
}
