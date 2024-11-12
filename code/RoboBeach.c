#include "RoboBeach.h"

#include "Engine.h"

int runRoboBeach(int* argc, char** argv) {
	engineInit(argc, argv);

	setupInputMapping();

	engineStartLoop();

	return 0;
}

void setupInputMapping() {
}
