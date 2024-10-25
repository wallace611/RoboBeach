#pragma once

#include "object/Object.h"

#define OC_DEFAULT_SIZE 8

typedef struct container {
	unsigned size;
	unsigned end;
	Object** list;
} ObjContainer;

ObjContainer* ocCreate();
int ocPushBack(ObjContainer* container, Object* obj);
int ocRemove(ObjContainer* container, Object* target);
int ocClear(ObjContainer* container);

static int doubleSize(ObjContainer* container);
static int divideSize(ObjContainer* container);