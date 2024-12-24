#include "CollisionShape.h"

CollisionShape* newCollisionShape() {
	CollisionShape* cs = (CollisionShape*)malloc(sizeof(CollisionShape));
	if (cs == NULL) return NULL;

	cs->obj_type = COLLISION;
    cs->channel = CC_DEFAULT;
	cs->width = 1.0f;
	cs->height = 1.0f;
	cs->depth = 1.0f;
	cs->bInFront = 1;
	cs->bIsVisible = 0;
	cs->obj = inheriteObj(cs, COLLISION);

	cs->obj->ready = csReady;
	cs->obj->update = csUpdate;
	cs->obj->render = csRender;

	cs->collided = csCollided;
	return cs;
}

void csReady(Object* obj) {
	objReadyChild(obj);
}

void csUpdate(Object* obj, float deltatime) {
	objUpdateChild(obj, deltatime);
}

void csRender(Object* obj) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glDisable(GL_LIGHTING);

    CollisionShape* cs = cast(obj, COLLISION);
    if (cs != NULL && cs->bIsVisible) {
        glPushMatrix();

        mat4 worldTrans;
        objGetWorldTransform(worldTrans, obj);

        // 提取縮放因子
        vec3 scl;
        glm_decompose_scalev(worldTrans, scl);

        glTranslatef(worldTrans[3][0], worldTrans[3][1], worldTrans[3][2]);

        if (cs->bInFront) {
            glDisable(GL_DEPTH_TEST);
        }

        // 渲染盒子的面
        glBegin(GL_QUADS);
        float halfWidth = (cs->width * scl[0]) / 2;
        float halfHeight = (cs->height * scl[1]) / 2;
        float halfDepth = (cs->depth * scl[2]) / 2;

        float vertices[8][3] = {
            {-halfWidth, -halfHeight, -halfDepth},
            { halfWidth, -halfHeight, -halfDepth},
            { halfWidth,  halfHeight, -halfDepth},
            {-halfWidth,  halfHeight, -halfDepth},
            {-halfWidth, -halfHeight,  halfDepth},
            { halfWidth, -halfHeight,  halfDepth},
            { halfWidth,  halfHeight,  halfDepth},
            {-halfWidth,  halfHeight,  halfDepth}
        };

        float color[3];

        switch (cs->channel) {
        case CC_DEFAULT:
            color[0] = 0.8f;
            color[1] = 0.8f;
            color[2] = 0.8f;
            break;

        case CC_COLLISION:
            color[0] = 0.8f;
            color[1] = 0.1f;
            color[2] = 0.1f;
            break;

        case CC_PICKUP:
            color[0] = 0.2f;
            color[1] = 0.8f;
            color[2] = 0.2f;
            break;

        case CC_WATER:
            color[0] = 0.1f;
            color[1] = 0.1f;
            color[2] = 0.8f;
            break;

        default:
            color[0] = 0.0f;
            color[1] = 0.0f;
            color[2] = 0.0f;
            break;
        }
        glColor4f(color[0], color[1], color[2], .2f);

        // 前面
        glVertex3fv(vertices[0]);
        glVertex3fv(vertices[1]);
        glVertex3fv(vertices[2]);
        glVertex3fv(vertices[3]);

        // 後面
        glVertex3fv(vertices[4]);
        glVertex3fv(vertices[5]);
        glVertex3fv(vertices[6]);
        glVertex3fv(vertices[7]);

        // 左面
        glVertex3fv(vertices[0]);
        glVertex3fv(vertices[3]);
        glVertex3fv(vertices[7]);
        glVertex3fv(vertices[4]);

        // 右面
        glVertex3fv(vertices[1]);
        glVertex3fv(vertices[2]);
        glVertex3fv(vertices[6]);
        glVertex3fv(vertices[5]);

        // 上面
        glVertex3fv(vertices[3]);
        glVertex3fv(vertices[2]);
        glVertex3fv(vertices[6]);
        glVertex3fv(vertices[7]);

        // 下面
        glVertex3fv(vertices[0]);
        glVertex3fv(vertices[1]);
        glVertex3fv(vertices[5]);
        glVertex3fv(vertices[4]);

        glEnd();

        // 渲染盒子的邊緣
        glColor3fv(color);

        glBegin(GL_LINES);
        for (int i = 0; i < 4; i++) {
            glVertex3fv(vertices[i]);
            glVertex3fv(vertices[(i + 1) % 4]);
            glVertex3fv(vertices[i + 4]);
            glVertex3fv(vertices[(i + 1) % 4 + 4]);
            glVertex3fv(vertices[i]);
            glVertex3fv(vertices[i + 4]);
        }
        glEnd();

        glEnable(GL_DEPTH_TEST);

        glPopMatrix();
    }

    glMultMatrixf(obj->transform);
    glEnable(GL_LIGHTING);
    objRenderChild(obj);

    glPopMatrix();
}

unsigned char csIsCollideWith(CollisionShape* self, CollisionShape* target) {
    if (self == NULL || target == NULL) return 0;

    // 獲取 self 的世界坐標和縮放
    mat4 selfWorldTrans;
    objGetWorldTransform(selfWorldTrans, self->obj);
    vec3 scl;
    glm_decompose_scalev(selfWorldTrans, scl);

    float selfScaleX = scl[0];
    float selfScaleY = scl[1];
    float selfScaleZ = scl[2];
    float selfMinX = selfWorldTrans[3][0] - (self->width * selfScaleX) / 2;
    float selfMaxX = selfWorldTrans[3][0] + (self->width * selfScaleX) / 2;
    float selfMinY = selfWorldTrans[3][1] - (self->height * selfScaleY) / 2;
    float selfMaxY = selfWorldTrans[3][1] + (self->height * selfScaleY) / 2;
    float selfMinZ = selfWorldTrans[3][2] - (self->depth * selfScaleZ) / 2;
    float selfMaxZ = selfWorldTrans[3][2] + (self->depth * selfScaleZ) / 2;

    // 獲取 target 的世界坐標和縮放
    mat4 targetWorldTrans;
    objGetWorldTransform(targetWorldTrans, target->obj);
    glm_decompose_scalev(targetWorldTrans, scl);

    float targetScaleX = scl[0];
    float targetScaleY = scl[1];
    float targetScaleZ = scl[2];
    float targetMinX = targetWorldTrans[3][0] - (target->width * targetScaleX) / 2;
    float targetMaxX = targetWorldTrans[3][0] + (target->width * targetScaleX) / 2;
    float targetMinY = targetWorldTrans[3][1] - (target->height * targetScaleY) / 2;
    float targetMaxY = targetWorldTrans[3][1] + (target->height * targetScaleY) / 2;
    float targetMinZ = targetWorldTrans[3][2] - (target->depth * targetScaleZ) / 2;
    float targetMaxZ = targetWorldTrans[3][2] + (target->depth * targetScaleZ) / 2;

    // 檢查 AABB 是否重疊
    if (selfMaxX < targetMinX || selfMinX > targetMaxX) return 0;
    if (selfMaxY < targetMinY || selfMinY > targetMaxY) return 0;
    if (selfMaxZ < targetMinZ || selfMinZ > targetMaxZ) return 0;

    return 1;
}

void csCollided(Object* self, CollisionShape* selfcs, Object* other, CollisionShape* othercs) {
	
}
