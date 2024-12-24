#include "RoboBeach.h"

#include "Engine.h"
#include "utils/Shapes.h"

int runRoboBeach(int* argc, char** argv) {
	engineInit(argc, argv);

	setupObjects();

	setupInputMapping();

	engineStartLoop();

	return 0;
}

void setupObjects() {
	Scene.sand = newFloor();
	check(Scene.sand);
	worldSpawnObj(world, Scene.sand->obj);
	Scene.sand->color[0] = .76f;
	Scene.sand->color[1] = .76f;
	Scene.sand->color[2] = .33f;
	Scene.sand->type = 0;
	glm_translate(Scene.sand->obj->transform, (vec3) { .0f, -2.0f, -15.0f });
	glm_scale(Scene.sand->obj->transform, (vec3) { 50.0f, 1.0f, 50.0f });

	Scene.water = newFloor();
	check(Scene.water);
	worldSpawnObj(world, Scene.water->obj);
	Scene.water->color[0] = .33f;
	Scene.water->color[1] = .47f;
	Scene.water->color[2] = .76f;
	Scene.water->type = 1;
	glm_translate(Scene.water->obj->transform, (vec3) { .0f, -2.0f, 35.0f });
	glm_scale(Scene.water->obj->transform, (vec3) { 50.0f, 1.0f, 50.0f });
	glm_translate(Scene.water->csCollide->obj->transform, (vec3) { .0f, -2.0f, .0f });
	CollisionShape* water = newCollisionShape();
	check(water);
	water->channel = CC_WATER;
	worldSpawnObj(world, water->obj);
	objAttachmentTo(Scene.water->obj, water->obj);

	Scene.rock1 = newRock();
	check(Scene.rock1);
	worldSpawnObj(world, Scene.rock1->obj);
	glm_translate(Scene.rock1->obj->transform, (vec3) { 2.0f, -1.0f, 2.0f });

	Scene.umb1 = newUmbrella();
	check(Scene.umb1);
	worldSpawnObj(world, Scene.umb1->obj);
	glm_translate(Scene.umb1->obj->transform, (vec3) { 5.0f, 2.0f, -15.0f });

	Scene.umb2 = newUmbrella();
	check(Scene.umb2);
	worldSpawnObj(world, Scene.umb2->obj);
	glm_translate(Scene.umb2->obj->transform, (vec3) { -5.0f, 2.0f, -10.0f });

	Scene.umb3 = newUmbrella();
	check(Scene.umb3);
	worldSpawnObj(world, Scene.umb3->obj);
	glm_translate(Scene.umb3->obj->transform, (vec3) { 15.0f, 2.0f, -12.0f });

	Scene.umb4 = newUmbrella();
	check(Scene.umb4);
	worldSpawnObj(world, Scene.umb4->obj);
	glm_translate(Scene.umb4->obj->transform, (vec3) { 7.0f, 2.0f, -1.0f });

	Scene.umb5 = newUmbrella();
	check(Scene.umb5);
	worldSpawnObj(world, Scene.umb5->obj);
	glm_translate(Scene.umb5->obj->transform, (vec3) { -8.0f, 2.0f, -5.0f });

	Scene.umb6 = newUmbrella();
	check(Scene.umb6);
	worldSpawnObj(world, Scene.umb6->obj);
	glm_translate(Scene.umb6->obj->transform, (vec3) { 0.0f, 2.0f, -20.0f });

	Scene.light1 = newLight(GL_LIGHT0);
	check(Scene.light1);
	worldSpawnObj(world, Scene.light1->obj);
	glm_translate(Scene.light1->obj->transform, (vec3) { .0f, .0f, -10.0f });

	Scene.sun = newLight(GL_LIGHT1);
	check(Scene.sun);
	worldSpawnObj(world, Scene.sun->obj);
	Scene.sun->type = 0;
	glm_translate(Scene.sun->obj->transform, (vec3) { .0f, 1.0f, .0f });
	Scene.sun->ambient[0] = .5f;
	Scene.sun->ambient[1] = .5f;
	Scene.sun->ambient[2] = .3f;
	Scene.sun->ambient[3] = 1.0f;
	Scene.sun->diffuse[0] = .3f;
	Scene.sun->diffuse[1] = .3f;
	Scene.sun->diffuse[2] = .3f;
	Scene.sun->diffuse[3] = 1.0f;
	Scene.sun->specular[0] = 1.0f;
	Scene.sun->specular[1] = 1.0f;
	Scene.sun->specular[2] = 1.0f;
	Scene.sun->specular[3] = 1.0f;
	Scene.sun->obj->update = sunUpdate;
	Scene.sunOrbit = 1;

	Scene.flashLight = newFlashlight(GL_LIGHT2);
	check(Scene.flashLight);
	worldSpawnObj(world, Scene.flashLight->obj);
	glm_translate(Scene.flashLight->obj->transform, (vec3) { .0f, .0f, -10.0f });

	Scene.fan1 = newFan();
	check(Scene.fan1);
	worldSpawnObj(world, Scene.fan1->obj);
	glm_translate(Scene.fan1->obj->transform, (vec3) { -2.0f, 2.0f, 2.0f });

	Scene.axis = newObject();
	check(Scene.axis);
	Scene.axis->render = axisRender;
	worldSpawnObj(world, Scene.axis);
	glm_scale(Scene.axis->transform, (vec3) { .3f, .3f, .3f });

	Scene.bot = newRobot();
	check(Scene.bot);
	worldSpawnObj(world, Scene.bot->obj);
	glm_translate(Scene.bot->obj->transform, (vec3) { .0f, 2.0f, .0f });

	Scene.cameras[0] = Scene.bot->cam;
	glm_vec3_copy((vec3) { 0.0f, 0.0f, 3.0f }, Scene.bot->cam->camPosition);
	glm_vec3_copy((vec3) { 0.0f, 0.0f, -1.0f }, Scene.bot->cam->camFront);
	glm_vec3_copy((vec3) { 0.0f, 1.0f, 0.0f }, Scene.bot->cam->camUp);
	glm_vec3_copy((vec3) { 0.0f, -90.0f, 0.0f }, Scene.bot->cam->camRot);

	Scene.cameras[1] = newCamera();
	check(Scene.cameras[1]);
	Scene.cameras[1]->obj->update = fpsCamUpdate;
	glm_vec3_copy((vec3) { 0.0f, 0.0f, 3.0f }, Scene.cameras[1]->camPosition);
	glm_vec3_copy((vec3) { 0.0f, 0.0f, -1.0f }, Scene.cameras[1]->camFront);
	glm_vec3_copy((vec3) { 0.0f, 1.0f, 0.0f }, Scene.cameras[1]->camUp);
	glm_vec3_copy((vec3) { 0.0f, -90.0f, 0.0f }, Scene.cameras[1]->camRot);
	
	Scene.currentCamera = 0;
	worldSetCamera(world, Scene.cameras[Scene.currentCamera]);

	fov = 60.0f;
}

