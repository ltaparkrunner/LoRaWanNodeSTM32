#include "stm32l4xx_hal_tim.h"
#include "stm32_systime.h"

TIM_HandleTypeDef htim3;


/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 64;//65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
	
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */
	TIM_InitTypeDef sConfig;
	
	
  /* USER CODE END TIM3_Init 2 */

}

static UTIL_TIMER_Object_t USBFullReceivedTimer;

int32_t OnePulseTimer(void)
{
	UTIL_TIMER_Create(&USBFullReceivedTimer, 100, UTIL_TIMER_ONESHOT, OnUSBFullReceivedEvent, NULL);
	UTIL_TIMER_SetPeriod(&USBFullReceivedTimer, 100);
	UTIL_TIMER_Start(&USBFullReceivedTimer);
}

void OnUSBFullReceivedEvent(void *context)
{
	receivedLen = receivedLen;
}