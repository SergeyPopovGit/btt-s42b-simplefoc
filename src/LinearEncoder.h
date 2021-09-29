#ifndef LINEARENCODER_LIB_H
#define LINEARENCODER_LIB_H

#include "Arduino.h"

/**
 *  Quadrature mode configuration structure
 */
enum LE_Quadrature{
  QUAD_ON, //!<  Enable quadrature mode CPR = 4xPPR
  QUAD_OFF //!<  Disable quadrature mode / CPR = PPR
};

enum LE_Pullup{
    USE_INTERN_PU, //!< Use internal pullups
    USE_EXTERN_PU //!< Use external pullups
};

class LinearEncoder{
 public:
    /**
    Encoder class constructor
    @param encA  encoder A pin
    @param encB  encoder B pin
    @param _ppmm  impulses per millimeter  
    */

    LinearEncoder(int encA, int encB, long _ppmm);
  

    /** encoder initialise pins */
    void init();
    /**
     *  function enabling hardware interrupts for the encoder channels with provided callback functions
     *  if callback is not provided then the interrupt is not enabled
     * 
     * @param doA pointer to the A channel interrupt handler function
     * @param doB pointer to the B channel interrupt handler function
     * 
     */
    void enableInterrupts(void (*doA)() = nullptr, void(*doB)() = nullptr);
    
    //  Encoder interrupt callback functions
    /** A channel callback function */
    void handleA();
    /** B channel callback function */
    void handleB();    
    
    // pins A and B
    int pinA; //!< encoder hardware pin A
    int pinB; //!< encoder hardware pin B
    long cpmm; //!< counts per millimeter
    // Encoder configuration
    LE_Pullup pullup; //!< Configuration parameter internal or external pullups
    LE_Quadrature quadrature;//!< Configuration parameter enable or disable quadrature mode

    // Abstract functions of the Sensor class implementation
    /** get current angle (rad) */
    float getPosition() ;
    /**  get current angular velocity (rad/s) */
    float getVelocity() ;
    //monitor position
    

 private:
    volatile int32_t pulse_counter;//!< current pulse counter
    //volatile long pulse_timestamp;//!< last impulse timestamp in us
    volatile int A_active; //!< current active states of A channel
    volatile int B_active; //!< current active states of B channel
  
    // velocity calculation variables
    volatile int32_t prev_pulse_counter;
    volatile long prev_timestamp_us;
};


#endif
