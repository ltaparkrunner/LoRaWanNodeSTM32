#include "stm32l4xx_mu.h"
#include "blinks.h"

int32_t waiting_for(int32_t num);
/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler2(int32_t num)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
	while(1)
	{
		blinking(num);
		for(int32_t i2 = 0; i2 < 10000000; i2++)
			waiting_for(num);
	}
	return;
  /* USER CODE END Error_Handler_Debug */
}

void blinking(int32_t num)
{
	MU_LED_On(HL1);
	MU_LED_Off(HL2);
	for(int32_t i2 = 0; i2 < num*2; i2++)
	{
		MU_LED_Toggle(HL1);
		MU_LED_Toggle(HL2);
		for(int32_t i2 = 0; i2 < 1000000; i2++)
			waiting_for(num);
	}
	MU_LED_Off(HL1);
	MU_LED_Off(HL2);
	return;
}
