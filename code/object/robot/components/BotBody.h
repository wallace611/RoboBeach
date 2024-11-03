#pragma once

#include "object/component/Component.h"
#include "object/component/Connector.h"
#include "head/BotHead.h"
#include "arm/BotArm.h"

typedef struct _bbody BotBody;

struct _bbody {
	comp_type_t comp_type;

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

	Component* comp;
};

BotBody* newBotBody();

void bbodyUpdate(Component* comp, float deltatime);
void bbodyRender(Component* comp);

static void build_body_part(BotBody* bbody);