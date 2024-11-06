#include "Robot.h"

#include "utils/Shapes.h"
#include "object/component/Connector.h"
#include "scene/World.h"
#include "Engine.h"

Robot* newRobot() {
	Robot* bot = (Robot*)malloc(sizeof(Robot));
	if (bot == NULL) return NULL;

	bot->obj_type = ROBOT;
	bot->bOnFloor = 0;
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

	bot->walkAnimation = newAnimation(bot->obj, ROBOT);
	if (bot->walkAnimation == NULL) return NULL;
	bot->walkAnimation->comp->update = botWalkingAnim;
	ocPushBack(bot->obj->child_list, bot->walkAnimation->comp);

	bot->jumpAnimation = newAnimation(bot->obj, ROBOT);
	if (bot->jumpAnimation == NULL) return NULL;
	bot->jumpAnimation->comp->update = botJumpingAnim;
	ocPushBack(bot->obj->child_list, bot->jumpAnimation->comp);

	bot->flipAnimation = newAnimation(bot->obj, ROBOT);
	if (bot->flipAnimation == NULL) return NULL;
	bot->flipAnimation->comp->update = botFlipingAnim;
	ocPushBack(bot->obj->child_list, bot->flipAnimation->comp);

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

		// animation
		if (obj->vloc[0] > 0.1f || obj->vloc[2] > 0.1f && bot->bOnFloor) {
			bot->walkAnimation->start = 1;
			bot->walkAnimation->magn = 1.0f;
			bot->jumpAnimation->start = 0;
			bot->jumpAnimation->timer = 0.0f;
			bot->flipAnimation->start = 0;
			bot->flipAnimation->timer = 0.0f;
		}
		else if (obj->vloc[0] < -0.1f || obj->vloc[2] < -0.1f && bot->bOnFloor) {
			bot->walkAnimation->start = -1;
			bot->walkAnimation->magn = 1.0f;
			bot->jumpAnimation->start = 0;
			bot->jumpAnimation->timer = 0.0f;
			bot->flipAnimation->start = 0;
			bot->flipAnimation->timer = 0.0f;
		}
		else {
			bot->walkAnimation->start = 0;
			//bot->walkAnimation->timer = 0.0f;
		}

		ocClear(bot->pickupSpace);
		bot->bOnFloor = 0;
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

		Robot* bot = cast(self, ROBOT);
		bot->bOnFloor = 1;
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

void botJump(Robot* bot) {
	static float jmp = 1.0f;
	if (bot->bOnFloor) {
		jmp = 1.0f;
		bot->jumpAnimation->start = 0;
		bot->jumpAnimation->timer = 0.0f;
	}
	if (bot->obj->vloc[1] >= -0.2) {
		bot->obj->vloc[1] += 5.0f * jmp;
		if (jmp > .9f) {
			bot->jumpAnimation->start = 1;
			bot->walkAnimation->start = 0;
			bot->walkAnimation->timer = 0.0f;
			bot->flipAnimation->start = 0;
			bot->flipAnimation->timer = 0.0f;
		}
		else {
			bot->walkAnimation->start = 0;
			bot->walkAnimation->timer = 0.0f;
			bot->jumpAnimation->start = 0;
			bot->jumpAnimation->timer = 0.0f;
			bot->flipAnimation->start = 1;
			bot->flipAnimation->timer = 0.0f;
		}
		jmp *= .8f;
	}
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

void botWalkingAnim(Component* comp, float deltatime) {
	Animation* anim = cast(comp, ANIMATION);
	if (anim == NULL) return;
	if (!anim->start) return;
	Robot* bot = cast(anim->parent, anim->parent_type);
	if (bot == NULL) return;

	BotBody* body = bot->bbody;
	Connector* rightFKnee = bot->bbody->rightThighConnector;
	Connector* rightKnee = bot->bbody->rightCalfConnector;
	Connector* leftFKnee = bot->bbody->leftThighConnector;
	Connector* leftKnee = bot->bbody->leftCalfConnector;

	if (anim->timer < 0) {
		anim->timer = 2.0f;
	}

	if (anim->timer == 0.0f) {
		glm_mat4_identity(body->obj->transform);
		glm_translate(body->obj->transform, (vec3) { 0.0f, 1.7f, 0.0f });
		glm_rotate(body->obj->transform, .1f, (vec3) { 0.0f, 0.0f, 1.0f });

		glm_mat4_identity(rightFKnee->obj->transform);
		glm_translate(rightFKnee->obj->transform, (vec3) { .25f, -.5f, .0f });
		glm_rotate(rightFKnee->obj->transform, -1.0f, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_mat4_identity(rightKnee->obj->transform);
		glm_translate(rightKnee->obj->transform, (vec3) { .0f, -.8f, .0f });
		glm_rotate(rightKnee->obj->transform, 1.0f, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_mat4_identity(leftFKnee->obj->transform);
		glm_translate(leftFKnee->obj->transform, (vec3) { -.25f, -.5f, .0f });
		glm_rotate(leftFKnee->obj->transform, .5f, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_mat4_identity(leftKnee->obj->transform);
		glm_translate(leftKnee->obj->transform, (vec3) { .0f, -.8f, .0f });
		glm_rotate(leftKnee->obj->transform, .2f, (vec3) { 1.0f, 0.0f, 0.0f });
	}

	if (anim->timer > .0f && anim->timer < .5f) {
		glm_rotate(body->obj->transform, -.1f * deltatime * 2, (vec3) { 0.0f, 0.0f, 1.0f });

		glm_rotate(rightFKnee->obj->transform, 1.0f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(rightKnee->obj->transform, -1.0f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftFKnee->obj->transform, -.5f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftKnee->obj->transform, -.2f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });
	}

	if (anim->timer >= .5f && anim->timer < 1.0f) {
		glm_rotate(body->obj->transform, -.1f * deltatime * 2, (vec3) { 0.0f, 0.0f, 1.0f });

		glm_rotate(rightFKnee->obj->transform, .5f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(rightKnee->obj->transform, .2f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftFKnee->obj->transform, -1.0f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftKnee->obj->transform, 1.0f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });
	}

	if (anim->timer >= 1.0f && anim->timer < 1.5f) {
		glm_rotate(body->obj->transform, .1f * deltatime * 2, (vec3) { 0.0f, 0.0f, 1.0f });

		glm_rotate(rightFKnee->obj->transform, -.5f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(rightKnee->obj->transform, -.2f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftFKnee->obj->transform, 1.0f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftKnee->obj->transform, -1.0f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });
	}

	if (anim->timer >= 1.5f && anim->timer < 2.0f) {
		glm_rotate(body->obj->transform, .1f * deltatime * 2, (vec3) { 0.0f, 0.0f, 1.0f });

		glm_rotate(rightFKnee->obj->transform, -1.0f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(rightKnee->obj->transform, 1.0f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftFKnee->obj->transform, .5f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftKnee->obj->transform, .2f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });

	}

	if (anim->timer >= 2.0f) {
		anim->timer = 0.001f;
	}

	anim->timer += deltatime * anim->magn;
}

void botJumpingAnim(Component* comp, float deltatime) {
	Animation* anim = cast(comp, ANIMATION);
	if (anim == NULL) return;
	if (!anim->start) return;
	Robot* bot = cast(anim->parent, anim->parent_type);
	if (bot == NULL) return;

	BotBody* body = bot->bbody;
	Connector* rightFKnee = bot->bbody->rightThighConnector;
	Connector* rightKnee = bot->bbody->rightCalfConnector;
	Connector* leftFKnee = bot->bbody->leftThighConnector;
	Connector* leftKnee = bot->bbody->leftCalfConnector;
	
	if (anim->timer == 0.0f) {
		glm_mat4_identity(body->obj->transform);
		glm_translate(body->obj->transform, (vec3) { 0.0f, 1.7f, 0.0f });
		glm_rotate(body->obj->transform, .2f, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_mat4_identity(rightFKnee->obj->transform);
		glm_translate(rightFKnee->obj->transform, (vec3) { .25f, -.5f, .0f });
		glm_rotate(rightFKnee->obj->transform, -1.0f, (vec3) { .2f, -.02f, 0.0f });

		glm_mat4_identity(rightKnee->obj->transform);
		glm_translate(rightKnee->obj->transform, (vec3) { .0f, -.8f, .0f });
		glm_rotate(rightKnee->obj->transform, 1.0f, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_mat4_identity(leftFKnee->obj->transform);
		glm_translate(leftFKnee->obj->transform, (vec3) { -.25f, -.5f, .0f });
		glm_rotate(leftFKnee->obj->transform, -1.0f, (vec3) { .2f, .02f, 0.0f });

		glm_mat4_identity(leftKnee->obj->transform);
		glm_translate(leftKnee->obj->transform, (vec3) { .0f, -.8f, .0f });
		glm_rotate(leftKnee->obj->transform, 1.0f, (vec3) { 1.0f, 0.0f, 0.0f });
	}

	if (anim->timer < .2f) {
		glm_rotate(body->obj->transform, -.2f * deltatime * 5, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(rightFKnee->obj->transform, 1.0f * deltatime * 5, (vec3) { .2f, -.02f, 0.0f });

		glm_rotate(rightKnee->obj->transform, -1.0f * deltatime * 5, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftFKnee->obj->transform, 1.0f * deltatime * 5, (vec3) { .2f, .02f, 0.0f });

		glm_rotate(leftKnee->obj->transform, -1.0f * deltatime * 5, (vec3) { 1.0f, 0.0f, 0.0f });
	}

	anim->timer += deltatime;
}

void botFlipingAnim(Component* comp, float deltatime) {
	Animation* anim = cast(comp, ANIMATION);
	if (anim == NULL) return;
	if (!anim->start) return;
	Robot* bot = cast(anim->parent, anim->parent_type);
	if (bot == NULL) return;

	BotBody* body = bot->bbody;
	Connector* rightFKnee = bot->bbody->rightThighConnector;
	Connector* rightKnee = bot->bbody->rightCalfConnector;
	Connector* leftFKnee = bot->bbody->leftThighConnector;
	Connector* leftKnee = bot->bbody->leftCalfConnector;

	if (anim->timer == 0.0f) {
		glm_mat4_identity(body->obj->transform);
		glm_translate(body->obj->transform, (vec3) { 0.0f, 1.7f, 0.0f });
	}

	if (anim->timer < .25f) {
		glm_rotate(body->obj->transform, M_PI * 2 * deltatime * 4, (vec3) { 1.0f, 0.0f, 0.0f });
	}

	anim->timer += deltatime;
}
