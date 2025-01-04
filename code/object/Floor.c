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

    // ���] x �M y �O�����q�A�z�i�H�ھڻݨD�N���̦s�x�b Floor ���c��
    float xOffset = flr->x; // ���z X ����
    float yOffset = flr->y; // ���z Y ����

    // ���Φa�O���@���ܴ��x�}
    glMultMatrixf(obj->transform);

    // �]�m�����ݩ�
    GLfloat ambient[4], diffuse[4], specular[4], shininess = .0f;
    float scale = 5.0f;  // �Y��]�l
    glEnable(GL_TEXTURE_2D);

    if (flr->type == 0) {
        // �ҥΨF�y����
        scale = 5.0f;
        glBindTexture(GL_TEXTURE_2D, textureName[2]); // �j�w 2������
        ambient[0] = 0.6f; ambient[1] = 0.5f; ambient[2] = 0.4f; ambient[3] = 1.0f; // �F�y����
        diffuse[0] = 0.9f; diffuse[1] = 0.8f; diffuse[2] = 0.6f; diffuse[3] = 1.0f;
        specular[0] = 0.2f; specular[1] = 0.2f; specular[2] = 0.1f; specular[3] = 1.0f;
        shininess = 10.0f;
    } else if (flr->type == 1) {
        // �ҥΤ�����
        scale = 1.0f;
        glBindTexture(GL_TEXTURE_2D, textureName[3]);
        ambient[0] = 0.0f; ambient[1] = 0.1f; ambient[2] = 0.2f; ambient[3] = 1.0f;
        diffuse[0] = 0.0f; diffuse[1] = 0.5f; diffuse[2] = 0.7f; diffuse[3] = 1.0f;
        specular[0] = 0.8f; specular[1] = 0.9f; specular[2] = 1.0f; specular[3] = 1.0f;
        shininess = 80.0f;
    }

    // ���Χ���
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    // ø�s��Ӧa�O����
    int gridSize = 30;   // ���檺�j�p
    float cellSize = 1.0f / gridSize; // �C�Ӥp���������

    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            // �p��C�Ӥp�������|�ӳ��I
            float x0 = -0.5f + i * cellSize;
            float x1 = x0 + cellSize;
            float z0 = -0.5f + j * cellSize;
            float z1 = z0 + cellSize;

            // �]�m���z�y�а�������檺�۹��m�A�����ΰ����q
            float u0 = (float)i / gridSize * scale + xOffset; // ���z�y�� U�A�[�J�����q
            float u1 = (float)(i + 1) / gridSize * scale + xOffset;
            float v0 = (float)j / gridSize * scale + yOffset; // ���z�y�� V�A�[�J�����q
            float v1 = (float)(j + 1) / gridSize * scale + yOffset;

            glBegin(GL_QUADS);
            glNormal3f(0.0f, 1.0f, 0.0f); // �k�u���V +Y
            glTexCoord2f(u0, v0); glVertex3f(x0, 0.5f, z0); // ���U
            glTexCoord2f(u1, v0); glVertex3f(x1, 0.5f, z0); // �k�U
            glTexCoord2f(u1, v1); glVertex3f(x1, 0.5f, z1); // �k�W
            glTexCoord2f(u0, v1); glVertex3f(x0, 0.5f, z1); // ���W
            glEnd();
        }
    }

    glDisable(GL_TEXTURE_2D); // �T�ί��z�A�קK�v�T��L��V����
    glPopMatrix();
}
