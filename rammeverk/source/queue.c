#include "queue.h"

int checkForOrders() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < N_FLOORS; j++) {
			if (queue[i][j] == 1) return MOVE;
		}
	}
	return IDLE;
}

// Check buttons and floor sensors.
void updateOrderQueue() {
	// Check floor panel
	for (int i = 0; i < N_FLOORS; i++) {
		for (int j = BUTTON_CALL_UP; j < BUTTON_COMMAND; j++) {
			// j = (BUTTON_CALL_UP = 0, BUTTON_CALL_DOWN = 1)
			if (elev_get_button_signal(j, i) == 1) {
				queue[j][i] = 1;
			}
		}
	}

	// Check elevator panel
	// If button is pressed, calculate the direction from current floor
	// Update queue matrix
	for (int i = 0; i < N_FLOORS; i++) {
		int cfloor = getCurrentFloor();
		if ( elev_get_button_signal(BUTTON_COMMAND, i) && (cfloor != -1) ) {
			// dir = (down = -1, stop = 0, up = 1)
			int dir = ((i + 1) - cfloor) == 0 ? 0 : (abs((i + 1) - cfloor) / ((i + 1) - cfloor));
			if (dir == -1) queue[0][i] = 1;
			else if (dir == 1) queue[1][i] = 1;
		}
	}
}

void removeFromOrder(int f) {
	int flr = f;
	if (flr < 0 || flr > 3) {
		printf("ERROR removing floor from order!");
		return;
	}
	for (int i = 0; i < 2; i++) {
		queue[i][flr] = 0;
	}
}
