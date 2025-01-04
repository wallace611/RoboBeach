#include "World.h"

#include "utils/Shapes.h"
#include "texture/Texture.h"
#include "object/CollisionShape.h"

World* newWorld() {
    World* world = (World*)malloc(sizeof(World));
    if (world == NULL) return NULL;

    world->instances = newObjContainer();
    world->collisionList = newObjContainer();
    world->bUseCamera = 1;
    return world;
}

void worldUpdate(World* world, float deltatime) {
    if (world->bUseCamera)
        world->cam->obj->update(world->cam->obj, deltatime);
    for (int i = 0; i < (size_t) world->instances->end; i++) {
        Object* obj = cast(world->instances->list[i], OBJECT);
        obj->update(world->instances->list[i], deltatime);
    }
}

void renderSkyDome() {
    glPushMatrix();

    // 禁用深度寫入，確保天空在背景
    glDepthMask(GL_FALSE);

    // 啟用紋理並綁定天空紋理
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, textureName[10]);

    // 繪製半球體作為天空
    glRotatef(90.0f, 1.0f, .0f, .0f);
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricOrientation(quadric, GLU_INSIDE);
    gluSphere(quadric, 500.0f, 64, 64);
    gluDeleteQuadric(quadric);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glDepthMask(GL_TRUE); // 恢復深度寫入

    GLfloat skyAmbient[4] = { 0.2f, 0.3f, 0.5f, 1.0f }; // 假設天空為藍色
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, skyAmbient);

    glPopMatrix();
}

void worldRender(World* world) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    if (world->bUseCamera)
        world->cam->obj->render(world->cam->obj);

    for (int i = 0; i < (size_t) world->instances->end; i++) {
        Object* obj = cast(world->instances->list[i], OBJECT);
        obj->render(obj);
    }
    for (int i = 0; i < (size_t)world->collisionList->end; i++) {
        Object* obj = ((CollisionShape*)world->collisionList->list[i])->obj;
        obj->render(obj);
    }
    renderSkyDome();
    glPopMatrix();
}

int worldSpawnObj(World* world, Object* obj) {
    if (world == NULL || obj == NULL) return -1;

    CollisionShape* cs = cast(obj, COLLISION);
    if (cs != NULL) {
        ocPushBack(world->collisionList, cs);
    }
    else {
        ocPushBack(world->instances, obj);
    }

    obj->ready(obj);
    return 1;
}

int worldKillObj(World* world, Object* obj) {
    if (world == NULL || obj == NULL) return -1;

    int flag = ocRemove(world->instances, obj);
    if (flag != 1) return -1;

    free(obj);
    return 1;
}

void worldSetCamera(World* world, Camera* cam) {
    if (world == NULL || cam == NULL) return;

    world->cam = cam;
    cam->obj->ready(cam->obj);
}

void worldCollisionDetection(World* world) {
    for (int i = 0; i < (size_t)world->collisionList->end; i++) {
        for (int j = 0; j < (size_t) world->collisionList->end; j++) {
            if (i == j) continue;
            CollisionShape* c1 = world->collisionList->list[i];
            if (c1 == NULL) continue;

            CollisionShape* c2 = world->collisionList->list[j];
            if (c2 == NULL) {
                free(c1);
                continue;
            }

            if (csIsCollideWith(c1, c2)) {
                c1->collided(c1->obj->owner, c1, c2->obj->owner, c2);
            }
        }
    }
}

void worldToggleCollisionVision(World* world) {
    for (int i = 0; i < world->collisionList->end; i++) {
        CollisionShape* cs = world->collisionList->list[i];
        cs->bIsVisible = !cs->bIsVisible;
    }
}
