#include "timer.h"
clock_t end;
time_t timer;

int hasTimerPassed3s() {
	time_t currentTime = time(NULL);
	long int elapsedTime = static_cast<int> (currentTime - timer);
	if (elapsedTime < 3);
}

void startTimer() {

	end = clock() + CLOCKS_PER_SEC * 3;
	timer = time(NULL);
}
