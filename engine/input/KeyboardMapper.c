#include "KeyboardMapper.h"

#include <stdio.h>
#include <GL/freeglut.h>

#include "Engine.h"

void kmInit() {
	glutKeyboardFunc(keyboardPressedCallback);
	glutKeyboardUpFunc(keyboardReleaseCallback);
}

void kmMapActionKey(unsigned char key, int actions, void (*callbackFunc)()) {
	key_map[key].activate = 1;
	key_map[key].actions = actions;
	key_map[key].funcType = ACTION_FUNC;
	key_map[key].functions[actions].ActionFunction = callbackFunc;
}

void kmMapFloat1Key(unsigned char key, int actions, void(*callbackFunc)(float), float arg) {
	key_map[key].activate = 1;
	key_map[key].actions = actions;
	key_map[key].funcType = FLOAT1_FUNC;
	key_map[key].functions[actions].float1Function = callbackFunc;
	key_map[key].functions[actions].arg = arg;
}

void kmMapFloat2Key(unsigned char key, int actions, void(*callbackFunc)(float, float), float argx, float argy) {
	key_map[key].activate = 1;
	key_map[key].actions = actions;
	key_map[key].funcType = FLOAT2_FUNC;
	key_map[key].functions[actions].float2Function = callbackFunc;
	key_map[key].functions[actions].argx = argx;
	key_map[key].functions[actions].argy = argy;
}

void kmPressUpdate() {
	for (int i = 0; i < 256; i++) {
		if (key_map[i].activate) {
			unsigned char keyAction = key_map[i].actions;

			switch (keyAction) {
			case KEY_PRESS:
				if (key_pressed[i] == 1 && last_pressed[i] == 0) {
					callFunc(key_map[i].funcType, key_map[i].functions[keyAction]);
				}
				break;

			case KEY_RELEASE:
				if (key_pressed[i] == 0 && last_pressed[i] == 1) {
					callFunc(key_map[i].funcType, key_map[i].functions[keyAction]);
				}
				break;

			case KEY_HOLD:
				if (key_pressed[i] == 1) {
					callFunc(key_map[i].funcType, key_map[i].functions[keyAction]);
				}
				break;
			}
			last_pressed[i] = key_pressed[i];
		}
	}
}

inline void keyboardPressedCallback(unsigned char key, int x, int y) {
	key_pressed[key] = 1;
}

inline void keyboardReleaseCallback(unsigned char key, int x, int y) {
	key_pressed[key] = 0;
}

void callFunc(unsigned char type, CallbackFunction func) {
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
