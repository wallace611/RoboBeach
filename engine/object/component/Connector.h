#pragma once

#include "object/component/Component.h"

typedef struct _conn Connector;

struct _conn {
	comp_type_t comp_type;

	Component* comp;

	unsigned char isVisible;

	Component* parent;
	Component* child;
};

Connector* newConnector(Component* child);

void connReady(Component* comp);
void connUpdate(Component* comp, float deltatime);
void connRender(Component* comp);