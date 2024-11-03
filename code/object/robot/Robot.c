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
	glm_translate(bot->bbody->comp->transform, (vec3) { .0f, .5f, .0f });

	bot->obj->ready = botReady;
	bot->obj->update = botUpdate;
	bot->obj->render = botRender;

	return bot;
}

void botReady(Object* obj) {
	objReady(obj);
}

void botUpdate(Object* obj, float deltatime) {
	objUpdateChild(obj, deltatime);
}

void botRender(Object* obj) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(obj->transform);

	objRenderChild(obj);
	glPopMatrix();
}
