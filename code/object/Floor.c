#include "Floor.h"

#include "Engine.h"
#include "utils/Shapes.h"

Floor* newFloor() {
	Floor* floor = (Floor*)malloc(sizeof(Floor));
	if (floor == NULL) return NULL;

	floor->obj_type = FLOOR;
	floor->color[0] = .023f;
	floor->color[1] = .72f;
	floor->color[2] = .09f;
    floor->type = 0;

	CollisionShape* cs = floor->csCollide = newCollisionShape();
	if (cs == NULL) {
		free(floor);
		return NULL;
	}
	cs->channel = CC_FLOOR;

	floor->obj = inheriteObj(floor, FLOOR);
	if (floor->obj == NULL) {
		free(floor), free(cs);
		return NULL;
	}

	worldSpawnObj(world, cs->obj);
	objAttachmentTo(floor->obj, cs->obj);

	floor->obj->render = floorRender;

	return floor;
}

void floorRender(Object* obj) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    Floor* flr = cast(obj, FLOOR);
    if (flr == NULL) return;

    // 應用地板的世界變換矩陣
    glMultMatrixf(obj->transform);

    // 設置材質屬性
    GLfloat ambient[4], diffuse[4], specular[4], shininess = .0f;

    if (flr->type == 0) {
        // 沙灘材質
        ambient[0] = 0.6f; ambient[1] = 0.5f; ambient[2] = 0.4f; ambient[3] = 1.0f; // 環境光 (沙灘色)
        diffuse[0] = 0.9f; diffuse[1] = 0.8f; diffuse[2] = 0.6f; diffuse[3] = 1.0f; // 漫射光 (亮沙色)
        specular[0] = 0.2f; specular[1] = 0.2f; specular[2] = 0.1f; specular[3] = 1.0f; // 高光 (微弱光澤)
        shininess = 10.0f; // 低光澤，模擬粗糙的沙灘表面
    } else if (flr->type == 1) {
        // 水材質
        ambient[0] = 0.0f; ambient[1] = 0.1f; ambient[2] = 0.2f; ambient[3] = 1.0f; // 環境光 (深藍色)
        diffuse[0] = 0.0f; diffuse[1] = 0.5f; diffuse[2] = 0.7f; diffuse[3] = 1.0f; // 漫射光 (亮藍色)
        specular[0] = 0.8f; specular[1] = 0.9f; specular[2] = 1.0f; specular[3] = 1.0f; // 高光 (強烈反射)
        shininess = 80.0f; // 高光澤，模擬光滑的水面
    }

    // 應用材質
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    // 開始繪製平面
    glBegin(GL_POLYGON);
    {
        // 定義平面的法線
        glNormal3f(0.0f, 1.0f, 0.0f);

        // 定義平面四個角的頂點
        glVertex3f( 0.5f, 0.5f,  0.5f);
        glVertex3f( 0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f,  0.5f);
    }
    glEnd();

    glPopMatrix();
}
