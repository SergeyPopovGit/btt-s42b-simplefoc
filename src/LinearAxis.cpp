#include "LinearAxis.h"

 //Create a motor object with 
 // 200 steps/4 poles = 50 pole pairs
StepperMotor _motor = StepperMotor(50) ;  
//Create and save a sensor object
TLE5012B _sensor = TLE5012B(); 
//Create and save a stepperdrive object
StepperDriver2PWM _driver = StepperDriver2PWM(COIL_A_PWM, COIL_A_DIR_1, COIL_A_DIR_2, COIL_B_PWM, COIL_B_DIR_1, COIL_B_DIR_2, NOT_SET, NOT_SET);
//Create and save a linear encoder object
LinearEncoderHwT _scale = LinearEncoderHwT();
PIDController  PID_axis(500,1000,0,0,MAX_AXIS_TORQUE) ;

  //Create method of LinearAxis object
LinearAxis::LinearAxis()
{


}

// init LinearAxis procedure   
void LinearAxis::init() {
  
   //save a motor object pointer
   motor = &_motor;  
  //save a sensor object pointer
   sensor = &_sensor;  
  //save a stepperdrive object pointer
   driver = &_driver;
  //Save incremental linear encoder pointer
   scale = &_scale;

  //init magnet encoder
  sensor->init();
    //init optical scale hardware
  scale->init();
  
    // power supply voltage [V]
  // driver.pwm_frequency = 50000;
  driver->voltage_power_supply = 24;
    // Max DC voltage allowed - default voltage_power_supply
  // driver init
  driver->init();
    // init sensor
  // link the motor to the sensor
  motor->linkSensor(sensor);

    // init driver
  // link the motor to the driver
  motor->linkDriver(driver);
   // set control loop type to be used
  motor->controller = MotionControlType::torque;
  
  motor->useMonitoring(Serial);
  /*
    

   //disable vel commander monitor
  //commander.add('C',onPid,"PID vel");
  //commander.add('L',onLpf,"LPF vel");
  //commander.add('T',onTarget,"target vel");
  
   motor.monitor_variables = _MON_TARGET|_MON_VOLT_Q|_MON_VOLT_D|_MON_VEL|_MON_ANGLE;
   motor.monitor_downsample = 2000;
  */
 //setup voltage in zero electrical angle align procedure
  motor->voltage_sensor_align = 12;
 
  // default voltage_power_supply
  motor->voltage_limit = 10;

  // initialize motor
  motor->init();
  
 
  //load parameters from EEPROM
  if ( LoadParams() != EepromFlag::VALID){ // if no valid parameters align encoder and save parameters
     RecalibrateParams() ;
   };


  //++++ADD DEBUG

  motor->target = 0 ;      //set target position
} 

// disable LinearAxis
void LinearAxis::disable()
{
  
};

// enable LinearAxis
void LinearAxis::enable()
{
  
};


// Iterative function looping LinearAxis algorithm
// The faster it can be run the better
void LinearAxis::loop() {
  //Update scale position
    scale->loop();
 // FOC algorithm function
    motor->loopFOC();
 // Axis position PID controller
  //calculate torque value by curent error value
    error = targ_position - scale->cur_position; //calculate position error
    torque = PID_axis(error);   //process error value to motor torque value
    motor->target = torque ;  // copy torgur value to motor target
 // Motion control function 
 // Not used for testing in command mode
    motor->move();  //execute foc algorithm of motor
}

// Iterative function running outer loop of the LinearAxis algorithm
// Behavior of this function is determined by the motor.controller variable

void LinearAxis::move(float new_target) {
  
};

 int saveArrayE (int address, uint8_t* pData, int len){
  // Old version work without erase eeprom
  int i=0;
   for( ;i < len; i++){
    EEPROM[address+i]=pData[i];
  };
  //
  return i; 
 };

 int loadArrayE (int address, uint8_t* pData, int len){
  // Old version work without erase eeprom
  int i=0;
   for( ;i < len; i++){
    pData[i]=EEPROM[address+i];
  };
  //
  return i; 
 };

  //Save curent movement parameters to EEPROM for axis objec
int LinearAxis::SaveParams(){
  AxisParameters cur_params ; //<!crate object for save parameters
 
  int cur_address = PARAMETERS_ADDRESS ; //address of curent variable
  uint8_t end[]="end";
  uint8_t tableName[] = "CalibrationTable=";
  uint8_t paramName[] = "Parameter=";
  //cur_address +=saveArrayE(cur_address,paramName,sizeof(paramName));

  cur_params.motor_dir = this->motor->sensor_direction;
  cur_params.zero_angle = this->motor->zero_electric_angle;
  cur_params.scale_dir = this->scale_direction;
  for (int i=0; i < _POLE_PAIRS ; i++){
    cur_params.ctable[i] = this->motor->correctionTable[i];
    };
  cur_params.control_flag = EepromFlag::VALID;
  cur_address+=saveArrayE(cur_address,(uint8_t*)&cur_params,sizeof(cur_params));
  
  
  //cur_address +=saveArrayE(cur_address,tableName,sizeof(tableName));
  //cur_address +=saveArrayE(cur_address, (uint8_t*) this->motor->correctionTable,sizeof(this->motor->correctionTable));
  //cur_address +=saveArrayE(cur_address,end,sizeof(end));
  return cur_address; 
};
  
  //Load saved movement parameters from EEPROM
char LinearAxis::LoadParams(){
   AxisParameters params; //<!crate object for load parameters

   unsigned int  cur_address = PARAMETERS_ADDRESS ; //address of curent variable 
   
   loadArrayE(cur_address, (uint8_t*)&params, sizeof(params)); //load data to parameters structure
      //if validation good then load parameters to object
   if (params.control_flag == EepromFlag::VALID){  
        this->motor->sensor_direction = params.motor_dir;
        this->motor->zero_electric_angle = params.zero_angle;
        this->scale_direction = params.scale_dir;
        for (int i= 0 ; i < _POLE_PAIRS ;  i++){
          this->motor->correctionTable[i] = params.ctable[i] ;  //save curent value
        };    
      };

    return params.control_flag; //return value of read operation
 };
    
    //<! recalibrate movement parameters and save to EEPROM
  void LinearAxis::RecalibrateParams(){
          //clear main motor parametre for recalibrate
        motor->zero_electric_angle = NOT_SET;
        motor->sensor_direction = NOT_SET;
          //calibrate parameters 
        motor->initFOC(); //align encoder
        SaveParams(); 
  }; 