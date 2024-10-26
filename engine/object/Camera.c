#include "Camera.h"

#include "input/KeyboardMapper.h"

Object* newCamera() {
	Object* cam = (Object*)malloc(sizeof(Object));
	if (cam == NULL) return NULL;

	cam->id = current_id++;
	glm_mat4_identity(cam->transform);
	glm_mat4_identity(cam->velocity);
	cam->obj_type = CAMERA;

	cam->readyFunction = camReady;
	cam->updateFunction = camUpdate;
	cam->renderFunction = camRender;
}

float camGetFOV() {
	return fov;
}

float camGetZNear() {
	return zNear;
}

float camGetZFar() {
	return zFar;
}

void camReady(Object* cam) {
	
}

void camUpdate(Object* cam, float deltatime) {
	mat4 ttmp;
	glm_mat4_copy(cam->transform, ttmp);
	mat4 vtmp;
	glm_mat4_copy(cam->velocity, vtmp);
	vtmp[3][0] *= deltatime * 5;
	vtmp[3][1] *= deltatime * 5;
	vtmp[3][2] *= deltatime * 5;
	glm_mat4_mul(vtmp, ttmp, cam->transform);
	glm_mat4_identity(cam->velocity);
}

void camRender(Object* cam) {
	glMultMatrixf(cam->transform);
}
