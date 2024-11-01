#pragma once

#include "object/Object.h"

typedef struct _cam Camera;

struct _cam {
	float fov;
	float zNear;
	float zFar;

	vec3 camPosition;
	vec3 camFront;
	vec3 camUp;
	vec3 camRot;

	float forwardVal;
	float sideVal;
	float pitchVal;
	float yawVal;

	float pitchSensitivity;
	float yawSensitivity;

	float fraction;

	Object* obj;
};

Camera* newCamera();

static void camReady(Camera* cam);
static void camUpdate(Camera* cam, float deltatime);
static void camRender(Camera* cam);