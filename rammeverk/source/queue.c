#include "queue.h"

int checkForOrders() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < N_FLOORS; j++) {
			if (queue[i][j] == 1) return MOVE;
		}
	}
	return IDLE;
}

// Check buttons sensors.
void updateOrderQueue() {
	// Check floor panel
	for (int flr = 0; flr < N_FLOORS; flr++) {
		for (int butn = 0; butn < 3; butn++) {
			// j = (BUTTON_CALL_UP = 0, BUTTON_CALL_DOWN = 1)
			if (elev_get_button_signal(butn, flr) == 1) {
				queue[butn][flr] = 1;
				elev_set_button_lamp(butn,flr,1);
			}
		}
	}
}

void removeFromOrder(int f) {
	int flr = f;
	if (flr < 0 || flr > 3) {
		printf("ERROR removing floor from order!");
		return;
	}
	for (int butn = 0; butn < 3; butn++) {
		queue[butn][flr] = 0;
		elev_set_button_lamp(butn,flr,0);
	}
}

void resetQueueAndLights() {
	for (int flr = 0; flr < N_FLOORS; flr++) {
		for (int butn = 0; butn < 3; butn++) {
			queue[butn][flr] = 0;
			elev_set_button_lamp(butn,flr,0);
		}
	}
}
