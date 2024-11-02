#pragma once

#include "object/Object.h"
#include "object/robot/components/BotBody.h"

typedef struct _bot Robot;

struct _bot {
	obj_type_t obj_type;

	BotBody* bbody;

	Object* obj;
};

Robot* newRobot();

static void botReady(Object* bot);
static void botUpdate(Object* bot, float deltatime);
static void botRender(Object* bot);