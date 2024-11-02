#include "BotBody.h"

#include "utils/Shapes.h"

BotBody* newBotBody() {
	BotBody* bbody = (BotBody*)malloc(sizeof(BotBody));
	if (bbody == NULL) return NULL;

	bbody->comp_type = BOT_BODY;

	bbody->comp = inheriteComp(bbody, BOT_BODY);
	if (bbody->comp == NULL) return NULL;

	bbody->comp->ready = bbodyReady;
	bbody->comp->update = bbodyUpdate;
	bbody->comp->render = bbodyRender;

	return bbody;
}

void bbodyReady(Component* bbody) {
	
}

void bbodyUpdate(Component* bbody, float deltatime) {
}

void bbodyRender(Component* bbody) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(bbody->transform);
	drawUnitCube();
	compRenderChild(bbody);
	glPopMatrix();
}
