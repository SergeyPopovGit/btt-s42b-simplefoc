
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef MANUAL_CONTROL_LIB_H
#define MANUAL_CONTROL_LIB_H

#include "Arduino.h"
#include "LinearAxis.h"
#include "config.h"
/*
* Manual control driver

* PIN sct| PIN |   SB42   |   Signal |
*__________________________________________
* PIN 18 |PB0  | SW-K3    | Move +   |
* PIN 19 |PB1  | SW-K4    | MOVE -   | 
* */
   //Button PARAMETERS
#define MANUAL_CONTROL_PERIOD    100000   //period of button state request in microseconds
#define TIME_TO_SECOND_SPEED     3000000/MANUAL_CONTROL_PERIOD       //time to switch to second speed in microseconds
#define TIME_TO_THIRD_SPEED      6000000/MANUAL_CONTROL_PERIOD      //time to switch to third speed in microsecons
#define FIRS_SPEED               0.01
#define SECOND_SPEED             0.1
#define THIRD_SPEED              1
#define MANUAL_CONTROL_GPIO_PORT     GPIOB
#define INC_PIN                      GPIO_PIN_1                    
#define DEC_PIN                      GPIO_PIN_0 
#define MODE_CONTROL_GPIO_PORT       GPIOA
#define MODE_PIN                     GPIO_PIN_3  
/*
* HW onboard switches
** PIN sct| PIN |   SB42      |   Signal     |
*__________________________________________
* PIN 21 |PB10  | MICROSTEP_1 | EXT_ENCODER |
* PIN 22 |PB11  | MICROSTEP_2 |    |
* PIN 39 |PB3   | CLOSED_LOOP |    |
* PIN 38 |PA15  | CALIB_SW    |    |

   */
#define EXTERNAL_ENCODER_MODE_PIN 



class ManualControl{
 public:
    ManualControl();

    int8_t direction; //!< direction of manual control


    /* manual control initialise function */
    void init(LinearAxis* _control_value);

    // /**manual control function in loop cycle
    void loop();

    //

 private:
    LinearAxis* axis   ; //!< pointer to target object  
    float cur_increment      ;//!< current increment counter 
    unsigned long timestamp  ;  //!< move up counter
    int upTime ;  //!< move up counter
    int downTime ; //!< move down counter
        //start Manual control function
    void start();
    //stop manual control function
    void stop();
    //update value function by manual control
    void update(float _value);

};


#endif
