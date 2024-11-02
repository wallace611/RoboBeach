#include "Engine.h"

#include <stdio.h>
#include <time.h>

#include <GL/freeglut.h>

#include "input/InputMapper.h"
#include "object/Camera.h"
#include "object/Object.h"

void engineInit(int* argc, char** argv) {
	target_delta_nano = (long long) 1e9 / ENG_DEFAULT_FPS;
	current_fps = 0.0f;

	window_wid = ENG_DEFAULT_WINDOW_WID;
	window_hei = ENG_DEFAULT_WINDOW_HEI;

	mouse_center_x = window_wid >> 1;
	mouse_center_y = window_hei >> 1;

	is_paused = 0;
	timer = 0.0;

	world = newWorld();
	Camera* cam = newCamera();
	worldSetCamera(world, cam);

	objDebug = newDebugText();

	ocPushBack(objDebug->objToShow, cam->obj);

	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowPosition(ENG_DEFAULT_WINDOW_POS_X, ENG_DEFAULT_WINDOW_POS_Y);
	glutInitWindowSize(window_wid, window_hei);

	glutCreateWindow("Robo Beach");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GLUT_MULTISAMPLE);

	glutDisplayFunc(displayCallback);
	glutIdleFunc(idleCallback);
	glutReshapeFunc(reshapeCallback);

	glutSetCursor(GLUT_CURSOR_NONE);

	engineRenderInit();

	imInit();
	mappingKey();

	return 0;
}

void engineRenderInit() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);
}

void engineStartLoop() {
	// setup nano second
	struct timespec tmp;

	int get_time_state = timespec_get(&tmp, TIME_UTC);
	if (get_time_state == 0) {
		return -1;
	}
	last_nano = (long long) tmp.tv_sec * 1e9 + tmp.tv_nsec;

	glutMainLoop();
}

void engineSetTargetFPS(unsigned fps) {
	if (fps == 0 || fps > 10000) return;
	target_delta_nano = (long long) 1e9 / fps;
}

float engineGetCurrentFPS() {
	return current_fps;
}

double engineGetTimer() {
	return timer;
}

void mappingKey() {
	imMapFloat2Key('w', KEY_HOLD, moving, 1, 0);
	imMapFloat2Key('s', KEY_HOLD, moving, -1, 0);
	imMapFloat2Key('a', KEY_HOLD, moving, 0, -1);
	imMapFloat2Key('d', KEY_HOLD, moving, 0, 1);
	imMapFloat2Key('q', KEY_HOLD, rotating, 0, -1);
	imMapFloat2Key('e', KEY_HOLD, rotating, 0, 1);
	imMapFloat2Key('r', KEY_HOLD, rotating, 1, 0);
	imMapFloat2Key('f', KEY_HOLD, rotating, -1, 0);
	imMapFloat2Key(MOUSE_MOTION, NULL, rotating, 0, 0);
	imMapActionKey(27, KEY_PRESS, pause);
}

void tick(float deltatime) {
	timer += deltatime;

	imPressUpdate();

	if (!is_paused) {
		worldUpdate(world, deltatime);

		objDebug->update(objDebug, deltatime);

		// warp the cursor to the center
		if (allowMouseMotion) {
			glutWarpPointer(mouse_center_x, mouse_center_y);
			allowMouseMotion = 0;
			warp_time = timer;
		}
		else if (timer - warp_time > deltatime) {
			allowMouseMotion = 1;
		}
	}

	glutPostRedisplay();
}

void displayCallback() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	worldRender(world);

	objDebug->render(objDebug);
	
	glutSwapBuffers();
}

void idleCallback() {
	struct timespec tmp;
	long long currentNanoSec = -1;

	while ((currentNanoSec - last_nano) < target_delta_nano) {

		int get_time_state = timespec_get(&tmp, TIME_UTC);
		if (get_time_state) {
			currentNanoSec = (long long) tmp.tv_sec * 1e9 + tmp.tv_nsec;
		}
	}

	// update frame rate info
	current_deltatime = (float) (currentNanoSec - last_nano) / 1e9;
	current_fps = 1 / current_deltatime;
	last_nano = currentNanoSec;

	// do everything here
	tick(current_deltatime);
}

void reshapeCallback(int w, int h) {
	window_wid = w;
	window_hei = h;

	mouse_center_x = window_wid >> 1;
	mouse_center_y = window_hei >> 1;

	double aspectRatio = (double) w / (double) h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(world->cam->fov, aspectRatio, world->cam->zNear, world->cam->zFar);

	glViewport(0, 0, w, h);
	glMatrixMode(GL_MODELVIEW);
}

void moving(float forward, float side) {
	if (forward != 0) world->cam->forwardVal = forward;
	if (side != 0) world->cam->sideVal = side;
}

void rotating(float pitch, float yaw) {
	if (pitch != 0) world->cam->pitchVal += pitch;
	if (yaw != 0) world->cam->yawVal += yaw;
}

void pause() {
	if (is_paused) {
		glutSetCursor(GLUT_CURSOR_NONE);
		is_paused = !is_paused;
		allowMouseMotion = 0;
		glutWarpPointer(mouse_center_x, mouse_center_y);
		warp_time = timer;
	}
	else {
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		is_paused = !is_paused;
		allowMouseMotion = 0;
	}
}
