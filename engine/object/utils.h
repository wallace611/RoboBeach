#pragma once

typedef int class_type_t;
typedef int obj_type_t;
typedef int comp_type_t;

// Registered object type 0 < x <= 1024
#define OBJECT    1
#define CAMERA	  2
#define TRIANGLE  3
#define ROBOT	  4
#define COLLISION 5
#define FLOOR	  6

// Registered component type > 1024
#define COMPONENT 1025
#define CONNECTOR 1027
#define BOT_BODY  1028
#define BOT_HEAD  1029
#define BOT_ARM	  1030

int generate_checkcode(void* obj);
int is_checkcode_valid(void* obj, int code);
void* cast(void* pt, int type);

static void* obj_casting(void* pt, obj_type_t type);
static void* comp_casting(void* pt, comp_type_t type);