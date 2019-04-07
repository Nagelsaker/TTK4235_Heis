#ifndef __ELEVATOR_H__
#define __ELEVATOR_H__

typedef enum state {
    INIT = 0,
    IDLE = 1,
    MOVE = 2,
    WAIT = 3,
    EM_STOP = 4
} state;

void emergencyStop();

int lastDirection;

int lastFloor;

int floorIndicatorLights();

int getCurrentFloor();

int determineDirection();
#endif
