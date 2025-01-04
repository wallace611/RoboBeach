#include "Rock.h"

#include "texture/Texture.h"
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

    // 啟用紋理並綁定編號 textureName[5]
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureName[5]);

    // 可調整的 scale，用於控制紋理平鋪密集度
    float scale = 2.0f; // scale 越大，紋理平鋪越密集

    // 設置岩石材質屬性
    GLfloat rockAmbient[] = { 0.3f, 0.2f, 0.1f, 1.0f };  // 環境光 (暗棕色)
    GLfloat rockDiffuse[] = { 0.5f, 0.35f, 0.2f, 1.0f }; // 漫射光 (中棕色)
    GLfloat rockSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // 高光反射 (微弱的光澤)
    GLfloat rockShininess = 10.0f;                        // 光澤度 (低光澤)

    glMaterialfv(GL_FRONT, GL_AMBIENT, rockAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, rockDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, rockSpecular);
    glMaterialf(GL_FRONT, GL_SHININESS, rockShininess);

    // 使用球體紋理坐標映射來繪製岩石
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE); // 啟用紋理映射
    gluSphere(quadric, 1.0f, 16, 16);   // 繪製球體（岩石）
    gluDeleteQuadric(quadric);

    glPopMatrix();

    // 繪製子對象
    objRenderChild(obj);

    // 禁用紋理
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}
