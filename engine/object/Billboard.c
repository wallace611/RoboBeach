#include "Billboard.h"
#include "RoboBeach.h"
#include <GL/freeglut.h>
#include <cglm/cglm.h>

Billboard* newBillboard() {
    Billboard* bill = (Billboard*)malloc(sizeof(Billboard));
    if (bill == NULL) return NULL;

    bill->obj_type = BILL;
    bill->obj = inheriteObj(bill, BILL);
    bill->texture = 1; // 默認使用的紋理編號
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

    // 獲取相機的世界變換矩陣
    mat4 camTrans;
    glm_mat4_copy(Scene.cameras[Scene.currentCamera]->obj->transform, camTrans);

    // 提取相機位置（相機的第四列）
    vec3 cameraPos = { camTrans[3][0], camTrans[3][1], camTrans[3][2] };

    // 提取物體的位置（物體的第四列）
    vec3 objPos = { obj->transform[3][0], obj->transform[3][1], obj->transform[3][2] };

    // 計算物體到相機的向量
    vec3 lookDir;
    glm_vec3_sub(cameraPos, objPos, lookDir);
    glm_vec3_normalize(lookDir);

    // 設置物體面向相機的旋轉矩陣
    mat4 billboardMatrix;
    glm_mat4_identity(billboardMatrix);

    billboardMatrix[0][0] = 1.0f; // 保持 X 軸不變
    billboardMatrix[1][1] = 1.0f; // 保持 Y 軸不變
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

    // 動態應用 x, y 偏移和 scale 調整
    glTranslatef(bill->x, bill->y, 0.0f); // 應用平移
    glScalef(bill->scale, bill->scale, 1.0f); // 應用縮放

    // 綁定紋理
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureName[bill->texture]);

    // 繪製 Billboard 平面
    float size = 2.0f; // Billboard 的初始大小
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, 0.0f); // 左下
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size, -size, 0.0f);  // 右下
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, 0.0f);   // 右上
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, 0.0f);  // 左上
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

