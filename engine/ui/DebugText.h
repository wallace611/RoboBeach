#pragma once

#include <GL/freeglut.h>
#include <stdlib.h>

#include "utils/ObjContainer.h"

typedef struct _debug_text DebugText;

struct _debug_text {
	ObjContainer* objToShow;
	int visible;

	float deltatime;

	void (*ready)(DebugText* debtxt);
	void (*update)(DebugText* debtxt, float deltatime);
	void (*render)(DebugText* dbgtxt);
};

DebugText* newDebugText();

static void dbtReady(DebugText* dbtxt);
static void dbtUpdate(DebugText* dbtxt, float deltatime);
static void dbtRender(DebugText* dbgtxt);