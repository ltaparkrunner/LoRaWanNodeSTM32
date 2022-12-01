
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MU_L476VG_SPI_H
#define __MU_L476VG_SPI_H

//#include "stm32l4xx_nucleo_conf.h"
//#include "nucleo_l476rg_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef USE_HAL_SPI_REGISTER_CALLBACKS
#define USE_HAL_SPI_REGISTER_CALLBACKS 0U
#endif
#ifndef USE_HAL_I2C_REGISTER_CALLBACKS
#define USE_HAL_I2C_REGISTER_CALLBACKS 0U
#endif

/* Needed for SPI_GetPrescaler */
#define RADIO_SPI_BAUDRATE                  10000000U /* 16M Sigfox, 10M Lora */

/* Ticket https://intbugzilla.st.com/show_bug.cgi?id=54043 */
#define MU_SPI2_SCK_GPIO_PIN            GPIO_PIN_10
#define MU_SPI2_MISO_GPIO_PIN           GPIO_PIN_2
#define MU_SPI2_MOSI_GPIO_PIN           GPIO_PIN_3
#define MU_SPI2_SCK_GPIO_PORT           GPIOB
#define MU_SPI2_MISO_GPIO_PORT          GPIOC
#define MU_SPI2_MOSI_GPIO_PORT          GPIOC
#define MU_SPI2_SCK_GPIO_AF             GPIO_AF5_SPI2
#define MU_SPI2_MOSI_GPIO_AF            GPIO_AF5_SPI2
#define MU_SPI2_MISO_GPIO_AF            GPIO_AF5_SPI2
#define MU_SPI2_SCK_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define MU_SPI2_MOSI_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()
#define MU_SPI2_MISO_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()

#define MU_SPI2_NSS_GPIO_PIN						GPIO_PIN_9
#define MU_SPI2_NSS_GPIO_PORT						GPIOB

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
typedef struct
{
  pI2C_CallbackTypeDef  pMspI2cInitCb;
  pI2C_CallbackTypeDef  pMspI2cDeInitCb;
} BSP_I2C_Cb_t;
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1) */

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
typedef struct
{
  pSPI_CallbackTypeDef  pMspSpiInitCb;
  pSPI_CallbackTypeDef  pMspSpiDeInitCb;
} BSP_SPI_Cb_t;
#endif /* (USE_HAL_SPI_REGISTER_CALLBACKS == 1) */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* BUS IO driver over SPI Peripheral */
int32_t MU_SPI2_Init(void);
int32_t MU_SPI2_DeInit(void);
int32_t MU_SPI2_Send(uint8_t *pData, uint16_t len);
int32_t MU_SPI2_Recv(uint8_t *pData, uint16_t len);
int32_t MU_SPI2_SendRecv(uint8_t *pTxData, uint8_t *pRxData, uint16_t len);

int32_t MU_GetTick(void);

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1)
int32_t BSP_BUS_RegisterDefaultMspCallbacks(void);
int32_t BSP_BUS_RegisterMspCallbacks(BSP_BUS_Cb_t *Callbacks);
#endif 


#ifdef __cplusplus
}
#endif

#endif /* ____MU_L476VG_SPI_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
