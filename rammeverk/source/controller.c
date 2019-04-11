#include "elev.h"
#include "controller.h"
#include "queue.h"
#include "timer.h"

state stopState(state returnState, int cfloor, int isMoreUpOrders, int isMoreDownOrders);
state downState(state returnState, int cfloor, int isMoreUpOrders, int isMoreDownOrders);
state upState(state returnState, int cfloor, int isMoreUpOrders, int isMoreDownOrders);


state emergencyStop(){
    state returnState = EM_STOP;
    elev_set_motor_direction(DIRN_STOP);          //stop elevator
    int stopButtonPressed = elev_get_stop_signal();

    if ( elev_get_floor_sensor_signal() != -1 ){    //open doors if on a floor
        elev_set_door_open_lamp(1);
        startTimer();
        if (hasTimerPassedXs(3)){
          elev_set_door_open_lamp(0);
        }
    }
    resetQueueAndLights();
    if (stopButtonPressed != 1) {
        elev_set_stop_lamp(0);  //L6
        returnState = IDLE;
        lastDirection = DIRN_STOP;
    }
    return returnState;
}


  int getCurrentFloor(){
      if ( elev_get_floor_sensor_signal() != -1 ){
          return elev_get_floor_sensor_signal();
      }
	    return -1;
  }


  void updateFloorIndicatorLights(){
        //t_floor.last has random value the first time this function is called
        int cfloor = getCurrentFloor();
        if (!(t_floor.last < -1 || t_floor.last > 3)) {
          if ((cfloor != -1) && (cfloor != t_floor.last)){
              t_floor.last = cfloor;
              elev_set_floor_indicator(t_floor.last);
          } else if (cfloor == -1){
              elev_set_floor_indicator(t_floor.last);
          }

          if (lastDirection != DIRN_STOP) {
              if (!(t_floor.last == 3 && lastDirection == DIRN_UP) && !(t_floor.last == 0 && lastDirection == DIRN_DOWN)) {
                if ((t_floor.last < t_floor.next && lastDirection == DIRN_DOWN)
                  || (t_floor.last > t_floor.next && lastDirection == DIRN_UP)) {
                  int temp = t_floor.last;
                  t_floor.last = t_floor.next;
                  t_floor.next = temp;
                } else {
                  t_floor.next = t_floor.last + lastDirection;
                }
              } else {
                  t_floor.next = t_floor.last;
              }
          }

          return;
        }
        return;
    }



state moveOrWait() {
    // returnState is nominally either MOVE or WAIT,
    // could be EM_STOP if an error occurs
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
            returnState = stopState(returnState, cfloor, isMoreUpOrders, isMoreDownOrders);
            break;

        case DIRN_DOWN:
            returnState = downState(returnState, cfloor, isMoreUpOrders, isMoreDownOrders);
            break;

        case DIRN_UP:
            returnState = upState(returnState, cfloor, isMoreUpOrders, isMoreDownOrders);
          break;
    }
    if (returnState != EM_STOP) {
        return returnState;
    } else {
        printf("ERROR while moving\n");
        return EM_STOP;
  }
}

state stopState(state returnState, int cfloor, int isMoreUpOrders, int isMoreDownOrders) {
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
          } else if (cfloor == -1) {
          // After an EM_STOP, the following state will be triggered
            if (t_floor.next > t_floor.last) {
                if (t_floor.next > flr) {
                    lastDirection = DIRN_DOWN;
                } else if (t_floor.next <= flr) {
                    lastDirection = DIRN_UP;
                }
            } else if (t_floor.next < t_floor.last) {
                if (t_floor.next < flr) {
                    lastDirection = DIRN_UP;
                } else if (t_floor.next >= flr) {
                    lastDirection = DIRN_DOWN;
                }
            } else if (t_floor.next == t_floor.last){
                if (t_floor.next < flr) {
                    lastDirection = DIRN_UP;
                } else if (t_floor.next > flr) {
                    lastDirection = DIRN_DOWN;
                } else if (t_floor.next == flr && flr == 0) {
                  lastDirection = DIRN_DOWN;
                } else if (t_floor.next == flr && flr == 3) {
                  lastDirection = DIRN_UP;
                }
            }
            returnState = MOVE;
          // End of EM_STOP
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
}

state downState(state returnState, int cfloor, int isMoreUpOrders, int isMoreDownOrders) {
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
}

state upState(state returnState, int cfloor, int isMoreUpOrders, int isMoreDownOrders) {
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
}
