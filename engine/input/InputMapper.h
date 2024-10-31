#pragma once

// action type
typedef unsigned char action_t;
#define	KEY_PRESS	0
#define KEY_RELEASE 1
#define KEY_HOLD	2

// function type
typedef unsigned char func_t;
#define ACTION_FUNC 0
#define FLOAT1_FUNC 1
#define FLOAT2_FUNC 2

#define LEFT_MOUSE_BTN 256
#define MID_MOUSE_BTN 257
#define RIGHT_MOUSE_BTN 258
#define MOUSE_WHEEL 259
#define MOUSE_MOTION 260

#define MAPPER_SIZE 261
#define MAPPER_KEY_SIZE 256
#define MAPPER_BTN_SIZE 3
#define MAPPER_WHEEL_SIZE 1
#define MAPPER_MOTION_SIZE 1

typedef union {
	struct Action {
		void (*ActionFunction)(void);
	};
	
	struct Float1 {
		float arg;
		void (*float1Function)(float);
	};
	struct Float2 {
		float argx, argy;
		void (*float2Function)(float, float);
	};
} CallbackFunction;

// it cost lots of memory, BRUH. I think I'll optimize it later (?
typedef struct _map {
	unsigned char isActivate;
	action_t actions;
	func_t funcType;
	CallbackFunction functions[3];
} ActionMapper;

// Keyboard handle
static unsigned char key_pressed[256] = { 0 };
static unsigned char last_key_pressed[256] = { 0 };

// Mouse handle
typedef struct _btn {
	unsigned char pressed;
	int x;
	int y;
} BtnInfo;
// 0~2: mouse btn, 3: mouse wheel, 4: mouse movement
static BtnInfo btn_pressed[5] = { 0 };
static BtnInfo last_btn_pressed[5] = { 0 };

// 0~255: keyboard, 256~258: mouse btn, 259: mouse wheel, 260: mouse movement
static ActionMapper key_map[MAPPER_SIZE] = { 0 };

void imInit();
void imMapActionKey(int key, int actions, void (*callbackFunc)());
void imMapFloat1Key(int key, int actions, void (*callbackFunc)(float), float arg);
void imMapFloat2Key(int key, int actions, void (*callbackFunc)(float, float), float argx, float argy);
void imPressUpdate();

// private function
inline static void keyboardPressedCallback(unsigned char key, int x, int y);
inline static void keyboardReleaseCallback(unsigned char key, int x, int y);
inline static void mouseActionCallback(int btn, int state, int x, int y);
inline static void mouseWheelCallback(int btn, int dir, int x, int y);
inline static void mouseMovementCallback(int x, int y);
static void callFunc(func_t type, CallbackFunction func);