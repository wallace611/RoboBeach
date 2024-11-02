#include "utils.h"

#include "object/Object.h"
#include "object/Camera.h"
#include "object/component/Component.h"
#include "object/component/CollisionShape.h"
#include "object/Triangle.h"

int generate_code(void* pt) {
	Object* obj = (Object*) pt;
	int id = obj->id;
	int value = obj->obj_type;
	char combined[256];
	snprintf(combined, sizeof(combined), "%d%d", id, value);

	int total = 0;
	for (size_t i = 0; i < strlen(combined); i++) {
		total += (int) combined[i];
	}

	return total % (1 << 31);
}

int is_valid(void* obj, int code) {
	return generate_code(obj) == code;
}

void* cast(void* pt, int target_type) {
	if (pt == NULL) return NULL;
	if (target_type <= 1024) {
		return obj_casting(pt, target_type);
	}
	else {
		return comp_casting(pt, target_type);
	}
}

void* obj_casting(void* pt, obj_type_t type) {
	Object* obj = (Object*)pt;
	if (!is_valid(obj, obj->check_code)) return NULL;

	void* up = obj->inheritance;
	if (up == NULL) return NULL;

	switch (type) {
	case OBJECT:
		return obj;

	case CAMERA:
		if (((Camera*)up)->obj_type == CAMERA) {
			return up;
		}
		break;

	case TRIANGLE:
		if (((Triangle*)up)->obj_type == TRIANGLE) {
			return up;
		}
		break;
	}
	return NULL;
}

void* comp_casting(void* pt, comp_type_t type) {
	Component* comp = (Component*)pt;
	if (!is_valid(comp, comp->check_code)) return NULL;

	void* up = comp->inheritance;
	if (up == NULL) return NULL;

	switch (type) {
	case COMPONENT:
		return comp;

	case COLLISION:
		if (((CollisionShape*)up)->comp_type == COLLISION) {
			return up;
		}
		break;
	}
	return NULL;
}
