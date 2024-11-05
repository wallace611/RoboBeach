#include "Component.h"

Component* newComponent() {
    Component* obj = (Component*)malloc(sizeof(Component));
    if (obj == NULL) return NULL;

    obj->obj_type = COMPONENT;
    obj->id = comp_next_id++;
    glm_mat4_identity(obj->transform);

    obj->child_list = newObjContainer();

    obj->inheritance = NULL;

    obj->ready = compReadyChild;
    obj->update = compUpdateChild;
    obj->render = compRenderChild;

    obj->check_code = generate_checkcode(obj);

    return obj;
}

Component* inheriteComp(void* self, comp_type_t self_type) {
    if (self == NULL) return NULL;

    Component* obj = newComponent();
    if (obj == NULL) return NULL;

    obj->inheritance = self;
    obj->obj_type = self_type;

    obj->check_code = generate_checkcode(obj);

    return obj;
}

void compReadyChild(Component* obj) {
    for (int i = 0; i < obj->child_list->end; i++) {
        Component* child = cast(obj->child_list->list[i], COMPONENT);
        child->ready(child);
    }
}

void compUpdateChild(Component* obj, float deltatime) {
    for (int i = 0; i < obj->child_list->end; i++) {
        Component* child = cast(obj->child_list->list[i], COMPONENT);
        child->update(child, deltatime);
    }
}

void compRenderChild(Component* obj) {
    for (int i = 0; i < obj->child_list->end; i++) {
        Component* child = cast(obj->child_list->list[i], COMPONENT);
        child->render(child);
    }
}
