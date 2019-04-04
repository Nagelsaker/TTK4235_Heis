#include "elev.h"
#include "elevator.h"
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

    // elev_set_motor_direction(DIRN_UP);
    lastDirection = 0;


    enum state elev_state = INIT; //start state is init

    while (1) {
        // Change direction when we reach top/bottom floor
        /*
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
        } */

        //check buttons, floor sensor, check timer
    		updateOrderQueue();
    		if (!hasTimerPassed3s() && !elev_get_stop_signal()) {
            elev_state = WAIT;
        } else {
            elev_set_door_open_lamp(0);
            stopTimer();
        }

        //set state to emergencyStop if stop button is pressed
        if (elev_get_stop_signal()){
            elev_set_stop_lamp(1); //L6
            elev_state = EM_STOP;
        }

        switch (elev_state){
            case INIT:
                //moves elevator to 1st floor and switches to IDLE state
                //printf("INIT\n");
                elev_set_motor_direction(DIRN_DOWN);
                if (getCurrentFloor() == 0) {
                    elev_set_motor_direction(DIRN_STOP);
                    elev_state = IDLE;
                }

                break;

            case IDLE:
              //printf("IDLE\n");
              //finnes ordre?
              //gjør noe med det
              //bytt state
      				elev_state = checkForOrders();
      				break;

            case MOVE:
                //printf("MOVE\n");
                //elev_motor_direction_t(?);      //set direction from queue
                //handle orders while moving
                elev_state = determineDirection();
				    break;

            case WAIT:
                //printf("WAIT\n");
                elev_set_motor_direction(DIRN_STOP);    //stops elevator
        				elev_set_door_open_lamp(1);   //opens doors for 3 seconds
        				if (!hasTimerStarted()) {
                   startTimer();
                 }
                removeFromOrder(getCurrentFloor());
                elev_state = IDLE;
		            break;

            case EM_STOP:
                //printf("EM_STOP\n");
                emergencyStop(); //lights stop button, stops elevator, opens doors if on floor.
                //needs to be implemented: reset buttons and queue
                if (elev_get_stop_signal() != 1) {
                    elev_set_stop_lamp(0);  //L6
                    elev_state = IDLE;
                }
                break;

        }
    }

    return 0;
}
