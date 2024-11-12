#include "Robot.h"

#include "utils/Shapes.h"
#include "object/component/Connector.h"
#include "scene/World.h"
#include "Engine.h"

#include <stdlib.h>

Robot* newRobot() {
	Robot* bot = (Robot*)malloc(sizeof(Robot));
	if (bot == NULL) return NULL;

	bot->obj_type = ROBOT;
	bot->bOnFloor = 0;
	bot->bIsJumping = 0;
	bot->bIsHolding = 0;
	bot->bIsFalling = 0;
	bot->bIsFalled = 0;
	bot->bCollideWithBlock = 0;
	bot->bIsKneeDown = 0;
	bot->airTime = 0.0f;
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
	bcs->channel = CC_COLLISION;
	bcs->height = 2.0f;
	bcs->obj->transform[3][1] = 1.0f;
	bcs->collided = botCollided;

	CollisionShape* pcs = bot->pickCollision = newCollisionShape();
	if (pcs == NULL) return NULL;
	objAttachmentTo(bot->obj, pcs->obj);
	worldSpawnObj(world, pcs->obj);
	pcs->channel = CC_PICKUP;
	glm_translate(pcs->obj->transform, (vec3) { 0.0f, 2.0f, 0.0f });
	pcs->height = 4.0f;
	pcs->width = 2.0f;
	pcs->depth = 2.0f;
	pcs->collided = botObjEnterPickupSpace;

	ObjContainer* ps = bot->pickupSpace = newObjContainer();
	if (ps == NULL) return NULL;

	bot->obj->ready = botReady;
	bot->obj->update = botUpdate;
	bot->obj->render = botRender;

	bot->idleAnimation = newAnimation(bot->obj, ROBOT);
	if (bot->idleAnimation == NULL) return NULL;
	bot->idleAnimation->comp->update = botIdleAnim;
	ocPushBack(bot->obj->child_list, bot->idleAnimation->comp);

	bot->walkAnimation = newAnimation(bot->obj, ROBOT);
	if (bot->walkAnimation == NULL) return NULL;
	bot->walkAnimation->comp->update = botWalkingAnim;
	ocPushBack(bot->obj->child_list, bot->walkAnimation->comp);

	bot->jumpAnimation = newAnimation(bot->obj, ROBOT);
	if (bot->jumpAnimation == NULL) return NULL;
	bot->jumpAnimation->comp->update = botJumpingAnim;
	ocPushBack(bot->obj->child_list, bot->jumpAnimation->comp);

	bot->touchDownAnimation = newAnimation(bot->obj, ROBOT);
	if (bot->touchDownAnimation == NULL) return NULL;
	bot->touchDownAnimation->comp->update = botTouchDownAnim;
	ocPushBack(bot->obj->child_list, bot->touchDownAnimation->comp);

	bot->flipAnimation = newAnimation(bot->obj, ROBOT);
	if (bot->flipAnimation == NULL) return NULL;
	bot->flipAnimation->comp->update = botFlipingAnim;
	ocPushBack(bot->obj->child_list, bot->flipAnimation->comp);

	bot->pickupAnimation = newAnimation(bot->obj, ROBOT);
	if (bot->pickupAnimation == NULL) return NULL;
	bot->pickupAnimation->comp->update = botPickupAnim;
	ocPushBack(bot->obj->child_list, bot->pickupAnimation->comp);

	bot->dropAnimation = newAnimation(bot->obj, ROBOT);
	if (bot->dropAnimation == NULL) return NULL;
	bot->dropAnimation->comp->update = botDropAnim;
	ocPushBack(bot->obj->child_list, bot->dropAnimation->comp);

	bot->fallAnimation = newAnimation(bot->obj, ROBOT);
	if (bot->fallAnimation == NULL) return NULL;
	bot->fallAnimation->comp->update = botFallAnim;
	ocPushBack(bot->obj->child_list, bot->fallAnimation->comp);

	bot->kneeDownAnimation = newAnimation(bot->obj, ROBOT);
	if (bot->kneeDownAnimation == NULL) return NULL;
	bot->kneeDownAnimation->comp->update = botKneeDownAnim;
	ocPushBack(bot->obj->child_list, bot->kneeDownAnimation->comp);

	return bot;
}

void botReady(Object* obj) {
	obj->aloc[1] = -9.8f;

	objReadyChild(obj);
}

