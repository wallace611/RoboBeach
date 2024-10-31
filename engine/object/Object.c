#include "Object.h"

#include "utils/Shapes.h"

Object* newObject() {
	Object* obj = (Object*)malloc(sizeof(Object));
	if (obj == NULL) return NULL;

	obj->id = current_id++;
	glm_mat4_identity(obj->transform);
	obj->movSpeed = 1.0f;
	obj->obj_type = OBJECT;

	obj->ready = objReady;
	obj->update = objUpdate;
	obj->render = objRender;
	return obj;
}

Object* inheriteObject() {
	Object* obj = (Object*)malloc(sizeof(Object));
	if (obj == NULL) return NULL;

	obj->id = current_id++;
	glm_mat4_identity(obj->transform);
	obj->movSpeed = 1.0f;
	return obj;
}

void objReady(Object* obj) {
	
}

void objUpdate(Object* obj, float deltatime) {
	glm_rotate(obj->transform, -1.0f * deltatime, (vec3) { 0.0f, 1.0f, 0.0f });
}

void objRender(Object* obj) {
	glPushMatrix();
	glMultMatrixf(obj->transform);
	drawTriangle();
	glPopMatrix();
}
