#include "LinearEncoder.h"


/*
  LinearEncoder(int encA, int encB , int cpmm, int index)
  - encA, encB    - encoder A and B pins
  - cpr           - counts per millimeter (cpm=ppm*4)
*/

LinearEncoder::LinearEncoder(int _encA, int _encB , long _ppmm){

  // Encoder measurement structure init
  // hardware pins
  pinA = _encA;
  pinB = _encB;
  // counter setup
  pulse_counter = 0;

  cpmm = _ppmm;
  A_active = 0;
  B_active = 0;


  // velocity calculation variables
  prev_pulse_counter = 0;
  prev_timestamp_us = micros();

  // extern pullup as default
  pullup = LE_Pullup::USE_INTERN_PU;
  // enable quadrature encoder by default
  quadrature = LE_Quadrature::QUAD_OFF;
}

//  Encoder interrupt callback functions
// A channel
void LinearEncoder::handleA() {
  bool A = digitalRead(pinA);
  switch (quadrature){
    case LE_Quadrature::QUAD_ON:
      // CPR = 4xPPR
      if ( A != A_active ) {
        pulse_counter += (A_active == B_active) ? 1 : -1;
        A_active = A;
      }
      break;
    case LE_Quadrature::QUAD_OFF:
      // CPR = PPR
      if(A && !digitalRead(pinB)){
        pulse_counter++;
      }
      break;
  }
}
// B channel
void LinearEncoder::handleB() {
  bool B = digitalRead(pinB);
  switch (quadrature){
    case LE_Quadrature::QUAD_ON:
  //     // CPR = 4xPPR
      if ( B != B_active ) {
        pulse_counter += (A_active != B_active) ? 1 : -1;
        B_active = B;
      }
      break;
    case LE_Quadrature::QUAD_OFF:
      // CPR = PPR
      if(B && !digitalRead(pinA)){
        pulse_counter--;
      }
      break;
  }
}

/*
	Shaft angle calculation
*/
float LinearEncoder::getPosition(){
  return  (pulse_counter) / ((float)cpmm);
}
/*
  Shaft velocity calculation
  function using mixed time and frequency measurement technique
*/
float LinearEncoder::getVelocity(){
  // timestamp
  long timestamp_us = micros();
  // sampling time calculation
  float Ts = (timestamp_us - prev_timestamp_us) * 1e-6;
  // save variables for next pass
  prev_timestamp_us = timestamp_us;  
  // quick fix for strange cases (micros overflow)
  if(Ts <= 0 || Ts > 0.5) Ts = 1e-3;

  // count from last velocity calculation
  long dN = pulse_counter - prev_pulse_counter;
   prev_pulse_counter = pulse_counter;

  // velocity calculation
  return  (float) cpmm* dN / Ts ; 
}

// encoder initialisation of the hardware pins
// and calculation variables
void LinearEncoder::init(){

  // Encoder - check if pullup needed for your encoder
  if(pullup == LE_Pullup::USE_INTERN_PU){
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
  }else{
    pinMode(pinA, INPUT);
    pinMode(pinB, INPUT);
  }

  // counter setup
  pulse_counter = 0;
  // velocity calculation variables
  prev_pulse_counter = 0;
  prev_timestamp_us = micros();

  // initial cpr = PPR
  // change it if the mode is quadrature
  if(quadrature == LE_Quadrature::QUAD_ON) cpmm = 4*cpmm;

}

// function enabling hardware interrupts of the for the callback provided
// if callback is not provided then the interrupt is not enabled
void LinearEncoder::enableInterrupts(void (*doA)(), void(*doB)()){
  // attach interrupt if functions provided
  switch(quadrature){
    case LE_Quadrature::QUAD_ON :
      // A callback and B callback
      if(doA != nullptr) attachInterrupt(digitalPinToInterrupt(pinA), doA, CHANGE);
      if(doB != nullptr) attachInterrupt(digitalPinToInterrupt(pinB), doB, CHANGE);
      break;
    case LE_Quadrature::QUAD_OFF:
      // A callback and B callback
      if(doA != nullptr) attachInterrupt(digitalPinToInterrupt(pinA), doA, RISING);
      if(doB != nullptr) attachInterrupt(digitalPinToInterrupt(pinB), doB, RISING);
      break;
  }
}