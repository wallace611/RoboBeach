#include "Engine.h"

#include <stdio.h>
#include <time.h>

void engineInit() {
	targetDeltaNanoTime = (long long) 1e9 / ENG_DEFAULT_FPS;
	currentFPS = 0.0f;
}

int engineStartLoop(void *thr_id) {
	struct timespec tmp;

	int get_time_state = timespec_get(&tmp, TIME_UTC);
	if (get_time_state == 0) {
		return -1;
	}
	lastNanoSec = (long long) tmp.tv_sec * 1e9 + tmp.tv_nsec;

	while (1) {
		long long currentNanoSec = -1;
		while ((currentNanoSec - lastNanoSec) < targetDeltaNanoTime) {

			get_time_state = timespec_get(&tmp, TIME_UTC);
			if (get_time_state) {
				currentNanoSec = (long long) tmp.tv_sec * 1e9 + tmp.tv_nsec;
			}

		}
		// update frame rate info
		currentDeltatime = (float) (currentNanoSec - lastNanoSec) / 1e9;
		currentFPS = 1 / currentDeltatime;
		lastNanoSec = currentNanoSec;
		
		// do everything here
		printf("%f\n", engineGetCurrentFPS());
	}

	return 1;
}

void engineSetTargetFPS(unsigned fps) {
	if (fps == 0 || fps > 10000) return;
	targetDeltaNanoTime = (long long) 1e9 / fps;
}

float engineGetCurrentFPS() {
	return currentFPS;
}
