#include "queue.h"
#include "elev.h"

int checkForOrders() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < N_FLOORS; j++) {
			if (queue[j][i] == 1) return MOVE;
		}
	}
	return IDLE;
}

// Check buttons and floor sensors.
void updateOrderQueue() {
	// Check floor panel

	// Check elevator panel
	// If button pressed, calculate 
}