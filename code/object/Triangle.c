#include "Triangle.h"

#include "object/CollisionShape.h"
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
	mat4 tmp;
	glm_mat4_copy(tri->transform, tmp);
	glm_rotate(tri->transform, -1.0f * deltatime, (vec3) { 0.0f, 1.0f, 0.0f });
	objUpdateChild(tri, deltatime);
}

void triRender(Object* tri) {
	glPushMatrix();
	mat4 worldTrans;
	objGetWorldTransform(worldTrans, tri);
	glMultMatrixf(worldTrans);
	drawTriangle();
	objRenderChild(tri);
	glPopMatrix();
}
