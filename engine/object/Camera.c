#include "Camera.h"

#include "input/KeyboardMapper.h"

#include <math.h>

Camera* newCamera() {
	Camera* cam = (Camera*)malloc(sizeof(Camera));
	if (cam == NULL) return NULL;

	cam->fov = 60.0f;
	cam->zNear = 0.1f;
	cam->zFar = 1000.0f;

	cam->forwardVal = 0.0f;
	cam->sideVal = 0.0f;
	cam->pitchVal = 0.0f;
	cam->yawVal = -90.0f;

	cam->obj = inheriteObject();
	if (cam->obj == NULL) return NULL;

	cam->obj->obj_type = CAMERA;
	cam->obj->movSpeed = 5.0f;

	cam->obj->ready = camReady;
	cam->obj->update = camUpdate;
	cam->obj->render = camRender;

	return cam;
}

void camReady(Camera* cam) {
	glm_vec3_copy((vec3) { 0.0f, 0.0f, 3.0f }, cam->camPosition);
	glm_vec3_copy((vec3) { 0.0f, 0.0f, -1.0f }, cam->camFront);
	glm_vec3_copy((vec3) { 0.0f, 1.0f, 0.0f }, cam->camUp);

}

void camUpdate(Camera* cam, float deltatime) {
	vec3 dir = {
		cosf(glm_rad(cam->yawVal)) * cosf(glm_rad(cam->pitchVal)),
		sinf(glm_rad(cam->pitchVal)),
		sinf(glm_rad(cam->yawVal)) * cosf(glm_rad(cam->pitchVal))
	};
	glm_vec3_normalize_to(dir, cam->camFront);

	// camPosition += movSpeed (scalar) * forwardVal (scalar) * camFront (vec3);
	glm_vec3_muladds(cam->camFront, cam->obj->movSpeed * cam->forwardVal * deltatime, cam->camPosition);

	// camPosition -= normalize(camFront X camUp) * movSpeed * sideVal;
	vec3 frontXup;
	glm_vec3_crossn(cam->camFront, cam->camUp, frontXup);
	glm_vec3_muladds(frontXup, cam->obj->movSpeed * cam->sideVal * deltatime, cam->camPosition);

	vec3 frontPos;
	glm_vec3_add(cam->camPosition, cam->camFront, frontPos);

	glm_lookat(cam->camPosition, frontPos, cam->camUp, cam->obj->transform);

	cam->forwardVal = 0.0f;
	cam->sideVal = 0.0f;

	printf("[%f %f %f]\n", cam->camPosition[0], cam->camPosition[1], cam->camPosition[2]);
}

void camRender(Camera* cam) {
	mat4 mirrorXZ;
	glm_mat4_identity(mirrorXZ);
	mirrorXZ[0][0] = -1;
	mirrorXZ[2][2] = -1;

	mat4 ttmp;
	glm_mat4_mul(cam->obj->transform, mirrorXZ, ttmp);
	glMultMatrixf(ttmp);
}