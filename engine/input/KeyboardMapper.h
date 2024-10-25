#pragma once

#define	KEY_PRESS	0
#define KEY_RELEASE 1
#define KEY_HOLD	2

#define ACTION_FUNC 0
#define FLOAT1_FUNC 1
#define FLOAT2_FUNC 2

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

// it cost lots of memory, BRUH. I'll optimize it later (?
typedef struct map {
	unsigned char activate;
	unsigned char actions;
	unsigned char funcType;
	CallbackFunction functions[3];
} KeyAction;

// I/O handle
static unsigned char key_pressed[256] = { 0 };
static unsigned char last_pressed[256] = { 0 };
static KeyAction key_map[256] = { 0 };

void kmInit();
void kmMapActionKey(unsigned char key, int actions, void (*callbackFunc)());
void kmMapFloat1Key(unsigned char key, int actions, void (*callbackFunc)(float), float arg);
void kmMapFloat2Key(unsigned char key, int actions, void (*callbackFunc)(float, float), float argx, float argy);
void kmPressUpdate();

// private function
inline static void keyboardPressedCallback(unsigned char key, int x, int y);
inline static void keyboardReleaseCallback(unsigned char key, int x, int y);
static void callFunc(unsigned char type, CallbackFunction func);