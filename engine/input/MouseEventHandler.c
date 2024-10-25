#include "MouseEventHandler.h"

static void defaultMouseFunction(int btn, int state, int x, int y) {
	if (dragingBtn == -1 && state == 0) {
		dragingBtn = btn;
	}
	else if (dragingBtn == btn && state == 1) {
		dragingBtn = -1;
	}
	if ((btn == 3 || btn == 4) && state == 0) {
		return;
	}
	targetMouseFunction(btn, state, x, y);
}

static void defaultMotionFunction(int x, int y) {
	if (dragingBtn == -1) return;
	targetMotionFunction(dragingBtn, x, y);
}

void SetupMouseFunc(void(*mouseCallback)(int, int, int, int), void(*motionCallback)(int, int, int)) {
	targetMouseFunction = mouseCallback;
	targetMotionFunction = motionCallback;
	dragingBtn = -1;

	glutMouseFunc(defaultMouseFunction);
	glutMotionFunc(defaultMotionFunction);
}
