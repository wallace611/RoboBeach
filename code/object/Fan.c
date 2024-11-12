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
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 4.0, 0.0);
	glVertex3f(1.0, 8.0, 0.0);
	glVertex3f(-1.0, 8.0, 0.0);
	glVertex3f(-1.0, 4.0, 0.0);
	glEnd();
}

void draw_cube() {
	int    i;
	float  points[][3] = {{-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, 
		{0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, 
		{-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
		{0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}};
	int    face[][4] = {{0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, 
		{4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3}};

	glColor3f(0.20, 0.75, 0.0);  /* Set the current color */
	for(i=0;i<6;i++){
		glBegin(GL_POLYGON);  /* Draw the face */
		glVertex3fv(points[face[i][0]]);
		glVertex3fv(points[face[i][1]]);
		glVertex3fv(points[face[i][2]]);
		glVertex3fv(points[face[i][3]]);
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

	glPushMatrix();

	glPushMatrix();              /* Save M1 coord. sys */
	glScalef(6.0, 6.0, 6.0);     /* Scale up the axes by 6 times */
	draw_cube();                 /* Draw a unit cube in the new coord. sys. */
	glPopMatrix();               /* Get M1 back */

	glTranslatef(0.0, 3.0, 0.0); /* Go up by 2 units, move to M2 coord. sys */

	/*-------Draw the cylinder, arm of the windmill------*/
	glColor3f(0.68, 0.68, 0.68);    /* Gray colored */

	if(cylind==NULL){               /* allocate a quadric object, if necessary */
		cylind = gluNewQuadric();
		gluQuadricDrawStyle(cylind, GLU_FILL);
		gluQuadricNormals(cylind, GLU_SMOOTH);
	}

	glPushMatrix();              /* Save M2 system */
	glRotatef(-90.0, 1.0, 0.0, 0.0);  /* Roate about x axis, z become y,
	and y become -z */
	/*--- Draw a cylinder ---*/
	gluCylinder(cylind, 1.0, 1.0, /* radius of top and bottom circle */
		7.0,              /* height of the cylinder */
		12,               /* use 12-side polygon approximating circle*/
		3);               /* Divide it into 3 sections */
	glPopMatrix();         /* Get M2 back */

	glTranslatef(0.0, 8.0, 0.0); /* Go up 8 units, become M3 coord. sys*/

	/*----- Draw a unit sphere ---*/
	if(sphere==NULL){
		sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
	}
	glColor3f(0.95, 0.2, 0.2);
	gluSphere(sphere, 2.0,   /* radius=2.0 */
		12,            /* composing of 12 slices*/
		12);           /* composing of 12 stacks */

	/*---- Draw forearm of the windmill ---*/
	glRotatef(.0f, 0.0, 1.0, 0.0);

	glColor3f(0.68, 0.68, 0.68);
	gluCylinder(cylind, 1.0, 1.0, 4.0, 12, 3);

	glTranslatef(0.0, 0.0, 5.0); /* goto end of forearm, M4 coord. sys. */
	glColor3f(0.2, 0.2, 0.95);
	gluSphere(sphere, 1.5,   /* radius=1.5 */
		12,            /* composing of 12 slices*/
		12);           /* composing of 12 stacks */

	/*------Draw three blades ---*/
	glColor3f(1.0, 1.0, 1.0);

	glRotatef(fan->angle, 0.0, 0.0, 1.0);  /* Rotate about x axis, M5 coord. sys. */
	draw_blade();/* draw the first blade */

	glRotatef(120.0, 0.0, 0.0, 1.0);    /* rotate by 120 degree, M6 coord. sys */
	draw_blade();/* draw 2nd blade */

	glRotatef(120.0, 0.0, 0.0, 1.0);    /* rotate by 120 degree,M7 coord. sys */
	draw_blade();/* draw 3rd blade */
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
