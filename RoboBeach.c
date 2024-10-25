#include "Engine.h"

#include "utils/ObjContainer.h"

#include <stdio.h>

#include <cglm/cglm.h>

int main(int argc, char** argv) {
	engineInit(&argc, argv);
	//engineStartLoop();

	ObjContainer* oc = ocCreate();
	Object* tmp[10000] = { 0 };
	for (int i = 0; i < 10000; i++) {
		tmp[i] = newObject();
		ocPushBack(oc, tmp[i]);
	}
	for (int i = 0; i < 9998; i++) {
		ocRemove(oc, tmp[i]);
	}
	return 0;
}