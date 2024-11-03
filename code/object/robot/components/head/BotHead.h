#pragma once

#include "object/component/Component.h"

typedef struct _bhead BotHead;

struct _bhead {
	comp_type_t comp_type;

	Component* comp;
};

BotHead* newBotHead();
void bheadRender(Component* comp);