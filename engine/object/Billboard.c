#include "Billboard.h"
#include "RoboBeach.h"
#include <GL/freeglut.h>
#include <cglm/cglm.h>

Billboard* newBillboard() {
    Billboard* bill = (Billboard*)malloc(sizeof(Billboard));
    if (bill == NULL) return NULL;

    bill->obj_type = BILL;
    bill->obj = inheriteObj(bill, BILL);
    bill->texture = 1; // �q�{�ϥΪ����z�s��
    if (bill->obj == NULL) {
        free(bill);
        return NULL;
    }
    bill->x = 0.0f;
    bill->y = 0.0f;
    bill->scale = 1.0f;
    bill->timer = .0f;

    bill->obj->render = billRender;

    return bill;
}

void billRender(Object* obj) {
    Billboard* bill = cast(obj, BILL);
    if (bill == NULL) return;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixf(obj->transform);

    // ����۾����@���ܴ��x�}
    mat4 camTrans;
    glm_mat4_copy(Scene.cameras[Scene.currentCamera]->obj->transform, camTrans);

    // �����۾���m�]�۾����ĥ|�C�^
    vec3 cameraPos = { camTrans[3][0], camTrans[3][1], camTrans[3][2] };

    // �������骺��m�]���骺�ĥ|�C�^
    vec3 objPos = { obj->transform[3][0], obj->transform[3][1], obj->transform[3][2] };

    // �p�⪫���۾����V�q
    vec3 lookDir;
    glm_vec3_sub(cameraPos, objPos, lookDir);
    glm_vec3_normalize(lookDir);

    // �]�m���魱�V�۾�������x�}
    mat4 billboardMatrix;
    glm_mat4_identity(billboardMatrix);

    billboardMatrix[0][0] = 1.0f; // �O�� X �b����
    billboardMatrix[1][1] = 1.0f; // �O�� Y �b����
    billboardMatrix[2][0] = -lookDir[0];
    billboardMatrix[2][1] = -lookDir[1];
    billboardMatrix[2][2] = -lookDir[2];

    glm_mat4_inv(camTrans, camTrans);
    vec3 scl;
    glm_decompose_rs(camTrans, camTrans, scl);
    mat4 tmp;
    glm_mat4_identity(tmp);
    tmp[2][2] = -1;
    glm_mat4_mul(tmp, camTrans, camTrans);
    glMultMatrixf(camTrans);

    // �ʺA���� x, y �����M scale �վ�
    glTranslatef(bill->x, bill->y, 0.0f); // ���Υ���
    glScalef(bill->scale, bill->scale, 1.0f); // �����Y��

    // �j�w���z
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureName[bill->texture]);

    // ø�s Billboard ����
    float size = 2.0f; // Billboard ����l�j�p
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, 0.0f); // ���U
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, 0.0f);  // �k�U
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, 0.0f);   // �k�W
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, 0.0f);  // ���W
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

