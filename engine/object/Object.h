#pragma once

#include <GL/freeglut.h>
#include <cglm/cglm.h>

typedef int obj_type_t;

// Registered object type
#define OBJECT    0
#define CAMERA	  1
#define TRIANGLE  2

static int current_id = 0;

typedef struct _obj Object;

typedef struct _obj {
	obj_type_t obj_type;
	int id;
	mat4 transform;

	float movSpeed;

	void* inheritance;
	
	void (*ready) (Object* obj);
	void (*update) (Object* obj, float deltatime);
	void (*render) (Object* obj);
};

Object* newObject();
Object* inheriteObj(void* self, int self_type);

void* cast(Object* obj, obj_type_t type);

static void objReady(Object* obj);
static void objUpdate(Object* obj, float deltatime);
static void objRender(Object* obj);