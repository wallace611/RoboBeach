#pragma once

#include "utils/ObjContainer.h"
#include "object/utils.h"

#include <GL/freeglut.h>
#include <cglm/cglm.h>

static int comp_next_id = 0;

typedef struct _comp Component;

struct _comp {
	class_type_t check_code;
	comp_type_t obj_type;
	int id;
	mat4 transform;

	/* The children of the component */
	ObjContainer* child_list;

	void* inheritance;

	void (*ready) (Component* obj);
	void (*update) (Component* obj, float deltatime);
	void (*render) (Component* obj);
};

Component* newComponent();
Component* inheriteComp(void* self, comp_type_t self_type);

void compReadyChild(Component* obj);
void compUpdateChild(Component* obj, float deltatime);
void compRenderChild(Component* obj);