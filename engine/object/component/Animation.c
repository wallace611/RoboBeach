#include "Animation.h"

Animation* newAnimation(Object* parent, obj_type_t parentType) {
	Animation* anim = (Animation*)malloc(sizeof(Animation));
	if (anim == NULL) return NULL;

	anim->obj_type = ANIMATION;

	anim->parent = parent;
	anim->parent_type = parentType;
	anim->timer = 0.0f;
	anim->start = 0;
	anim->magn = 1.0f;

	anim->comp = inheriteComp(anim, ANIMATION);
	if (anim->comp == NULL) return NULL;

	anim->comp->ready = animReady;
	anim->comp->update = animUpdate;

	return anim;
}

void animReady(Component* comp) {
}

void animUpdate(Component* comp, float deltatime) {
}
