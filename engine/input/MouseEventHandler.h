#pragma once

#include <GL/freeglut.h>

void (*targetMouseFunction)(int, int, int, int);
void (*targetMotionFunction)(int, int, int);

int dragingBtn;

static void defaultMouseFunction(int, int, int, int);
static void defaultMotionFunction(int, int);
void SetupMouseFunc(void (*mouseCallback)(int, int, int, int), void (*motionCallback)(int, int, int));