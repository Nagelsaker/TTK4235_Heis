#include <stdio.h>
#include <stdlib.h>
#include "elev.h"
#include "elevator.h"
typedef enum State { INIT, IDLE, MOVE, WAIT, EM_STOP } State;

int a = IDLE;

// queue is a matrix on the form:
//		| 1	| 2	| 3	| 4	|
//	UP	| x	| x	| x	| x	|
// Down	| x	| x	| x	| x	|
//
int queue[2][N_FLOORS];

int checkForOrders();

void updateOrderQueue();
void removeFromOrder(int floor);
