#include "Engine.h"

#include <stdio.h>
#include <time.h>

void engineInit() {
	targetDeltaNanoTime = 1e9 / ENG_DEFAULT_FPS;
	currentFPS = 0.0f;
}

int engineStartLoop() {
	struct timespec tmp;

	int getTimeSucc = timespec_get(&tmp, TIME_UTC);
	if (getTimeSucc == 0) {
		return -1;
	}
	lastNanoSec = tmp.tv_sec * 1e9 + tmp.tv_nsec;

	while (1) {
		long long currentNanoSec = -1;
		while ((currentNanoSec - lastNanoSec) < targetDeltaNanoTime) {

			getTimeSucc = timespec_get(&tmp, TIME_UTC);
			if (getTimeSucc) {
				currentNanoSec = tmp.tv_sec * 1e9 + tmp.tv_nsec;
			}

		}
		// update frame rate info
		currentDeltatime = (currentNanoSec - lastNanoSec) / 1e9;
		currentFPS = 1 / currentDeltatime;
		lastNanoSec = currentNanoSec;
		
		// do everything here
	}

	return 1;
}

void engineSetTargetFPS(unsigned fps) {
	if (fps == 0 || fps > 10000) return;
	targetDeltaNanoTime = 1e9 / fps;
}

float engineGetCurrentFPS() {
	return currentFPS;
}
