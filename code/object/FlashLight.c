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

        // 設置光源位置 (作為聚光燈的光源位置)
        GLfloat lightPosition[] = { obj->transform[3][0], obj->transform[3][1], obj->transform[3][2], 1.0f };
        glLightfv(light->light_numb, GL_POSITION, lightPosition);

        // 設置聚光燈方向
        GLfloat spotDirection[] = { 0.0f, .0f, 1.0f }; // 聚光燈指向 -Y 軸（可以根據需要調整）
        glLightfv(light->light_numb, GL_SPOT_DIRECTION, spotDirection);

        // 設置聚光角度
        glLightf(light->light_numb, GL_SPOT_CUTOFF, 30.0f); // 30 度的光束角度

        // 設置聚光集中度
        glLightf(light->light_numb, GL_SPOT_EXPONENT, 10.0f); // 聚光集中度，越大光束越集中

        // 設置光的強度
        glLightfv(light->light_numb, GL_AMBIENT, light->ambient);
        glLightfv(light->light_numb, GL_DIFFUSE, light->diffuse);
        glLightfv(light->light_numb, GL_SPECULAR, light->specular);

        

        // 畫出光源位置的 wired sphere 作為可視化
        glDisable(GL_LIGHTING);  // 暫時禁用光照，以便繪製球體
        glColor3f(1.0f, 1.0f, 0.0f);  // 設置顏色為黃色
        glutWireSphere(0.1, 10, 10);  // 繪製一個半徑為 0.1 的 wired sphere
        glEnable(GL_LIGHTING);   // 啟用光照
    }

    objRenderChild(obj);

    glPopMatrix();
}


