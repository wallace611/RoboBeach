#include "Fan.h"

#include "Engine.h"

Fan* newFan() {
	Fan* fan = (Fan*)malloc(sizeof(Fan));
	if (fan == NULL) return NULL;

	fan->obj_type = FAN;
	fan->angle = 0.0f;
	fan->obj = inheriteObj(fan, FAN);
	if (fan->obj == NULL) return NULL;

	fan->csCollide = newCollisionShape();
	if (fan->csCollide == NULL) return NULL;
	fan->csCollide->channel = CC_COLLISION;
	fan->csCollide->collided = fanTouchFloor;
	worldSpawnObj(world, fan->csCollide->obj);
	objAttachmentTo(fan->obj, fan->csCollide->obj);
	glm_scale(fan->csCollide->obj->transform, (vec3) { .3f, 1.f, .3f });

	fan->csPickup = newCollisionShape();
	if (fan->csPickup == NULL) return NULL;
	fan->csPickup->channel = CC_PICKUP;
	worldSpawnObj(world, fan->csPickup->obj);
	objAttachmentTo(fan->obj, fan->csPickup->obj);

	fan->csBlow = newCollisionShape();
	if (fan->csBlow == NULL) return NULL;
	fan->csBlow->channel = CC_DEFAULT;
	fan->csBlow->collided = fanBlowObj;
	worldSpawnObj(world, fan->csBlow->obj);
	objAttachmentTo(fan->obj, fan->csBlow->obj);
	glm_translate(fan->csBlow->obj->transform, (vec3) { .0f, .2f, 5.0f });
	glm_scale(fan->csBlow->obj->transform, (vec3) { 7.0f, 3.0f, 7.0f });

	fan->obj->aloc[1] = -9.8f;

	fan->obj->update = fanUpdate;
	fan->obj->render = fanRender;

	return fan;
}

void fanUpdate(Object* obj, float deltatime) {
	Fan* fan = cast(obj, FAN);
	if (fan == NULL) return;
	fan->angle += deltatime * 1000;

	if (obj->owner == NULL) {
		vec3 atmp, vtmp;
		glm_vec3_scale(obj->aloc, deltatime, atmp);
		glm_vec3_copy(obj->vloc, vtmp);
		glm_vec3_add(vtmp, atmp, obj->vloc);

		glm_vec3_scale(obj->vloc, deltatime, vtmp);
		glm_translate(obj->transform, vtmp);
		obj->vloc[1] *= 0.9995;
	}
}

void draw_blade() {
	glBegin(GL_POLYGON);
	// All points lie in z=0 plane, so normal is +Z
	glNormal3f(0.0f, 0.0f, 1.0f);

	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 4.0, 0.0);
	glVertex3f(1.0, 8.0, 0.0);
	glVertex3f(-1.0, 8.0, 0.0);
	glVertex3f(-1.0, 4.0, 0.0);
	glEnd();
}


