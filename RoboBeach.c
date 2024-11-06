#include "Engine.h"

#include "utils/ObjContainer.h"
#include "object/Object.h"
#include "utils/String.h"
#include "code/object/Triangle.h"
#include "code/object/robot/Robot.h"
#include "code/object/Floor.h"
#include "input/InputMapper.h"

#include <stdio.h>

#include <cglm/cglm.h>

typedef unsigned char perspective_t;
#define RB_ROBOT	1
#define RB_WORLD	2

perspective_t persp;

Triangle* t1;
Triangle* t2;
Robot* r;
Floor* flr;
Camera* worldCam;
Camera* botCam;

void switchPerspective() {
	if (persp == RB_WORLD) {
		persp = RB_ROBOT;
		worldSetCamera(world, botCam);
	}
	else {
		persp = RB_WORLD;
		worldSetCamera(world, worldCam);
	}
}

void moving(float forward, float side) {
	if (persp == RB_WORLD) {
		if (forward != 0) world->cam->forwardVal = forward;
		if (side != 0) world->cam->sideVal = side;
	}
	else {
		botMoving(r, forward, side);
	}
}

void rotating(float pitch, float yaw) {
	if (persp == RB_WORLD) {
		if (pitch != 0) world->cam->pitchVal += pitch;
		if (yaw != 0) world->cam->yawVal += yaw;
	}
	else {
		if (pitch != 0) world->cam->pitchVal -= pitch;
		if (yaw != 0) world->cam->yawVal += yaw;
	}
}

void jump() {
	if (persp == RB_ROBOT) {
		r->obj->vloc[1] += 5.0f;
	}
}

void pickup() {
	botPickup(r);
}

void drop() {
	botDrop(r);
}

void mappingKey() {
	imMapFloat2Key('w', KEY_HOLD, moving, 1, 0);
	imMapFloat2Key('s', KEY_HOLD, moving, -1, 0);
	imMapFloat2Key('a', KEY_HOLD, moving, 0, -1);
	imMapFloat2Key('d', KEY_HOLD, moving, 0, 1);
	imMapActionKey('f', KEY_PRESS, pickup);
	imMapActionKey('q', KEY_PRESS, drop);
	imMapActionKey(32, KEY_PRESS, jump);
	imMapFloat2Key(MOUSE_MOTION, NULL, rotating, 0, 0);
	imMapActionKey(27, KEY_PRESS, pause);
	imMapActionKey('v', KEY_PRESS, switchPerspective);
}

int main(int argc, char** argv) {
	engineInit(&argc, argv);
	
	t1 = newTriangle();
	glm_translate(t1->obj->transform, (vec3) { 1.0f, 0.0f, 10.0f });
	worldSpawnObj(world, t1->obj);
	CollisionShape* cs = newCollisionShape();
	cs->bIsVisible = 1;
	worldSpawnObj(world, cs->obj);
	objAttachmentTo(t1->obj, cs->obj);
	cs->channel = CC_PICKUP;

	t1 = newTriangle();
	glm_translate(t1->obj->transform, (vec3) { 0.0f, 0.0f, 10.0f });
	worldSpawnObj(world, t1->obj);
	cs = newCollisionShape();
	cs->bIsVisible = 1;
	worldSpawnObj(world, cs->obj);
	objAttachmentTo(t1->obj, cs->obj);
	cs->channel = CC_PICKUP;

	t2 = newTriangle();
	glm_translate(t2->obj->transform, (vec3) { 2.0f, 2.0f, 0.0f });
	worldSpawnObj(world, t2->obj);
	objAttachmentTo(t1->obj, t2->obj);

	r = newRobot();
	worldSpawnObj(world, r->obj);
	glm_translate(r->obj->transform, (vec3) { 0.0f, 20.0f, 0.0f });

	flr = newFloor();
	worldSpawnObj(world, flr->obj);

	worldCam = newCamera();

	botCam = r->cam;
	worldSetCamera(world, botCam);
	persp = RB_ROBOT;

	mappingKey();

	engineStartLoop();

	return 0;
}