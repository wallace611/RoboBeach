#include "Triangle.h"

#include "utils/Shapes.h"

Triangle* newTriangle() {
	Triangle* tri = (Triangle*)malloc(sizeof(Triangle));
	if (tri == NULL) return NULL;

	tri->obj_type = TRIANGLE;
	tri->obj = inheriteObj(tri, tri->obj_type);
	if (tri->obj == NULL) return NULL;

	tri->obj->update = triUpdate;
	tri->obj->render = triRender;

	return tri;
}

void triUpdate(Object* tri, float deltatime) {
	glm_rotate(tri->transform, -1.0f * deltatime, (vec3) { 0.0f, 1.0f, 0.0f });
}

void triRender(Object* tri) {
	glPushMatrix();
	glMultMatrixf(tri->transform);
	drawTriangle();
	glPopMatrix();
}