void botUpdate(Object* obj, float deltatime) {
	Robot* bot = cast(obj, ROBOT);
	if (bot != NULL) {
		vec3 dtTmp;

		glm_vec3_scale(obj->vrot, deltatime * !bot->bIsFalled, dtTmp);
		glm_rotate(obj->transform, dtTmp[1], (vec3) { 0.0f, 1.0f, 0.0f });

		glm_vec3_scale(obj->vloc, deltatime, dtTmp);
		dtTmp[0] *= 5 * !bot->bIsFalled * obj->movSpeed;
		dtTmp[2] *= 5 * !bot->bIsFalled * obj->movSpeed;
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


		if (obj->vloc[1] > 0.1f) { // y velocity > 0.1 -> jumping
			bot->bIsJumping = 1;
		}
		else {
			bot->bIsJumping = 0;
		}

		if (!bot->bOnFloor) { // bot not on the ground
			bot->airTime += deltatime;
			if (bot->airTime > 2.0f && bot->obj->vloc[1] < -0.1f) {
				bot->bIsFalling = 1;
			}
		}
		else { // bot is on the ground
			if (bot->bIsFalling) {
				bot->bIsFalled = 1;
			}
			else if (bot->airTime > 0.0f && !bot->bIsFalling) {
				// just touch down
				bot->touchDownAnimation->start = 1;
				bot->touchDownAnimation->timer = 0.0f;
			}
			bot->airTime = 0;
			bot->bIsFalling = 0;
		}

		char tmp[512];
		sprintf_s(tmp,
			512,
			"on floor: %d, is jumping: %d, is holding: %d, is falling: %d, falled: %d, is collide with block: %d\n" \
			"location: %3.2f %3.2f %3.2f\n" \
			"velocity: %3.2f %3.2f %3.2f\n",
			bot->bOnFloor,
			bot->bIsJumping,
			bot->bIsHolding,
			bot->bIsFalling,
			bot->bIsFalled,
			bot->bCollideWithBlock,
			obj->transform[3][0],
			obj->transform[3][1],
			obj->transform[3][2],
			obj->vloc[0],
			obj->vloc[1],
			obj->vloc[2]
		);
		dbtPushChars(objDebug, tmp);

		// animation
		if (obj->vloc[0] > 0.1f || obj->vloc[2] > 0.1f && bot->bOnFloor && !bot->bIsJumping && !bot->bIsFalled) {
			bot->walkAnimation->start = 1;
			bot->walkAnimation->magn = 1.0f;
			bot->idleAnimation->start = 0;
			bot->touchDownAnimation->start = 0;
			bot->touchDownAnimation->timer = 0.0f;
			bot->bIsKneeDown = 0;
		}
		else if (obj->vloc[0] < -0.1f || obj->vloc[2] < -0.1f && bot->bOnFloor && !bot->bIsJumping && !bot->bIsFalled) {
			bot->walkAnimation->start = -1;
			bot->walkAnimation->magn = 1.0f;
			bot->idleAnimation->start = 0;
			bot->touchDownAnimation->start = 0;
			bot->touchDownAnimation->timer = 0.0f;
			bot->bIsKneeDown = 0;
		}
		// is falling
		else if (bot->airTime > 2.0f && bot->obj->vloc[1] < -0.1f) {
			bot->fallAnimation->start = 1;
			botDrop(bot);
			bot->bIsKneeDown = 0;
		}
		else {
			bot->fallAnimation->start = 0;
			bot->fallAnimation->timer = 0.0f;
			bot->walkAnimation->start = 0;
			bot->walkAnimation->timer = 0.0f;
			if (!bot->bIsJumping) {
				bot->idleAnimation->start = 1;
			}
			if (bot->bIsFalled) {
				bot->idleAnimation->start = 0;
			}
			bot->bIsKneeDown = 0;
		}

		ocClear(bot->pickupSpace);
		bot->bOnFloor = 0;
		bot->bCollideWithBlock = 0;
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
	if (other == NULL || othercs == NULL || self == other) return;
	if (othercs->channel == CC_FLOOR && selfcs->channel == CC_COLLISION) {
		mat4 worldTrans;
		objGetWorldTransform(worldTrans, othercs->obj);
		vec3 scl;
		glm_decompose_scalev(worldTrans, scl);
		float y = worldTrans[3][1] + scl[1] * othercs->height / 2;
		self->transform[3][1] = y;
		self->vloc[1] = 0.0f;

		Robot* bot = cast(self, ROBOT);
		if (bot == NULL) return;
		bot->bOnFloor = 1;
	}
	else if (othercs->channel == CC_COLLISION) {
		// Handle collision with a blocking object
		vec4 sloc, oloc;
		mat4 srot, orot;
		vec3 sscl, oscl;
		glm_decompose(self->transform, sloc, srot, sscl);
		glm_decompose(other->transform, oloc, orot, oscl);

		// Calculate the direction of the collision
		vec3 sub;
		glm_vec3_sub((vec3){sloc[0], sloc[1], sloc[2]}, (vec3){oloc[0], oloc[1], oloc[2]}, sub);
		glm_vec3_normalize(sub);

		// Calculate the penetration depth
		float penetrationDepth = .08f;

		// Move the object back along the collision direction
		vec3 correction;
		glm_vec3_scale(sub, penetrationDepth, correction);
		self->transform[3][0] += correction[0];
		self->transform[3][1] += correction[1];
		self->transform[3][2] += correction[2];

		Robot* bot = cast(self, ROBOT);
		if (bot == NULL) return NULL;
		bot->bCollideWithBlock = 1;
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
	if (side != 0.0f) bot->obj->vrot[1] = side * (bot->bOnFloor ? 1 : 5);
}

void botJump(Robot* bot) {
	static float jmp = 1.0f;
	bot->bIsFalled = 0;
	if (bot->bOnFloor) {
		jmp = 1.0f;
		bot->jumpAnimation->start = 0;
		bot->jumpAnimation->timer = 0.0f;
		bot->walkAnimation->start = 0;
		bot->idleAnimation->start = 0;
		bot->touchDownAnimation->timer = 0.0f;
		bot->touchDownAnimation->start = 0;
	}
	if (bot->obj->vloc[1] >= -0.2) {
		bot->obj->vloc[1] += 5.0f * jmp;
		if (jmp > .9f) {
			bot->jumpAnimation->start = 1;
			bot->walkAnimation->start = 0;
			bot->walkAnimation->timer = 0.0f;
			bot->flipAnimation->start = 0;
			bot->flipAnimation->timer = 0.0f;
			bot->idleAnimation->start = 0;
		}
		else {
			bot->walkAnimation->start = 0;
			bot->walkAnimation->timer = 0.0f;
			bot->jumpAnimation->start = 0;
			bot->jumpAnimation->timer = 0.0f;
			bot->flipAnimation->start = 1;
			bot->flipAnimation->timer = 0.0f;
			bot->idleAnimation->start = 0;

			botDrop(bot);
		}
		jmp *= .8f;
	}
}

void botToggleKneeDown(Robot* bot) {
	bot->bIsKneeDown = !bot->bIsKneeDown;
	if (bot->bIsKneeDown) {
		bot->kneeDownAnimation->start = 1;
		bot->kneeDownAnimation->timer = 0.0f;
	}
	else {
		botJump(bot);
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
		bot->dropAnimation->start = 0;
		bot->dropAnimation->timer = 0.0f;
		bot->bIsHolding = 1;
		objAttachmentTo(bot->obj, minDistObj);
		bot->pickedObj = minDistObj;
		
		mat4 newTrans;
		glm_mat4_identity(newTrans);
		glm_translate(newTrans, (vec3) { 0.0f, 1.0f, 1.0f });
		glm_mat4_copy(newTrans, bot->pickedObj->transform);
		bot->pickupAnimation->start = 1;
		bot->pickupAnimation->timer = 0.0f;
	}
}

void botDrop(Robot* bot) {
	if (bot->pickedObj == NULL) return;
	mat4 worldTrans;
	objGetWorldTransform(worldTrans, bot->pickedObj);
	objDeattachment(bot->obj, bot->pickedObj);
	glm_mat4_copy(worldTrans, bot->pickedObj->transform);
	bot->pickedObj = NULL;
	bot->bIsHolding = 0;
	bot->dropAnimation->start = 1;
	bot->dropAnimation->timer = 0.0f;
}

void botObjEnterPickupSpace(Object* self, CollisionShape* selfcs, Object* other, CollisionShape* othercs) {
	if (self == NULL || other == NULL || othercs == NULL || othercs->channel != CC_PICKUP) return;

	Robot* bot = cast(self, ROBOT);
	if (bot != NULL) {
		ocPushBack(bot->pickupSpace, other);
	}
}

void botIdleAnim(Component* comp, float deltatime) {
	Animation* anim = cast(comp, ANIMATION);
	if (anim == NULL) return;
	if (!anim->start) return;
	Robot* bot = cast(anim->parent, anim->parent_type);
	if (bot == NULL) return;

	BotBody* body = bot->bbody;

	if (anim->timer == 0.0f) {
		glm_mat4_identity(body->obj->transform);
		glm_translate(body->obj->transform, (vec3) { 0.0f, 1.7f, 0.0f });
	}

	if (anim->timer < 1.0f) {
		glm_translate(body->obj->transform, (vec3) { 0.0f, .05f * deltatime, 0.0f });
	}

	if (anim->timer >= 1.0f && anim->timer < 2.0f) {
		glm_translate(body->obj->transform, (vec3) { 0.0f, -.05f * deltatime, 0.0f });
	}

	if (anim->timer > 2.0f) {
		anim->timer = 0.0001f;
	}

	anim->timer += deltatime;
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
	Connector* rightFArm = bot->bbody->rightFArmConnector;
	Connector* rightArm = bot->bbody->rightArmConnector;
	Connector* leftFArm = bot->bbody->leftFArmConnector;
	Connector* leftArm = bot->bbody->leftArmConnector;

	if (anim->timer < 0) {
		anim->timer = 2.0f;
	}

	if (anim->timer == 0.0f) {
		glm_mat4_identity(body->obj->transform);
		glm_translate(body->obj->transform, (vec3) { 0.0f, 1.6f, 0.0f });
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
		glm_rotate(leftKnee->obj->transform, .4f, (vec3) { 1.0f, 0.0f, 0.0f });

		if (!bot->bIsHolding) {
			glm_mat4_identity(rightFArm->obj->transform);
			glm_translate(rightFArm->obj->transform, (vec3) { .5f, .5f, .0f });
			glm_rotate(rightFArm->obj->transform, 1.0f, (vec3) { 1.0f, .0f, .0f });

			glm_mat4_identity(rightArm->obj->transform);
			glm_translate(rightArm->obj->transform, (vec3) { .0f, -.85f, .0f });
			glm_rotate(rightArm->obj->transform, -1.0f, (vec3) { 1.0f, .0f, .0f });

			glm_mat4_identity(leftFArm->obj->transform);
			glm_translate(leftFArm->obj->transform, (vec3) { -.5f, .5f, .0f });
			glm_rotate(leftFArm->obj->transform, -1.0f, (vec3) { 1.0f, .0f, .0f });

			glm_mat4_identity(leftArm->obj->transform);
			glm_translate(leftArm->obj->transform, (vec3) { .0f, -.85f, .0f });
			glm_rotate(leftArm->obj->transform, -1.0f, (vec3) { 1.0f, .0f, .0f });
		}
	}

	if (anim->timer > .0f && anim->timer < .5f / bot->obj->movSpeed) {
		glm_translate(body->obj->transform, (vec3) { 0.0f, .1 * deltatime * 2 * bot->obj->movSpeed, 0.0f });
		glm_rotate(body->obj->transform, -.1f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 0.0f, 0.0f, 1.0f });

		glm_rotate(rightFKnee->obj->transform, 1.0f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(rightKnee->obj->transform, -1.0f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftFKnee->obj->transform, -.5f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftKnee->obj->transform, .4f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, 0.0f, 0.0f });

		if (!bot->bIsHolding) {
			glm_rotate(rightFArm->obj->transform, -1.0f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, .0f, .0f });

			glm_rotate(rightArm->obj->transform, .5f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, .0f, .0f });

			glm_rotate(leftFArm->obj->transform, 1.0f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, .0f, .0f });

			glm_rotate(leftArm->obj->transform, .5f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, .0f, .0f });
		}
	}

	if (anim->timer >= .5f / bot->obj->movSpeed && anim->timer < 1.0f / bot->obj->movSpeed) {
		glm_translate(body->obj->transform, (vec3) { 0.0f, -.1 * deltatime * 2 * bot->obj->movSpeed, 0.0f });
		glm_rotate(body->obj->transform, -.1f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 0.0f, 0.0f, 1.0f });

		glm_rotate(rightFKnee->obj->transform, .5f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(rightKnee->obj->transform, .4f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftFKnee->obj->transform, -1.0f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftKnee->obj->transform, .2f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, 0.0f, 0.0f });

		if (!bot->bIsHolding) {
			glm_rotate(rightFArm->obj->transform, -1.0f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, .0f, .0f });

			glm_rotate(rightArm->obj->transform, .5f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, .0f, .0f });

			glm_rotate(leftFArm->obj->transform, 1.0f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, .0f, .0f });

			glm_rotate(leftArm->obj->transform, .5f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, .0f, .0f });
		}
	}

	if (anim->timer >= 1.0f / bot->obj->movSpeed && anim->timer < 1.5f / bot->obj->movSpeed) {
		glm_translate(body->obj->transform, (vec3) { 0.0f, .1 * deltatime * 2 * bot->obj->movSpeed, 0.0f });
		glm_rotate(body->obj->transform, .1f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 0.0f, 0.0f, 1.0f });

		glm_rotate(rightFKnee->obj->transform, -.5f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(rightKnee->obj->transform, .4f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftFKnee->obj->transform, 1.0f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftKnee->obj->transform, -1.0f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, 0.0f, 0.0f });

		if (!bot->bIsHolding) {
			glm_rotate(rightFArm->obj->transform, 1.0f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, .0f, .0f });

			glm_rotate(rightArm->obj->transform, -.5f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, .0f, .0f });

			glm_rotate(leftFArm->obj->transform, -1.0f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, .0f, .0f });

			glm_rotate(leftArm->obj->transform, -.5f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, .0f, .0f });
		}
	}

	if (anim->timer >= 1.5f / bot->obj->movSpeed && anim->timer < 2.0f / bot->obj->movSpeed) {
		glm_translate(body->obj->transform, (vec3) { 0.0f, -.1 * deltatime * 2 * bot->obj->movSpeed, 0.0f });
		glm_rotate(body->obj->transform, .1f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 0.0f, 0.0f, 1.0f });

		glm_rotate(rightFKnee->obj->transform, -1.0f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(rightKnee->obj->transform, .2f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftFKnee->obj->transform, .5f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftKnee->obj->transform, .4f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, 0.0f, 0.0f });

		if (!bot->bIsHolding) {
			glm_rotate(rightFArm->obj->transform, 1.0f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, .0f, .0f });

			glm_rotate(rightArm->obj->transform, -.5f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, .0f, .0f });

			glm_rotate(leftFArm->obj->transform, -1.0f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, .0f, .0f });

			glm_rotate(leftArm->obj->transform, -.5f * deltatime * 2 * bot->obj->movSpeed, (vec3) { 1.0f, .0f, .0f });
		}
	}

	if (anim->timer >= 2.0f / bot->obj->movSpeed) {
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
	Connector* rightFArm = bot->bbody->rightFArmConnector;
	Connector* rightArm = bot->bbody->rightArmConnector;
	Connector* leftFArm = bot->bbody->leftFArmConnector;
	Connector* leftArm = bot->bbody->leftArmConnector;
	
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

		if (!bot->bIsHolding) {
			glm_mat4_identity(rightFArm->obj->transform);
			glm_translate(rightFArm->obj->transform, (vec3) { .5f, .5f, .0f });

			glm_mat4_identity(rightArm->obj->transform);
			glm_translate(rightArm->obj->transform, (vec3) { .0f, -.85f, .0f });

			glm_mat4_identity(leftFArm->obj->transform);
			glm_translate(leftFArm->obj->transform, (vec3) { -.5f, .5f, .0f });

			glm_mat4_identity(leftArm->obj->transform);
			glm_translate(leftArm->obj->transform, (vec3) { .0f, -.85f, .0f });
		}
	}

	if (anim->timer < .2f) {
		glm_rotate(body->obj->transform, -.2f * deltatime * 5, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(rightFKnee->obj->transform, 1.0f * deltatime * 5, (vec3) { .2f, -.02f, 0.0f });

		glm_rotate(rightKnee->obj->transform, -1.0f * deltatime * 5, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftFKnee->obj->transform, 1.0f * deltatime * 5, (vec3) { .2f, .02f, 0.0f });

		glm_rotate(leftKnee->obj->transform, -1.0f * deltatime * 5, (vec3) { 1.0f, 0.0f, 0.0f });

		if (!bot->bIsHolding) {
			glm_rotate(rightFArm->obj->transform, -1.0f * deltatime * 5, (vec3) { .2f, -.02f, 0.0f });

			glm_rotate(rightArm->obj->transform, -.3f * deltatime * 5, (vec3) { 1.0f, 0.0f, 0.0f });

			glm_rotate(leftFArm->obj->transform, -1.0f * deltatime * 5, (vec3) { .2f, .02f, 0.0f });

			glm_rotate(leftArm->obj->transform, -.3f * deltatime * 5, (vec3) { 1.0f, 0.0f, 0.0f });
		}
	}

	if (anim->timer >= .2f) {
		anim->start = 0;
		anim->timer = 0.0f;
		return;
	}

	anim->timer += deltatime;
}