void setupInputMapping() {
	imMapFloat2Key('w', KEY_HOLD, rbMove, 1, 0);
	imMapFloat2Key('s', KEY_HOLD, rbMove, -1, 0);
	imMapFloat2Key('a', KEY_HOLD, rbMove, 0, -1);
	imMapFloat2Key('d', KEY_HOLD, rbMove, 0, 1);
	imMapActionKey('f', KEY_PRESS, rbPickup);
	imMapActionKey('q', KEY_PRESS, rbDrop);
	imMapActionKey(32, KEY_PRESS, rbJump);
	imMapFloat2Key(MOUSE_MOTION, NULL, rbRotate, 0, 0);
	imMapActionKey(27, KEY_PRESS, rbPause);
	imMapActionKey('b', KEY_PRESS, rbKneeDown);
	imMapFloat1Key('r', KEY_PRESS, rbRun, 2.0f);
	imMapFloat1Key('r', KEY_RELEASE, rbRun, 1.0f);
	imMapActionKey('v', KEY_PRESS, rbSwitchCamera);
	imMapActionKey('z', KEY_PRESS, rbShowCollision);
	imMapActionKey('t', KEY_PRESS, rbThrow);
	imMapFloat1Key('i', KEY_HOLD, rbZoom, 1);
	imMapFloat1Key('o', KEY_HOLD, rbZoom, -1);
	imMapActionKey('b', KEY_PRESS, rbSwitchView);
	imMapFloat1Key('1', KEY_PRESS, toggleLight, 1.0f);
	imMapFloat1Key('2', KEY_PRESS, toggleLight, 2.0f);
	imMapFloat1Key('3', KEY_PRESS, toggleLight, 3.0f);
	imMapFloat1Key('!', KEY_PRESS, selectLight, 1.0f); // switch to dir
	imMapFloat1Key('@', KEY_PRESS, selectLight, 2.0f); // switch to point
	imMapFloat1Key('#', KEY_PRESS, selectLight, 3.0f); // switch to spot
	imMapFloat1Key('m', KEY_HOLD, changeColor, 0.0f); // intensity
	imMapFloat1Key(',', KEY_HOLD, changeColor, 1.0f); // red
	imMapFloat1Key('.', KEY_HOLD, changeColor, 2.0f); // green
	imMapFloat1Key('/', KEY_HOLD, changeColor, 3.0f); // blue
	imMapFloat1Key('k', KEY_HOLD, changeAttribute, 1.0f); // add cutoff, add dir x
	imMapFloat1Key('K', KEY_HOLD, changeAttribute, -1.0f); // reduce cutoff, minus dir x
	imMapFloat1Key('l', KEY_HOLD, changeAttribute, 2.0f); // add dir y
	imMapFloat1Key('L', KEY_HOLD, changeAttribute, -2.0f); // minus dir y
	imMapFloat1Key(';', KEY_HOLD, changeAttribute, 3.0f); // add dir z
	imMapFloat1Key(':', KEY_HOLD, changeAttribute, -3.0f); // minus dir z
	imMapFloat1Key('p', KEY_PRESS, changeAttribute, 4.0f); // toggle orbiting

}

