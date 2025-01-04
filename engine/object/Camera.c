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

	cam->pitchSensitivity = 3.0f;
	cam->yawSensitivity = 3.0f;

	cam->moveFraction = 0.9f;
	cam->rotateFraction = 0.9f;

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
	/*Camera* cam = cast(obj, CAMERA);
	*/
}

void camUpdate(Object* obj, float deltatime) {
	
}

void camRender(Object* obj) {
	Camera* cam = cast(obj, CAMERA);
	mat4 mirrorZ;
	glm_mat4_identity(mirrorZ);
	mirrorZ[2][2] = -1;

	mat4 ttmp;
	glm_mat4_mul(cam->obj->transform, mirrorZ, ttmp);
	glMultMatrixf(ttmp);

}