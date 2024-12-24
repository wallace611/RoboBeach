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
        glColor3f(0.5f, 0.5f, 0.5f);

        // Top face (normal: (0, 1, 0))
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.2f, 0.0f, 0.2f);
        glVertex3f(-0.2f, 0.0f, 0.2f);
        glVertex3f(-0.2f, 0.0f, -0.2f);
        glVertex3f(0.2f, 0.0f, -0.2f);

        // Bottom face (normal: (0, -1, 0))
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.2f, -0.8f, 0.2f);
        glVertex3f(0.2f, -0.8f, -0.2f);
        glVertex3f(-0.2f, -0.8f, -0.2f);
        glVertex3f(-0.2f, -0.8f, 0.2f);

        // Front face (normal: (0, 0, 1))
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.2f, 0.0f, 0.2f);
        glVertex3f(0.2f, -0.8f, 0.2f);
        glVertex3f(-0.2f, -0.8f, 0.2f);
        glVertex3f(-0.2f, 0.0f, 0.2f);

        // Back face (normal: (0, 0, -1))
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(0.2f, 0.0f, -0.2f);
        glVertex3f(-0.2f, 0.0f, -0.2f);
        glVertex3f(-0.2f, -0.8f, -0.2f);
        glVertex3f(0.2f, -0.8f, -0.2f);

        // Left face (normal: (-1, 0, 0))
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-0.2f, 0.0f, 0.2f);
        glVertex3f(-0.2f, -0.8f, 0.2f);
        glVertex3f(-0.2f, -0.8f, -0.2f);
        glVertex3f(-0.2f, 0.0f, -0.2f);

        // Right face (normal: (1, 0, 0))
        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.2f, 0.0f, 0.2f);
        glVertex3f(0.2f, 0.0f, -0.2f);
        glVertex3f(0.2f, -0.8f, -0.2f);
        glVertex3f(0.2f, -0.8f, 0.2f);
    }
    glEnd();

    // Render child components
    compRenderChild(obj);

    glPopMatrix();
}

