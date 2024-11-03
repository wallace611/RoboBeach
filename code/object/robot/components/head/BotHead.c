#include "BotHead.h"

#include "utils/Shapes.h"

BotHead* newBotHead() {
	BotHead* bhead = (BotHead*)malloc(sizeof(BotHead));
	if (bhead == NULL) return NULL;

	bhead->comp_type = BOT_HEAD;
	bhead->comp = inheriteComp(bhead, BOT_HEAD);
	if (bhead->comp == NULL) return NULL;

	bhead->comp->render = bheadRender;

	return bhead;
}

void bheadRender(Component* comp) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glMultMatrixf(comp->transform);

	glPushMatrix();
	
	glScalef(.5f, .6f, .4f);
	glBegin(GL_QUADS);
	{
		// bottom
		glColor3f(.5f, .5f, .5f);
		glVertex3f(.5f, -.5f, .5f);
		glVertex3f(-.5f, -.5f, .5f);
		glVertex3f(-.5f, -.5f, -.5f);
		glVertex3f(.5f, -.5f, -.5f);

		// top
		glColor3f(.65f, .65f, .65f);
		glVertex3f(.5f, .5f, .5f);
		glVertex3f(.5f, .5f, -.5f);

		glColor3f(.7f, .7f, .7f);
		glVertex3f(-.5f, .5f, -.5f);
		glVertex3f(-.5f, .5f, .5f);

		// front
		glColor3f(.65f, .65f, .65f);
		glVertex3f(.5f, .5f, .5f);

		glColor3f(.7f, .7f, .7f);
		glVertex3f(-.5f, .5f, .5f);

		glColor3f(.5f, .5f, .5f);
		glVertex3f(-.5f, -.5f, .5f);
		glVertex3f(.5f, -.5f, .5f);

		// back
		glColor3f(.65f, .65f, .65f);
		glVertex3f(.5f, .5f, -.5f);

		glColor3f(.5f, .5f, .5f);
		glVertex3f(.5f, -.5f, -.5f);
		glVertex3f(-.5f, -.5f, -.5f);

		glColor3f(.7f, .7f, .7f);
		glVertex3f(-.5f, .5f, -.5f);

		// left
		glColor3f(.65f, .65f, .65f);
		glVertex3f(.5f, .5f, .5f);

		glColor3f(.5f, .5f, .5f);
		glVertex3f(.5f, -.5f, .5f);
		glVertex3f(.5f, -.5f, -.5f);

		glColor3f(.65f, .65f, .65f);
		glVertex3f(.5f, .5f, -.5f);

		// right
		glColor3f(.7f, .7f, .7f);
		glVertex3f(-.5f, .5f, .5f);
		glVertex3f(-.5f, .5f, -.5f);
		glColor3f(.5f, .5f, .5f);
		glVertex3f(-.5f, -.5f, -.5f);
		glVertex3f(-.5f, -.5f, .5f);
	}
	glEnd();

	glPopMatrix();

	glPopMatrix();
}
