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

    elev_set_motor_direction(DIRN_UP);


    enum State elev_state = INIT; //start state is init

    while (1) {
        // Change direction when we reach top/bottom floor
        if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
            elev_set_motor_direction(DIRN_DOWN);
        } else if (elev_get_floor_sensor_signal() == 0) {
            elev_set_motor_direction(DIRN_UP);
        }

        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }


        //set state to emergencyStop if stop button is pressed
        if ( elev_get_stop_signal() ==1 ){
            elev_state = EM_STOP;
            elev_set_motor_direction(0);
        }

        //check buttons, floor sensor, check timer
		updateOrderQueue();
		if (getTimer() < 3) { elev_state = WAIT; }
		else elev_set_door_open_lamp(0);

        switch (elev_state){
            case INIT:
                //moves elevator to 1st floor and switches to IDLE state
                elev_set_motor_direction_t(-1);
                if (getCurrentFloor() == 0) {
                    elev_state = IDLE;
                }
                //what else needs initializing?


            case IDLE:
            //finnes ordre?
            //gjør noe med det
            //bytt state
				elev_state = checkForOrders();
				break;

            case MOVE:
                //elev_motor_direction_t(?);      //set direction from queue
                //handle orders while moving
				break;

            case WAIT:
                elev_set_motor_direction_t(0);    //stops elevator
				elev_set_door_open_lamp(1);   //opens doors for 3 seconds
				startTimer();
				break;

            case EM_STOP:
                emergencyStop(); //lights stop button, stops elevator, opens doors if on floor.
                //needs to be implemented: reset buttons and queue
				break;

        }
    }

    return 0;
}
