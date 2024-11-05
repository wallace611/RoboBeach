#include "Robot.h"

#include "utils/Shapes.h"
#include "scene/World.h"
#include "Engine.h"

Robot* newRobot() {
	Robot* bot = (Robot*)malloc(sizeof(Robot));
	if (bot == NULL) return NULL;

	bot->obj_type = ROBOT;
	bot->obj = inheriteObj(bot, ROBOT);
	if (bot->obj == NULL) return NULL;

	bot->cam = newCamera();
	if (bot->cam == NULL) return NULL;
	worldSetCamera(world, bot->cam);
	objAttachmentTo(bot->obj, bot->cam->obj);

	bot->bbody = newBotBody();
	if (bot->bbody == NULL) return NULL;
	ocPushBack(bot->obj->child_list, bot->bbody->obj);
	glm_translate(bot->bbody->obj->transform, (vec3) { 0.0f, 1.7f, 0.0f });

	CollisionShape* bcs = bot->bodyCollision = newCollisionShape();
	if (bcs == NULL) return NULL;
	objAttachmentTo(bot->obj, bcs->obj);
	worldSpawnObj(world, bcs->obj);
	bcs->height = 2.0f;
	bcs->bIsVisible = 1;
	bcs->obj->transform[3][1] = 1.0f;
	bcs->collided = botCollided;

	bot->obj->ready = botReady;
	bot->obj->update = botUpdate;
	bot->obj->render = botRender;

	return bot;
}

void botReady(Object* obj) {
	objReadyChild(obj);
}

void botUpdate(Object* obj, float deltatime) {
	obj->vloc[1] = -9.8 * deltatime;

	glm_translate(obj->transform, obj->vloc);
	//glm_rotate(obj->transform, obj->vrot);
	glm_scale(obj->transform, obj->vscl);
	objUpdateChild(obj, deltatime);
}

void botRender(Object* obj) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	mat4 worldTrans;
	objGetWorldTransform(worldTrans, obj);
	glMultMatrixf(worldTrans);

	objRenderChild(obj);
	glPopMatrix();
}

void botCollided(Object* self, CollisionShape* selfcs, Object* other, CollisionShape* othercs) {
	if (other == NULL || othercs == NULL) return;
	if (other->obj_type == FLOOR) {
		vec3 scl;
		glm_decompose_scalev(other->transform, scl);
		float y = other->transform[3][1] + scl[1] * othercs->height / 2;
		self->transform[3][1] = y;
	}
}
