//#define HWSERIAL_NONE

#include <Arduino.h>

#include "config.h" // s42b 2.0 specific details
#include "StepperMotor.h" //Declare new Stepper motor object befor native SimpleFoc  definition
#include "SimpleFOC.h"
#include "TLE5012b.h"
#include "LinearAxis.h"
#include "Manual_control.h"
//#include "LinearEncoder.h"

  //Create and save object linear axis
LinearAxis axis = LinearAxis() ; //create LinearAxis object
ManualControl _mControl = ManualControl() ; //Create handle of encoder hw timer
  //Create and save a commander object
 //Commander commander = Commander(Serial,(char)'\n',true);
  char command_buffer[48];
  Commander commander = Commander((char)'\n',false); 
  void doMotor(char* cmd) { commander.motor(axis.motor, cmd); };
  void doRecalibrate(char* cmd) { axis.RecalibrateParams(); }; //new calibration procedure
  void doSave(char* cmd){ axis.SaveParams();}; // Save parameters

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  //Serial.setTx(PA2);  //Set TX port of UART2
  //Serial.setRx(PA3);  //Set RX port of UART2
  Serial.begin(115200);
  Serial.end();
  //Serial.println("Init begin...");
  
  command_buffer[0] = 0; //command buffer empty
  commander.add('M', doMotor, "motor");
  commander.add('R', doRecalibrate,"Recalibrate");
  commander.add('S', doSave, "Save_Parameters");
  
  //Serial.println("Done. RUNNING!");
  digitalWrite(LED_PIN, LOW);

   //add serial port stream to motor object for debug information
  // axis.motor->useMonitoring(Serial);
  
  //initiate axis object and variable
  axis.init();

   //init manual control  
   //set pointer of value for manual control 
  _mControl.init(&axis);

}

void loop() {
    // run commander loop function if command receive
    if(command_buffer[0]){ //command buff not empty
    // commander.run(); //execute command
    }
    command_buffer[0] = 0 ; //clear command buffer 
    

    //run axis loop function
    axis.loop();
    //manual control loop
    _mControl.loop();
   
}