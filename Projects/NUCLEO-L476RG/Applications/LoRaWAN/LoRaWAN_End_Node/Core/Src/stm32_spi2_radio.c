#include "MU_l476vg_spi.h"
#include "MU_l476vg_errno.h"
#include "stm32l4xx_hal.h"


static void SPI2_MspInit(SPI_HandleTypeDef *spiHandle);
static void SPI2_MspDeInit(SPI_HandleTypeDef *spiHandle);
HAL_StatusTypeDef MX_SPI2_Init(SPI_HandleTypeDef *hspi);
static uint32_t SPI_GetPrescaler(uint32_t clock_src_hz, uint32_t baudrate_mbps);

#define TIMEOUT_DURATION 1000

SPI_HandleTypeDef hspi2;

int32_t MU_SPI2_Init(void)
{
  int32_t ret = MU_ERROR_NONE;

  hspi2.Instance  = SPI2;
  if (HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_RESET)
  {
//#if (USE_HAL_SPI_REGISTER_CALLBACKS == 0)
    /* Init the SPI Msp */
    SPI2_MspInit(&hspi2);
/*#else
    if (IsSPI2MspCbValid == 0U)
    {
      if MU_SPI2_RegisterDefaultMspCallbacks() != MU_ERROR_NONE)
      {
        return MU_ERROR_MSP_FAILURE;
      }
    }
#endif
*/
    /* Init the SPI */
    if (MX_SPI2_Init(&hspi2) != HAL_OK)
    {
      ret = MU_ERROR_SPI_FAILURE;
    }
  }

  return ret;
}

/**
  * @brief  DeInitializes SPI HAL.
  * @retval None
  * @retval BSP status
  */
int32_t MU_SPI2_DeInit(void)
{
  int32_t ret = MU_ERROR_SPI_FAILURE;

//#if (USE_HAL_SPI_REGISTER_CALLBACKS == 0)
  SPI2_MspDeInit(&hspi2);
//#endif

  if (HAL_SPI_DeInit(&hspi2) == HAL_OK)
  {
    ret = MU_ERROR_NONE;
  }

  return ret;
}
int32_t MU_GetTick(void)
{
  return HAL_GetTick();
}

/* SPI2 init function */
static uint32_t SpiFrequency( uint32_t hz )
{
  uint32_t divisor = 0;
  uint32_t SysClkTmp = SystemCoreClock;
  uint32_t baudRate;
  
  while( SysClkTmp > hz)
  {
    divisor++;
    SysClkTmp= ( SysClkTmp >> 1);
    
    if (divisor >= 7)
      break;
  }
  
  baudRate =((( divisor & 0x4 ) == 0 )? 0x0 : SPI_CR1_BR_2  )| 
            ((( divisor & 0x2 ) == 0 )? 0x0 : SPI_CR1_BR_1  )| 
            ((( divisor & 0x1 ) == 0 )? 0x0 : SPI_CR1_BR_0  );
  
  return baudRate;
}


