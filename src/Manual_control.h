
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef MANUAL_CONTROL_LIB_H
#define MANUAL_CONTROL_LIB_H

#include "Arduino.h"
/*
* Manual control driver

* PIN sct| PIN |   SB42   |   Signal |
*__________________________________________
* PIN 18 |PB0  | SW-K3    | Move +   |
* PIN 19 |PB1  | SW-K4    | MOVE -   | 
* */
#define MANUAL_CONTROL_PERIOD    250000
#define MANUAL_CONTROL_GPIO_PORT     GPIOB
#define MANUAL_MODE_PIN                     GPIO_PIN_10
#define INC_PIN                      GPIO_PIN_1                    
#define DEC_PIN                      GPIO_PIN_0  

class ManualControl{
 public:
    ManualControl();

    int8_t direction; //!< direction of manual control


    /** manual control initialise pins */
    void init(float* _control_value);

    // /**manual control function in loop cycle
    void loop();

 private:
    float* control_value   ; //!< pointer to target value of manual control 
    float cur_increment      ;//!< current increment counter 
    unsigned long timestamp  ;  //!< move up counter
    int upTime ;  //!< move up counter
    int downTime ; //!< move down counter

};


#endif
