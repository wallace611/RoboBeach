#include "Engine.h"

#include <stdio.h>
#include <time.h>

#include <GL/freeglut.h>

#include "input/KeyboardMapper.h"
#include "input/MouseEventHandler.h"
#include "object/Camera.h"
#include "object/Object.h"

void engineInit(int* argc, char** argv) {
	target_delta_nano = (long long) 1e9 / ENG_DEFAULT_FPS;
	current_fps = 0.0f;

	window_wid = ENG_DEFAULT_WINDOW_WID;
	window_hei = ENG_DEFAULT_WINDOW_HEI;

	world = newWorld();
	Object* cam = newCamera();
	worldSetCamera(world, cam);

	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowPosition(ENG_DEFAULT_WINDOW_POS_X, ENG_DEFAULT_WINDOW_POS_Y);
	glutInitWindowSize(window_wid, window_hei);

	glutCreateWindow("Robo Beach");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glutDisplayFunc(displayCallback);
	glutIdleFunc(idleCallback);
	glutReshapeFunc(reshapeCallback);

	engineRenderInit();

	kmInit();
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

void mappingKey() {
	kmMapFloat2Key('w', KEY_HOLD, moving, 0, 1);
	kmMapFloat2Key('s', KEY_HOLD, moving, 0, -1);
	kmMapFloat2Key('a', KEY_HOLD, moving, 1, 0);
	kmMapFloat2Key('d', KEY_HOLD, moving, -1, 0);
	kmMapFloat2Key('q', KEY_HOLD, rotating, 0, -1);
	kmMapFloat2Key('e', KEY_HOLD, rotating, 0, 1);
	kmMapFloat2Key('r', KEY_HOLD, rotating, 1, 0);
	kmMapFloat2Key('f', KEY_HOLD, rotating, -1, 0);
	
}

void tick(float deltatime) {
	kmPressUpdate();

	worldUpdate(world, deltatime);

	glutPostRedisplay();
}

void displayCallback() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	world->camera->renderFunction(world->camera);

	worldRender(world);
	
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
	double aspectRatio = (double) w / (double) h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(camGetFOV(), aspectRatio, camGetZNear(), camGetZFar());

	glViewport(0, 0, w, h);
	glMatrixMode(GL_MODELVIEW);
}

void moving(float x, float y) {
	mat4 shift;
	glm_mat4_identity(shift);

	shift[3][0] = x;
	shift[3][2] = y;

	mat4 tmp;
	glm_mat4_copy(world->camera->velocity, tmp);
	glm_mat4_mul(shift, tmp, world->camera->velocity);
}

void rotating(float pitch, float yaw) {
	mat4 rot;
	glm_mat4_identity(rot);
	
	glm_rotate_y(rot, yaw * 0.01, rot);
	glm_rotate_x(rot, pitch * 0.01, rot);

	mat4 tmp;
	glm_mat4_copy(world->camera->velocity, tmp);
	glm_mat4_mul(rot, tmp, world->camera->velocity);
}
