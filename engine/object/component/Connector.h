#pragma once

#include "object/component/Component.h"

typedef struct _conn Connector;

struct _conn {
	comp_type_t obj_type;

	Component* obj;

	unsigned char bIsVisible;

	Component* parent;
	Component* child;
};

Connector* newConnector(Component* child);

void connReady(Component* obj);
void connUpdate(Component* obj, float deltatime);
void connRender(Component* obj);