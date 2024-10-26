#include "Engine.h"

#include <stdio.h>
#include <time.h>

#include <GL/freeglut.h>

#include "input/KeyboardMapper.h"
#include "input/MouseEventHandler.h"
#include "utils/Shapes.h"

void engineInit(int* argc, char** argv) {
	target_delta_nano = (long long) 1e9 / ENG_DEFAULT_FPS;
	current_fps = 0.0f;

	window_wid = ENG_DEFAULT_WINDOW_WID;
	window_hei = ENG_DEFAULT_WINDOW_HEI;

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

	SetupMouseFunc(mouseCallback, mouseMotionCallback);

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

float rot = 0;
float ax = 0.0f, az = 0.0f;

void mappingKey() {
	kmMapFloat2Key('w', KEY_HOLD, moving, 1, 0);
	kmMapFloat2Key('s', KEY_HOLD, moving, -1, 0);
	kmMapFloat2Key('a', KEY_HOLD, moving, 0, -1);
	kmMapFloat2Key('d', KEY_HOLD, moving, 0, 1);
	
}

void tick(float deltatime) {
	kmPressUpdate();

	rot += deltatime * 100;
	glutPostRedisplay();
}

void displayCallback() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(ax, 0.0f, az);

	glPushMatrix();
	glTranslatef(-1.0, 0.0f, -2.0);
	glRotatef(rot, 0.0f, 1.0f, 0.0f);

	drawUnitSphere(10, 10);

	glPopMatrix();
	
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
	gluPerspective(60, aspectRatio, 0.1f, 100.0f);

	glViewport(0, 0, w, h);
	glMatrixMode(GL_MODELVIEW);
}

void mouseCallback(int btn, int state, int x, int y) {
}

void mouseMotionCallback(int btn, int x, int y) {
}

void moving(float x, float y) {
	az += x * 0.01;
	ax -= y * 0.01;
}

void rotating(float x) {
}
