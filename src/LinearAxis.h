/**
 *  @file LinearAxis.h
 * 
 */

#ifndef LinearAxis_h
#define LinearAxis_h

#include "Arduino.h"
#include "TLE5012b.h"
#include "LinearEncoder.h"
#include "common/base_classes/FOCMotor.h"
#include "common/base_classes/StepperDriver.h"
#include "common/base_classes/Sensor.h"
#include "common/foc_utils.h"
#include "common/time_utils.h"
#include "common/defaults.h"

/**
 Linear axis class
*/
class LinearAxis
{
  public:
    /**
      LinearAxis class constructor
      @param motor added motor
      @param scale added linear encoder
    */
    LinearAxis();



    /** 
      * LinearAxis class object
    */
    TLE5012B* sensor ; //!<declare rotate sensor on stepmotor
    StepperMotor* motor; //!< pointer to motor added to axis
    LinearEncoder* scale; //<! pointer to linear optical scale added to axis
    StepperDriver2PWM* driver; //<! pointer to stepper driver

    /**  LinearAxis init function */
  	void init() ;
    /** LinearAxis disable function */
  	void disable() ;
    /** LinearAxis enable function */
    void enable() ;
 

    /**
     * Function running LinearAxis control algorithm in real-time
     * it calculates the LinearEncoder position and form torgue (voltage ) on stepper motor
     */ 
    void loop();
    
    /**
     * Function calculates movemen on axis
     *   
     * @param target
     * 
     * This function doesn't need to be run upon each loop execution - depends of the use case
     */
    void move(float target = NOT_SET);

  private:
    
 

};


#endif
