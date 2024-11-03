#include "BotArm.h"

#include "utils/Shapes.h"

BotArm* newBotArm() {
	BotArm* barm = (BotArm*)malloc(sizeof(BotArm));
	if (barm == NULL) return NULL;

	barm->comp_type = BOT_ARM;
	barm->comp = inheriteComp(barm, BOT_ARM);
	if (barm->comp == NULL) return NULL;

	barm->comp->render = barmRender;

	return barm;
}

void barmRender(Component* comp) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(comp->transform);
	
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
	compRenderChild(comp);

	glPopMatrix();
}