void botTouchDownAnim(Component* comp, float deltatime) {
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
	Connector* rightFArm = bot->bbody->rightFArmConnector;
	Connector* rightArm = bot->bbody->rightArmConnector;
	Connector* leftFArm = bot->bbody->leftFArmConnector;
	Connector* leftArm = bot->bbody->leftArmConnector;

	if (anim->timer == 0.0f) {
		glm_mat4_identity(body->obj->transform);
		glm_translate(body->obj->transform, (vec3) { 0.0f, 1.7f, 0.0f });

		glm_mat4_identity(rightFKnee->obj->transform);
		glm_translate(rightFKnee->obj->transform, (vec3) { .25f, -.5f, .0f });

		glm_mat4_identity(rightKnee->obj->transform);
		glm_translate(rightKnee->obj->transform, (vec3) { .0f, -.8f, .0f });

		glm_mat4_identity(leftFKnee->obj->transform);
		glm_translate(leftFKnee->obj->transform, (vec3) { -.25f, -.5f, .0f });

		glm_mat4_identity(leftKnee->obj->transform);
		glm_translate(leftKnee->obj->transform, (vec3) { .0f, -.8f, .0f });

		if (!bot->bIsHolding) {
			glm_mat4_identity(rightFArm->obj->transform);
			glm_translate(rightFArm->obj->transform, (vec3) { .5f, .5f, .0f });

			glm_mat4_identity(rightArm->obj->transform);
			glm_translate(rightArm->obj->transform, (vec3) { .0f, -.85f, .0f });

			glm_mat4_identity(leftFArm->obj->transform);
			glm_translate(leftFArm->obj->transform, (vec3) { -.5f, .5f, .0f });

			glm_mat4_identity(leftArm->obj->transform);
			glm_translate(leftArm->obj->transform, (vec3) { .0f, -.85f, .0f });

			glm_rotate(rightFArm->obj->transform, -1.0f, (vec3) { .2f, -.02f, 0.0f });

			glm_rotate(rightArm->obj->transform, -.3f, (vec3) { 1.0f, 0.0f, 0.0f });

			glm_rotate(leftFArm->obj->transform, -1.0f, (vec3) { .2f, .02f, 0.0f });

			glm_rotate(leftArm->obj->transform, -.3f, (vec3) { 1.0f, 0.0f, 0.0f });
		}
	}

	if (anim->timer < 0.1f) {
		glm_translate(body->obj->transform, (vec3) { 0.0f, -.2f * deltatime * 10, 0.0f });
		glm_rotate(rightFKnee->obj->transform, 0.6 * deltatime * 10, (vec3) { -1.0f, 0.0f, 0.0f });
		glm_rotate(leftFKnee->obj->transform, 0.6 * deltatime * 10, (vec3) { -1.0f, 0.0f, 0.0f });
		glm_rotate(rightKnee->obj->transform, 1 * deltatime * 10, (vec3) { 1.0f, 0.0f, 0.0f });
		glm_rotate(leftKnee->obj->transform, 1 * deltatime * 10, (vec3) { 1.0f, 0.0f, 0.0f });
	}

	if (anim->timer >= 0.1f && anim->timer < 0.3f) {
		glm_translate(body->obj->transform, (vec3) { 0.0f, .2f * deltatime * 5, 0.0f });
		glm_rotate(rightFKnee->obj->transform, -0.6 * deltatime * 5, (vec3) { -1.0f, 0.0f, 0.0f });
		glm_rotate(leftFKnee->obj->transform, -0.6 * deltatime * 5, (vec3) { -1.0f, 0.0f, 0.0f });
		glm_rotate(rightKnee->obj->transform, -1 * deltatime * 5, (vec3) { 1.0f, 0.0f, 0.0f });
		glm_rotate(leftKnee->obj->transform, -1 * deltatime * 5, (vec3) { 1.0f, 0.0f, 0.0f });
	}

	if (anim->timer < .2f && !bot->bIsHolding) {
		glm_rotate(rightFArm->obj->transform, 1.0f * deltatime * 5, (vec3) { .2f, -.02f, 0.0f });

		glm_rotate(rightArm->obj->transform, .3f * deltatime * 5, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftFArm->obj->transform, 1.0f * deltatime * 5, (vec3) { .2f, .02f, 0.0f });

		glm_rotate(leftArm->obj->transform, .3f * deltatime * 5, (vec3) { 1.0f, 0.0f, 0.0f });
	}

	if (anim->timer >= 0.3f) {
		anim->start = 0;
		anim->timer = 0.0f;
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
	Connector* rightFArm = bot->bbody->rightFArmConnector;
	Connector* rightArm = bot->bbody->rightArmConnector;
	Connector* leftFArm = bot->bbody->leftFArmConnector;
	Connector* leftArm = bot->bbody->leftArmConnector;

	if (anim->timer == 0.0f) {
		glm_mat4_identity(body->obj->transform);
		glm_translate(body->obj->transform, (vec3) { 0.0f, 1.7f, 0.0f });

		glm_mat4_identity(rightFKnee->obj->transform);
		glm_translate(rightFKnee->obj->transform, (vec3) { .25f, -.5f, .0f });
		glm_rotate(rightFKnee->obj->transform, -1.5f, (vec3) { .2f, -.02f, 0.0f });

		glm_mat4_identity(rightKnee->obj->transform);
		glm_translate(rightKnee->obj->transform, (vec3) { .0f, -.8f, .0f });
		glm_rotate(rightKnee->obj->transform, 1.5f, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_mat4_identity(leftFKnee->obj->transform);
		glm_translate(leftFKnee->obj->transform, (vec3) { -.25f, -.5f, .0f });
		glm_rotate(leftFKnee->obj->transform, -1.5f, (vec3) { .2f, .02f, 0.0f });

		glm_mat4_identity(leftKnee->obj->transform);
		glm_translate(leftKnee->obj->transform, (vec3) { .0f, -.8f, .0f });
		glm_rotate(leftKnee->obj->transform, 1.5f, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_mat4_identity(rightFArm->obj->transform);
		glm_translate(rightFArm->obj->transform, (vec3) { .5f, .5f, .0f });
		glm_rotate(rightFArm->obj->transform, -1.0f, (vec3) { 1.5f, .5f, .0f });

		glm_mat4_identity(rightArm->obj->transform);
		glm_translate(rightArm->obj->transform, (vec3) { .0f, -.85f, .0f });
		glm_rotate(rightArm->obj->transform, 1.0f, (vec3) { .0f, .0f, -1.0f });

		glm_mat4_identity(leftFArm->obj->transform);
		glm_translate(leftFArm->obj->transform, (vec3) { -.5f, .5f, .0f });
		glm_rotate(leftFArm->obj->transform, -1.0f, (vec3) { 1.5f, -.5f, .0f });

		glm_mat4_identity(leftArm->obj->transform);
		glm_translate(leftArm->obj->transform, (vec3) { .0f, -.85f, .0f });
		glm_rotate(leftArm->obj->transform, 1.0f, (vec3) { .0f, .0f, 1.0f });
	}

	if (anim->timer < .25f) {
		glm_rotate(body->obj->transform, M_PI * 2 * deltatime * 4, (vec3) { 1.0f, 0.0f, 0.0f });
	}

	if (anim->timer >= .25f) {
		anim->timer = 0.0f;
		anim->start = 0;
		return;
	}

	anim->timer += deltatime;
}

void botPickupAnim(Component* comp, float deltatime) {
	Animation* anim = cast(comp, ANIMATION);
	if (anim == NULL) return;
	if (!anim->start) return;
	Robot* bot = cast(anim->parent, anim->parent_type);
	if (bot == NULL) return;

	BotBody* body = bot->bbody;
	Connector* rightFArm = bot->bbody->rightFArmConnector;
	Connector* rightArm = bot->bbody->rightArmConnector;
	Connector* leftFArm = bot->bbody->leftFArmConnector;
	Connector* leftArm = bot->bbody->leftArmConnector;

	if (anim->timer == 0.0f) {
		glm_mat4_identity(body->obj->transform);
		glm_translate(body->obj->transform, (vec3) { 0.0f, 1.7f, 0.0f });

		glm_mat4_identity(rightFArm->obj->transform);
		glm_translate(rightFArm->obj->transform, (vec3) { .5f, .5f, .0f });

		glm_mat4_identity(rightArm->obj->transform);
		glm_translate(rightArm->obj->transform, (vec3) { .0f, -.85f, .0f });

		glm_mat4_identity(leftFArm->obj->transform);
		glm_translate(leftFArm->obj->transform, (vec3) { -.5f, .5f, .0f });

		glm_mat4_identity(leftArm->obj->transform);
		glm_translate(leftArm->obj->transform, (vec3) { .0f, -.85f, .0f });

	}

	if (anim->timer < .5f) {
		glm_rotate(rightFArm->obj->transform, -.8f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });
		glm_rotate(leftFArm->obj->transform, -.8f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });
		glm_rotate(rightArm->obj->transform, -.2f * deltatime * 2, (vec3) { 0.0f, 0.0f, 1.0f });
		glm_rotate(leftArm->obj->transform, .2f * deltatime * 2, (vec3) { 0.0f, 0.0f, 1.0f });
	}

	if (anim->timer >= .5f) {
		anim->timer = 0.0f;
		anim->start = 0;
		return;
	}

	anim->timer += deltatime;
}

void botDropAnim(Component* comp, float deltatime) {
	Animation* anim = cast(comp, ANIMATION);
	if (anim == NULL) return;
	if (!anim->start) return;
	Robot* bot = cast(anim->parent, anim->parent_type);
	if (bot == NULL) return;

	BotBody* body = bot->bbody;
	Connector* rightFArm = bot->bbody->rightFArmConnector;
	Connector* rightArm = bot->bbody->rightArmConnector;
	Connector* leftFArm = bot->bbody->leftFArmConnector;
	Connector* leftArm = bot->bbody->leftArmConnector;

	if (anim->timer < .5f) {
		glm_rotate(rightFArm->obj->transform, .8f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });
		glm_rotate(leftFArm->obj->transform, .8f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });
		glm_rotate(rightArm->obj->transform, .2f * deltatime * 2, (vec3) { 0.0f, 0.0f, 1.0f });
		glm_rotate(leftArm->obj->transform, -.2f * deltatime * 2, (vec3) { 0.0f, 0.0f, 1.0f });
	}

	if (anim->timer >= .5f) {
		anim->timer = 0.0f;
		anim->start = 0;
		return;
	}

	anim->timer += deltatime;
}

