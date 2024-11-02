#pragma once

#include "object/component/Component.h"

typedef struct _bbody BotBody;

struct _bbody {
	comp_type_t comp_type;

	Component* comp;
};

BotBody* newBotBody();

void bbodyReady(Component* bbody);
void bbodyUpdate(Component* bbody, float deltatime);
void bbodyRender(Component* bbody);