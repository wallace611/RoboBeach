#pragma once

#include "input/InputMapper.h"

#include "object/Floor.h"
#include "object/robot/Robot.h"
#include "object/Rock.h"
#include "object/Umbrella.h"
#include "object/Fan.h"

struct _scene {
	Floor* sand;
	Floor* water;
	Rock* rock1;
	Umbrella* umb1;
	Umbrella* umb2;
	Umbrella* umb3;
	Umbrella* umb4;
	Umbrella* umb5;
	Umbrella* umb6;
	Fan* fan1;
	
	Robot* bot;
	unsigned int currentCamera;
	Camera* cameras[2];
} Scene;

int runRoboBeach(int* argc, char** argv);

void setupObjects();
void setupInputMapping();

// input callback
void rbMove(float forward, float side);
void rbRotate(float pitch, float yaw);
void rbJump();
void rbSwitchCamera();
void rbKneeDown();
void rbPickup();
void rbDrop();
void rbRun(float val);
void rbPause();
void rbShowCollision();