void botFallAnim(Component* comp, float deltatime) {
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
	Connector* rightFArm = bot->bbody->rightFArmConnector;
	Connector* rightArm = bot->bbody->rightArmConnector;
	Connector* leftFArm = bot->bbody->leftFArmConnector;
	Connector* leftArm = bot->bbody->leftArmConnector;

	if (anim->timer == 0.0f) {
		glm_mat4_identity(body->obj->transform);
		glm_translate(body->obj->transform, (vec3) { 0.0f, .3f, 0.0f });
	}

	glm_rotate(body->obj->transform, deltatime * (rand() % 100) / 5, (vec3) { (float) (rand() % 100) / 100, (float) (rand() % 100) / 100, (float) (rand() % 100) / 100 });
	glm_rotate(rightFKnee->obj->transform, deltatime * (rand() % 100) / 5, (vec3) { (float) (rand() % 100) / 1000, (float) (rand() % 100) / 100, (float) (rand() % 100) / 500 });
	glm_rotate(leftFKnee->obj->transform, deltatime * (rand() % 100) / 5, (vec3) { (float) (rand() % 100) / 1000, (float) (rand() % 100) / 100, (float) (rand() % 100) / 500 });
	glm_rotate(rightKnee->obj->transform, deltatime * (rand() % 100) / 5, (vec3) { (float) (rand() % 100) / 1000, (float) (rand() % 100) / 100, (float) (rand() % 100) / 500 });
	glm_rotate(leftKnee->obj->transform, deltatime * (rand() % 100) / 5, (vec3) { (float) (rand() % 100) / 1000, (float) (rand() % 100) / 100, (float) (rand() % 100) / 500 });
	glm_rotate(rightFArm->obj->transform, deltatime * (rand() % 100) / 5, (vec3) { (float) (rand() % 100) / 1000, (float) (rand() % 100) / 100, (float) (rand() % 100) / 500 });
	glm_rotate(leftFArm->obj->transform, deltatime * (rand() % 100) / 5, (vec3) { (float) (rand() % 100) / 1000, (float) (rand() % 100) / 100, (float) (rand() % 100) / 500 });
	glm_rotate(rightArm->obj->transform, deltatime * (rand() % 100) / 5, (vec3) { (float) (rand() % 100) / 1000, (float) (rand() % 100) / 100, (float) (rand() % 100) / 500 });
	glm_rotate(leftArm->obj->transform, deltatime * (rand() % 100) / 5, (vec3) { (float) (rand() % 100) / 1000, (float) (rand() % 100) / 100, (float) (rand() % 100) / 500 });

	anim->timer += deltatime;
}

