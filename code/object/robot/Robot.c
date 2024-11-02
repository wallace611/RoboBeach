#include "Robot.h"

#include "utils/Shapes.h"

Robot* newRobot() {
	Robot* bot = (Robot*)malloc(sizeof(Robot));
	if (bot == NULL) return NULL;

	bot->obj_type = ROBOT;
	bot->obj = inheriteObj(bot, ROBOT);
	if (bot->obj == NULL) return NULL;

	bot->bbody = newBotBody();
	if (bot->bbody == NULL) return NULL;
	ocPushBack(bot->obj->child_list, bot->bbody->comp);

	bot->obj->ready = botReady;
	bot->obj->update = botUpdate;
	bot->obj->render = botRender;

	return bot;
}

void botReady(Object* bot) {
}

void botUpdate(Object* bot, float deltatime) {
}

void botRender(Object* bot) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(bot->transform);
	objRenderChild(bot);
	glPopMatrix();
}
