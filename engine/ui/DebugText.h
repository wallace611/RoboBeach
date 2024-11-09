#pragma once

#include <GL/freeglut.h>
#include <stdlib.h>

#include "utils/ObjContainer.h"
#include "utils/String.h"

typedef struct _debug_text DebugText;

struct _debug_text {
	ObjContainer* objToShow;
	int visible;

	String* output;

	void (*render)(DebugText* dbgtxt);
};

DebugText* newDebugText();

static void dbtRender(DebugText* dbgtxt);

void dbtPushString(DebugText* dbgtxt, String* s);
void dbtPushChars(DebugText* dbgtxt, char* c);