#include "timer.h"

time_t getTimer() {
	return (timer - time(NULL));
}

void startTimer() {
	timer = time(NULL);
}