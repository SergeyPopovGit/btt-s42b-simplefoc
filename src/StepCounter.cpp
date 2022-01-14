#include "StepCounter.h"

#ifndef STEPPING_INTERFACE_CONFIGURED
  #define HW_T_MAXVALUE 65535 
  #define STEP_COUNT_TIMER_INSTANCE TIM2
  #define STEP_COUNT_TIMER_CLK_ENABLE __TIM2_CLK_ENABLE() //Enable timer clock of step counter
  #define STEP_COUNT_CLK_ENABLE  __GPIOA_CLK_ENABLE() //Enable GPIO clock of step count  
  #define STEP_COUNT_PORT GPIOA    // Step count port A 
  #define STEP_PIN        GPIO_PIN_1 // old version PA1 // ! according to the previous files, but this is the same as the dir pin
  #define ENABLE_PIN      GPIO_PIN_2 // old version PA2
  #define DIRECTION_PIN   GPIO_PIN_0 // old version PA0
#endif

/*Add Hardware Timer pulse conter block */

TIM_HandleTypeDef stepCountTimer ; //Create handle of pulse count hw timer

//Start pulse counter
void Counter_Init(void) {
  HAL_TIM_Encoder_Start(&stepCountTimer, TIM_CHANNEL_ALL);
}

// Read pulse counter value
uint16_t Counter_Read() {
  uint16_t val = __HAL_TIM_GET_COUNTER(&stepCountTimer);
  return val;
}


/**
  * @brief TIM1 Initialization Function (Encoder Mode)
  * @param None
  * @retval None
  */
void Counter_Config(void)
{
  //enable clock of step counter timer
  STEP_COUNT_TIMER_CLK_ENABLE ;
    //Step counter pin configurate
  GPIO_InitTypeDef GPIO_InitStruct;

    STEP_COUNT_CLK_ENABLE ;

    GPIO_InitStruct.Pin = STEP_PIN | DIRECTION_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM; 
    HAL_GPIO_Init(STEP_COUNT_PORT, &GPIO_InitStruct);


  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  stepCountTimer.Instance = STEP_COUNT_TIMER_INSTANCE;
  stepCountTimer.Init.Prescaler = 0;
  stepCountTimer.Init.CounterMode = TIM_COUNTERMODE_UP;
  stepCountTimer.Init.Period = HW_T_MAXVALUE;
  stepCountTimer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  stepCountTimer.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1; //
  sConfig.IC1Polarity = TIM_ENCODERINPUTPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;     // Capture performed each time an edge is detected on the capture input
  sConfig.IC1Filter = 16;
  sConfig.IC2Polarity = TIM_ENCODERINPUTPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_INDIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;     // Capture performed each time an edge is detected on the capture input
  sConfig.IC2Filter = 16;
  
  HAL_TIM_Encoder_Init(&stepCountTimer, &sConfig);
  
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  
  HAL_TIMEx_MasterConfigSynchronization(&stepCountTimer, &sMasterConfig);

}

/**
 * Declare LinearEncoderHwT block 
 * create and initialize linear encoder object
 */
StepCounter::StepCounter(int _ppmm){
 ppmm=_ppmm ;
}

/*
	 get measured position
*/
float StepCounter::getPosition(){
    int32_t diff_counter ;  //create diff encoder
    int32_t cur_counter = Counter_Read();  //read curent value
    diff_counter = cur_counter-prev_counter; //difference beetwin curenv and previos encoder value
    prev_counter = cur_counter;  //save previos encoder value
    pulse_counter += diff_counter ; //add pulse counter
    if (diff_counter > HW_T_MAXVALUE/2){pulse_counter -= HW_T_MAXVALUE;}; //up overload counter correction
    if (diff_counter < -HW_T_MAXVALUE/2){pulse_counter += HW_T_MAXVALUE;}; //down overload counter correction
    return  direction*(pulse_counter) / ((float) ppmm);
}

// encoder initialisation of the hardware pins
// and calculation variables
void StepCounter::init(void){
  Counter_Config(); // Config timer as hardware encoder
  Counter_Init() ;  //Start timer
  cur_position = 0; //init current position 
  direction = 1;
}

//Loop function for main loop procedure
void StepCounter::loop(){
  cur_position = getPosition() ;  //get curent position
}
