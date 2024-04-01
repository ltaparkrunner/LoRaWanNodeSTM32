#include <stdint.h>
#include "platform.h"
#include "sys_debug.h"
#include "radio_board_if.h"
#include "stm32_lpm.h"
#include "stm32_timer.h"

static void Gpio_PreInit(void);

void SystemApp_Init(void)
{
  /* USER CODE BEGIN SystemApp_Init_1 */

  /* USER CODE END SystemApp_Init_1 */

  /* Ensure that MSI is wake-up system clock */
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);

  /*Initialize timer and RTC*/
  UTIL_TIMER_Init();

  Gpio_PreInit();

  /* Debug config : disable serial wires and DbgMcu pins settings */
  DBG_Disable();

  /* Initializes the SW probes pins and the monitor RF pins via Alternate Function */
  DBG_ProbesInit();

  /*Initialize the terminal */
//  UTIL_ADV_TRACE_Init();
//  UTIL_ADV_TRACE_RegisterTimeStampFunction(TimestampNow);

  /*Set verbose LEVEL*/
//  UTIL_ADV_TRACE_SetVerboseLevel(VERBOSE_LEVEL);

  /*Initialize the temperature and Battery measurement services */
  //SYS_InitMeasurement();

  Sx_Board_Bus_Init();

  Sx_Board_IoInit();

  /*Initialize the Sensors */
  //EnvSensors_Init();

  /*Init low power manager*/
  UTIL_LPM_Init();
  /* Disable Stand-by mode */
  UTIL_LPM_SetOffMode((1 << CFG_LPM_APPLI_Id), UTIL_LPM_DISABLE);

#if defined (LOW_POWER_DISABLE) && (LOW_POWER_DISABLE == 1)
  /* Disable Stop Mode */
  UTIL_LPM_SetStopMode((1 << CFG_LPM_APPLI_Id), UTIL_LPM_DISABLE);
#elif !defined (LOW_POWER_DISABLE)
#error LOW_POWER_DISABLE not defined
#endif /* LOW_POWER_DISABLE */

  /* USER CODE BEGIN SystemApp_Init_2 */

  /* USER CODE END SystemApp_Init_2 */
}

uint32_t GetDevAddr(void)
{
  return ((HAL_GetUIDw0()) ^ (HAL_GetUIDw1()) ^ (HAL_GetUIDw2()));
}


void GetUniqueId(uint8_t *id)
{
  /* USER CODE BEGIN GetUniqueId_1 */

  /* USER CODE END GetUniqueId_1 */
  uint32_t ID_1_3_val = HAL_GetUIDw0() + HAL_GetUIDw2();
  uint32_t ID_2_val = HAL_GetUIDw1();

  id[7] = (ID_1_3_val) >> 24;
  id[6] = (ID_1_3_val) >> 16;
  id[5] = (ID_1_3_val) >> 8;
  id[4] = (ID_1_3_val);
  id[3] = (ID_2_val) >> 24;
  id[2] = (ID_2_val) >> 16;
  id[1] = (ID_2_val) >> 8;
  id[0] = (ID_2_val);

  /* USER CODE BEGIN GetUniqueId_2 */

  /* USER CODE END GetUniqueId_2 */
}

static void Gpio_PreInit(void)
{
  /* USER CODE BEGIN Gpio_PreInit_1 */

  /* USER CODE END Gpio_PreInit_1 */
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Configure all IOs in analog input              */
  /* Except PA143 and PA14 (SWCLK and SWD) for debug*/
  /* PA13 and PA14 are configured in debug_init     */
  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Enable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  /* All GPIOs except debug pins (SWCLK and SWD) */
  GPIO_InitStruct.Pin = GPIO_PIN_All & (~(GPIO_PIN_13 | GPIO_PIN_14));
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* All GPIOs */
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /* Disable GPIOs clock */
  __HAL_RCC_GPIOA_CLK_DISABLE();
  __HAL_RCC_GPIOB_CLK_DISABLE();
  __HAL_RCC_GPIOC_CLK_DISABLE();
  __HAL_RCC_GPIOH_CLK_DISABLE();
  /* USER CODE BEGIN Gpio_PreInit_2 */

  /* USER CODE END Gpio_PreInit_2 */
}

