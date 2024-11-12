#include "Umbrella.h"

#include "Engine.h"

Umbrella* newUmbrella() {
	Umbrella* umb = (Umbrella*)malloc(sizeof(Umbrella));
	if (umb == NULL) return NULL;

	umb->obj_type = UMBRELLA;
	umb->obj = inheriteObj(umb, UMBRELLA);
	if (umb->obj == NULL) return NULL;

	umb->csCollide = newCollisionShape();
	if (umb->csCollide == NULL) return NULL;
	umb->csCollide->channel = CC_COLLISION;
	umb->csCollide->collided = botTouchFloor;
	objAttachmentTo(umb->obj, umb->csCollide->obj);
	worldSpawnObj(world, umb->csCollide->obj);
	glm_scale(umb->csCollide->obj->transform, (vec3) { .3f, 1.f, .3f });

	umb->csPickup = newCollisionShape();
	if (umb->csPickup == NULL) return NULL;
	umb->csPickup->channel = CC_PICKUP;
	objAttachmentTo(umb->obj, umb->csPickup->obj);
	worldSpawnObj(world, umb->csPickup->obj);
	glm_scale(umb->csPickup->obj->transform, (vec3) { 2.0f, 2.0f, 2.0f });

	umb->csFloor = newCollisionShape();
	if (umb->csFloor == NULL) return NULL;
	umb->csFloor->channel = CC_FLOOR;
	objAttachmentTo(umb->obj, umb->csFloor->obj);
	worldSpawnObj(world, umb->csFloor->obj);
	glm_translate(umb->csFloor->obj->transform, (vec3) { .0f, 3.1f, .0f });
	glm_scale(umb->csFloor->obj->transform, (vec3) { 3.0f, 1.f, 3.0f });

	umb->obj->aloc[1] = -9.8;

	umb->obj->update = umbUpdate;
	umb->obj->render = umbRender;
	
	return umb;
}

void umbUpdate(Object* obj, float deltatime) {
	if (obj->owner == NULL) {
		vec3 atmp, vtmp;
		glm_vec3_scale(obj->aloc, deltatime, atmp);
		glm_vec3_copy(obj->vloc, vtmp);
		glm_vec3_add(vtmp, atmp, obj->vloc);

		glm_vec3_scale(obj->vloc, deltatime, vtmp);
		glm_translate(obj->transform, vtmp);
		obj->vloc[1] *= 0.9995;
		obj->vloc[0] *= 0.97;
		obj->vloc[2] *= 0.97;
	}
}

void umbRender(Object* obj) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	mat4 worldTrans;
	objGetWorldTransform(worldTrans, obj);
	glMultMatrixf(worldTrans);

	glPushMatrix();
	{
		glTranslatef(.0f, -.5f, .0f);
		glRotatef(90.0f, -1.0f, .0f, .0f);
		glColor3f(.7f, .7f, .7f);
		glutSolidCylinder(.1, 4.0, 8, 8);
		glTranslatef(.0f, .0f, 4.0f);
		glColor3f(.3f, .2f, .8f);
		glutSolidCylinder(3.0, .1, 16, 16);
	}
	glPopMatrix();

	objRenderChild(obj);

	glPopMatrix();
}

void botTouchFloor(Object* self, CollisionShape* selfcs, Object* other, CollisionShape* othercs) {
	if (other == NULL || othercs == NULL || self == other) return;
	if (othercs->channel == CC_FLOOR && selfcs->channel == CC_COLLISION) {
		mat4 worldTrans;
		objGetWorldTransform(worldTrans, othercs->obj);
		float y = worldTrans[3][1] + othercs->height / 2 + selfcs->height / 2;
		self->transform[3][1] = y;
		self->vloc[1] = 0.0f;
	}
}
