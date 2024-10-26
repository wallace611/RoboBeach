#pragma once

#include "object/Object.h"

static float fov = 60.0f;
static float zNear = 0.1f;
static float zFar = 1000.0f;

Object* newCamera();

float camGetFOV();
float camGetZNear();
float camGetZFar();

static void camReady(Object* cam);
static void camUpdate(Object* cam, float deltatime);
static void camRender(Object* cam);