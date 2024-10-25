#pragma once

#include <GL/freeglut.h>
#include <cglm/cglm.h>

typedef struct obj {
	int id;
	mat4 transform;
} Object;

static int current_id = 0;

Object* newObject();