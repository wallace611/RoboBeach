#include "BotHead.h"

#include "utils/Shapes.h"

BotHead* newBotHead() {
	BotHead* bhead = (BotHead*)malloc(sizeof(BotHead));
	if (bhead == NULL) return NULL;

	bhead->obj_type = BOT_HEAD;
	bhead->obj = inheriteComp(bhead, BOT_HEAD);
	if (bhead->obj == NULL) {
		free(bhead);
		return NULL;
	}

	bhead->obj->render = bheadRender;

	return bhead;
}

void bheadRender(Component* obj) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // Apply the object's transformation matrix
    glMultMatrixf(obj->transform);

    glPushMatrix();
    glScalef(0.5f, 0.6f, 0.4f);

    glBegin(GL_QUADS);
    {
        // Bottom face (normal: (0, -1, 0))
        glNormal3f(0.0f, -1.0f, 0.0f);
        glColor3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);

        // Top face (normal: (0, 1, 0))
        glNormal3f(0.0f, 1.0f, 0.0f);
        glColor3f(0.65f, 0.65f, 0.65f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glColor3f(0.7f, 0.7f, 0.7f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);

        // Front face (normal: (0, 0, 1))
        glNormal3f(0.0f, 0.0f, 1.0f);
        glColor3f(0.65f, 0.65f, 0.65f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glColor3f(0.7f, 0.7f, 0.7f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glColor3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);

        // Back face (normal: (0, 0, -1))
        glNormal3f(0.0f, 0.0f, -1.0f);
        glColor3f(0.65f, 0.65f, 0.65f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glColor3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glColor3f(0.7f, 0.7f, 0.7f);
        glVertex3f(-0.5f, 0.5f, -0.5f);

        // Left face (normal: (1, 0, 0))
        glNormal3f(1.0f, 0.0f, 0.0f);
        glColor3f(0.65f, 0.65f, 0.65f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glColor3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glColor3f(0.65f, 0.65f, 0.65f);
        glVertex3f(0.5f, 0.5f, -0.5f);

        // Right face (normal: (-1, 0, 0))
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glColor3f(0.7f, 0.7f, 0.7f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glColor3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
    }
    glEnd();

    glPopMatrix(); // End head rendering
    glPopMatrix(); // Restore previous matrix state
}
