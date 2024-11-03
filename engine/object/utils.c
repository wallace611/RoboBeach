#include "utils.h"

#include "object/Object.h"
#include "object/Camera.h"
#include "object/component/Component.h"
#include "object/component/CollisionShape.h"
#include "object/component/Connector.h"
#include "object/Triangle.h"
#include "object/robot/Robot.h"
#include "object/robot/components/BotBody.h"
#include "object/robot/components/head/BotHead.h"
#include "object/robot/components/arm/BotArm.h"

int generate_checkcode(void* pt) {
	if (pt == NULL) return -1;
	int* tmp = (int*)pt;

	int id, value;

	if (tmp[1] <= 1024) {
		Object* obj = (Object*)pt;
		id = obj->id;
		value = obj->obj_type;
	}
	else {
		Component* comp = (Component*)pt;
		id = comp->id;
		value = comp->comp_type;
	}

	char combined[32];
	snprintf(combined, sizeof(combined), "%d%d", id, value);

	int total = 0;
	for (size_t i = 0; i < strlen(combined); i++) {
		total += (int) combined[i];
	}

	return total % (1 << 31);
}

int is_checkcode_valid(void* obj, int code) {
	return generate_checkcode(obj) == code;
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
	if (!is_checkcode_valid(obj, obj->check_code)) return NULL;

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

	case ROBOT:
		if (((Robot*)up)->obj_type == ROBOT) {
			return up;
		}
		break;
	}
	return NULL;
}

void* comp_casting(void* pt, comp_type_t type) {
	Component* comp = (Component*)pt;
	if (!is_checkcode_valid(comp, comp->check_code)) return NULL;

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

	case CONNECTOR:
		if (((Connector*)up)->comp_type == CONNECTOR) {
			return up;
		}
		break;

	case BOT_BODY:
		if (((BotBody*)up)->comp_type == BOT_BODY) {
			return up;
		}
		break;

	case BOT_HEAD:
		if (((BotHead*)up)->comp_type == BOT_HEAD) {
			return up;
		}
		break;

	case BOT_ARM:
		if (((BotArm*)up)->comp_type == BOT_ARM) {
			return up;
		}
		break;
	}
	return NULL;
}
