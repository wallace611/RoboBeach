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

    light->isRenderSphere = 1;
    light->isOn = 1;

	light->obj->render = lightRender;

	return light;
}

void lightRender(Object* obj) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    mat4 worldTrans;
    objGetWorldTransform(worldTrans, obj);
    glMultMatrixf(worldTrans);

    Light* light = cast(obj, LIGHT);
    if (light != NULL) {
        if (light->isOn) glEnable(light->light_numb);
        else glDisable(light->light_numb);


        if (light->type == 1) {
            // �]�m������m�� (0, 0, 0) (��V��)
            glLightfv(light->light_numb, GL_POSITION, (GLfloat[]) { .0f, .0f, .0f, light->type });

            // �]�m�I��Ѽ�
            glLightf(light->light_numb, GL_CONSTANT_ATTENUATION, 1.0f); // �`�q�I��
            glLightf(light->light_numb, GL_LINEAR_ATTENUATION, 0.1f);   // �u�ʰI��
            glLightf(light->light_numb, GL_QUADRATIC_ATTENUATION, 0.01f); // �G���I��

            if (light->isRenderSphere) {
                glPushMatrix();
                glScalef(.1f, .1f, .1f);

                // �]�m�����ݩ�
                GLfloat ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };  // ���ҥ�
                GLfloat diffuse[] = { 0.6f, 0.2f, 0.2f, 1.0f };  // ���g��
                GLfloat specular[] = { 0.8f, 0.8f, 0.8f, 1.0f }; // ����
                GLfloat shininess = 50.0f;                       // ���A��
                GLfloat emission[] = { .6f, .6f, .0f, 1.0f }; // �۵o���C�� (������)

                // ���Χ���
                /*glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
                glMaterialf(GL_FRONT, GL_SHININESS, shininess);*/
                glMaterialfv(GL_FRONT, GL_EMISSION, emission);

                // ø�s�y��
                glutSolidSphere(1.0, 32, 32);

                // ���m�o���ĪG�A�קK�v�T��L����
                GLfloat noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
                glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);

                glPopMatrix();
            }
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


