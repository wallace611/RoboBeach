#pragma once

#include "utils/ObjContainer.h"
#include "object/utils.h"

#include <GL/freeglut.h>
#include <cglm/cglm.h>

static int comp_next_id = 0;

typedef struct _comp Component;

struct _comp {
	class_type_t check_code;
	comp_type_t comp_type;
	int id;
	mat4 transform;

	/* The children of the component */
	ObjContainer* child_list;

	void* inheritance;

	void (*ready) (Component* comp);
	void (*update) (Component* comp, float deltatime);
	void (*render) (Component* comp);
};

Component* newComponent();
Component* inheriteComp(void* self, comp_type_t self_type);

void compReadyChild(Component* comp);
void compUpdateChild(Component* comp, float deltatime);
void compRenderChild(Component* comp);