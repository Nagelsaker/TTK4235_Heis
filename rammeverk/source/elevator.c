#include "elev.h"
#include "queue.h"
#include "timer.h"



void emergencyStop(){

    elev_set_motor_direction(DIRN_STOP);          //stop elevator

    if ( elev_get_floor_sensor_signal() != -1 ){    //open doors if on a floor
        elev_set_door_open_lamp(1);
        startTimer();
        if (hasTimerPassed3s()){
          elev_set_door_open_lamp(0);
        }
    }

    //reset queue
}


  int getCurrentFloor(){
      if ( elev_get_floor_sensor_signal() != -1 ){
          return elev_get_floor_sensor_signal();
      }
	    return -1;
  }


int determineDirection() {
    int cfloor;
    switch(lastDirection) {
        case DIRN_STOP:
        cfloor = getCurrentFloor();
            // Check orders in both directions
            // Can only be in this state if there were no orders previously
            for (int i = 0; i < N_FLOORS; i++) {
                for (int j = 0; j < 2; j++) {
                    if (queue[j][i] == 1) {
                        lastDirection = (j == 0) ? DIRN_UP : DIRN_DOWN;
                        if ((j == 1) && (i > cfloor)) {
                            elev_set_motor_direction(DIRN_UP);
                        }
                        if (getCurrentFloor() == i) {
                            return WAIT;
                        } else {
                            return MOVE;
                        }
                    }
                }
            }
            break;
        case DIRN_DOWN:
            // Check orders from current floor and down
            cfloor = getCurrentFloor(); // (-1) to 3
            int qfloor = -1;
            if ((cfloor != -1) || (cfloor != 0)) {
                // Check for more orders in the same direction
                int isMoreOrders = 0;
                for (int i = cfloor - 1; i > -1; i--) {
                    if (queue[1][i] == 1) {
                        isMoreOrders++;
                        qfloor = i;
                    }
                }
                // Return correct next elevator state
                if ((queue[1][cfloor] == 1) && isMoreOrders) {
                    return WAIT;
                } else if (queue[0][cfloor] == 1) {
                    lastDirection = DIRN_STOP;
                    return WAIT;
                } else if (isMoreOrders && (cfloor > qfloor)) {
                    elev_set_motor_direction(DIRN_DOWN);
                    return MOVE;
                } else if (isMoreOrders) {
                    return MOVE;
                } else {
                    printf("ERROR while moving downwards\n");
                    return EM_STOP;
                }
            } else if (cfloor == 0) {
                    lastDirection = DIRN_STOP;
                    return WAIT;
            }
            break;
        case DIRN_UP:
            // Check orders from current floor and up
            cfloor = getCurrentFloor(); // (-1) to 3
            qfloor = 4;
            if ((cfloor != -1) && (cfloor != 3)) {
                // Check for more orders in the same direction
                int isMoreOrders = 0;
                for (int i = cfloor + 1; i < N_FLOORS; i++) {
                    if (queue[0][i] == 1) {
                        isMoreOrders++;
                        qfloor = i;
                    }
                }
                // return correct next elevator state
                if ((queue[0][cfloor] == 1) && isMoreOrders) {
                    return WAIT;
                } else if (queue[0][cfloor] == 1) {
                    lastDirection = DIRN_STOP;
                    return WAIT;
                } else if (isMoreOrders && (cfloor < qfloor)) {
                    elev_set_motor_direction(DIRN_UP);
                    return MOVE;
                } else if (isMoreOrders) {

                } else {
                    printf("ERROR while moving upwards\n");
                    return EM_STOP;
                }
            } else if (cfloor == 3) {
                lastDirection = DIRN_STOP;
                return WAIT;
            }
            break;
    }
    printf("ERROR while moving\n");
    return EM_STOP;
}
