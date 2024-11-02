#include "Camera.h"

#include "input/InputMapper.h"

#include <math.h>

Camera* newCamera() {
	Camera* cam = (Camera*)malloc(sizeof(Camera));
	if (cam == NULL) return NULL;

	cam->obj_type = CAMERA;
	cam->fov = 60.0f;
	cam->zNear = 0.9f;
	cam->zFar = 1000.0f;

	cam->forwardVal = 0.0f;
	cam->sideVal = 0.0f;
	cam->pitchVal = 0.0f;
	cam->yawVal = 0.0f;

	cam->pitchSensitivity = 2.0f;
	cam->yawSensitivity = 2.0f;

	cam->fraction = 0.9f;

	cam->obj = inheriteObj(cam, cam->obj_type);
	if (cam->obj == NULL) return NULL;

	cam->obj->obj_type = CAMERA;
	cam->obj->movSpeed = 5.0f;

	cam->obj->ready = camReady;
	cam->obj->update = camUpdate;
	cam->obj->render = camRender;

	return cam;
}

void camReady(Object* obj) {
	Camera* cam = cast(obj, CAMERA);
	glm_vec3_copy((vec3) { 0.0f, 0.0f, 3.0f }, cam->camPosition);
	glm_vec3_copy((vec3) { 0.0f, 0.0f, -1.0f }, cam->camFront);
	glm_vec3_copy((vec3) { 0.0f, 1.0f, 0.0f }, cam->camUp);
	glm_vec3_copy((vec3) { 0.0f, -90.0f, 0.0f }, cam->camRot);
}

void camUpdate(Object* obj, float deltatime) {
	Camera* cam = cast(obj, CAMERA);
	cam->camRot[0] = fmod(cam->camRot[0], 360.0f);
	cam->camRot[1] = fmod(cam->camRot[1], 360.0f);
	cam->camRot[2] = fmod(cam->camRot[2], 360.0f);

	float lastPitch = cam->camRot[0];
	cam->camRot[0] += cam->pitchVal * deltatime * cam->pitchSensitivity;
	cam->camRot[1] += cam->yawVal * deltatime * cam->yawSensitivity;
	if (cam->camRot[0] > 90 || cam->camRot[0] < -90) {
		cam->camRot[0] = lastPitch;
	}

	vec3 dir = {
		cosf(glm_rad(cam->camRot[1])) * cosf(glm_rad(cam->camRot[0])),
		sinf(glm_rad(cam->camRot[0])),
		sinf(glm_rad(cam->camRot[1])) * cosf(glm_rad(cam->camRot[0]))
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

	cam->forwardVal *= cam->fraction;
	cam->sideVal *= cam->fraction;
	cam->pitchVal *= cam->fraction;
	cam->yawVal *= cam->fraction;
}

void camRender(Object* obj) {
	Camera* cam = cast(obj, CAMERA);
	mat4 mirrorXZ;
	glm_mat4_identity(mirrorXZ);
	mirrorXZ[0][0] = -1;
	mirrorXZ[2][2] = -1;

	mat4 ttmp;
	glm_mat4_mul(cam->obj->transform, mirrorXZ, ttmp);
	glMultMatrixf(ttmp);
}