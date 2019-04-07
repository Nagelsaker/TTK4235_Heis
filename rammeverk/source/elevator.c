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


  int updateFloorIndicatorLights(){
        //lastFloor has random value the first time this function is called
        if (lastFloor < 0 || lastFloor > 3){
            return 0;
        } else {
          if ((getCurrentFloor() != -1) && (getCurrentFloor() != lastFloor)){
              lastFloor = getCurrentFloor();
              elev_set_floor_indicator(lastFloor);
              return 0;
          }
          if (getCurrentFloor() == -1){
              elev_set_floor_indicator(lastFloor);
              return 0;
          }
          return 0;
        }
    }



int determineDirection() {
    state returnState = EM_STOP;
    int cfloor = getCurrentFloor();
    int isMoreUpOrders = 0;
    int isMoreDownOrders = 0;
    // Find any future up or down orders that we need to know about later on
    for (int flr = 0; flr < N_FLOORS; flr++) {
      for (int butn = 0; butn < 3; butn++) {
        if (queue[butn][flr] == 1 && flr != cfloor) {
          if (butn == BUTTON_CALL_DOWN || (cfloor > flr && butn == BUTTON_COMMAND)) {
            isMoreDownOrders = 1;
          } else if (butn == BUTTON_CALL_UP || (cfloor < flr && butn == BUTTON_COMMAND)) {
            isMoreUpOrders = 1;
          }
        }
      }
    } // end of double for loop
    switch(lastDirection) {
        case DIRN_STOP:
            // Check orders in both directions
            // Can only be in this state if there were no orders previously
            // Check UP- Down- and Command-buttons
            // Determines next state
            returnState = IDLE;
            for (int flr = 0; flr < N_FLOORS; flr++) {
                for (int butn = 0; butn < 3; butn++) {
                  if (queue[butn][flr] == 1) {
                    if (cfloor == flr) {
                      returnState = WAIT;
                    } else if (cfloor == -1) { // After an EM_STOP

                      if (lastFloor < flr) { elev_set_motor_direction(DIRN_UP); }
                      else if (lastFloor > flr) { elev_set_motor_direction(DIRN_DOWN); }
                      else if (lastFloor == flr) { elev_set_motor_direction(DIRN_DOWN); }
                      returnState = MOVE;

                    } else if ( cfloor < flr) {
                      elev_set_motor_direction(DIRN_UP);
                      lastDirection = DIRN_UP;
                      returnState = MOVE;
                    } else if (cfloor > flr) {
                      elev_set_motor_direction(DIRN_DOWN);
                      lastDirection = DIRN_DOWN;
                      returnState = MOVE;
                    }
                  }
                }
              } // end of double for loop
            return returnState;
            break;

        case DIRN_DOWN:
        // Checks if the elevator is at floor nr 2, 3 or 4
        if ((cfloor != -1) && (cfloor != 0)) {
          // Check for down- and command-orders at current floor, and determine next state
          for (int butn = 1; butn < 3; butn++) {
            if ((queue[butn][cfloor] == 1) && isMoreDownOrders) {
              lastDirection = DIRN_DOWN;
              returnState = WAIT;
              return returnState;
            } else if (queue[butn][cfloor] == 1) {
              lastDirection = DIRN_STOP;
              returnState = WAIT;
              return returnState;
            } else if (isMoreDownOrders) {
              lastDirection = DIRN_DOWN;
              returnState = MOVE;
            }
          } // end of for loop
          //if elevator is moving down to where a BUTTON_CALL_UP has been pressed
          if (queue[BUTTON_CALL_UP][cfloor] == 1 && !isMoreDownOrders){
              lastDirection = DIRN_STOP;
              returnState = WAIT;
          } else if (isMoreUpOrders && !isMoreDownOrders) {
            lastDirection = DIRN_DOWN;
            returnState = MOVE;
          }

        } else if (cfloor == -1) {
          lastDirection = DIRN_DOWN;
          returnState = MOVE;
        } else if (cfloor == 0) {
          lastDirection = DIRN_STOP;
          returnState = WAIT;
        }
        if (returnState == MOVE) {
          elev_set_motor_direction(DIRN_DOWN);
        }
        return returnState;
        break;

        case DIRN_UP:
            // Checks if the elevator is at floor nr 1, 2, or 3
            if ((cfloor != -1) && (cfloor != 3)) {
              // Check for up- and command-orders at current floor, and determine next state
              for (int butn = 0; butn < 3; butn += 2) {
                if ((queue[butn][cfloor] == 1) && isMoreUpOrders) {
                  lastDirection = DIRN_UP;
                  returnState = WAIT;
                  return returnState;
                } else if (queue[butn][cfloor] == 1) {
                  lastDirection = DIRN_STOP;
                  returnState = WAIT;
                  return returnState;
                } else if (isMoreUpOrders) {
                  lastDirection = DIRN_UP;
                  returnState = MOVE;
                }
              }
            //if elevator is moving up to where a BUTTON_CALL_DOWN has been pressed
            if (queue[BUTTON_CALL_DOWN][cfloor] == 1 && !isMoreUpOrders){
                lastDirection = DIRN_STOP;
                returnState = WAIT;
            } else if (isMoreDownOrders && !isMoreUpOrders) {
                lastDirection = DIRN_UP;
                returnState = MOVE;
            }

          } else if (cfloor == -1) {
            lastDirection = DIRN_UP;
            returnState = MOVE;
          } else if (cfloor == 3) {
            lastDirection = DIRN_STOP;
            returnState = WAIT;
          }
          if (returnState == MOVE) {
            elev_set_motor_direction(DIRN_UP);
          }
          return returnState;
          break;
    }
    printf("ERROR while moving\n");
    return EM_STOP;
}
