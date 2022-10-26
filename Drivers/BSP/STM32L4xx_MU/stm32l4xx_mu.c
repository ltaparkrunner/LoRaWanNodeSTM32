#include "stm32l4xx_mu.h"

void MU_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Enable the BUTTON Clock */
  BUTTONx_GPIO_CLK_ENABLE(Button);

  if (ButtonMode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    GPIO_InitStruct.Pin    = MU_BUTTON_PIN;
    GPIO_InitStruct.Mode   = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull   = GPIO_NOPULL;
    GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(MU_BUTTON_GPIO_PORT, &GPIO_InitStruct);
  }
  else if (ButtonMode == BUTTON_MODE_EXTI)
  {
    /* Configure Button pin as input with External interrupt */
    GPIO_InitStruct.Pin    = MU_BUTTON_PIN;
    GPIO_InitStruct.Mode   = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull   = GPIO_NOPULL;
    GPIO_InitStruct.Speed  = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(MU_BUTTON_GPIO_PORT, &GPIO_InitStruct);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(MU_BUTTON_EXTI_IRQn, 0x0F, 0);
    HAL_NVIC_EnableIRQ(MU_BUTTON_EXTI_IRQn);
  }
}
