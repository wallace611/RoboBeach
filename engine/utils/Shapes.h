#pragma once

#include <GL/freeglut.h>

void drawTriangle() {
	glBegin(GL_TRIANGLES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0, 1, 0);
	glVertex3f(-1, -1, 1);
	glVertex3f(1, -1, 1);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0, 1, 0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1, -1, 1);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1, -1, -1);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0, 1, 0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1, -1, -1);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-1, -1, -1);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0, 1, 0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1, -1, 1);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-1, -1, -1);
	glEnd();
}