void draw_cube() {
	// One normal per face, corresponding to the 'face' array order
	static float normals[6][3] = {
		{ 0.0f,  0.0f, -1.0f},   // face[0] -> z = -0.5
		{ 0.0f, -1.0f,  0.0f},   // face[1] -> y = -0.5
		{ 1.0f,  0.0f,  0.0f},   // face[2] -> x =  0.5
		{ 0.0f,  0.0f,  1.0f},   // face[3] -> z =  0.5
		{ 0.0f,  1.0f,  0.0f},   // face[4] -> y =  0.5
		{-1.0f,  0.0f,  0.0f}    // face[5] -> x = -0.5
	};

	float  points[][3] = {
		{-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, 
		{ 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, 
		{-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f},
		{ 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}
	};

	int face[][4] = {
		{0, 3, 2, 1},  // face[0]
		{0, 1, 5, 4},  // face[1]
		{1, 2, 6, 5},  // face[2]
		{4, 5, 6, 7},  // face[3]
		{2, 3, 7, 6},  // face[4]
		{0, 4, 7, 3}   // face[5]
	};

	glColor3f(0.20f, 0.75f, 0.0f);  /* Set the current color */

	for(int i = 0; i < 6; i++){
		glBegin(GL_POLYGON);

		// Specify the face normal before the face’s vertices
		glNormal3fv(normals[i]);

		// Draw the four vertices of this face
		glVertex3fv(points[ face[i][0] ]);
		glVertex3fv(points[ face[i][1] ]);
		glVertex3fv(points[ face[i][2] ]);
		glVertex3fv(points[ face[i][3] ]);

		glEnd();
	}
}


GLUquadricObj  *sphere=NULL, *cylind=NULL;

void fanRender(Object* obj) {
	Fan* fan = cast(obj, FAN);
	if (fan == NULL) return;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	mat4 worldTrans;
	objGetWorldTransform(worldTrans, obj);
	glMultMatrixf(worldTrans);
	glScalef(.1, .1, .1);

	// 材質屬性變數
	GLfloat materialAmbient[4], materialDiffuse[4], materialSpecular[4], materialShininess;

	glPushMatrix();

	// 底部立方體
	glPushMatrix();
	glScalef(6.0, 6.0, 6.0);

	materialAmbient[0] = 0.2f; materialAmbient[1] = 0.2f; materialAmbient[2] = 0.2f; materialAmbient[3] = 1.0f;
	materialDiffuse[0] = 0.5f; materialDiffuse[1] = 0.5f; materialDiffuse[2] = 0.5f; materialDiffuse[3] = 1.0f;
	materialSpecular[0] = 0.8f; materialSpecular[1] = 0.8f; materialSpecular[2] = 0.8f; materialSpecular[3] = 1.0f;
	materialShininess = 50.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	draw_cube();
	glPopMatrix();

	// 中間支柱（圓柱）
	glTranslatef(0.0, 3.0, 0.0);
	materialAmbient[0] = 0.3f; materialAmbient[1] = 0.3f; materialAmbient[2] = 0.3f; materialAmbient[3] = 1.0f;
	materialDiffuse[0] = 0.68f; materialDiffuse[1] = 0.68f; materialDiffuse[2] = 0.68f; materialDiffuse[3] = 1.0f;
	materialSpecular[0] = 0.8f; materialSpecular[1] = 0.8f; materialSpecular[2] = 0.8f; materialSpecular[3] = 1.0f;
	materialShininess = 30.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	if (cylind == NULL) {
		cylind = gluNewQuadric();
		gluQuadricDrawStyle(cylind, GLU_FILL);
		gluQuadricNormals(cylind, GLU_SMOOTH);
	}

	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(cylind, 1.0, 1.0, 7.0, 12, 3);
	glPopMatrix();

	// 頂部球體
	glTranslatef(0.0, 8.0, 0.0);
	materialAmbient[0] = 0.4f; materialAmbient[1] = 0.1f; materialAmbient[2] = 0.1f; materialAmbient[3] = 1.0f;
	materialDiffuse[0] = 0.95f; materialDiffuse[1] = 0.2f; materialDiffuse[2] = 0.2f; materialDiffuse[3] = 1.0f;
	materialSpecular[0] = 0.9f; materialSpecular[1] = 0.3f; materialSpecular[2] = 0.3f; materialSpecular[3] = 1.0f;
	materialShininess = 70.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	if (sphere == NULL) {
		sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
	}
	gluSphere(sphere, 2.0, 12, 12);

	// 風扇臂（小圓柱）
	materialAmbient[0] = 0.3f; materialAmbient[1] = 0.3f; materialAmbient[2] = 0.3f; materialAmbient[3] = 1.0f;
	materialDiffuse[0] = 0.68f; materialDiffuse[1] = 0.68f; materialDiffuse[2] = 0.68f; materialDiffuse[3] = 1.0f;
	materialSpecular[0] = 0.8f; materialSpecular[1] = 0.8f; materialSpecular[2] = 0.8f; materialSpecular[3] = 1.0f;
	materialShininess = 30.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	gluCylinder(cylind, 1.0, 1.0, 4.0, 12, 3);
	glTranslatef(0.0, 0.0, 5.0);

	// 小球（風扇連接處）
	materialAmbient[0] = 0.2f; materialAmbient[1] = 0.2f; materialAmbient[2] = 0.8f; materialAmbient[3] = 1.0f;
	materialDiffuse[0] = 0.2f; materialDiffuse[1] = 0.2f; materialDiffuse[2] = 0.95f; materialDiffuse[3] = 1.0f;
	materialSpecular[0] = 0.5f; materialSpecular[1] = 0.5f; materialSpecular[2] = 1.0f; materialSpecular[3] = 1.0f;
	materialShininess = 60.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	gluSphere(sphere, 1.5, 12, 12);

	// 風扇葉片
	materialAmbient[0] = 0.2f; materialAmbient[1] = 0.2f; materialAmbient[2] = 0.2f; materialAmbient[3] = 1.0f;
	materialDiffuse[0] = 1.0f; materialDiffuse[1] = 1.0f; materialDiffuse[2] = 1.0f; materialDiffuse[3] = 1.0f;
	materialSpecular[0] = 0.9f; materialSpecular[1] = 0.9f; materialSpecular[2] = 0.9f; materialSpecular[3] = 1.0f;
	materialShininess = 90.0f;

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	glRotatef(fan->angle, 0.0, 0.0, 1.0);
	draw_blade();

	glRotatef(120.0, 0.0, 0.0, 1.0);
	draw_blade();

	glRotatef(120.0, 0.0, 0.0, 1.0);
	draw_blade();

	glPopMatrix();
	glPopMatrix();
}


void fanTouchFloor(Object* self, CollisionShape* selfcs, Object* other, CollisionShape* othercs) {
	if (other == NULL || othercs == NULL || self == other || self->owner != NULL) return;
	if (othercs->channel == CC_FLOOR && selfcs->channel == CC_COLLISION) {
		mat4 worldTrans;
		objGetWorldTransform(worldTrans, othercs->obj);
		float y = worldTrans[3][1] + othercs->height / 2 + selfcs->height / 2;
		self->transform[3][1] = y;
		self->vloc[1] = 0.0f;
	}
}

void fanBlowObj(Object* self, CollisionShape* selfcs, Object* other, CollisionShape* othercs) {
	if (othercs->channel != CC_COLLISION || self->owner == other || self == other) return;

	mat4 worldTrans;
	objGetWorldTransform(worldTrans, self);

	vec3 forward;
	forward[0] = worldTrans[2][0];
	forward[1] = worldTrans[2][1];
	forward[2] = worldTrans[2][2];

	// Normalize the forward vector
	glm_vec3_normalize(forward);

	vec3 vtmp;
	glm_vec3_copy(other->vloc, vtmp);
	glm_vec3_add(vtmp, forward, other->vloc);
}
