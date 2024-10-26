#pragma once

#include <GL/freeglut.h>
#include <cglm/cglm.h>

typedef int obj_type_t;

#define OBJECT    0
#define COMPONENT 1
#define CAMERA	  2

static int current_id = 0;

typedef struct _obj Object;

typedef struct _obj {
	int id;
	mat4 transform;
	mat4 velocity;
	obj_type_t obj_type;
	
	void (*readyFunction) (Object* obj);
	void (*updateFunction) (Object* obj, float deltatime);
	void (*renderFunction) (Object* obj);
};

Object* newObject();
static void objReady(Object* obj);
static void objUpdate(Object* obj, float deltatime);
static void objRender(Object* obj);