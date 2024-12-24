#include "Rock.h"

#include "Engine.h"

Rock* newRock() {
	Rock* rock = (Rock*)malloc(sizeof(Rock));
	if (rock == NULL) return NULL;

	rock->obj_type = ROCK;
	rock->obj = inheriteObj(rock, ROCK);
	if (rock->obj == NULL) return NULL;

	rock->collision = newCollisionShape();
	if (rock->collision == NULL) return NULL;
	rock->collision->channel = CC_COLLISION;
	worldSpawnObj(world, rock->collision->obj);
	objAttachmentTo(rock->obj, rock->collision->obj);

	rock->obj->ready = rockReady;
	rock->obj->update = rockUpdate;
	rock->obj->render = rockRender;

	return rock;
}

void rockReady(Object* obj) {
	objReadyChild(obj);
}

void rockUpdate(Object* obj, float deltatime) {
	vec3 atmp, vtmp;
	glm_vec3_scale(obj->aloc, deltatime, atmp);
	glm_vec3_copy(obj->vloc, vtmp);
	glm_vec3_add(vtmp, atmp, obj->vloc);

	glm_vec3_scale(obj->vloc, deltatime, vtmp);
	glm_translate(obj->transform, vtmp);
	obj->vloc[1] *= 0.9995;
	obj->vloc[0] *= 0.8;
	obj->vloc[2] *= 0.8;
	objUpdateChild(obj, deltatime);
}

void rockRender(Object* obj) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glMultMatrixf(obj->transform);

	glPushMatrix();

	// �]�m���ۧ���
	GLfloat rockAmbient[] = { 0.3f, 0.2f, 0.1f, 1.0f };  // ���ҥ� (�t�Ħ�)
	GLfloat rockDiffuse[] = { 0.5f, 0.35f, 0.2f, 1.0f }; // ���g�� (���Ħ�)
	GLfloat rockSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // �����Ϯg (�L�z�����A)
	GLfloat rockShininess = 10.0f;                        // ���A�� (�C���A)

	glMaterialfv(GL_FRONT, GL_AMBIENT, rockAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, rockDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, rockSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, rockShininess);

	// ø�s���� (�y��)
	glutSolidSphere(1.0f, 16, 16);

	glPopMatrix();

	// ø�s�l��H
	objRenderChild(obj);

	glPopMatrix();
}
