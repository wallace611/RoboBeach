#include "Flashlight.h"

#include "object/Light.h"
#include "Engine.h"

Flashlight* newFlashlight(int numb) {
	Flashlight* flash = (Flashlight*) malloc(sizeof(Flashlight));
	if (flash == NULL) return NULL;

	flash->obj_type = FLASH;
	flash->obj = inheriteObj(flash, FLASH);
    flash->isOn = 1;
	if (flash->obj == NULL) return NULL;

	Light* light = flash->light = newLight(numb);
	if (light == NULL) return NULL;

	CollisionShape* cs = newCollisionShape();
	if (cs == NULL) return NULL;
	cs->channel = CC_PICKUP;

	objAttachmentTo(flash->obj, light->obj);
	objAttachmentTo(flash->obj, cs->obj);

	worldSpawnObj(world, light->obj);
	worldSpawnObj(world, cs->obj);

    light->obj->render = flashLightRender;
    flash->obj->update = flashUpdate;

	return flash;
}

void flashUpdate(Object* obj, float deltatime) {
    Flashlight* flash = cast(obj, FLASH);
    if (flash != NULL) {
        flash->light->isOn = flash->isOn;
    }
}

void flashLightRender(Object* obj) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    mat4 worldTrans;
    objGetWorldTransform(worldTrans, obj);
    glMultMatrixf(worldTrans);

    Light* light = cast(obj, LIGHT);
    if (light != NULL) {
        if (light->isOn) glEnable(light->light_numb);
        else glDisable(light->light_numb);

        // �]�m������m (�@���E���O��������m)
        GLfloat lightPosition[] = { obj->transform[3][0], obj->transform[3][1], obj->transform[3][2], 1.0f };
        glLightfv(light->light_numb, GL_POSITION, lightPosition);

        // �]�m�E���O��V
        GLfloat spotDirection[] = { 0.0f, .0f, 1.0f }; // �E���O���V -Y �b�]�i�H�ھڻݭn�վ�^
        glLightfv(light->light_numb, GL_SPOT_DIRECTION, spotDirection);

        // �]�m�E������
        glLightf(light->light_numb, GL_SPOT_CUTOFF, 30.0f); // 30 �ת���������

        // �]�m�E��������
        glLightf(light->light_numb, GL_SPOT_EXPONENT, 10.0f); // �E�������סA�V�j�����V����

        // �]�m�����j��
        glLightfv(light->light_numb, GL_AMBIENT, light->ambient);
        glLightfv(light->light_numb, GL_DIFFUSE, light->diffuse);
        glLightfv(light->light_numb, GL_SPECULAR, light->specular);

        

        // �e�X������m�� wired sphere �@���i����
        glDisable(GL_LIGHTING);  // �ȮɸT�Υ��ӡA�H�Kø�s�y��
        glColor3f(1.0f, 1.0f, 0.0f);  // �]�m�C�⬰����
        glutWireSphere(0.1, 10, 10);  // ø�s�@�ӥb�|�� 0.1 �� wired sphere
        glEnable(GL_LIGHTING);   // �ҥΥ���
    }

    objRenderChild(obj);

    glPopMatrix();
}


