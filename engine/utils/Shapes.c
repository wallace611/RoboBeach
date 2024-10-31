#include "Shapes.h"


void drawUnitArrow() {
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();
}

void drawUnitPlane() {
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.5f, 0.0f, 0.5f);
	glVertex3f(-0.5f, 0.0f, 0.5f);
	glVertex3f(-0.5f, 0.0f, -0.5f);
	glVertex3f(0.5f, 0.0f, -0.5f);
	glEnd();
}

void drawUnitCube() {
	glBegin(GL_QUADS);
	// top
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);

	// bottom
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);

	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);

	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);

	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);

	glEnd();
}

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

void drawUnitSphere(int slices, int stacks) {
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glutSolidSphere(1.0, slices, stacks);
	glColor4f(0.5f, 0.5f, 0.5f, 0.3f);
	glutWireSphere(1.0, slices, stacks);
}