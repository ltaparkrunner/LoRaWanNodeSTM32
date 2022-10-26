/**
  ******************************************************************************
  * @file    stm32l4xx_mu.h
  * @author  MCD Application Team
  * @brief   This file contains definitions for:
  *          - LEDs and push-button available on STM32L4XX-Nucleo Kit
  *            from STMicroelectronics
  *          - LCD, joystick and microSD available on Adafruit 1.8" TFT LCD
  *            shield (reference ID 802), external SMPS
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L4XX_MU_H
#define __STM32L4XX_MU_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32L4XX_MU
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"


/** @defgroup STM32L4XX_MU_Exported_Types Exported Types
  * @{
  */

typedef enum
{
  LED4 = 0,

  LED_GREEN = LED4
}
Led_TypeDef;
/*
typedef enum
{
  LED2 = 0,

  LED_GREEN = LED2
} Led_TypeDef;
*/

typedef enum
{
  BUTTON_USER = 0,
  /* Alias */
  BUTTON_KEY  = BUTTON_USER
} Button_TypeDef;

typedef enum
{
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

/**
  * @}
  */

/** @defgroup STM32L4XX_MU_Exported_Constants Exported Constants
  * @{
  */

/**
* @brief  Define for STM32L4XX_MU board
  */
#if !defined (USE_STM32L4XX_MU)
#define USE_STM32L4XX_MU
#endif

/** @defgroup STM32L4XX_MU_LED LED
  * @{
  */
#define LEDn                               2

#define LED4_PIN                           GPIO_PIN_13
#define LED4_GPIO_PORT                     GPIOB
#define LED4_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOB_CLK_DISABLE()
#define LEDx_GPIO_CLK_ENABLE(__LED__)      do { if((__LED__) == LED4) { LED4_GPIO_CLK_ENABLE(); } } while(0)
#define LEDx_GPIO_CLK_DISABLE(__LED__)     do { if((__LED__) == LED4) { LED4_GPIO_CLK_DISABLE(); } } while(0)

#define LED2_PIN                           GPIO_PIN_5
#define LED2_GPIO_PORT                     GPIOA
#define LED2_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOA_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOA_CLK_DISABLE()
#define LEDx2_GPIO_CLK_ENABLE(__LED__)      do { if((__LED__) == LED2) { LED2_GPIO_CLK_ENABLE(); } } while(0)
#define LEDx2_GPIO_CLK_DISABLE(__LED__)     do { if((__LED__) == LED2) { LED2_GPIO_CLK_DISABLE(); } } while(0)



/**
  * @}
  */

/** @defgroup STM32L4XX_NUCLEO_BUTTON BUTTON
  * @{
  */
#define BUTTONn                            1

/**
  * @brief Key push-button
  */
#define USER_BUTTON_PIN                         GPIO_PIN_13
#define USER_BUTTON_GPIO_PORT                   GPIOC
#define USER_BUTTON_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOC_CLK_ENABLE()
#define USER_BUTTON_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOC_CLK_DISABLE()
#define USER_BUTTON_EXTI_LINE                   GPIO_PIN_13
#define USER_BUTTON_EXTI_IRQn                   EXTI15_10_IRQn
/* Aliases */
#define MU_BUTTON_PIN                          USER_BUTTON_PIN
#define MU_BUTTON_GPIO_PORT                    USER_BUTTON_GPIO_PORT
#define MU_BUTTON_GPIO_CLK_ENABLE()            USER_BUTTON_GPIO_CLK_ENABLE()
#define MU_BUTTON_GPIO_CLK_DISABLE()           USER_BUTTON_GPIO_CLK_DISABLE()
#define MU_BUTTON_EXTI_LINE                    USER_BUTTON_EXTI_LINE
#define MU_BUTTON_EXTI_IRQn                    USER_BUTTON_EXTI_IRQn


#define BUTTONx_GPIO_CLK_ENABLE(__BUTTON__)    do { if((__BUTTON__) == BUTTON_USER) { USER_BUTTON_GPIO_CLK_ENABLE(); } } while(0)

#define BUTTONx_GPIO_CLK_DISABLE(__BUTTON__)   do { if((__BUTTON__) == BUTTON_USER) { USER_BUTTON_GPIO_CLK_DISABLE(); } } while(0)
/**
  * @}
  */

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */
#define MU_SPIx_TIMEOUT_MAX                   1000

/*##################### ADC1 ###################################*/
/**
  * @brief  ADC Interface pins
  *         used for LCD Keyboard Shield buttons
  */
#define NUCLEO_ADCx                                 ADC1
#define NUCLEO_ADCx_CLK_ENABLE()                    __HAL_RCC_ADC_CLK_ENABLE()
#define NUCLEO_ADCx_CLK_DISABLE()                   __HAL_RCC_ADC_CLK_DISABLE()


#define NUCLEO_ADCx_CHANNEL                         ADC_CHANNEL_15
#define NUCLEO_ADCx_GPIO_PORT                       GPIOB
#define NUCLEO_ADCx_GPIO_PIN                        GPIO_PIN_0
#define NUCLEO_ADCx_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
#define NUCLEO_ADCx_GPIO_CLK_DISABLE()              __HAL_RCC_GPIOB_CLK_DISABLE()



/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup STM32L4XX_MU_Exported_Functions
  * @{
  */
//uint32_t         MU_GetVersion(void);

/** @addtogroup STM32L4XX_MU_LED_Functions
  * @{
  */
//void             MU_LED_Init(Led_TypeDef Led);
//void             MU_LED_DeInit(Led_TypeDef Led);
//void             MU_LED_On(Led_TypeDef Led);
//void             MU_LED_Off(Led_TypeDef Led);
//void             MU_LED_Toggle(Led_TypeDef Led);
/**
  * @}
  */

/** @addtogroup STM32L4XX_MU_BUTTON_Functions
  * @{
  */
void             MU_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
//void             MU_PB_DeInit(Button_TypeDef Button);
//uint32_t         MU_PB_GetState(Button_TypeDef Button);

/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


#ifdef __cplusplus
}
#endif

#endif /* __STM32L4XX_MU_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

