#pragma once

#define OC_DEFAULT_SIZE 8

typedef struct container {
	size_t size;
	size_t end;
	void** list;
} ObjContainer;

ObjContainer* newObjContainer();
int ocPushBack(ObjContainer* container, void* obj);
int ocRemove(ObjContainer* container, void* target);
int ocClear(ObjContainer* container);
unsigned ocSize(ObjContainer* container);

static int doubleSize(ObjContainer* container);
static int divideSize(ObjContainer* container);