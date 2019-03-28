#include "elev.h"
#include "timer.h"



void emergencyStop(){
    while ( elev_get_stop_signal() ){   //lights stop button
        elev_set_stop_lamp(1);
    }

    elev_motor_direction_t(0);          //stop elevator

    if ( elev_get_floor_sensor_signal() != -1 ){    //open doors if on a floor
        elev_set_door_open_lamp(1);
        timer();
        elev_set_door_open_lamp(0);
    }

    //reset queue
}


  void updateCurrentFloor(){
      if ( elev_get_floor_sensor_signal() != -1 ){
          currentFloor = elev_get_floor_sensor_signal;
      }
  }
