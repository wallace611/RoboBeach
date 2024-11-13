#pragma once

#include "object/Camera.h"
#include "object/Object.h"
#include "object/CollisionShape.h"
#include "object/robot/components/BotBody.h"
#include "object/component/Animation.h"

typedef struct _bot Robot;

struct _bot {
	obj_type_t obj_type;

	unsigned char bOnFloor;
	unsigned char bIsJumping;
	unsigned char bIsHolding;
	unsigned char bIsFalling;
	unsigned char bIsFalled;
	unsigned char bCollideWithBlock;
	unsigned char bIsKneeDown;
	unsigned char bIsSwimming;
	float airTime;
	Camera* cam;

	BotBody* bbody;

	Object* pickedObj;
	CollisionShape* bodyCollision;
	CollisionShape* pickCollision;

	ObjContainer* pickupSpace;

	Animation* idleAnimation;
	Animation* walkAnimation;
	Animation* swimAnimation;
	Animation* jumpAnimation;
	Animation* touchDownAnimation;
	Animation* flipAnimation;
	Animation* pickupAnimation;
	Animation* dropAnimation;
	Animation* fallAnimation;
	Animation* kneeDownAnimation;

	Object* obj;
};

Robot* newRobot();

static void botReady(Object* obj);
static void botUpdate(Object* obj, float deltatime);
static void botRender(Object* obj);
void botCollided(Object* self, CollisionShape* selfcs, Object* other, CollisionShape* othercs);
void botCamUpdate(Object* obj, float deltatime);
void botMoving(Robot* bot, float forward, float side);
void botJump(Robot* bot);
void botToggleKneeDown(Robot* bot);
void botPickup(Robot* bot);
void botDrop(Robot* bot);
void botThrow(Robot* bot);
void botObjEnterPickupSpace(Object* self, CollisionShape* selfcs, Object* other, CollisionShape* othercs);

void botIdleAnim(Component* comp, float deltatime);
void botWalkingAnim(Component* comp, float deltatime);
void botSwimingAnim(Component* comp, float deltatime);
void botJumpingAnim(Component* comp, float deltatime);
void botTouchDownAnim(Component* comp, float deltatime);
void botFlipingAnim(Component* comp, float deltatime);
void botPickupAnim(Component* comp, float deltatime);
void botDropAnim(Component* comp, float deltatime);
void botFallAnim(Component* comp, float deltatime);
void botKneeDownAnim(Component* comp, float deltatime);