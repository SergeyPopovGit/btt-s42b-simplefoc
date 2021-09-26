#include <Arduino.h>

#include "config.h" // s42b 2.0 specific details

#include "SimpleFOC.h"
#include "TLE5012b.h"

TLE5012B sensor = TLE5012B();

StepperDriver2PWM driver = StepperDriver2PWM(COIL_A_PWM, COIL_A_DIR_1, COIL_A_DIR_2, COIL_B_PWM, COIL_B_DIR_1, COIL_B_DIR_2, NOT_SET, NOT_SET);

// 200 steps/4 poles = 50 pole pairs
StepperMotor motor = StepperMotor(50, 2.2);

Commander commander = Commander(Serial,(char)'\n',true);
void doMotor(char* cmd) { commander.motor(&motor, cmd); }
void onPid(char* cmd){commander.pid(&motor.PID_velocity, cmd);}
void onLpf(char* cmd){commander.lpf(&motor.LPF_velocity, cmd);}
void onTarget(char* cmd){commander.scalar(&motor.target, cmd);}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  Serial.setTx(PA9);
  Serial.setRx(PA10);
  Serial.begin(115200);
  Serial.println("Init begin...");

  sensor.init();

  // power supply voltage [V]
  // driver.pwm_frequency = 50000;
  driver.voltage_power_supply = 24;
  // Max DC voltage allowed - default voltage_power_supply
  // driver init
  driver.init();

  // init sensor
  // link the motor to the sensor
  motor.linkSensor(&sensor);

  // init driver
  // link the motor to the driver
  motor.linkDriver(&driver);

  // set control loop type to be used
  motor.controller = MotionControlType::torque;
  
  motor.useMonitoring(Serial);

  commander.add('M', doMotor, "motor");

    //disable vel commander monitor
  //commander.add('C',onPid,"PID vel");
  //commander.add('L',onLpf,"LPF vel");
  //commander.add('T',onTarget,"target vel");
  
   motor.monitor_variables = _MON_TARGET|_MON_VOLT_Q|_MON_VOLT_D|_MON_VEL|_MON_ANGLE;
   motor.monitor_downsample = 2000;

  motor.voltage_sensor_align = 9;
  motor.current_limit = 800;
  motor.velocity_limit = 500;


  // controller configuration based on the control type 
  motor.PID_velocity.P = 0.2;
  motor.PID_velocity.I = 0;
  motor.PID_velocity.D = 0;
  // default voltage_power_supply
  motor.voltage_limit = 5;

  // velocity low pass filtering time constant
  motor.LPF_velocity.Tf = 0.01;

  // angle loop controller
  motor.P_angle.P = 20;
  // angle loop velocity limit
  motor.velocity_limit = 50;


  // initialize motor
  motor.init();
  
  // align encoder and start FOC
  motor.initFOC();

  motor.target = 0 ;      //set target position
  
  Serial.println("Done. RUNNING!");
  digitalWrite(LED_PIN, LOW);

}

void loop() {
    commander.run();
    // FOC algorithm function
    motor.loopFOC();

    // monitor function
    motor.monitor();
    
    // Motion control function 
    //Not used for testing in command mode
   // motor.move();
    //motor.move(motor.shaft_angle_sp + 1);
}