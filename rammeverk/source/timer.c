#include "timer.h"
#include <stdio.h>
clock_t end;
time_t timer;

int hasTimerPassed3s() {
	time_t currentTime = time(NULL);
	long int elapsedTime = (int) (currentTime - timer);
	if (elapsedTime < 3) {
		return 0;
	} else return 1;
}

void startTimer() {

	end = clock() + CLOCKS_PER_SEC * 3;
	timer = time(NULL);
}
