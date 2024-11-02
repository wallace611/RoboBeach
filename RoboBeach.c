#include "Engine.h"

#include "utils/ObjContainer.h"
#include "object/Object.h"
#include "utils/String.h"
#include "code/object/Triangle.h"

#include <stdio.h>

#include <cglm/cglm.h>

int main(int argc, char** argv) {
	engineInit(&argc, argv);
	
	Triangle* t1 = newTriangle();
	glm_translate(t1->obj->transform, (vec3) { 10.0f, 0.0f, 10.0f });
	worldSpawnObj(world, t1->obj);

	Triangle* t2 = newTriangle();
	glm_translate(t2->obj->transform, (vec3) { 2.0f, 2.0f, 0.0f });
	worldSpawnObj(world, t2->obj);
	objAttachmentTo(t1->obj, t2->obj);

	engineStartLoop();

	return 0;
}