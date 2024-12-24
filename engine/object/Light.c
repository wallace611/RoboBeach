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
            // 設置光源位置為 (0, 0, 0) (方向光)
            glLightfv(light->light_numb, GL_POSITION, (GLfloat[]) { .0f, .0f, .0f, light->type });

            // 畫出光源的 wired sphere 作為可視化
            glDisable(GL_LIGHTING);  // 暫時禁用光照，以便繪製球體
            glColor3f(1.0f, 1.0f, 0.0f);  // 設置顏色為黃色
            glutWireSphere(0.1, 10, 10);  // 繪製一個半徑為 0.1 的 wired sphere
            glEnable(GL_LIGHTING);   // 啟用光照
        } else {
            // 設置光源位置為物體的世界位置 (點光源)
            glLightfv(light->light_numb, GL_POSITION, (GLfloat[]) {
                obj->transform[3][0],
                    obj->transform[3][1],
                    obj->transform[3][2],
                    light->type
            });
        }

        // 設置光的強度
        glLightfv(light->light_numb, GL_AMBIENT, light->ambient);
        glLightfv(light->light_numb, GL_DIFFUSE, light->diffuse);
        glLightfv(light->light_numb, GL_SPECULAR, light->specular);
    }

    objRenderChild(obj);

    glPopMatrix();
}

