#include "Light.h"

Light* newLight(int numb) {
	Light* light = (Light*) malloc(sizeof(Light));
	if (light == NULL) return NULL;

	light->obj_type = LIGHT;
	light->light_numb = numb;
	light->obj = inheriteObj(light, LIGHT);
	if (light->obj == NULL) return NULL;

	light->type = 1;
	float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float diffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	float specular[] = { 0.9f, 0.9f, 0.9f, 1.0f };

	for (int i = 0; i < 4; i++) {
		light->ambient[i] = ambient[i];
		light->diffuse[i] = diffuse[i];
		light->specular[i] = specular[i];
	}

	light->obj->render = lightRender;

	return light;
}

void lightRender(Object* obj) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(obj->transform);

    Light* light = cast(obj, LIGHT);
    if (light != NULL) {
        glEnable(light->light_numb);

        if (light->type == 1) {
            // �]�m������m�� (0, 0, 0) (��V��)
            glLightfv(light->light_numb, GL_POSITION, (GLfloat[]) { .0f, .0f, .0f, light->type });

            // �e�X������ wired sphere �@���i����
            glDisable(GL_LIGHTING);  // �ȮɸT�Υ��ӡA�H�Kø�s�y��
            glColor3f(1.0f, 1.0f, 0.0f);  // �]�m�C�⬰����
            glutWireSphere(0.1, 10, 10);  // ø�s�@�ӥb�|�� 0.1 �� wired sphere
            glEnable(GL_LIGHTING);   // �ҥΥ���
        } else {
            // �]�m������m�����骺�@�ɦ�m (�I����)
            glLightfv(light->light_numb, GL_POSITION, (GLfloat[]) {
                obj->transform[3][0],
                    obj->transform[3][1],
                    obj->transform[3][2],
                    light->type
            });
        }

        // �]�m�����j��
        glLightfv(light->light_numb, GL_AMBIENT, light->ambient);
        glLightfv(light->light_numb, GL_DIFFUSE, light->diffuse);
        glLightfv(light->light_numb, GL_SPECULAR, light->specular);
    }

    objRenderChild(obj);

    glPopMatrix();
}

