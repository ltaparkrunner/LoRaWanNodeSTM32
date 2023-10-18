#include <stdint.h>
#include "stm32l4xx_hal.h"
#include "LmHandlerTypes.h"
#include "sensors.h"

/**
* @brief ADC MSP Initialization
* This function configures the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(hadc->Instance==ADC2)
  {
  /* USER CODE BEGIN ADC2_MspInit 0 */

  /* USER CODE END ADC2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_ADC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC2 GPIO Configuration
    PA1     ------> ADC2_IN6
    PA2     ------> ADC2_IN7
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;	//NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC2 interrupt Init */
    HAL_NVIC_SetPriority(ADC1_2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
  /* USER CODE BEGIN ADC2_MspInit 1 */

  /* USER CODE END ADC2_MspInit 1 */
  }

}

/**
* @brief ADC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  /* USER CODE BEGIN ADC2_MspDeInit 0 */

  /* USER CODE END ADC2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC_CLK_DISABLE();

    /**ADC2 GPIO Configuration
    PA1     ------> ADC2_IN6
    PA2     ------> ADC2_IN7
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1|GPIO_PIN_2);

    /* ADC2 interrupt DeInit */
  /* USER CODE BEGIN ADC2:ADC1_2_IRQn disable */
    /**
    * Uncomment the line below to disable the "ADC1_2_IRQn" interrupt
    * Be aware, disabling shared interrupt may affect other IPs
    */
     HAL_NVIC_DisableIRQ(ADC1_2_IRQn); 
  /* USER CODE END ADC2:ADC1_2_IRQn disable */

  /* USER CODE BEGIN ADC2_MspDeInit 1 */

  /* USER CODE END ADC2_MspDeInit 1 */
}

ADC_HandleTypeDef hadc2;

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(uint32_t channel)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc2.Init.LowPowerAutoWait = DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.Overrun = ADC_OVR_DATA_PRESERVED; // or ADC_OVR_DATA_OVERWRITTEN
  hadc2.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    //Error_Handler();
		while(1);
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = channel;//ADC_CHANNEL_5;  // is this tired with PA0 -- ADC2_IN5
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    //Error_Handler();
		while(1);
  }
  /* USER CODE BEGIN ADC2_Init 2 */
	
  /* USER CODE END ADC2_Init 2 */

}

	
	/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 8;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 20;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    //Error_Handler();
		while(1);
  }
}

#include "flash_mem.h"
#include "tiny-json_extra.h"
extern struct field_json json_descr[];
#define probes_sett_head		48
#define probes_sett_tail		53


int32_t CollectData(struct LmHandlerAppData_s* appData)
{
	uint32_t addr_w;
	uint32_t addr_r = ChooseReadFlashBank(&addr_w);
	uint32_t offset = json_descr[probes_sett_head].offset;
	uint32_t bf_len = json_descr[probes_sett_tail].offset - json_descr[probes_sett_head].offset;
	int32_t ret =0;
	static uint8_t temp_buff[10];
	
	//	if(readflash(addr_r, (uint8_t*)temp_buff, FlashBuffLen) > 0) {
	if(readflash_8b(addr_r+offset, temp_buff, bf_len) > 0) {
		if(temp_buff[0] == truefl) {
			MX_ADC2_Init(ADC_CHANNEL_7);
			ret = ADCtoBuff(appData, 2);
			HAL_ADC_DeInit(&hadc2);
		}
		else if(temp_buff[1] == truefl) {
			MX_ADC2_Init(ADC_CHANNEL_6);
			ret = ADCtoBuff(appData, 1);
			HAL_ADC_DeInit(&hadc2);
		}
		else if(temp_buff[2] == truefl) {
			MX_DIG_Init();
			ret = DigtoBuff(appData);
			MX_Dig_DeInit();
		}
		else if(temp_buff[3] == truefl) {
			MX_DIG_Init();
			ret = DigtoBuff(appData);
			MX_Dig_DeInit();
		}
		else {
			ret = TexttoBuff(appData);
		}
	}
	return ret;
}
#include <stdio.h>
int32_t ADCtoBuff(struct LmHandlerAppData_s* appData, int32_t ch)
{
	int32_t ADCValue[3] = {0};
	  /*##-3- Calibrate ADC then Start the conversion process ####################*/
	HAL_StatusTypeDef res = HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
  if (res !=  HAL_OK)
  {
    /* ADC Calibration Error */
    //Error_Handler();
		while(1);
  }
	for(int32_t i1 = 0; i1 < 3 ; i1++)
	{  
		if (HAL_ADC_Start(&hadc2) != HAL_OK)
		{
			/* Start Conversation Error */
			//Error_Handler();
			while(1);
		}
	

		/*##-4- Wait for the end of conversion #####################################*/
		/*  For simplicity reasons, this example is just waiting till the end of the
				conversion, but application may perform other tasks while conversion
				operation is ongoing. */
		if (HAL_ADC_PollForConversion(&hadc2, 10) != HAL_OK)
		{
			/* End Of Conversion flag not set on time */
			//Error_Handler();
			while(1);
		}

		/* Check if the continous conversion of regular channel is finished */
		if ((HAL_ADC_GetState(&hadc2) & HAL_ADC_STATE_REG_EOC) == HAL_ADC_STATE_REG_EOC)
		{
			/*##-5- Get the converted value of regular channel  ########################*/
			ADCValue[i1] = HAL_ADC_GetValue(&hadc2);
		}
	}
	if(ADCValue[0] >= ADCValue[1] && ADCValue[1] >= ADCValue[2]) ADCValue[0] = ADCValue[1];
	else if(ADCValue[0] >= ADCValue[2] && ADCValue[2] >= ADCValue[1]) ADCValue[0] = ADCValue[2];
	else if(ADCValue[1] >= ADCValue[2] && ADCValue[2] >= ADCValue[0]) ADCValue[0] = ADCValue[2];
	else if(ADCValue[2] >= ADCValue[1] && ADCValue[1] >= ADCValue[0]) ADCValue[0] = ADCValue[1];
	
	//uhADCxConvertedValue = (uhADCxConvertedValue *3300) / 12288;
	ADCValue[0] = (ADCValue[0] *3300) / 6720;//6116;//18350;//18588;
	uint8_t hd, ld;
	hd = (uint8_t)(ADCValue[0] /1000);
	ld = (uint8_t)((ADCValue[0] /10) % 100);
	char *pBuf = (char *)appData->Buffer;
	sprintf(pBuf, "The PA%1d analog value is: %-1d,%02d", ch, hd, ld);
	return 29;
}

