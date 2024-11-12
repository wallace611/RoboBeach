#include "RoboBeach.h"
#include "Test.h"

int main(int argc, char** argv) {
	int flag = 1;
	while (flag) {
		int ret = scanf("%d", &flag);

		switch (flag) {
		case 1:
			runTest(&argc, argv);
			break;

		case 0:
			break;

		default:
			runRoboBeach(&argc, argv);
			break;
		}
	}
}