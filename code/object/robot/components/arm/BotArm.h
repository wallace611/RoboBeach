#pragma once

#include "object/component/Component.h"

typedef struct _barm BotArm;

struct _barm {
	comp_type_t obj_type;

	Component* obj;
};

BotArm* newBotArm();

void barmRender(Component* obj);