void rbMove(float forward, float side) {
	if (Scene.currentCamera == 1) {
		if (forward != 0.0f) Scene.cameras[1]->forwardVal = forward;
		if (side != 0.0f) Scene.cameras[1]->sideVal = side;
	}
	else {
		botMoving(Scene.bot, forward, side);
	}
}

void rbRotate(float pitch, float yaw) {
	if (Scene.currentCamera == 1) {
		if (pitch != 0) world->cam->pitchVal += pitch;
		if (yaw != 0) world->cam->yawVal += yaw;
	}
	else {
		if (pitch != 0) world->cam->pitchVal -= pitch;
		if (yaw != 0) world->cam->yawVal += yaw;
	}
	
}

void rbJump() {
	botJump(Scene.bot);
}

void rbSwitchCamera() {
	camera_mode = (camera_mode + 1) % 5;
	reshapeCallback(window_wid, window_hei);
	//Scene.currentCamera = (Scene.currentCamera + 1) % 4;
	//worldSetCamera(world, Scene.cameras[Scene.currentCamera]);
}

void rbKneeDown() {
	botToggleKneeDown(Scene.bot);
}

void rbPickup() {
	botPickup(Scene.bot);
}

void rbDrop() {
	botDrop(Scene.bot);
}

void rbThrow() {
	botThrow(Scene.bot);
}

void rbRun(float val) {
	Scene.bot->obj->movSpeed = val;
	Scene.bot->walkAnimation->timer = 0.0f;
}

