#include "Component.h"

Component* newComponent() {
    Component* comp = (Component*)malloc(sizeof(Component));
    if (comp == NULL) return NULL;

    comp->comp_type = COMPONENT;
    comp->id = comp_next_id++;
    glm_mat4_identity(comp->transform);

    comp->child_list = newObjContainer();

    comp->inheritance = NULL;

    comp->ready = compReady;
    comp->update = compUpdate;
    comp->render = compRender;

    comp->check_code = generate_code(comp);

    return comp;
}

Component* inheriteComp(void* self, comp_type_t self_type) {
    if (self == NULL) return NULL;

    Component* comp = newComponent();
    if (comp == NULL) return NULL;

    comp->inheritance = self;
    comp->comp_type = self_type;

    return comp;
}

void compReady(Component* comp) {
}

void compUpdate(Component* comp, float deltatime) {
}

void compRender(Component* comp) {
    for (int i = 0; i < comp->child_list->end; i++) {
        Component* child = cast(comp->child_list->list[i], COMPONENT);
        child->render(child);
    }
}
