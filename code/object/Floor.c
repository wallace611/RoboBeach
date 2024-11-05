#include "Floor.h"

#include "Engine.h"
#include "utils/Shapes.h"

Floor* newFloor() {
	Floor* floor = (Floor*)malloc(sizeof(Floor));
	if (floor == NULL) return NULL;

	floor->obj_type = FLOOR;

	CollisionShape* cs = floor->csShape = newCollisionShape();
	if (cs == NULL) {
		free(floor);
		return NULL;
	}

	floor->obj = inheriteObj(floor, FLOOR);
	if (floor->obj == NULL) {
		free(floor), free(cs);
		return NULL;
	}

	worldSpawnObj(world, cs->obj);
	objAttachmentTo(floor->obj, cs->obj);
	
	glm_translate(floor->obj->transform, (vec3) { 0.0f, -2.0f, 0.0f });
	glm_scale(floor->obj->transform, (vec3) { 100.0f, .1f, 100.0f });

	floor->obj->render = floorRender;

	return floor;
}

void floorRender(Object* obj) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glMultMatrixf(obj->transform);
	glBegin(GL_POLYGON);
	{
		// top
		glColor3f(.023f, .72f, .09f);
		glVertex3f(.5f, .5f, .5f);
		glVertex3f(.5f, .5f, -.5f);
		glVertex3f(-.5f, .5f, -.5f);
		glVertex3f(-.5f, .5f, .5f);
	}
	glEnd();
	glPopMatrix();
}
