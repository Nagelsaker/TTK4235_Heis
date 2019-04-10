#include "timer.h"
#include <stdio.h>
//clock_t end;
time_t timer = 0;


void startTimer() {

	//end = clock() + CLOCKS_PER_SEC * 3;
	timer = time(NULL);
}

void stopTimer() {
	timer = 0;
}

int hasTimerStarted() {
	if (timer != 0) {
		return 1;
	} else return 0;
}

int hasTimerPassedXs(int x) {
	time_t currentTime = time(NULL);
	long int elapsedTime = (int) (currentTime - timer);
	if (elapsedTime < x) {
		return 0;
	} else return 1;
}
