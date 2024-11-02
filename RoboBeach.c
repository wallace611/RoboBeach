#include "Engine.h"

#include "utils/ObjContainer.h"
#include "object/Object.h"
#include "utils/String.h"
#include "code/object/Triangle.h"

#include <stdio.h>

#include <cglm/cglm.h>

int main(int argc, char** argv) {
	engineInit(&argc, argv);
	Triangle* obj = newTriangle();
	glm_translate(obj->obj->transform, (vec3) { 0.0f, 0.0f, 10.0f });
	worldSpawnObj(world, obj->obj);

	ocPushBack(objDebug->objToShow, obj->obj);

	obj = newTriangle();
	glm_translate(obj->obj->transform, (vec3) { 10.0f, 1.0f, 10.0f });
	worldSpawnObj(world, obj->obj);

	ocPushBack(objDebug->objToShow, obj->obj);

	engineStartLoop();

	return 0;
}