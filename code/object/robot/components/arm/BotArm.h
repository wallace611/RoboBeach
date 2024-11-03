#pragma once

#include "object/component/Component.h"

typedef struct _barm BotArm;

struct _barm {
	comp_type_t comp_type;

	Component* comp;
};

BotArm* newBotArm();

void barmRender(Component* comp);