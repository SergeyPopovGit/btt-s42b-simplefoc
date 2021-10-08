#include <Arduino.h>

#include "config.h" // s42b 2.0 specific details

#include "SimpleFOC.h"
#include "TLE5012b.h"
#include "LinearAxis.h"
#include "LinearEncoder.h"

  //Create and save object linear axis
LinearAxis axis = LinearAxis() ; //create LinearAxis object
  //Create and save a commander object
 Commander commander = Commander(Serial,(char)'\n',true); 
  void doMotor(char* cmd) { commander.motor(axis.motor, cmd); };

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  Serial.setTx(PA9);
  Serial.setRx(PA10);
  Serial.begin(115200);
  Serial.println("Init begin...");



  commander.add('M', doMotor, "motor");
  
  Serial.println("Done. RUNNING!");
  digitalWrite(LED_PIN, LOW);

}

void loop() {
    // run commander loop function
    commander.run();

    //run axis loop function
    axis.loop();
   
}