HAL_StatusTypeDef MX_SPI2_Init(SPI_HandleTypeDef *hspi)
{
  HAL_StatusTypeDef ret = HAL_OK;
  hspi->Instance = SPI2;
  hspi->Init.Mode = SPI_MODE_MASTER;
  hspi->Init.Direction = SPI_DIRECTION_2LINES;
  hspi->Init.DataSize = SPI_DATASIZE_8BIT;
  hspi->Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi->Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi->Init.NSS = SPI_NSS_SOFT;
  /* SPI2 is on APB2 for L0x3 -> HAL_RCC_GetPCLK2Freq */
  //hspi->Init.BaudRatePrescaler = SPI_GetPrescaler(HAL_RCC_GetPCLK2Freq(), RADIO_SPI_BAUDRATE);
	hspi->Init.BaudRatePrescaler = SpiFrequency( 10000000 );
  hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi->Init.TIMode = SPI_TIMODE_DISABLE;
  hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi->Init.CRCPolynomial = 7;
	
	__HAL_RCC_SPI2_CLK_ENABLE();
	
  if (HAL_SPI_Init(hspi) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}

static void SPI2_MspInit(SPI_HandleTypeDef *spiHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /* USER CODE BEGIN SPI2_MspInit 0 */

  /* USER CODE END SPI2_MspInit 0 */
  /* Enable Peripheral clock */
  __HAL_RCC_SPI2_CLK_ENABLE();

  /**SPI2 GPIO Configuration
  PB10     ------> SPI2_SCK
  PC2      ------> SPI2_MISO
  PC3      ------> SPI2_MOSI
	PB9			 ------> SPI2_NSS
    */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = MU_SPI2_MOSI_GPIO_AF;
  GPIO_InitStruct.Pin = MU_SPI2_MOSI_GPIO_PIN;
  HAL_GPIO_Init(MU_SPI2_MOSI_GPIO_PORT, &GPIO_InitStruct);
	
  GPIO_InitStruct.Alternate = MU_SPI2_MISO_GPIO_AF;
  GPIO_InitStruct.Pin = MU_SPI2_MISO_GPIO_PIN;
  HAL_GPIO_Init(MU_SPI2_MISO_GPIO_PORT, &GPIO_InitStruct);
	
  GPIO_InitStruct.Alternate = MU_SPI2_SCK_GPIO_AF;
  GPIO_InitStruct.Pin = MU_SPI2_SCK_GPIO_PIN;
  HAL_GPIO_Init(MU_SPI2_SCK_GPIO_PORT, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI2_MspInit 1 */
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Pin = MU_SPI2_NSS_GPIO_PIN;
	HAL_GPIO_Init(MU_SPI2_NSS_GPIO_PORT, &GPIO_InitStruct);
  /* USER CODE END SPI2_MspInit 1 */
}

static void SPI2_MspDeInit(SPI_HandleTypeDef *spiHandle)
{
  /* USER CODE BEGIN SPI2_MspDeInit 0 */

  /* USER CODE END SPI2_MspDeInit 0 */
  /* Peripheral clock disable */
  __HAL_RCC_SPI2_CLK_DISABLE();

  /**SPI2 GPIO Configuration
  PB10     ------> SPI2_SCK
  PC2      ------> SPI2_MISO
  PC3      ------> SPI2_MOSI
    */
  HAL_GPIO_DeInit(MU_SPI2_MOSI_GPIO_PORT, MU_SPI2_MOSI_GPIO_PIN);
  HAL_GPIO_DeInit(MU_SPI2_MISO_GPIO_PORT, MU_SPI2_MISO_GPIO_PIN);
  HAL_GPIO_DeInit(MU_SPI2_SCK_GPIO_PORT, MU_SPI2_SCK_GPIO_PIN);

  /* USER CODE BEGIN SPI2_MspDeInit 1 */

  /* USER CODE END SPI2_MspDeInit 1 */
}


/**
  * @brief  Write Data through SPI BUS.
  * @param  pData: Data
  * @param  len: Length of data in byte
  * @retval BSP status
  */
int32_t MUPI2_Send(uint8_t *pData, uint16_t len)
{
  int32_t ret = MU_ERROR_UNKNOWN_FAILURE;

  if (HAL_SPI_Transmit(&hspi2, pData, len, TIMEOUT_DURATION) == HAL_OK)
  {
    ret = len;
  }
  return ret;
}

/**
  * @brief  Receive Data from SPI BUS
  * @param  pData: Data
  * @param  len: Length of data in byte
  * @retval BSP status
  */
int32_t  MU_SPI2_Recv(uint8_t *pData, uint16_t len)
{
  int32_t ret = MU_ERROR_UNKNOWN_FAILURE;

  if (HAL_SPI_Receive(&hspi2, pData, len, TIMEOUT_DURATION) == HAL_OK)
  {
    ret = len;
  }
  return ret;
}

/**
  * @brief  Send and Receive data to/from SPI BUS (Full duplex)
  * @param  pData: Data
  * @param  len: Length of data in byte
  * @retval BSP status
  */
int32_t MU_SPI2_SendRecv(uint8_t *pTxData, uint8_t *pRxData, uint16_t len)
{
  int32_t ret = MU_ERROR_UNKNOWN_FAILURE;

  if (HAL_SPI_TransmitReceive(&hspi2, pTxData, pRxData, len, TIMEOUT_DURATION) == HAL_OK)
  {
    ret = len;
  }
  return ret;
}


static uint32_t SPI_GetPrescaler(uint32_t clock_src_hz, uint32_t baudrate_mbps)
{
  uint32_t divisor = 0;
  uint32_t spi_clk = clock_src_hz;
  uint32_t presc = 0;

  static const uint32_t baudrate[] =
  {
    SPI_BAUDRATEPRESCALER_2,
    SPI_BAUDRATEPRESCALER_4,
    SPI_BAUDRATEPRESCALER_8,
    SPI_BAUDRATEPRESCALER_16,
    SPI_BAUDRATEPRESCALER_32,
    SPI_BAUDRATEPRESCALER_64,
    SPI_BAUDRATEPRESCALER_128,
    SPI_BAUDRATEPRESCALER_256,
  };

  while (spi_clk > baudrate_mbps)
  {
    presc = baudrate[divisor];
    if (++divisor > 7)
    {
      break;
    }

    spi_clk = (spi_clk >> 1);
  }

  return presc;
}
