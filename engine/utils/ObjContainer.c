#include "ObjContainer.h"

#include <stdlib.h>

ObjContainer* newObjContainer() {
	ObjContainer* c = (ObjContainer*)malloc(sizeof(ObjContainer));
	if (c == NULL) return NULL;

	c->size = OC_DEFAULT_SIZE;
	c->end = 0;
	c->list = (void**)malloc(sizeof(void*) * c->size);
	return c;
}

int ocPushBack(ObjContainer* c, void* obj) {
	if (c == NULL) return -2;

	if (c->size == c->end) {
		int flag = doubleSize(c);
		if (flag == -1) return -1;
	}
	c->list[c->end++] = obj;
	return 1;
}

int ocRemove(ObjContainer* c, void* target) {
	if (c == NULL) return -2;

	int k = 0;
	while (k < c->end && c->list[k] != target) {
		k += 1;
	}
	if (k == c->end) return -1;

	for (int i = k; i < c->end - 1; i++) {
		c->list[i] = c->list[i + 1];
	}
	c->end -= 1;
	if (c->end < (c->size >> 2) && c->size > 8) {
		divideSize(c);
	}
	return 1;	
}

int ocClear(ObjContainer* c) {
	if (c == NULL) return -2;


}

unsigned ocSize(ObjContainer* container) {
	return container->end;
}

int doubleSize(ObjContainer* c) {
	unsigned dsize = c->size << 1;
	void** tmp = (void**)realloc(c->list, sizeof(void*) * dsize);
	if (tmp == NULL) return -1;

	c->list = tmp;
	c->size = dsize;
	return 1;
}

int divideSize(ObjContainer* c) {
	unsigned dsize = c->size >> 2;

	if (dsize < 8) dsize = 8;

	void** tmp = (void**)realloc(c->list, sizeof(void*) * dsize);
	if (tmp == NULL) return -1;

	c->list = tmp;
	c->size = dsize;
	return 1;
}
