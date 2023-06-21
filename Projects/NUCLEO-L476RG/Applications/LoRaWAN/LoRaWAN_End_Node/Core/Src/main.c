/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_lorawan.h"
// TODO: erase below
//#include "stm32l4xx_mu.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "settings_json.h"
#include "tiny-json.h"
#include "tiny-json_extra.h"
#include "string.h"
#include "usart.h"
#include "dma.h"
#include "flash_mem.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "parseMessage.h"
//#include "sys_app.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define RXBUFFERSIZE  200	
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
json_t pool2[Num_Field ];
uint8_t buff2[Buff_Len];


/* USER CODE BEGIN PV */
extern char sets_JSON[];
extern struct json_sets_t json_sets;
extern UART_HandleTypeDef huart3;
uint8_t aRxBuffer[RXBUFFERSIZE];
uint32_t RxReady = 0;



//TIM_HandleTypeDef htim3;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void MX_GPIO_Init(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	json_sets.length = strlen(json_sets.p);
//	if(json_sets.array != 0) 
	json_sets.outplen = FormOutpJson(&json_sets);
	//memcpy(json_sets.array, json_sets.p, json_sets.length);
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	MU_board_LEDs_Init();
	MX_GPIO_Init();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
	//SystemApp_Init();
//		json_t pool[ Num_Field ];
//	uint8_t buff[Buff_Len];

//	HAL_Init_Flash();

//	init_flash(FLASH_PAGE_FOR_SETTINGS, buff, Buff_Len);
	
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_LoRaWAN_Init();
  /* USER CODE BEGIN 2 */

	MX_DMA_Init();
  MX_USART3_UART_Init();
	MX_USB_DEVICE_Init();
	//check();


//	json_t const *settings = json_create(sets_JSON, pool, Num_Field);
//	if (settings == NULL) return -1;
//	
//	///////////////
//	json_t const* namefield = json_getProperty(settings, "LoRa_text");
//	if (namefield == NULL) return -1;
//	if (json_getType(namefield) != JSON_TEXT) return -1;
//	
//	//////////////////
//	char const* namevalue = json_getValue(namefield);
//	static char name2[150];

//  strcpy(name2, namevalue);

	//HAL_UART_Transmit(&huart3, (uint8_t *)name2, 100, 800);
	if(HAL_UART_Transmit_DMA(&huart3, (uint8_t *)"forever", 100) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_Delay(100);

	RxReady = 0;
	if(HAL_UART_Receive_DMA(&huart3, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
	{
		Error_Handler();
	}
	
//	while(RxReady != 1)
//	{
//	}
	//strcpy(
	//printf("%s%s%s", "LoRa_text: '", namevalue, "'.\n");
	//printf(namevalue);
	//printf("forever");
	//printf(
	

//		write_read_flash();
	uint8_t result = USBD_OK;
	uint8_t result2 = PARSED_JSON_OK;
	uint8_t result3 = WRITTEN_FLASH_OK;
  /* USER CODE END 2 */
	
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	uint8_t cmd = NO_CMD;
  while (1)
  {

    /* USER CODE END WHILE */
//    MX_LoRaWAN_Process();
		result = CheckTransmit(cmd);
		if(result == USBD_FAIL)
		{
			MX_USB_DEVICE_Init();
		}
		else if(result == USBD_OK)
			cmd = NO_CMD;
		result2 = JsonSettingsToBuffer_wrap(&cmd);

		result3 = WriteBufferToFlash_wrap(result2);
		
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

//void USBClock_Config(void)
//{
//	
//	//RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
//		
////	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
////                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
////  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
////  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
////  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
////  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

////  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
////  {
////    Error_Handler();
////  }
////  PeriphClkInit.PeriphClockSelection = /*RCC_PERIPHCLK_RTC |*/ RCC_PERIPHCLK_USB;

////  //PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
////  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
////  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
////  PeriphClkInit.PLLSAI1.PLLSAI1M = 4;
////  PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
////  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
////  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV4;
////  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV4;
////  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;//|RCC_PLLSAI1_ADC1CLK;
////  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
////  {
////    Error_Handler();
////  }
//  /** Enables the Clock Security System
//  */
//	  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART3
//                              |RCC_PERIPHCLK_LPTIM1|RCC_PERIPHCLK_USB
//                              |RCC_PERIPHCLK_ADC;
//  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
//  PeriphClkInit.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_PCLK;
//  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
//  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
//  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
//  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
//  PeriphClkInit.PLLSAI1.PLLSAI1M = 4;
//  PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
//  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
//  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV4;
//  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV4;
//  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK|RCC_PLLSAI1_ADC1CLK;
//  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//  {
//    Error_Handler();
//  }
//	
//  HAL_RCCEx_EnableLSECSS();	
//}
/**
  * @brief System Clock Configuration
  * @retval None
  */
//void SystemClock_Config(void)
//{
//  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//	
//	  HAL_PWR_EnableBkUpAccess();
//  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

//  /* MSI is enabled after System reset, activate PLL with MSI as source */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
//  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
//  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
//  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
//  RCC_OscInitStruct.PLL.PLLM = 4;//1;
//  RCC_OscInitStruct.PLL.PLLN = 40;
//  RCC_OscInitStruct.PLL.PLLR = 2;
//  RCC_OscInitStruct.PLL.PLLP = 7;
//  RCC_OscInitStruct.PLL.PLLQ = 4;
//  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
//    Error_Handler();
//  }

//  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
//  clocks dividers */
//  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
//  {
//    Error_Handler();
//  }
//	
////	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
////                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
////  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
////  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
////  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
////  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

////  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
////  {
////    Error_Handler();
////  }
//		USBClock_Config();
//}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART3
                              |RCC_PERIPHCLK_LPTIM1|RCC_PERIPHCLK_USB
                              |RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_PCLK;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 4;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV4;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV4;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK|RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enables the Clock Security System
  */
  HAL_RCCEx_EnableLSECSS();
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	RxReady = 1;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
//  MX_LEDS_Init();

  /*Configure GPIO pin : Vbus_DET_Pin */
  GPIO_InitStruct.Pin = Vbus_DET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Vbus_DET_GPIO_Port, &GPIO_InitStruct);



}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  while (1)
  {
  }
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
