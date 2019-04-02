#ifndef __ELEVATOR_H__
#define __ELEVATOR_H__

void emergencyStop();

int lastDirection = 0;

int getCurrentFloor();

enum State handleOrders();
#endif
