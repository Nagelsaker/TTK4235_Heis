#include "elev.h"
#include <stdio.h>
#include "elevator.h"


int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");

    elev_set_motor_direction(DIRN_UP);



    enum State [INIT, IDLE, MOVE, WAIT, EM_STOP];
    State elev_state = INIT; //start state is init

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
        }

        //check button, floor sensor, check timer
        updateOrderQueue(pressedButton)

        switch (state){
            case INIT:
                //moves elevator to 1st floor and switches to IDLE state
                elev_motor_direction_t(-1);
                updateCurrentFloor();
                if currentFloor == 0{
                    elev_state = IDLE;
                }
                //what else needs initializing?


            case IDLE:
            //finnes ordre?
            //gjør noe med det
            //bytt state

            case MOVE:
                //elev_motor_direction_t(?);      //set direction from queue

                updateCurrentFloor();
                if currentFloor == TargetFloor{   //targetFloor from queue
                    elev_state = WAIT;
                }

                //handle orders while moving

            case WAIT:
                elev_motor_direction_t(0);    //stops elevator
                elev_set_door_open_lamp(1);   //opens doors for 3 seconds
                timer();
                elev_set_door_open_lamp(0);

                elev_state = IDLE;            //switch to IDLE, handles next order




            case EM_STOP:
                emergencyStop(); //lights stop button, stops elevator, opens doors if on floor.
                //needs to be implemented: reset buttons and queue

        }
    }

    return 0;
}
