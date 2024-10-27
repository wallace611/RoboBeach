#include "Camera.h"

#include "input/KeyboardMapper.h"

#include <math.h>

Camera* newCamera() {
	Camera* cam = (Camera*)malloc(sizeof(Camera));
	if (cam == NULL) return NULL;

	cam->fov = 60.0f;
	cam->zNear = 0.1f;
	cam->zFar = 1000.0f;
	cam->rotation[0] = 0.0f;
	cam->rotation[1] = 0.0f;
	cam->rotation[2] = 0.0f;

	cam->obj = inheriteObject();
	if (cam->obj == NULL) return NULL;

	cam->obj->obj_type = CAMERA;

	cam->obj->readyFunction = camReady;
	cam->obj->updateFunction = camUpdate;
	cam->obj->renderFunction = camRender;
	return cam;
}

void camReady(Camera* cam) {
	
}

void camUpdate(Camera* cam, float deltatime) {
	Object* camObj = cam->obj;

	// calculate next position
	vec4 tloc, vloc;
	mat4 trot, vrot;
	vec3 tscl, vscl;
	glm_decompose(camObj->transform, tloc, trot, tscl);
	glm_decompose(camObj->velocity, vloc, vrot, vscl);
	vec3 next = {
		tloc[0] + vloc[0] * deltatime * 5,
		tloc[1] + vloc[1] * deltatime * 5,
		tloc[2] + vloc[2] * deltatime * 5
	};
	glm_mat4_identity(camObj->velocity);

	// calculate direction
	vec3 direction = {
		cosf(cam->rotation[0]) * cosf(cam->rotation[1]) + next[0],
		sinf(cam->rotation[0]) + next[1],
		cosf(cam->rotation[0]) * sinf(cam->rotation[1]) + next[2]
	};

	// look at position + direction
	glm_mat4_identity(camObj->transform);
	glm_lookat(next, direction, (vec3) { 0.0f, 1.0f, 0.0f }, camObj->transform);
	printf("loc: %f %f %f\n", next[0], next[1], next[2]);
	printf("dir: %f %f %f\n", direction[0], direction[1], direction[2]);

	// move to next position
	for (int i = 0; i < 3; i++) {
		camObj->transform[3][i] = next[i];
	}
}

void camRender(Camera* cam) {
	glMultMatrixf(cam->obj->transform);
	glutSolidCube(1.0f);
}
