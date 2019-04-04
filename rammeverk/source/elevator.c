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
            // Check UP- Down- and Command-buttons
            // Determines next state
            for (int flr = 0; flr < N_FLOORS; flr++) {
                for (int butn = 0; butn < 3; butn++) {
                  if (queue[butn][flr] == 1) {
                    if (cfloor == flr) {
                      return WAIT;
                    } else if ( cfloor < flr) {
                      elev_set_motor_direction(DIRN_UP);
                      lastDirection = DIRN_UP;
                      return MOVE;
                    } else if (cfloor > flr) {
                      elev_set_motor_direction(DIRN_DOWN);
                      lastDirection = DIRN_DOWN;
                      return MOVE;
                    }
                  }
                }
              }

              // Check COMMAND buttons
              /*
              for (int flr = 0; flr < N_FLOORS; flr++) {
                if (queue[flr][2] == 1) {
                  if (cfloor == flr) {
                    return WAIT;
                  } else if (cfloor < flr) {
                    elev_set_motor_direction(DIRN_UP);
                    lastDirection = DIRN_UP;
                    return MOVE;
                  } else if (cfloor > flr) {
                    elev_set_motor_direction(DIRN_DOWN);
                    lastDirection = DIRN_DOWN;
                    return MOVE;
                  }
                }
              }*/
            return IDLE;
            break;

        case DIRN_DOWN:
        // Check orders from current floor and down
        cfloor = getCurrentFloor(); // (-1) to 3
        int isMoreOrders = 0;

        // Checks if the elevator is at floor nr 2, 3 or 4
        if ((cfloor != -1) && (cfloor != 0)) {
        // Check for more orders for every floor that is below currentfloor
          for (int flr = cfloor - 1; flr > -1; flr--) {
            for (int butn = 1; butn < 3; butn++) {
              if (queue[butn][flr] == 1) {
                isMoreOrders++;
              }
            }
          }
          // Check for down- and command-orders at current floor, and determine next state
          for (int butn = 1; butn < 3; butn++) {
            if ((queue[butn][cfloor] == 1) && isMoreOrders) {
              return WAIT;
            } else if (queue[butn][cfloor] == 1) {
              lastDirection = DIRN_STOP;
              return WAIT;
            } else if (isMoreOrders) {
              return MOVE;
            }
          }
        } else if (cfloor == -1) {
          return MOVE;
        } else if (cfloor == 0) {
          lastDirection = DIRN_STOP;
          return WAIT;
        }
        break;

        case DIRN_UP:
            // Check orders from current floor and up
            cfloor = getCurrentFloor(); // (-1) to 3
            isMoreOrders = 0;

            // Checks if the elevator is at floor nr 1, 2, or 3
            if ((cfloor != -1) && (cfloor != 3)) {
              // Check for more orders for every floor that is above currentfloor
              for (int flr = cfloor + 1; flr < N_FLOORS; flr++) {
                for (int butn = 0; butn < 3; butn += 2) {
                  if (queue[butn][flr] == 1) {
                    isMoreOrders++;
                  }
                }
              }
              // Check for up- and command-orders at current floor, and determine next state
              for (int butn = 0; butn < 3; butn += 2) {
                if ((queue[butn][cfloor] == 1) && isMoreOrders) {
                  return WAIT;
                } else if (queue[butn][cfloor] == 1) {
                  lastDirection = DIRN_STOP;
                  return WAIT;
                } else if (isMoreOrders) {
                  return MOVE;
                }
              }
            //if elevator is moving up to where a BUTTON_CALL_DOWN has been pressed
            if (queue[BUTTON_CALL_DOWN][cfloor] == 1 && !isMoreOrders){
                lastDirection = DIRN_DOWN;
                return WAIT;
            } else {
              return MOVE;
            }

          } else if (cfloor == -1) {
            return MOVE;
          } else if (cfloor == 3) {
            lastDirection = DIRN_STOP;
            return WAIT;
          }
            break;
    }
    printf("ERROR while moving\n");
    return EM_STOP;
}