int32_t MX_DIG_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
  /*Configure GPIO pin Output Level */
//  MX_LEDS_Init();

  /*Configure GPIO pin : Vbus_DET_Pin */
  GPIO_InitStruct.Pin = P0_PIN | P1_PIN | P2_PIN | P3_PIN  | P5_PIN | P6_PIN | P7_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN; //NOPULL;
  HAL_GPIO_Init(P0123567__Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = P4_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;	//NOPULL;
  HAL_GPIO_Init(P4__Port, &GPIO_InitStruct);
	return 0;
}

int32_t MX_Dig_DeInit(void)
{
	HAL_GPIO_DeInit(P0123567__Port, P0_PIN | P1_PIN | P2_PIN | P3_PIN  | P5_PIN | P6_PIN | P7_PIN);
	HAL_GPIO_DeInit(P4__Port, P4_PIN);
	return 0;
}

int32_t DigtoBuff(struct LmHandlerAppData_s* appData)
{
	const uint8_t str_h[] = "The digital pins are: ";
	int32_t ln = strlen((char*)str_h);
	uint8_t *pBuf = appData->Buffer;
	memcpy(pBuf, str_h, ln);
	pBuf += ln;
	if(HAL_GPIO_ReadPin(P0123567__Port, P7_PIN) == GPIO_PIN_RESET) *pBuf++ = '0';
	else *pBuf++ = '1';
	if(HAL_GPIO_ReadPin(P0123567__Port, P6_PIN) == GPIO_PIN_RESET) *pBuf++ = '0';
	else *pBuf++ = '1';
	if(HAL_GPIO_ReadPin(P0123567__Port, P5_PIN) == GPIO_PIN_RESET) *pBuf++ = '0';
	else *pBuf++ = '1';	
	if(HAL_GPIO_ReadPin(P4__Port, P4_PIN) == GPIO_PIN_RESET) *pBuf++ = '0';
	else *pBuf++ = '1';
	if(HAL_GPIO_ReadPin(P0123567__Port, P3_PIN) == GPIO_PIN_RESET) *pBuf++ = '0';
	else *pBuf++ = '1';
	if(HAL_GPIO_ReadPin(P0123567__Port, P2_PIN) == GPIO_PIN_RESET) *pBuf++ = '0';
	else *pBuf++ = '1';
	if(HAL_GPIO_ReadPin(P0123567__Port, P1_PIN) == GPIO_PIN_RESET) *pBuf++ = '0';
	else *pBuf++ = '1';
	if(HAL_GPIO_ReadPin(P0123567__Port, P0_PIN) == GPIO_PIN_RESET) *pBuf++ = '0';
	else *pBuf++ = '1';
	
	*pBuf++ = '.';
	*pBuf++ = 20;
	
	return 32;
}

#define FlashBuffLen Buff_Len
#define text_sett			46
int32_t TexttoBuff(struct LmHandlerAppData_s* appData)
{
	uint8_t *pBuf = appData->Buffer;
	
	uint32_t addr_w;
	uint32_t addr_r = ChooseReadFlashBank(&addr_w);
	uint32_t offset = json_descr[text_sett].offset;
	uint32_t bf_len = json_descr[text_sett + 1].offset - json_descr[text_sett].offset;
	
	static uint8_t temp_buff[50];
	
	//	if(readflash(addr_r, (uint8_t*)temp_buff, FlashBuffLen) > 0) {
	if(readflash_8b(addr_r+offset, temp_buff, bf_len) > 0) {
		memcpy(pBuf, temp_buff, bf_len);
	}
	return bf_len;
}
