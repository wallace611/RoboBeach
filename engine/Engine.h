#pragma once

#include "scene/World.h"
#include "ui/DebugText.h"

#define ENG_DEFAULT_FPS 60
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

// engine functions
void engineInit(int* argc, char** argv);
void engineRenderInit();
void engineStartLoop();
void engineSetTargetFPS(unsigned fps);
float engineGetCurrentFPS();
double engineGetTimer();

static void mappingKey();
static void tick(float deltatime);

// callback functions
static void displayCallback();
static void idleCallback();
static void reshapeCallback(int w, int h);

static void moving(float x, float y);
static void rotating(float pitch, float yaw);

static void pause();