#include "Manual_control.h"


/*Add manual control by keys  block */

//Create manual control object
//ManualControl _mControl  ; //Create handle of encoder hw timer

/**
* @brief TIM_Encoder MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_encoder: TIM_Encoder handle pointer
* @retval None
*/
void ManualControl::init(float* _control_value)
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

    upTime   = 0 ; //set up time 
    downTime = 0 ; 
    direction = 1 ; ////set up direction

    control_value = _control_value; //set_up pointer to manual control value
}

/**
 * Declare LinearEncoderHwT block 
 * create and initialize linear encoder object
 */
ManualControl::ManualControl(){
 
};

//Loop function for main loop procedure
void ManualControl::loop(){
  if(micros()-timestamp < MANUAL_CONTROL_PERIOD) return; //return if period does't reach
    //if control period reached
  timestamp = micros();  //save previos timestamp

    //if + button push
  if(!HAL_GPIO_ReadPin(MANUAL_CONTROL_GPIO_PORT, INC_PIN) ){
      upTime++  ; //incrace upTime counter
      if(upTime>0) cur_increment = 6  ; //set increment to 
      if(upTime>5) cur_increment = 8  ; //set increment to
      if(upTime>10) cur_increment = 10  ; //set increment to
      //if(upTime>15) cur_increment = 10  ; //set increment to
    }else{
      upTime = 0; //reset counter value  
    };
    //if - button push
  if(!HAL_GPIO_ReadPin(MANUAL_CONTROL_GPIO_PORT, DEC_PIN) ){
      downTime++  ; //incrace upTime counter
      if(downTime>0) cur_increment = -6  ; //set increment to 
      if(downTime>5) cur_increment = -8  ; //set increment to
      if(downTime>10) cur_increment = -10  ; //set increment to
      //if(downTime>15) cur_increment = -10  ; //set increment to
    }else{
      downTime = 0; //reset counter value
    };
      //if all counter == 0
    if(!(upTime || downTime)) {
      cur_increment = 0 ; //reset increment
    };
  control_value[0] = direction*cur_increment; //
}
