#include "LinearEncoderHwT.h"

#define HW_T_PERIOD 65535 
#define PULSE_PER_MM 1000

/*Add incremental linear encoder  block */

//HardwareTimer _hwLineEncoder = HardwareTimer(encoderTIM);
TIM_HandleTypeDef htim1 ; //Create handle of encoder hw timer

//Start encoder 
void Encoder_Init(void) {
  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
}

// Read encoder value
uint16_t Encoder_Read() {
  uint16_t val = __HAL_TIM_GET_COUNTER(&htim1);
  return val;
}


/**
  * @brief TIM1 Initialization Function (Encoder Mode)
  * @param None
  * @retval None
  */
void Encoder_Config(void)
{
  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim1.Instance = ENCODER_TIM;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = HW_T_PERIOD;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12; //
  sConfig.IC1Polarity = TIM_ENCODERINPUTPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;     // Capture performed each time an edge is detected on the capture input
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ENCODERINPUTPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;     // Capture performed each time an edge is detected on the capture input
  sConfig.IC2Filter = 0;
  
  HAL_TIM_Encoder_Init(&htim1, &sConfig);
  
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  
  HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);
}



/**
* @brief TIM_Encoder MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_encoder: TIM_Encoder handle pointer
* @retval None
*/
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* htim_encoder)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(htim_encoder->Instance==ENCODER_TIM)
  {
    /* Peripheral clock enable */
    __HAL_RCC_TIM1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM2 GPIO Configuration    
    PA8-WKUP     ------> TIM1_CH1
    PA9     ------> TIM1_CH2 
    */
    GPIO_InitStruct.Pin = ENCODER_GPIO_CH1|ENCODER_GPIO_CH2;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM; 
    HAL_GPIO_Init(ENCODER_GPIO_PORT, &GPIO_InitStruct);
  }

}

/**
* @brief TIM_Encoder MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_encoder: TIM_Encoder handle pointer
* @retval None
*/
void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* htim_encoder)
{
  if(htim_encoder->Instance==ENCODER_TIM)
  {
  /* USER CODE BEGIN TIM1_MspDeInit 0 */

  /* USER CODE END TIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();
  
    /**TIM1 GPIO Configuration    
    PA8-WKUP     ------> TIM1_CH1
    PA9     ------> TIM1_CH2 
    */
    HAL_GPIO_DeInit(ENCODER_GPIO_PORT, ENCODER_GPIO_CH1|ENCODER_GPIO_CH2 );

  /* USER CODE BEGIN TIM1_MspDeInit 1 */

  /* USER CODE END TIM1_MspDeInit 1 */
  }

}
//
//End linear encoder value

/**
 * Declare LinearEncoderHwT block 
 * create and initialize linear encoder object
 */
LinearEncoderHwT::LinearEncoderHwT(){
  // velocity calculation variables
  prev_timestamp_us = micros();
}

/*
	 get measured position
*/
float LinearEncoderHwT::getPosition(){
    int32_t diff_encoder ;  //create diff encoder
    int32_t cur_encoder = Encoder_Read();  //read curent value
    diff_encoder = cur_encoder-prev_encoder; //difference beetwin curenv and previos encoder value
    prev_encoder = cur_encoder;  //save previos encoder value
    pulse_counter += diff_encoder ; //add pulse counter
    if (diff_encoder > HW_T_PERIOD/2){pulse_counter -= HW_T_PERIOD;}; //up overload counter correction
    if (diff_encoder < -HW_T_PERIOD/2){pulse_counter += HW_T_PERIOD;}; //down overload counter correction
    return  direction*(pulse_counter) / ((float) PULSE_PER_MM );
}

/*
  Linear velocity calculation
  function using mixed time and frequency measurement technique
*/
/*
float LinearEncoderHwT::getVelocity(){
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
*/

// encoder initialisation of the hardware pins
// and calculation variables
void LinearEncoderHwT::init(void){
  Encoder_Config(); // Config timer as hardware encoder
  HAL_TIM_Encoder_MspInit(&htim1);  //On Clock source and GPIO for input
  Encoder_Init() ;  //Start timer
  cur_position = 0; //init current position 
  direction = 1;
}

//Loop function for main loop procedure
void LinearEncoderHwT::loop(){
  cur_position = getPosition() ;  //get curent position
}