void botKneeDownAnim(Component* comp, float deltatime) {
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
	Connector* rightFArm = bot->bbody->rightFArmConnector;
	Connector* rightArm = bot->bbody->rightArmConnector;
	Connector* leftFArm = bot->bbody->leftFArmConnector;
	Connector* leftArm = bot->bbody->leftArmConnector;

	if (anim->timer == 0.0f) {
		glm_mat4_identity(body->obj->transform);
		glm_translate(body->obj->transform, (vec3) { 0.0f, 1.7f, 0.0f });

		glm_mat4_identity(rightFKnee->obj->transform);
		glm_translate(rightFKnee->obj->transform, (vec3) { .25f, -.5f, .0f });

		glm_mat4_identity(rightKnee->obj->transform);
		glm_translate(rightKnee->obj->transform, (vec3) { .0f, -.8f, .0f });

		glm_mat4_identity(leftFKnee->obj->transform);
		glm_translate(leftFKnee->obj->transform, (vec3) { -.25f, -.5f, .0f });

		glm_mat4_identity(leftKnee->obj->transform);
		glm_translate(leftKnee->obj->transform, (vec3) { .0f, -.8f, .0f });

		if (!bot->bIsHolding) {
			glm_mat4_identity(rightFArm->obj->transform);
			glm_translate(rightFArm->obj->transform, (vec3) { .5f, .5f, .0f });

			glm_mat4_identity(rightArm->obj->transform);
			glm_translate(rightArm->obj->transform, (vec3) { .0f, -.85f, .0f });

			glm_mat4_identity(leftFArm->obj->transform);
			glm_translate(leftFArm->obj->transform, (vec3) { -.5f, .5f, .0f });

			glm_mat4_identity(leftArm->obj->transform);
			glm_translate(leftArm->obj->transform, (vec3) { .0f, -.85f, .0f });
		}
	}

	if (anim->timer < 0.5f) {
		glm_translate(body->obj->transform, (vec3) { 0.0f, -.5f * deltatime * 2, 0.0f });
		glm_rotate(body->obj->transform, .5f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(rightFKnee->obj->transform, -1.5f * deltatime * 2, (vec3) { .2f, -.02f, 0.0f });

		glm_rotate(rightKnee->obj->transform, 1.0f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });

		glm_rotate(leftFKnee->obj->transform, -1.5f * deltatime * 2, (vec3) { .2f, .02f, 0.0f });

		glm_rotate(leftKnee->obj->transform, 1.0f * deltatime * 2, (vec3) { 1.0f, 0.0f, 0.0f });
	}

	anim->timer += deltatime;
}
