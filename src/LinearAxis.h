/**
 *  @file LinearAxis.h
 * 
 */

#ifndef LinearAxis_h
#define LinearAxis_h

#include "Arduino.h"
#include <EEPROM.h>
#include "StepperMotor.h"
#include "TLE5012b.h"
#include "LinearEncoderHwT.h"
#include "common/base_classes/FOCMotor.h"
#include "common/base_classes/StepperDriver.h"
#include "common/base_classes/Sensor.h"
#include "common/foc_utils.h"
#include "common/time_utils.h"
#include "common/defaults.h"

#define PARAMETERS_ADDRESS 0 //! location of parsmeters in EEPROM 
#define MAX_AXIS_TORQUE 8  //! maximum value of torgue 

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
      //scale direction variable value
    enum ScaleDirection{
              DIRECT    = 1,  //forward direction
              INVERS    = -1, //invers direction
              UNKNOWN = 0     //not yet known or invalid state
          };
    /** 
      * LinearAxis class object
    */
    TLE5012B* sensor ; //!<declare rotate sensor on stepmotor
    StepperMotor* motor; //!< pointer to motor added to axis
    //LinearEncoder* scale; //<! pointer to linear optical scale added to axis
    LinearEncoderHwT* scale; //<! pointer to linear optical scale added to axis
    StepperDriver2PWM* driver; //<! pointer to stepper driver
    
    PIDController  PID_axis{800,30000,0,0,MAX_AXIS_TORQUE}; //<! PID controller of axis position  

    int scale_direction ; //<!scale direction value
    float error = 0 ; //!<error value
    float targ_position = 0 ; //<! target position of axis
    float torque = 0; //torgue value
    
    /**  LinearAxis init function */
  	void init() ;
    /** LinearAxis disable function */
  	void disable() ;
    /** LinearAxis enable function */
    void enable() ;
 

    /**
     * Function running LinearAxis control algorithm in real-time
     * it calculates the LinearEncoder position and form torque (voltage ) on stepper motor
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
      //function for save current movement parameters in EEPROM
 
    int SaveParams();  //<! Save curent movement parameters to EEPROM for axis object return error number
    char LoadParams();  //<! Load saved movement parameters from EEPROM 
    void RecalibrateParams(); //<! recalibrate movement parameters and save to EEPROM
      //EEPROM flag variant fot control of save and restore function
    enum EepromFlag:int8_t{
              VALID = 55, //valid eeprom value
              RECALIBRATE_ASK = 22 //value for recalibrate request 
            };
        
    
  private:
      //set LinearAxis structure 
    struct AxisParameters{
      int scale_dir;
      int motor_dir;
      float zero_angle;
      float ctable[_POLE_PAIRS];
      int8_t control_flag;
    };

};



#endif
