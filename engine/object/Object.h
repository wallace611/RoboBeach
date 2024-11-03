#pragma once

#include <GL/freeglut.h>
#include <cglm/cglm.h>

#include "object/component/Component.h"
#include "object/utils.h"
#include "utils/ObjContainer.h"

static int obj_next_id = 0;

typedef struct _obj Object;

struct _obj {
	class_type_t check_code;
	obj_type_t obj_type;
	int id;
	float movSpeed;
	mat4 transform;

	/* The child component of the object.
	The owner object will render all the component in the list*/
	ObjContainer* child_list;

	/* The other object which attach to the object.
	The object in the list won't be rendered by the owner object. */
	ObjContainer* attach_list;

	// This object is attached to the owner object
	Object* owner;

	void* inheritance;
	
	void (*ready) (Object* obj);
	void (*update) (Object* obj, float deltatime);
	void (*render) (Object* obj);
};

Object* newObject();
Object* inheriteObj(void* self, obj_type_t self_type);

void objReady(Object* obj);
void objUpdateChild(Object* obj, float deltatime);
void objRenderChild(Object* obj);
void objAddChild(Object* owner, Component* child);
void objRemoveChild(Object* owner, Component* target);
void objAttachmentTo(Object* parent, Object* child);
void objDeattachment(Object* parent, Object* target);
void objGetWorldTransform(mat4 trans, Object* obj);