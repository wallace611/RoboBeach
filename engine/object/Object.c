#include "Object.h"

Object* newObject() {
	Object* obj = (Object*)malloc(sizeof(Object));
	if (obj == NULL) return NULL;

	obj->id = current_id++;
	return obj;
}
