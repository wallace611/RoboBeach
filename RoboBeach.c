#include "Engine.h"
#include "utils/Threading.h"

int main() {
	engineInit();

	thrd_t engineLoop;
	int thrd_state = thrd_create(&engineLoop, engineStartLoop, NULL);
	if (thrd_state != thrd_success) {
		return -1;
	}
	thrd_join(engineLoop, NULL);
}