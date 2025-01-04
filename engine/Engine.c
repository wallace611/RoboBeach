#include "Engine.h"

#include <stdio.h>
#include <time.h>

#include <GL/freeglut.h>

#include "input/InputMapper.h"
#include "object/Camera.h"
#include "object/Object.h"
#include "utils/Shapes.h"

#include "RoboBeach.h"

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

	objDebug = newDebugText();

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

	o = newCamera();

	return 0;
}

void engineRenderInit() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, .5);

	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	glShadeModel(GL_SMOOTH);

	glEnable(GL_NORMALIZE);

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

void tick(float deltatime) {
	timer += deltatime;

	imPressUpdate();

	// update debug info
	char tmp[128];
	sprintf_s(tmp,
		128,
		"frame rate: %3.1f, deltatime: %7.4f, timer: %7.2f\n",
		engineGetCurrentFPS(),
		deltatime,
		engineGetTimer()
	);
	dbtPushChars(objDebug, tmp);
	sprintf_s(tmp,
		128,
		"windows width: %4d, height: %4d\n" \
		"mouse center x: %4d, y: %4d\n",
		window_wid,
		window_hei,
		mouse_center_x,
		mouse_center_y
	);
	dbtPushChars(objDebug, tmp);

	if (!is_paused) {
		worldUpdate(world, deltatime);

		// warp the cursor to the center
		if (allowMouseMotion) {
			glutWarpPointer(mouse_center_x, mouse_center_y);
			allowMouseMotion = 0;
			warp_time = timer;
		}
		else if (timer - warp_time > deltatime) {
			allowMouseMotion = 1;
		}

		// collision detection
		worldCollisionDetection(world);
	}

	glutPostRedisplay();
}

void make_view(int x) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float view_distance = 40.0f;  // 視角距離，可以根據需要調整
	float window_ratio = (float)window_wid / (float)window_hei;  // 計算寬高比

	switch(x) {
	case 1:   // X方向平行視角
		gluLookAt(view_distance, 0.0, 0.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0);
		break;
	case 2:   // Y方向平行視角
		gluLookAt(0.0, view_distance, 0.0,  0.0, 0.0, 0.0,  1.0, 0.0, 0.0);
		break;
	case 3:   // Z方向平行視角
		gluLookAt(0.0, 0.0, view_distance,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0);
		break;
	}
}

/*------------------------------------------------------
* Procedure to make projection matrix
*/
void make_projection(int x)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Check if we are using perspective projection
	if(x == 4) {
		gluPerspective(world->cam->fov, (double)window_wid / (double)window_hei, world->cam->zNear, world->cam->zFar);
	} else {
		// Adjust orthographic projection based on camera FOV
		float scale_factor = 60.0f / (fov); // Scaling factor, assuming 60 is a baseline FOV

		// Use the scale factor to modify the orthographic bounds
		if(window_wid > window_hei)
			glOrtho(
				-40.0 * scale_factor, 
				40.0 * scale_factor, 
				-40.0 * (float)window_hei / (float)window_hei * scale_factor, 
				40.0 * (float)window_hei / (float)window_wid * scale_factor, 
				0.1, 10000.0
			);
		else
			glOrtho(
				-40.0 * (float)window_wid / (float)window_hei * scale_factor, 
				40.0 * (float)window_wid / (float)window_hei * scale_factor, 
				-40.0 * scale_factor, 
				40.0 * scale_factor, 
				0.1, 10000.0
			);
	}
	glMatrixMode(GL_MODELVIEW);
}


double aspectRatio;

void displayCallback() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int width = window_wid;
	int height = window_hei;

	switch (camera_mode) {
	case CAM_ALL:
		world->bUseCamera = 1;
		worldSetCamera(world, Scene.cameras[Scene.currentCamera]);
		make_view(4);
		make_projection(4);
		glViewport(width/2, 0, width/2, height/2);
		worldRender(world);

		glm_mat4_identity(o->obj->transform);
		glm_translate(o->obj->transform, (vec3) { -Scene.bot->obj->transform[3][0], -Scene.bot->obj->transform[3][1], -1000.0f });
		worldSetCamera(world, o);
		make_view(1);
		make_projection(1);
		glViewport(0, height/2, width/2, height/2);
		worldRender(world);

		glm_mat4_identity(o->obj->transform);
		glm_translate(o->obj->transform, (vec3) { -Scene.bot->obj->transform[3][2], -Scene.bot->obj->transform[3][1], -1000.0f });
		glm_rotate(o->obj->transform, glm_rad(90), (vec3) { .0f, 1.0f, .0f });
		make_view(2);
		make_projection(2);
		glViewport(width/2, height/2, width/2, height/2);
		worldRender(world);

		glm_mat4_identity(o->obj->transform);
		glm_translate(o->obj->transform, (vec3) { -Scene.bot->obj->transform[3][0], -Scene.bot->obj->transform[3][2], -1000.0f });
		glm_rotate(o->obj->transform, glm_rad(90), (vec3) { 1.0f, .0f, .0f });
		make_view(3);
		make_projection(3);
		glViewport(0, 0, width/2, height/2);
		worldRender(world);
		worldSetCamera(world, Scene.cameras[0]);
		break;
	case CAM_Z:
		worldSetCamera(world, o);
		glm_translate(o->obj->transform, (vec3) { -Scene.bot->obj->transform[3][0], -Scene.bot->obj->transform[3][1], -1000.0f });
		make_view(1);
		make_projection(1);
		glViewport(0, 0, width, height);
		worldRender(world);
		break;

	case CAM_Y:
		worldSetCamera(world, o);
		glm_translate(o->obj->transform, (vec3) { -Scene.bot->obj->transform[3][2], -Scene.bot->obj->transform[3][1], -1000.0f });
		glm_rotate(o->obj->transform, glm_rad(-90), (vec3) { .0f, 1.0f, .0f });
		make_view(2);
		make_projection(2);
		glViewport(0, 0 , width, height);
		worldRender(world);
		break;

	case CAM_X:
		worldSetCamera(world, o);
		glm_translate(o->obj->transform, (vec3) { -Scene.bot->obj->transform[3][0], -Scene.bot->obj->transform[3][2], -1000.0f });
		glm_rotate(o->obj->transform, glm_rad(90), (vec3) { 1.0f, .0f, .0f });
		make_view(3);
		make_projection(3);
		glViewport(0, 0, width, height);
		worldRender(world);
		break;

	case CAM_PERSPECTIVE:
		world->bUseCamera = 1;
		worldSetCamera(world, Scene.cameras[Scene.currentCamera]);
		make_view(4);
		make_projection(4);
		glViewport(0, 0, width, height);
		worldRender(world);
		break;

	}

	glm_mat4_identity(o->obj->transform);

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

	aspectRatio = (double) w / (double) h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(120, aspectRatio, world->cam->zNear, world->cam->zFar);

	glViewport(0, 0, w, h);
	glMatrixMode(GL_MODELVIEW);
}

void enginePause() {
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

void check(void* pt) {
	if (pt == NULL) {
		exit(-1);
	}
}
