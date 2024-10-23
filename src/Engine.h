#pragma once

#define ENG_DEFAULT_FPS 120

// frame rate handle
static long long lastNanoSec;
static long long targetDeltaNanoTime;
static float currentFPS;
static float currentDeltatime;

void engineInit();
int engineStartLoop(void *thr_id);
void engineSetTargetFPS(unsigned fps);
float engineGetCurrentFPS();