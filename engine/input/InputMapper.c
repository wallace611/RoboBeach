#include "InputMapper.h"

#include <stdio.h>
#include <GL/freeglut.h>

#include "Engine.h"

void imInit() {
	glutKeyboardFunc(keyboardPressedCallback);
	glutKeyboardUpFunc(keyboardReleaseCallback);
	glutMouseFunc(mouseActionCallback);
	glutMouseWheelFunc(mouseWheelCallback);
	glutPassiveMotionFunc(mouseMovementCallback);
}

void imMapActionKey(int key, int actions, void (*callbackFunc)()) {
	key_map[key].isActivate = 1;
	key_map[key].actions = actions;
	key_map[key].funcType = ACTION_FUNC;
	key_map[key].functions[actions].ActionFunction = callbackFunc;
}

void imMapFloat1Key(int key, int actions, void(*callbackFunc)(float), float arg) {
	key_map[key].isActivate = 1;
	key_map[key].actions = actions;
	key_map[key].funcType = FLOAT1_FUNC;
	key_map[key].functions[actions].float1Function = callbackFunc;
	key_map[key].functions[actions].arg = arg;
}

void imMapFloat2Key(int key, int actions, void(*callbackFunc)(float, float), float argx, float argy) {
	key_map[key].isActivate = 1;
	key_map[key].actions = actions;
	key_map[key].funcType = FLOAT2_FUNC;
	key_map[key].functions[actions].float2Function = callbackFunc;
	key_map[key].functions[actions].argx = argx;
	key_map[key].functions[actions].argy = argy;
}

void imPressUpdate() {
	int i;
	// keyboard
	for (i = 0; i < MAPPER_KEY_SIZE; i++) {
		if (key_map[i].isActivate) {
			if (i == 119 && key_pressed[i]) {
				printf("nice");
			}
			action_t keyAction = key_map[i].actions;

			if ((keyAction == KEY_PRESS) && key_pressed[i] == 1 && last_key_pressed[i] == 0) {
				callFunc(key_map[i].funcType, key_map[i].functions[0]);
			}
			if ((keyAction == KEY_RELEASE) && key_pressed[i] == 0 && last_key_pressed[i] == 1) {
				callFunc(key_map[i].funcType, key_map[i].functions[1]);
			}

			if ((keyAction == KEY_HOLD) && key_pressed[i] == 1) {
				callFunc(key_map[i].funcType, key_map[i].functions[2]);
			}
			last_key_pressed[i] = key_pressed[i];
		}
	}
	// mouse button
	for (i = MAPPER_KEY_SIZE; i < MAPPER_KEY_SIZE + MAPPER_BTN_SIZE; i++) {

	}
	// mouse wheel
	i = MOUSE_WHEEL;

	// mouse motion
	i = MOUSE_MOTION;
	if (key_map[i].isActivate) {
		action_t keyAction = key_map[i].actions;
	}

}

inline void keyboardPressedCallback(unsigned char key, int x, int y) {
	key_pressed[key] = 1;
}

inline void keyboardReleaseCallback(unsigned char key, int x, int y) {
	key_pressed[key] = 0;
}

inline void mouseActionCallback(int btn, int state, int x, int y) {
	btn_pressed[btn].pressed = !state;
	btn_pressed[btn].x = x;
	btn_pressed[btn].y = y;
}

inline void mouseWheelCallback(int btn, int dir, int x, int y) {
	btn_pressed[3].pressed = dir;
	btn_pressed[3].x = x;
	btn_pressed[3].y = y;
}

inline void mouseMovementCallback(int x, int y) {
	btn_pressed[4].x = x;
	btn_pressed[4].y = y;
}

void callFunc(func_t type, CallbackFunction func) {
	switch (type) {
	case ACTION_FUNC:
		func.ActionFunction();
		break;

	case FLOAT1_FUNC:
		func.float1Function(func.arg);
		break;

	case FLOAT2_FUNC:
		func.float2Function(func.argx, func.argy);
		break;
	}
}
