#include "Shapes.h"


void drawUnitArrow() {
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();
}

void drawUnitPlane() {
	glBegin(GL_POLYGON);
	// The plane is in the XZ-plane, so its normal is (0, 1, 0)
	glNormal3f(0.0f, 1.0f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.5f, 0.0f, 0.5f);
	glVertex3f(-0.5f, 0.0f, 0.5f);
	glVertex3f(-0.5f, 0.0f, -0.5f);
	glVertex3f(0.5f, 0.0f, -0.5f);
	glEnd();
}


void drawUnitCube() {
    glBegin(GL_QUADS);
    // Top face
    glColor3f(1.0f, 0.0f, 0.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);  // +Y
    glVertex3f(0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f, 0.5f,  0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f,  0.5f, -0.5f);

    // Bottom face
    glColor3f(0.0f, 1.0f, 0.0f);
    glNormal3f(0.0f, -1.0f, 0.0f); // -Y
    glVertex3f(0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    // Front face
    glColor3f(0.0f, 0.0f, 1.0f);
    glNormal3f(0.0f, 0.0f, 1.0f);  // +Z
    glVertex3f(0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f, 0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f,  -0.5f, 0.5f);

    // Left face
    glColor3f(1.0f, 0.0f, 1.0f);
    glNormal3f(-1.0f, 0.0f, 0.0f); // -X
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);

    // Back face
    glColor3f(0.0f, 1.0f, 1.0f);
    glNormal3f(0.0f, 0.0f, -1.0f); // -Z
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f(0.5f,   0.5f, -0.5f);
    glVertex3f(0.5f,  -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    // Right face
    glColor3f(1.0f, 0.0f, 1.0f);
    glNormal3f(1.0f, 0.0f, 0.0f);  // +X
    glVertex3f(0.5f,  0.5f,  0.5f);
    glVertex3f(0.5f,  0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f,  0.5f);

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