#pragma once

#include "object/component/Component.h"
#include "object/component/Connector.h"
#include "head/BotHead.h"
#include "arm/BotArm.h"

typedef struct _bbody BotBody;

struct _bbody {
	comp_type_t obj_type;

	BotHead* bhead;
	Connector* headConnector;

	BotArm* rightFArm;
	Connector* rightFArmConnector;
	BotArm* leftFArm;
	Connector* leftFArmConnector;
	BotArm* rightArm;
	Connector* rightArmConnector;
	BotArm* leftArm;
	Connector* leftArmConnector;

	BotArm* rightThigh;
	Connector* rightThighConnector;
	BotArm* leftThigh;
	Connector* leftThighConnector;
	BotArm* rightCalf;
	Connector* rightCalfConnector;
	BotArm* leftCalf;
	Connector* leftCalfConnector;

	Component* obj;
};

BotBody* newBotBody();

void bbodyUpdate(Component* obj, float deltatime);
void bbodyRender(Component* obj);

static void build_body_part(BotBody* bbody);