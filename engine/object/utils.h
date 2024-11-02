#pragma once

typedef int class_type_t;
typedef int obj_type_t;
typedef int comp_type_t;

// Registered object type 0 < x <= 1024
#define OBJECT    1
#define CAMERA	  2
#define TRIANGLE  3
#define ROBOT	  4

// Registered component type > 1024
#define COMPONENT 1025
#define COLLISION 1026
#define BOT_BODY  1027

int generate_code(void* obj);
int is_valid(void* obj, int code);
void* cast(void* pt, int type);

static void* obj_casting(void* pt, obj_type_t type);
static void* comp_casting(void* pt, comp_type_t type);