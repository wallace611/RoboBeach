#include "BotArm.h"

#include "utils/Shapes.h"

BotArm* newBotArm() {
	BotArm* barm = (BotArm*)malloc(sizeof(BotArm));
	if (barm == NULL) return NULL;

	barm->obj_type = BOT_ARM;
	barm->obj = inheriteComp(barm, BOT_ARM);
	if (barm->obj == NULL) {
		free(barm);
		return NULL;
	}

	barm->obj->render = barmRender;

	return barm;
}

void barmRender(Component* obj) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(obj->transform);
	
	glBegin(GL_QUADS);
	{
		glColor3f(.5f, .5f, .5f);
		
		// top
		glVertex3f(.2f, .0f, .2f);
		glVertex3f(-.2f, .0f, .2f);
		glVertex3f(-.2f, .0f, -.2f);
		glVertex3f(.2f, .0f, -.2f);

		// bottom
		glVertex3f(.2f, -.8f, .2f);
		glVertex3f(.2f, -.8f, -.2f);
		glVertex3f(-.2f, -.8f, -.2f);
		glVertex3f(-.2f, -.8f, .2f);

		// front
		glVertex3f(.2f, .0f, .2f);
		glVertex3f(.2f, -.8f, .2f);
		glVertex3f(-.2f, -.8f, .2f);
		glVertex3f(-.2f, .0f, .2f);

		// back
		glVertex3f(.2f, .0f, -.2f);
		glVertex3f(-.2f, .0f, -.2f);
		glVertex3f(-.2f, -.8f, -.2f);
		glVertex3f(.2f, -.8f, -.2f);

		// left
		glVertex3f(-.2f, .0f, .2f);
		glVertex3f(-.2f, -.8f, .2f);
		glVertex3f(-.2f, -.8f, -.2f);
		glVertex3f(-.2f, .0f, -.2f);

		// right
		glVertex3f(.2f, .0f, .2f);
		glVertex3f(.2f, .0f, -.2f);
		glVertex3f(.2f, -.8f, -.2f);
		glVertex3f(.2f, -.8f, .2f);
	}
	glEnd();

	compRenderChild(obj);

	glPopMatrix();
}
