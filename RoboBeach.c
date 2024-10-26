#include "Engine.h"

#include "utils/ObjContainer.h"

#include <stdio.h>

#include <cglm/cglm.h>

int main(int argc, char** argv) {
	engineInit(&argc, argv);
	engineStartLoop();

}