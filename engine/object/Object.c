#include "Object.h"

#include "object/Camera.h"
#include "object/Triangle.h"
#include "utils/Shapes.h"

Object* newObject() {
	Object* obj = (Object*)malloc(sizeof(Object));
	if (obj == NULL) return NULL;

	obj->obj_type = OBJECT;
	obj->id = current_id++;
	glm_mat4_identity(obj->transform);
	obj->movSpeed = 1.0f;

	obj->ready = objReady;
	obj->update = objUpdate;
	obj->render = objRender;
	return obj;
}

Object* inheriteObj(void* self, int self_type) {
	if (self == NULL) return NULL;
	Object* obj = (Object*)malloc(sizeof(Object));
	if (obj == NULL) return NULL;

	obj->obj_type = self_type;
	obj->id = current_id++;
	glm_mat4_identity(obj->transform);
	obj->movSpeed = 1.0f;

	obj->inheritance = self;

	obj->ready = objReady;
	obj->update = objUpdate;
	obj->render = objRender;
	return obj;
}

void* cast(Object* obj, obj_type_t type) {
	void* up = obj->inheritance;
	switch (type) {
	case OBJECT:
		return obj;

	case CAMERA:
		if (((Camera*)up)->obj_type == CAMERA) {
			return up;
		}
		break;

	case TRIANGLE:
		if (((Triangle*)up)->obj_type == TRIANGLE) {
			return up;
		}
		break;
	}
	return NULL;
}

void objReady(Object* obj) {
	
}

void objUpdate(Object* obj, float deltatime) {
	
}

void objRender(Object* obj) {
	
}
