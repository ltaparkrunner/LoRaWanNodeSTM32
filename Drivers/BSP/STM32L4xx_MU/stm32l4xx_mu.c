/*************************************************
*************************************************/

#include "stm32l4xx_mu.h"

uint16_t LED_GPIO_PIN[2] = {LED1_PIN, LED2_PIN };
GPIO_TypeDef * LED_GPIO_PORT[2] = {LED1_GPIO_PORT, LED2_GPIO_PORT};

void MU_PB_Init(ButtonMode_TypeDef ButtonMode)	//Button_TypeDef Button,
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Enable the BUTTON Clock */
  SHIELD_RST_GPIO_CLK_ENABLE();

  if (ButtonMode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    GPIO_InitStruct.Pin    = SHIELD_RST_PIN;
    GPIO_InitStruct.Mode   = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull   = GPIO_NOPULL;
    GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(SHIELD_RST_GPIO_PORT, &GPIO_InitStruct);
  }
  else if (ButtonMode == BUTTON_MODE_EXTI)
  {
    /* Configure Button pin as input with External interrupt */
    GPIO_InitStruct.Pin    = SHIELD_RST_PIN;
    GPIO_InitStruct.Mode   = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull   = GPIO_NOPULL;
    GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SHIELD_RST_GPIO_PORT, &GPIO_InitStruct);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(SHIELD_RST_EXTI_IRQn, 0x0F, 0);
    HAL_NVIC_EnableIRQ(SHIELD_RST_EXTI_IRQn);
  }
}


/**
  * @brief  Configure LED GPIO.
  * @param  Led: LED to be configured.
  *         This parameter can be one of the following values:
  *            @arg  LED2 or LED4 on Nucleo-64 with external SMPS
  * @retval None
  */
void MU_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStruct = {0};

  /* Enable the GPIO_LED Clock */
  LEDx_GPIO_CLK_ENABLE(Led);
	if(Led == LED1 || Led == LED2){
  /* Configure the GPIO_LED pin */
		GPIO_InitStruct.Pin   = LED_GPIO_PIN[Led];
		GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull  = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(LED_GPIO_PORT[Led], &GPIO_InitStruct);
	}
}
void MU_Sound_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin   = SoundSign_Pin;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	// TODO: delete one of two WritePin below
	HAL_GPIO_WritePin(SoundSign_GPIO_Port, SoundSign_Pin, GPIO_PIN_RESET);
	HAL_GPIO_Init(SoundSign_GPIO_Port, &GPIO_InitStruct);
	HAL_GPIO_WritePin(SoundSign_GPIO_Port, SoundSign_Pin, GPIO_PIN_RESET);
}
/**
  * @brief  DeInitialize LED GPIO.
  * @param  Led: LED to be deinitialized.
  *         This parameter can be one of the following values:
  *            @arg  LED2 or LED4 on Nucleo-64 with external SMPS
  * @note BSP_LED_DeInit() does not disable the GPIO clock
  * @retval None
  */
void MU_LED_DeInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  /* Turn off LED */
  HAL_GPIO_WritePin(LED_GPIO_PORT[Led], LED_GPIO_PIN[Led], GPIO_PIN_RESET);
  /* DeInit the GPIO_LED pin */
  GPIO_InitStruct.Pin = LED_GPIO_PIN[Led];
  HAL_GPIO_DeInit(LED_GPIO_PORT[Led], GPIO_InitStruct.Pin);
}

/**
  * @brief  Turn selected LED On.
  * @param  Led: Specifies the Led to be set on.
  *   This parameter can be one of following parameters:
  *            @arg  LED2 or LED4 on Nucleo-64 with external SMPS
  * @retval None
  */
void MU_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_GPIO_PORT[Led], LED_GPIO_PIN[Led], GPIO_PIN_SET);
}

/**
  * @brief  Turn selected LED Off.
  * @param  Led: Specifies the Led to be set off.
  *   This parameter can be one of following parameters:
  *            @arg  LED2 or LED4 on Nucleo-64 with external SMPS
  * @retval None
  */
void MU_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_GPIO_PORT[Led], LED_GPIO_PIN[Led], GPIO_PIN_RESET);
}

/**
  * @brief  Toggle the selected LED.
  * @param  Led: Specifies the Led to be toggled.
  *   This parameter can be one of following parameters:
  *            @arg  LED2 or LED4 on Nucleo-64 with external SMPS
  * @retval None
  */
void MU_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(LED_GPIO_PORT[Led], LED_GPIO_PIN[Led]);
}
