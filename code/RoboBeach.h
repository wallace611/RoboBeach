#pragma once

#include "input/InputMapper.h"

#include "object/Floor.h"

struct _scene {
	Floor* sand;
	Floor* sea;
} Scene;

int runRoboBeach(int* argc, char** argv);

void setupInputMapping();
// input callback
void moving(float forward, float side);
void rotating(float pitch, float yaw);