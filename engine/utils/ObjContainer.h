#pragma once

#include "object/Object.h"

#define OC_DEFAULT_SIZE 8

typedef struct container {
	size_t size;
	size_t end;
	Object** list;
} ObjContainer;

ObjContainer* newObjContainer();
int ocPushBack(ObjContainer* container, Object* obj);
int ocRemove(ObjContainer* container, Object* target);
int ocClear(ObjContainer* container);
unsigned ocSize(ObjContainer* container);

static int doubleSize(ObjContainer* container);
static int divideSize(ObjContainer* container);