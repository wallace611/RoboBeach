#pragma once

#include "input/InputMapper.h"

#include "object/Floor.h"
#include "object/robot/Robot.h"
#include "object/Rock.h"
#include "object/Umbrella.h"
#include "object/Fan.h"
#include "object/Light.h"
#include "object/Flashlight.h"

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
	Light* light1;
	Light* sun;
	unsigned char sunOrbit;
	Flashlight* flashLight;
	Fan* fan1;
	Object* axis;
	Object* viewVolume;
	
	Robot* bot;
	unsigned int currentCamera;
	Camera* cameras[4];
} Scene;

float fov;

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
void rbThrow();
void rbRun(float val);
void rbPause();
void rbShowCollision();
void rbZoom(float val);
void rbSwitchView();

void axisRender(Object* obj);
void fpsCamUpdate(Object* obj, float deltatime);
void sunUpdate(Object* obj, float deltatime);

void toggleLight(float val);
void selectLight(float val);

void changeColor(float val);
void changeAttribute(float val);