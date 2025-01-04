#include "Umbrella.h"

#include "texture/Texture.h"
#include "Engine.h"

Umbrella* newUmbrella() {
	Umbrella* umb = (Umbrella*)malloc(sizeof(Umbrella));
	if (umb == NULL) return NULL;

	umb->obj_type = UMBRELLA;
	umb->obj = inheriteObj(umb, UMBRELLA);
	if (umb->obj == NULL) return NULL;

	umb->csCollide = newCollisionShape();
	if (umb->csCollide == NULL) return NULL;
	umb->csCollide->channel = CC_COLLISION;
	umb->csCollide->collided = botTouchFloor;
	objAttachmentTo(umb->obj, umb->csCollide->obj);
	worldSpawnObj(world, umb->csCollide->obj);
	glm_scale(umb->csCollide->obj->transform, (vec3) { .3f, 1.f, .3f });

	umb->csPickup = newCollisionShape();
	if (umb->csPickup == NULL) return NULL;
	umb->csPickup->channel = CC_PICKUP;
	objAttachmentTo(umb->obj, umb->csPickup->obj);
	worldSpawnObj(world, umb->csPickup->obj);
	glm_scale(umb->csPickup->obj->transform, (vec3) { 2.0f, 2.0f, 2.0f });

	umb->csFloor = newCollisionShape();
	if (umb->csFloor == NULL) return NULL;
	umb->csFloor->channel = CC_FLOOR;
	objAttachmentTo(umb->obj, umb->csFloor->obj);
	worldSpawnObj(world, umb->csFloor->obj);
	glm_translate(umb->csFloor->obj->transform, (vec3) { .0f, 3.1f, .0f });
	glm_scale(umb->csFloor->obj->transform, (vec3) { 3.0f, 1.f, 3.0f });

	umb->obj->aloc[1] = -9.8;

	umb->obj->update = umbUpdate;
	umb->obj->render = umbRender;
	
	return umb;
}

void umbUpdate(Object* obj, float deltatime) {
	if (obj->owner == NULL) {
		vec3 atmp, vtmp;
		glm_vec3_scale(obj->aloc, deltatime, atmp);
		glm_vec3_copy(obj->vloc, vtmp);
		glm_vec3_add(vtmp, atmp, obj->vloc);

		glm_vec3_scale(obj->vloc, deltatime, vtmp);
		glm_translate(obj->transform, vtmp);
		obj->vloc[1] *= 0.9995;
		obj->vloc[0] *= 0.97;
		obj->vloc[2] *= 0.97;
	}
}

void umbRender(Object* obj) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    mat4 worldTrans;
    objGetWorldTransform(worldTrans, obj);
    glMultMatrixf(worldTrans);

    glPushMatrix();
    {
        // �B�ʬ`
        glTranslatef(.0f, -.5f, .0f);
        glRotatef(90.0f, -1.0f, .0f, .0f);

        // �]�m�B�ʬ`�������ݩ�
        GLfloat handleAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
        GLfloat handleDiffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
        GLfloat handleSpecular[] = { 0.9f, 0.9f, 0.9f, 1.0f };
        GLfloat handleShininess = 50.0f;

        glMaterialfv(GL_FRONT, GL_AMBIENT, handleAmbient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, handleDiffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, handleSpecular);
        glMaterialf(GL_FRONT, GL_SHININESS, handleShininess);

        // ø�s�B�ʬ`
        glutSolidCylinder(.1, 4.0, 8, 8);

        // ���ʨ�ʭ���m
        glTranslatef(.0f, .0f, 4.0f);

        // �ҥί��z�øj�w�s�� textureName[6]
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureName[6]);

        // �]�m�i�վ㪺 scale�A�Ω󱱨�z���Q�K����
        float scale = .5f; // scale �V�j�A���z���Q�V�K��

        // �]�m�B�ʭ��������ݩ�
        GLfloat canopyAmbient[] = { 0.2f, 0.1f, 0.5f, 1.0f };
        GLfloat canopyDiffuse[] = { 0.3f, 0.2f, 0.8f, 1.0f };
        GLfloat canopySpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
        GLfloat canopyShininess = 3.0f;

        glMaterialfv(GL_FRONT, GL_AMBIENT, canopyAmbient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, canopyDiffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, canopySpecular);
        glMaterialf(GL_FRONT, GL_SHININESS, canopyShininess);

        // �ϥί��zø�s�B�ʭ�
        GLUquadric* quadric = gluNewQuadric();
        gluQuadricTexture(quadric, GL_TRUE); // �ҥί��z�M�g
        glMatrixMode(GL_TEXTURE);
        glPushMatrix();
        glScalef(scale, scale, 1.0f); // �վ㯾�z���Q�K����
        gluCylinder(quadric, 3.0, 0.0, 0.1, 16, 16);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        gluDeleteQuadric(quadric);

        // �T�ί��z
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    // ø�s�l��H
    objRenderChild(obj);

    glPopMatrix();
}



void botTouchFloor(Object* self, CollisionShape* selfcs, Object* other, CollisionShape* othercs) {
	if (other == NULL || othercs == NULL || self == other) return;
	if (othercs->channel == CC_FLOOR && selfcs->channel == CC_COLLISION) {
		mat4 worldTrans;
		objGetWorldTransform(worldTrans, othercs->obj);
		float y = worldTrans[3][1] + othercs->height / 2 + selfcs->height / 2;
		self->transform[3][1] = y;
		self->vloc[1] = 0.0f;
	}
}
