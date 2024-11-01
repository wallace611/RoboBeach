#include "DebugText.h"

#include "Engine.h"
#include "utils/String.h"

#include <math.h>

DebugText* newDebugText() {
	DebugText* dbtxt = (DebugText*)malloc(sizeof(DebugText));
	if (dbtxt == NULL) return NULL;
	
	dbtxt->objToShow = newObjContainer();
	if (dbtxt->objToShow == NULL) return NULL;

	dbtxt->ready = dbtReady;
	dbtxt->update = dbtUpdate;
	dbtxt->render = dbtRender;

	return dbtxt;
}

void dbtReady(DebugText* dbtxt) {
}

void dbtUpdate(DebugText* dbtxt, float deltatime) {
	dbtxt->deltatime = deltatime;
}

void dbtRender(DebugText* dbtxt) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, window_wid, 0, window_hei);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    glRasterPos2f(10, window_hei - 20);

    String* s = newString();

    char tmp[1024] = { 0 };

    sprintf(tmp, "Deltatime: %3.4f\nFrame rate: %3.1f\nTimer: %3.3f\n", dbtxt->deltatime, engineGetCurrentFPS(), engineGetTimer());
    strExpandc(s, tmp);

    for (int i = 0; i < dbtxt->objToShow->end; i++) {
        Object* obj = dbtxt->objToShow->list[i];

        switch (obj->obj_type) {
        case CAMERA:
            sprintf(tmp, "Camera id: %d\n", obj->id);
            break;

        default:
            sprintf(tmp, "Object id: %d\n", obj->id);
            break;
        }
        
        strExpandc(s, tmp);

        sprintf(tmp, "Transform Matrix:\n");
        for (int j = 0; j < 4; j++) {
            sprintf(tmp + strlen(tmp), "[ %8.4f, %8.4f, %8.4f, %8.4f ]\n", 
                obj->transform[j][0], obj->transform[j][1], 
                obj->transform[j][2], obj->transform[j][3]);
        }
        strExpandc(s, tmp);
        strAppend(s, '\n');

        vec4 position, scale;
        mat4 rotation;

        glm_decompose(obj->transform, position, rotation, scale);

        sprintf(tmp, "Position: [ %8.4f, %8.4f, %8.4f ]\n", position[0], position[1], position[2]);
        strExpandc(s, tmp);

        vec3 eulerAngles;
        glm_euler_angles(rotation, eulerAngles);

        eulerAngles[0] = eulerAngles[0] * 180 / M_PI;
        eulerAngles[1] = eulerAngles[1] * 180 / M_PI;
        eulerAngles[2] = eulerAngles[2] * 180 / M_PI;

        sprintf(tmp, "Rotation: [ %8.4f, %8.4f, %8.4f ]\n", 
            eulerAngles[0], eulerAngles[1], eulerAngles[2]);
        strExpandc(s, tmp);

        sprintf(tmp, "Scale: [ %8.4f, %8.4f, %8.4f ]\n", scale[0], scale[1], scale[2]);
        strExpandc(s, tmp);

        strAppend(s, '\n');
        strAppend(s, '\n');
    }

    glutBitmapString(GLUT_BITMAP_8_BY_13, s->texts);

    strFree(s);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}
