/**
* @file
* @brief Controls the elevator's movement behaviour for all the elevator states
*and emergency stop behaviour.
*/
#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

/**
* @brief Contains all the states the elevator can have.
*/
typedef enum state {
    INIT = 0,
    IDLE = 1,
    MOVE = 2,
    WAIT = 3,
    EM_STOP = 4
} state;

/**
* @brief Contains the last floor the elevator hit and the next floor the elevator will visit.
*/
struct t_floor {
    int last;
    int next;
};


struct t_floor t_floor;

/**
* @brief Contains the last direction the elevator moved.
* Possible values include [-1, 0, 1]
*/
int lastDirection;

/**
* @brief Elevator stops, queue and lights are reset and doors open for 3 seconds if the elevator is on a floor.
*
* @return @c EM_STOP if STOP button is pressed down,
* or if there's less than 3 seconds since the last STOP button push.
* Else return @c IDLE
*
* @warning When IDLE is returned, lastDirection = DIRN_STOP
*/
state emergencyStop();

/**
* @brief Returns the elevators current floor
*
* @return -1 when not at any floor, otherwise return values in range [0, 3]
*/
int getCurrentFloor();

/**
* @brief When the elevator stops at a floor or moves past a floor, the corresponding
*floor indicator light is lit. The floor indicator light corresponding to the
*the last floor the elavtor hit is lit until the elevator hits another floor.
*
*/
void updateFloorIndicatorLights();

/**
* @brief Determines if the elevator state should switch to @c WAIT or @c MOVE.
*if the state is switched to @c MOVE, movement direction is also determined.
*
* @return @c MOVE when there is a pending order. Return @c WAIT when
* the elevator has reached a floor with an active order in the same direction as the elevator.
*
* @warning can also return @c EM_STOP if some unforeseen error occurs.
*/
state moveOrWait();
#endif
