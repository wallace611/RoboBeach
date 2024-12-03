#pragma once

#include "scene/World.h"
#include "ui/DebugText.h"

#define ENG_DEFAULT_FPS 100
#define ENG_DEFAULT_WINDOW_WID 1200
#define ENG_DEFAULT_WINDOW_HEI 700
#define ENG_DEFAULT_WINDOW_POS_X 100
#define ENG_DEFAULT_WINDOW_POS_Y 100

// render handle
int window_wid, window_hei;

int mouse_center_x, mouse_center_y;

// frame rate handle
static long long last_nano;
static long long target_delta_nano;
static float current_fps;
static float current_deltatime;

static unsigned char is_paused;

static double timer;

static double warp_time;

World* world;

DebugText* objDebug;

Camera* o;

// camera mode
#define CAM_PERSPECTIVE		0
#define CAM_Z				1
#define CAM_Y				2
#define CAM_X				3
#define CAM_ALL				4

int camera_mode;

// engine functions
void engineInit(int* argc, char** argv);
void engineRenderInit();
void engineStartLoop();
void engineSetTargetFPS(unsigned fps);
float engineGetCurrentFPS();
double engineGetTimer();

static void tick(float deltatime);

// callback functions
static void displayCallback();
static void idleCallback();
void reshapeCallback(int w, int h);

void moving(float x, float y);
void rotating(float pitch, float yaw);

void enginePause();

void check(void* pt);