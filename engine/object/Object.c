#include "Object.h"

#include "object/Camera.h"
#include "object/Triangle.h"
#include "utils/Shapes.h"

Object* newObject() {
	Object* obj = (Object*)malloc(sizeof(Object));
	if (obj == NULL) return NULL;

	obj->obj_type = OBJECT;
	obj->id = obj_next_id++;
	glm_mat4_identity(obj->transform);
	glm_vec3_zero(obj->vloc);
	glm_vec3_zero(obj->vrot);
	glm_vec3_one(obj->vscl);
	obj->movSpeed = 1.0f;

	obj->child_list = newObjContainer();
	obj->attach_list = newObjContainer();
	obj->owner = NULL;

	obj->inheritance = NULL;

	obj->ready = objReadyChild;
	obj->update = objUpdateChild;
	obj->render = objRenderChild;

	obj->check_code = generate_checkcode(obj);
	return obj;
}

Object* inheriteObj(void* self, obj_type_t self_type) {
	if (self == NULL) return NULL;
	Object* obj = newObject();
	if (obj == NULL) return NULL;

	obj->obj_type = self_type;
	obj->inheritance = self;

	obj->check_code = generate_checkcode(obj);
	return obj;
}

void objReadyChild(Object* obj) {
	for (int i = 0; i < obj->child_list->end; i++) {
		Component* child = cast(obj->child_list->list[i], COMPONENT);
		child->ready(child);
	}
}

void objUpdateChild(Object* obj, float deltatime) {
	for (int i = 0; i < obj->child_list->end; i++) {
		Component* child = cast(obj->child_list->list[i], COMPONENT);
		child->update(child, deltatime);
	}
}

void objRenderChild(Object* obj) {
	for (int i = 0; i < obj->child_list->end; i++) {
		Component* child = cast(obj->child_list->list[i], COMPONENT);
		child->render(child);
	}
}

void objAddChild(Object* owner, Component* child) {
	ocPushBack(owner->child_list, child);
}

void objRemoveChild(Object* owner, Component* target) {
	ocRemove(owner, target);
}

void objAttachmentTo(Object* parent, Object* child) {
	ocPushBack(parent->attach_list, child);
	child->owner = parent;
}

void objDeattachment(Object* parent, Object* target) {
	ocRemove(parent->attach_list, target);
	target->owner = NULL;
}

void objGetWorldTransform(mat4 trans, Object* obj) {
	if (obj->owner == NULL) {
		glm_mat4_copy(obj->transform, trans);
		return;
	}
	objGetWorldTransform(trans, obj->owner);
	mat4 tmp;
	glm_mat4_copy(trans, tmp);
	glm_mat4_mul(tmp, obj->transform, trans);
}
