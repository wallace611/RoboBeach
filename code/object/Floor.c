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

    // ���Φa�O���@���ܴ��x�}
    glMultMatrixf(obj->transform);

    // �]�m�����ݩ�
    GLfloat ambient[4], diffuse[4], specular[4], shininess = .0f;

    if (flr->type == 0) {
        // �F�y����
        ambient[0] = 0.6f; ambient[1] = 0.5f; ambient[2] = 0.4f; ambient[3] = 1.0f; // ���ҥ� (�F�y��)
        diffuse[0] = 0.9f; diffuse[1] = 0.8f; diffuse[2] = 0.6f; diffuse[3] = 1.0f; // ���g�� (�G�F��)
        specular[0] = 0.2f; specular[1] = 0.2f; specular[2] = 0.1f; specular[3] = 1.0f; // ���� (�L�z���A)
        shininess = 10.0f; // �C���A�A�������W���F�y��
    } else if (flr->type == 1) {
        // ������
        ambient[0] = 0.0f; ambient[1] = 0.1f; ambient[2] = 0.2f; ambient[3] = 1.0f; // ���ҥ� (�`�Ŧ�)
        diffuse[0] = 0.0f; diffuse[1] = 0.5f; diffuse[2] = 0.7f; diffuse[3] = 1.0f; // ���g�� (�G�Ŧ�)
        specular[0] = 0.8f; specular[1] = 0.9f; specular[2] = 1.0f; specular[3] = 1.0f; // ���� (�j�P�Ϯg)
        shininess = 80.0f; // �����A�A�������ƪ�����
    }

    // ���Χ���
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    // ø�s 10��10 ������
    int gridSize = 30;  // ���檺�j�p
    float cellSize = 1.0f / gridSize; // �C�Ӥp���������

    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            // �p��C�Ӥp�������|�ӳ��I
            float x0 = -0.5f + i * cellSize;
            float x1 = x0 + cellSize;
            float z0 = -0.5f + j * cellSize;
            float z1 = z0 + cellSize;

            glBegin(GL_QUADS); // ø�s�C�Ӥp����
            glNormal3f(0.0f, 1.0f, 0.0f); // �k�u���V +Y
            glVertex3f(x0, 0.5f, z0); // ���U
            glVertex3f(x1, 0.5f, z0); // �k�U
            glVertex3f(x1, 0.5f, z1); // �k�W
            glVertex3f(x0, 0.5f, z1); // ���W
            glEnd();
        }
    }

    glPopMatrix();
}

