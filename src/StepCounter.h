
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef STEPCOUNTER_LIB_H
#define STEPCOUNTER_LIB_H

#include "Arduino.h"
#include "config.h"


#define STEP_COUNTER_TIM                            TIM2  

class StepCounter{
 public:
    /**
    StepCounter class constructor
    object for counting pulse on STEP_PIN 
    depends of DIRECTION_PIN calculation up or down
    @param _ppmm  impulses per millimeter  
    */

    StepCounter(int _ppmm);//set up pulse per millimeter value

    float direction; //!< direction of scale
    float cur_position  ; //!<curent position
    /** encoder initialise pins */
    void init();

    // /**encoder overload prevent function in loop cycle
    void loop();
    // Abstract functions of the Sensor class implementation
    /** get current angle (rad) */
    float getPosition() ;
    /**  get current angular velocity (rad/s) */
    //float getVelocity() ;
    //monitor position
 private:
    volatile int32_t pulse_counter;//!< current pulse counter  
    // velocity calculation variables
    volatile int32_t prev_counter;  //previos value of encoder
    int ppmm   ;  //pulse per millimeter value
};


#endif
