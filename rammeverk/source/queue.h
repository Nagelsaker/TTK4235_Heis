#include <stdio.h>
#include <stdlib.h>
#include "elev.h"
#include "elevator.h"



// queue is a matrix on the form:
//		| 1	| 2	| 3	| 4	|
//	UP	| x	| x	| x	| x	|
// Down	| x	| x	| x	| x	|
// Elev | x | x | x | x |
//
int queue[3][N_FLOORS];

int checkForOrders();

void updateOrderQueue();
void removeFromOrder(int floor);
