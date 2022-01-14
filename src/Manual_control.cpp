#include "Manual_control.h"
#include "LinearAxis.h"

/*Add manual control by keys  block */

//Create manual control object
//ManualControl _mControl  ; //Create handle of encoder hw timer

void ManualControl::init(LinearAxis* _axis)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
     __HAL_RCC_GPIOB_CLK_ENABLE();
    /**Keys GPIO Configuration    
    PB0 SW_KeyUP    Move +
    PB1 SW_KeyDown  -Move -
    */
    GPIO_InitStruct.Pin = INC_PIN|DEC_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM; 
    HAL_GPIO_Init(MANUAL_CONTROL_GPIO_PORT, &GPIO_InitStruct);
    /**Config mode select button
      PA3 
    */
    GPIO_InitStruct.Pin = MODE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM; 
    HAL_GPIO_Init(MANUAL_CONTROL_GPIO_PORT, &GPIO_InitStruct);

    upTime   = 0 ; //set up time 
    downTime = 0 ; 
    direction = 1 ; ////set up direction

    axis = _axis  ; //save pointer of axis object
  }

/**
 * Declare LinearEncoderHwT block 
 * create and initialize linear encoder object
 */
ManualControl::ManualControl(){
 
};

//start Manual control function
void ManualControl::start(){
  //axis->motor->enabled = 0;  //switch on  motor FOC 
}

// Manual control Up function
void ManualControl::update(float _value){
  axis->targ_position+=direction*_value; // update manual control value
}

//stop manual control function
void ManualControl::stop(){
   // axis->motor->enabled = 1;  //switch on  motor FOC 
   axis->targ_position = axis->scale->cur_position; //set target position to curent position
}

//M
//Loop function for main loop procedure
void ManualControl::loop(){
  if(micros()-timestamp < MANUAL_CONTROL_PERIOD) return; //return if period does't reach
    //if control period reached
  timestamp = micros();  //save previos timestamp

    //if + button push
  if(!HAL_GPIO_ReadPin(MANUAL_CONTROL_GPIO_PORT, INC_PIN) ){
      if(upTime == 0) {cur_increment = 0; start() ;} //start manual control mode
      if(upTime>0) cur_increment = FIRS_SPEED  ; //set increment to 
      if(upTime>TIME_TO_SECOND_SPEED) cur_increment = SECOND_SPEED  ; //set increment to
      if(upTime>TIME_TO_THIRD_SPEED) cur_increment = THIRD_SPEED  ; //set increment to
      upTime++  ; //incrace upTime counter
      update(cur_increment);
    }else{
      if (upTime){  //if button release and counter not null
         stop() ; // stop manual control mode
         upTime = 0; //reset counter value
         };     
    };
    //if - button push
  if(!HAL_GPIO_ReadPin(MANUAL_CONTROL_GPIO_PORT, DEC_PIN) ){
      if(downTime == 0) {cur_increment = 0; start() ;} //start manual control mode
      if(downTime>0) cur_increment = -FIRS_SPEED  ; //set increment to 
      if(downTime>TIME_TO_SECOND_SPEED) cur_increment = -SECOND_SPEED  ; //set increment to
      if(downTime>TIME_TO_THIRD_SPEED) cur_increment = -THIRD_SPEED  ; //set increment to
      downTime++  ; //incrace upTime counter
      update(cur_increment);
    }else{
      if (downTime){  //if button release and counter not null
         stop() ; // stop manual control mode
         downTime = 0; //reset counter value
        }; 
    };
}