void rbPause() {
	enginePause();
}

void rbShowCollision() {
	worldToggleCollisionVision(world);
}

void rbZoom(float val) {
	Scene.cameras[0]->fov -= val;
	fov += val;
}

void rbSwitchView() {
	Scene.currentCamera = !Scene.currentCamera;
}

void axisRender(Object* obj) {
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glLineWidth(1.0f);

	glLineWidth(10.0f);
	glColor3f(1.0f, .0f, .0f);
	glBegin(GL_LINES);
	glVertex3f(.0f, .0f, .0f);
	glVertex3f(5.0f, .0f, .0f);
	glEnd();

	glColor3f(.0f, 1.0f, .0f);
	glBegin(GL_LINES);
	glVertex3f(.0f, .0f, .0f);
	glVertex3f(.0f, 5.0f, .0f);
	glEnd();

	glColor3f(.0f, .0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(.0f, .0f, .0f);
	glVertex3f(.0f, .0f, 5.0f);
	glEnd();

	glLineWidth(1.0f);
	glScalef(.3f, .3f, .3f);
	drawUnitSphere(16, 16);

	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void fpsCamUpdate(Object* obj, float deltatime) {
	mat4 worldTrans;
	objGetWorldTransform(worldTrans, obj);

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

	cam->forwardVal *= cam->moveFraction;
	cam->sideVal *= cam->moveFraction;
	cam->pitchVal *= cam->rotateFraction;
	cam->yawVal *= cam->rotateFraction;
}

void sunUpdate(Object* obj, float deltatime) {
	if (!Scene.sunOrbit) return;
	static float angle = 0.0f; 
	angle += deltatime; 

	float x = cos(angle); 
	float y = sin(angle); 
	float z = 0.0f;     

	obj->transform[3][0] = x;
	obj->transform[3][1] = y;
	obj->transform[3][2] = z;
}

void toggleLight(float val) {
	int f = val;
	if (f == 1) Scene.sun->isOn = !Scene.sun->isOn;
	else if (f == 2) Scene.light1->isOn = !Scene.light1->isOn;
	else Scene.flashLight->isOn = !Scene.flashLight->isOn;
}

int selectedLight = 1;

void selectLight(float val) {
	selectedLight = (int) val;
}

void changeColor(float val) {
	int f = val;

	Light* target;
	switch (selectedLight) {
	case 1:
		target = Scene.sun;
		break;

	case 2:
		target = Scene.light1;
		break;

	default:
		target = Scene.flashLight->light;
		break;
	}

	if (f == 0) {
		target->diffuse[0] = fmodf(target->diffuse[0] + .01f, 1.0f);
		target->diffuse[1] = fmodf(target->diffuse[1] + .01f, 1.0f);
		target->diffuse[2] = fmodf(target->diffuse[2] + .01f, 1.0f);
	}
	else if (f == 1) {
		target->diffuse[0] = fmodf(target->diffuse[0] + .01f, 1.0f);
	}
	else if (f == 2) {
		target->diffuse[1] = fmodf(target->diffuse[1] + .01f, 1.0f);
	}
	else {
		target->diffuse[2] = fmodf(target->diffuse[2] + .01f, 1.0f);
	}
}

void changeAttribute(float val) {
	int f = abs(val);
	if (selectedLight == 1) {
		switch (f) {
		case 1:
			Scene.sun->obj->transform[3][0] += val > 0 ? .1f : -.1f;
			break;

		case 2:
			Scene.sun->obj->transform[3][1] += val > 0 ? .1f : -.1f;
			break;

		case 3:
			Scene.sun->obj->transform[3][2] += val > 0 ? .1f : -.1f;
			break;

		case 4:
			Scene.sunOrbit = !Scene.sunOrbit;
			break;
		}
	}
	else if (selectedLight == 3 && f == 1) {
		Scene.flashLight->cutoffAngle += val;
	}
}
