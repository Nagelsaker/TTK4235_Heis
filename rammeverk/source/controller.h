/**
* @file
* @brief Controls the elevators behaviour
*/
#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

/**
* @brief Contains all the states the FSM can have.
*/
typedef enum state {
    INIT = 0,
    IDLE = 1,
    MOVE = 2,
    WAIT = 3,
    EM_STOP = 4
} state;

/**
* @brief Contains the last floor and the next floor the elevator will visit.
*/
struct t_floor {
    int last;
    int next;
};


struct t_floor t_floor;
int lastDirection;

/**
* @brief Stops the elevator for as long as STOP button is pressed,
* and an additional 3 seconds after release.
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
* @brief Updates floor indicator lights,
* as well as t_floor.last and t_floor.next
*/
void updateFloorIndicatorLights();

/**
* @brief Determines which direction the elevator needs to travel or if the state should switch to @c WAIT.
*
* @return @c MOVE when there is a pending order. Return @c WAIT when
* the elevator has reached a floor with an active order in the same direction as the elevator.
*
* @warning can also return @c EM_STOP if some unforeseen error occurs.
*/
state determineDirectionOrWait();
#endif
