#include "Connector.h"

#include "utils/Shapes.h"

Connector* newConnector(Component* child) {
	if (child == NULL) return;
	Connector* conn = (Connector*)malloc(sizeof(Connector));
	if (conn == NULL) return NULL;

	conn->bIsVisible = 0;

	conn->obj_type = CONNECTOR;
	conn->obj = inheriteComp(conn, CONNECTOR);
	if (conn->obj == NULL) return NULL;

	conn->child = child;
	ocPushBack(conn->obj->child_list, child);

	conn->obj->ready = connReady;
	conn->obj->update = connUpdate;
	conn->obj->render = connRender;

	return conn;
}

void connReady(Component* obj) {
	compReadyChild(obj);
}

void connUpdate(Component* obj, float deltatime) {
	compUpdateChild(obj, deltatime);
}

void connRender(Component* obj) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(obj->transform);

	Connector* conn = cast(obj, CONNECTOR);
	if (conn->bIsVisible) {
		glPushMatrix();
		glScalef(.1f, .1f, .1f);
		drawUnitSphere(10, 10);
		glPopMatrix();
	}

	compRenderChild(obj);
	glPopMatrix();
}
