#pragma once

#include "object/Object.h"

typedef struct _cam Camera;

struct _cam {
	obj_type_t obj_type;

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

	float moveFraction;
	float rotateFraction;

	Object* obj;
};

Camera* newCamera();

static void camReady(Object* obj);
static void camUpdate(Object* obj, float deltatime);
static void camRender(Object* obj);