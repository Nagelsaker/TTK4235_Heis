#ifndef
enum State = [INIT, IDLE, MOVE, WAIT, EM_STOP];
#endif

// queue is a matrix on the form:
//		| 1	| 2	| 3	| 4	|
//	UP	| x	| x	| x	| x	|
// Down	| x	| x	| x	| x	|
//
int[N_FLOORS][2] queue;

int checkForOrders();

void updateOrderQueue();