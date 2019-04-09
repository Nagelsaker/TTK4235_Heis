#ifndef __ELEVATOR_H__
#define __ELEVATOR_H__

typedef enum state {
    INIT = 0,
    IDLE = 1,
    MOVE = 2,
    WAIT = 3,
    EM_STOP = 4
} state;

struct t_floor {
    int last;
    int next;
};

struct t_floor t_floor;
int lastDirection;

enum state stopState();
enum state downState();
enum state upState();
int updateFloorIndicatorLights();
int getCurrentFloor();
int determineDirection();
void emergencyStop();
#endif
