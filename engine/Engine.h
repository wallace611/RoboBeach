#pragma once

#include "scene/World.h"

#define ENG_DEFAULT_FPS 120
#define ENG_DEFAULT_WINDOW_WID 1200
#define ENG_DEFAULT_WINDOW_HEI 700
#define ENG_DEFAULT_WINDOW_POS_X 100
#define ENG_DEFAULT_WINDOW_POS_Y 100

// render handle
static int window_wid, window_hei;

// frame rate handle
static long long last_nano;
static long long target_delta_nano;
static float current_fps;
static float current_deltatime;

World* world;

// engine functions
void engineInit(int* argc, char** argv);
void engineRenderInit();
void engineStartLoop();
void engineSetTargetFPS(unsigned fps);
float engineGetCurrentFPS();

static void mappingKey();
static void tick(float deltatime);

// callback functions
static void displayCallback();
static void idleCallback();
static void reshapeCallback(int w, int h);
static void mouseCallback(int btn, int state, int x, int y);
static void mouseMotionCallback(int btn, int x, int y);

static void moving(float x, float y);
static void rotating(float pitch, float yaw);