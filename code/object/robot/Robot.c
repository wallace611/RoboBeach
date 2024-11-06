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
	bot->cam->obj->update = botCamUpdate;
	bot->cam->pitchSensitivity = 0.2f;
	bot->cam->yawSensitivity = 0.05f;

	bot->bbody = newBotBody();
	if (bot->bbody == NULL) return NULL;
	ocPushBack(bot->obj->child_list, bot->bbody->obj);
	glm_translate(bot->bbody->obj->transform, (vec3) { 0.0f, 1.7f, 0.0f });

	bot->pickedObj = NULL;

	CollisionShape* bcs = bot->bodyCollision = newCollisionShape();
	if (bcs == NULL) return NULL;
	objAttachmentTo(bot->obj, bcs->obj);
	worldSpawnObj(world, bcs->obj);
	bcs->height = 2.0f;
	bcs->obj->transform[3][1] = 1.0f;
	bcs->collided = botCollided;

	CollisionShape* pcs = bot->pickCollision = newCollisionShape();
	if (pcs == NULL) return NULL;
	objAttachmentTo(bot->obj, pcs->obj);
	worldSpawnObj(world, pcs->obj);
	pcs->bIsVisible = 1;
	pcs->height = 1.0f;
	pcs->width = 2.0f;
	pcs->depth = 2.0f;
	pcs->collided = botObjEnterPickupSpace;

	ObjContainer* ps = bot->pickupSpace = newObjContainer();
	if (ps == NULL) return NULL;

	bot->obj->ready = botReady;
	bot->obj->update = botUpdate;
	bot->obj->render = botRender;

	return bot;
}

void botReady(Object* obj) {
	obj->aloc[1] = -9.8f;

	objReadyChild(obj);
}

void botUpdate(Object* obj, float deltatime) {
	vec3 dtTmp;

	glm_vec3_scale(obj->vrot, deltatime, dtTmp);
	glm_rotate(obj->transform, dtTmp[1], (vec3) { 0.0f, 1.0f, 0.0f });

	glm_vec3_scale(obj->vloc, deltatime, dtTmp);
	dtTmp[0] *= 5;
	dtTmp[2] *= 5;
	glm_translate(obj->transform, dtTmp);

	objUpdateChild(obj, deltatime);

	vec3 vtmp;
	glm_vec3_copy(obj->vloc, vtmp);
	glm_vec3_scale(obj->aloc, deltatime, dtTmp);
	glm_vec3_add(vtmp, dtTmp, obj->vloc);

	obj->vloc[0] *= 0.9;
	obj->vloc[1] *= 0.9995;
	obj->vloc[2] *= 0.9;
	obj->vrot[1] *= 0.9;

	Robot* bot = cast(obj, ROBOT);
	if (bot != NULL) {
		ocClear(bot->pickupSpace);
	}
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
		self->vloc[1] = 0.0f;
	}
}

void botCamUpdate(Object* obj, float deltatime) {
	mat4 trans;
	const float radius = 10.0f;

	mat4 center;
	objGetWorldTransform(trans, obj->owner);
	glm_mat4_copy(trans, center);

	Camera* cam = cast(obj, CAMERA);
	cam->camRot[0] = fmod(cam->camRot[0], 360.0f);
	cam->camRot[1] = fmod(cam->camRot[1], 360.0f);
	cam->camRot[2] = fmod(cam->camRot[2], 360.0f);

	float lastPitch = cam->camRot[0];
	cam->camRot[0] += cam->pitchVal * deltatime * cam->pitchSensitivity;
	cam->camRot[1] += cam->yawVal * deltatime * cam->yawSensitivity;
	if (cam->camRot[0] > 100.0f || cam->camRot[0] < -1.0f) {
		cam->camRot[0] = lastPitch;
	}

	float x = sin(cam->camRot[1]) * radius;
	float y = cam->camRot[0];
	float z = cos(cam->camRot[1]) * radius;

	vec3 a = { -x + center[3][0], y, z - center[3][2]};
	vec3 b = { center[3][0], center[3][1], -center[3][2] };
	vec3 c = { 0.0f, 1.0f, 0.0f };
	glm_lookat(a, b, c, trans);

	glm_mat4_copy(trans, obj->transform);

	cam->forwardVal *= cam->moveFraction;
	cam->sideVal *= cam->moveFraction;
	cam->pitchVal *= cam->rotateFraction;
	cam->yawVal *= cam->rotateFraction;

}

void botMoving(Robot* bot, float forward, float side) {
	if (forward != 0.0f) bot->obj->vloc[2] = forward;
	if (side != 0.0f) bot->obj->vrot[1] = side;
}

void botPickup(Robot* bot) {
	float minDist = 1e9;
	Object* minDistObj = NULL;
	for (int i = 0; i < bot->pickupSpace->end; i++) {
		Object* target = cast(bot->pickupSpace->list[i], OBJECT);
		if (target != NULL && target != bot->pickedObj) {
			float dist = glm_vec3_distance(bot->obj->transform[3], target->transform[3]);
			if (dist < minDist) {
				minDistObj = target;
				minDist = dist;
			}
		}
	}
	if (minDistObj != NULL) {
		botDrop(bot);
		objAttachmentTo(bot->obj, minDistObj);
		bot->pickedObj = minDistObj;
		
		mat4 newTrans;
		glm_mat4_identity(newTrans);
		glm_translate(newTrans, (vec3) { 0.0f, 1.0f, 1.0f });
		glm_mat4_copy(newTrans, bot->pickedObj->transform);
	}
}

void botDrop(Robot* bot) {
	if (bot->pickedObj == NULL) return;
	mat4 worldTrans;
	objGetWorldTransform(worldTrans, bot->pickedObj);
	objDeattachment(bot->obj, bot->pickedObj);
	glm_mat4_copy(worldTrans, bot->pickedObj->transform);
	bot->pickedObj = NULL;
}

void botObjEnterPickupSpace(Object* self, CollisionShape* selfcs, Object* other, CollisionShape* othercs) {
	if (self == NULL || other == NULL || othercs == NULL || othercs->channel != CC_PICKUP) return;

	Robot* bot = cast(self, ROBOT);
	if (bot != NULL) {
		ocPushBack(bot->pickupSpace, other);
	}
}
