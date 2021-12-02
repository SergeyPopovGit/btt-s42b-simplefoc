#include <Arduino.h>


#include "config.h" // s42b 2.0 specific details
#include "StepperMotor.h" //Declare new Stepper motor object befor native SimpleFoc  definition
#include "SimpleFOC.h"
#include "TLE5012b.h"
#include "LinearAxis.h"
#include "LinearEncoder.h"

  //Create and save object linear axis
LinearAxis axis = LinearAxis() ; //create LinearAxis object
  //Create and save a commander object
 Commander commander = Commander(Serial,(char)'\n',true); 
  void doMotor(char* cmd) { commander.motor(axis.motor, cmd); };
  void doRecalibrate(char* cmd) { axis.RecalibrateParams(); }; //new calibration procedure
  void doSave(char* cmd){ axis.SaveParams();}; // Save parameters

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  Serial.setTx(PA9);
  Serial.setRx(PA10);
  Serial.begin(115200);
  Serial.println("Init begin...");



  commander.add('M', doMotor, "motor");
  commander.add('R', doRecalibrate,"Recalibrate");
  commander.add('S', doSave, "Save_Parameters");
  
  Serial.println("Done. RUNNING!");
  digitalWrite(LED_PIN, LOW);
   //add serial port stream to motor object for debug information
  axis.motor->useMonitoring(Serial);
  
  //initiate axis object and variable
  axis.init();

}

void loop() {
    // run commander loop function
    commander.run();

    //run axis loop function
    axis.loop();
   
}