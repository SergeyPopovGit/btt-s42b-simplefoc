
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef LINEARENCODERHWT_LIB_H
#define LINEARENCODERHWT_LIB_H

#include "Arduino.h"

/*
  LinearEncoder(int encA, int encB , int cpmm, int index)
  - encA, encB    - encoder A and B pins
  - cpr           - counts per millimeter (cpm=ppm*4)
*/
/*
*Create hardware encoder  added to TIM1 timer
* PIN sct| PIN |   SB42   |   Timer chanel |
*__________________________________________
* PIN 29 |PA8  | OLED RST | - TIM1_CH1     |
* PIN 30 |PA9  | TX1      | - TIM1_CH2     | 
* */


#define ENCODER_TIM                            TIM1  
#define ENCODER_GPIO_PORT                      GPIOA
#define ENCODER_GPIO_CH1                       GPIO_PIN_8 // pin number of A phase incremental linear encoder
#define ENCODER_GPIO_CH2                       GPIO_PIN_9 // pin number of B phase incremental linear encoder

class LinearEncoderHwT{
 public:
    /**
    Encoder class constructor

    @param _ppmm  impulses per millimeter  
    */

    LinearEncoderHwT();

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
    volatile int32_t prev_encoder;  //previos value of encoder
    volatile long prev_timestamp_us;   // previos megement of counter
};


#endif
