#include "queue.h"
#include "elev.h"

int checkForOrders() {
	for (int i = 0; i < N_FLOORS; i++) {
		for (int j = 0; j < 2; j++) {
			if (queue[i][j] == 1) return MOVE;
		}
	}
	return IDLE;
}