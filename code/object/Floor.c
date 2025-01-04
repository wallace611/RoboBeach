#include "Floor.h"

#include "Engine.h"
#include "utils/Shapes.h"
#include "texture/Texture.h"

Floor* newFloor() {
	Floor* floor = (Floor*)malloc(sizeof(Floor));
	if (floor == NULL) return NULL;

	floor->obj_type = FLOOR;
	floor->color[0] = .023f;
	floor->color[1] = .72f;
	floor->color[2] = .09f;
    floor->type = 0;
    floor->x = 0.0f;
    floor->y = 0.0f;

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

    floor->obj->update = floorUpdate;
	floor->obj->render = floorRender;

	return floor;
}

void floorUpdate(Object* obj, float deltatime) {
    Floor* flr = cast(obj, FLOOR);
    if (flr == NULL) return;

    if (flr->type == 1) flr->y += deltatime * 0.1;
}

void floorRender(Object* obj) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    Floor* flr = cast(obj, FLOOR);
    if (flr == NULL) return;

    // 假設 x 和 y 是偏移量，您可以根據需求將它們存儲在 Floor 結構中
    float xOffset = flr->x; // 紋理 X 偏移
    float yOffset = flr->y; // 紋理 Y 偏移

    // 應用地板的世界變換矩陣
    glMultMatrixf(obj->transform);

    // 設置材質屬性
    GLfloat ambient[4], diffuse[4], specular[4], shininess = .0f;
    float scale = 5.0f;  // 縮放因子
    glEnable(GL_TEXTURE_2D);

    if (flr->type == 0) {
        // 啟用沙灘材質
        scale = 5.0f;
        glBindTexture(GL_TEXTURE_2D, textureName[2]); // 綁定 2號材質
        ambient[0] = 0.6f; ambient[1] = 0.5f; ambient[2] = 0.4f; ambient[3] = 1.0f; // 沙灘光照
        diffuse[0] = 0.9f; diffuse[1] = 0.8f; diffuse[2] = 0.6f; diffuse[3] = 1.0f;
        specular[0] = 0.2f; specular[1] = 0.2f; specular[2] = 0.1f; specular[3] = 1.0f;
        shininess = 10.0f;
    } else if (flr->type == 1) {
        // 啟用水材質
        scale = 1.0f;
        glBindTexture(GL_TEXTURE_2D, textureName[3]);
        ambient[0] = 0.0f; ambient[1] = 0.1f; ambient[2] = 0.2f; ambient[3] = 1.0f;
        diffuse[0] = 0.0f; diffuse[1] = 0.5f; diffuse[2] = 0.7f; diffuse[3] = 1.0f;
        specular[0] = 0.8f; specular[1] = 0.9f; specular[2] = 1.0f; specular[3] = 1.0f;
        shininess = 80.0f;
    }

    // 應用材質
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    // 繪製整個地板網格
    int gridSize = 30;   // 網格的大小
    float cellSize = 1.0f / gridSize; // 每個小平面的邊長

    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            // 計算每個小平面的四個頂點
            float x0 = -0.5f + i * cellSize;
            float x1 = x0 + cellSize;
            float z0 = -0.5f + j * cellSize;
            float z1 = z0 + cellSize;

            // 設置紋理座標基於整體網格的相對位置，並應用偏移量
            float u0 = (float)i / gridSize * scale + xOffset; // 紋理座標 U，加入偏移量
            float u1 = (float)(i + 1) / gridSize * scale + xOffset;
            float v0 = (float)j / gridSize * scale + yOffset; // 紋理座標 V，加入偏移量
            float v1 = (float)(j + 1) / gridSize * scale + yOffset;

            glBegin(GL_QUADS);
            glNormal3f(0.0f, 1.0f, 0.0f); // 法線指向 +Y
            glTexCoord2f(u0, v0); glVertex3f(x0, 0.5f, z0); // 左下
            glTexCoord2f(u1, v0); glVertex3f(x1, 0.5f, z0); // 右下
            glTexCoord2f(u1, v1); glVertex3f(x1, 0.5f, z1); // 右上
            glTexCoord2f(u0, v1); glVertex3f(x0, 0.5f, z1); // 左上
            glEnd();
        }
    }

    glDisable(GL_TEXTURE_2D); // 禁用紋理，避免影響其他渲染部分
    glPopMatrix();
}
