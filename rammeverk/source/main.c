#include "elev.h"
#include "controller.h"
#include "queue.h"
#include "timer.h"
#include <stdio.h>
#include <time.h>

int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");

    lastDirection = DIRN_STOP; // Start direction is 0
    enum state elev_state = INIT; //start state is init

    while (1) {

      //check buttons, floor sensor, check timer
  		updateOrderQueue();
  		if (!hasTimerPassedXs(3) && !elev_get_stop_signal()) {
          elev_state = WAIT;
          } else {
              elev_set_door_open_lamp(0);
              stopTimer();
          }

      updateFloorIndicatorLights();

      //set state to emergencyStop if stop button is pressed
      if (elev_get_stop_signal()){
          elev_set_stop_lamp(1); //L6
          elev_state = EM_STOP;
      }else{
        elev_set_stop_lamp(0);
      }
      switch (elev_state){
          case INIT:
              //moves elevator to 1st floor and switches to IDLE state
              resetQueueAndLights();
              elev_set_motor_direction(DIRN_DOWN);
              if (getCurrentFloor() == 0) {
                  elev_set_motor_direction(DIRN_STOP);
                  elev_state = IDLE;
              }
              break;

          case IDLE:
              elev_state = checkForOrders();
              break;

          case MOVE:
              //set direction from queue
              //handle orders while moving
              elev_state = determineDirectionOrWait();
              break;

          case WAIT:
              elev_set_motor_direction(DIRN_STOP);    //stops elevator
              elev_set_door_open_lamp(1);   //opens doors for 3 seconds
              if (!hasTimerStarted()) {
                  startTimer();
              }
              removeFromOrder(getCurrentFloor());
              elev_state = IDLE;
              break;

          case EM_STOP:
              // lastDirection = DIRN_STOP;
              elev_state = emergencyStop(); //lights stop button, stops elevator, opens doors if on floor.
              break;

        }
    }
    return 0;
}
