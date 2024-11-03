#include "Connector.h"

#include "utils/Shapes.h"

Connector* newConnector(Component* child) {
	if (child == NULL) return;
	Connector* conn = (Connector*)malloc(sizeof(Connector));
	if (conn == NULL) return NULL;

	conn->isVisible = 0;

	conn->comp_type = CONNECTOR;
	conn->comp = inheriteComp(conn, CONNECTOR);
	if (conn->comp == NULL) return NULL;

	conn->child = child;
	ocPushBack(conn->comp->child_list, child);

	conn->comp->ready = connReady;
	conn->comp->update = connUpdate;
	conn->comp->render = connRender;

	return conn;
}

void connReady(Component* comp) {
	compReadyChild(comp);
}

void connUpdate(Component* comp, float deltatime) {
	compUpdateChild(comp, deltatime);
}

void connRender(Component* comp) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(comp->transform);

	Connector* conn = cast(comp, CONNECTOR);
	if (conn->isVisible) {
		glPushMatrix();
		glScalef(.1f, .1f, .1f);
		drawUnitSphere(10, 10);
		glPopMatrix();
	}

	compRenderChild(comp);
	glPopMatrix();
}
