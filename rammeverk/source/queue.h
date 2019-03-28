#ifndef
enum State = [INIT, IDLE, MOVE, WAIT, EM_STOP];
#endif

int[N_FLOORS][2] queue;

int checkForOrders();