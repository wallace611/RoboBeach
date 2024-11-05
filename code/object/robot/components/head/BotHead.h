#pragma once

#include "object/component/Component.h"

typedef struct _bhead BotHead;

struct _bhead {
	comp_type_t obj_type;

	Component* obj;
};

BotHead* newBotHead();
void bheadRender(Component* obj);