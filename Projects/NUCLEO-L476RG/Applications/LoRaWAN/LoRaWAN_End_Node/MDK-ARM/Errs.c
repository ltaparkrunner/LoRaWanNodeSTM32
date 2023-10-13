#include "stm32l4xx_mu.h"

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
  MU_LED_On(HL1);
	MU_LED_Off(HL2);
	while(1)
	{
		for(int32_t i2 = 0; i2 < num; i2++)
		{
			MU_LED_Toggle(HL1);
			MU_LED_Toggle(HL2);
			for(int32_t i2 = 0; i2 < 1000000; i2++)
				waiting_for(num);
		}
		for(int32_t i2 = 0; i2 < 10000000; i2++)
			waiting_for(num);
	}
  /* USER CODE END Error_Handler_Debug */
}
