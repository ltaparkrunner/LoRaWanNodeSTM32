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
	

//typedef enum{
//	BUTTON_MODE_GPIO,
//	BUTTON_MODE_EXTI	
//}	ButtonMode_TypeDef;


#define HL1_Pin GPIO_PIN_14   				// —ветодиоды 2 шт. на маленькой плате
#define HL1_GPIO_Port GPIOE
#define HL2_Pin GPIO_PIN_15
#define HL2_GPIO_Port GPIOE

#define SoundSign_Pin GPIO_PIN_7      // ?инамик на плате пульта
#define SoundSign_GPIO_Port GPIOE

#define HL3Sign_Pin GPIO_PIN_4				// Чветодиоды 3 шт. на плате пульта
#define HL3Sign_GPIO_Port GPIOE
#define HL2Sign_Pin GPIO_PIN_5
#define HL2Sign_GPIO_Port GPIOE
#define HL1Sign_Pin GPIO_PIN_6
#define HL1Sign_GPIO_Port GPIOE

#define USB_VBUS_Pin	GPIO_PIN_9
#define USB_VBUS_Port GPIOA
#define USB_VBUS_EXTI_IRQn EXTI9_5_IRQn
//#define VBUS_DET_Pin	GPIO_PIN_13
//#define VBUS_DET_Port GPIOE
//#define VBUS_DET_EXTI_IRQn EXTI15_10_IRQn

/*
#define button_Pin GPIO_PIN_4					// Эт кнопок на LED_Shield 
#define button_GPIO_Port GPIOC				// один аналоговый сигнал на 5 кнопок 
																			// SELECT, LEFT, UP, DOWN, RIGHT
#define Light_Pin GPIO_PIN_13					// ђкл-Эткл подсветки индикатора
#define Light_GPIO_Port GPIOE																			

#define RS_Pin GPIO_PIN_2							// Чигналы управлени§ индикатором
#define RS_GPIO_Port GPIOB						// LED_Shield
#define ENABLE_Pin GPIO_PIN_9
#define ENABLE_GPIO_Port GPIOC
#define D4_Pin GPIO_PIN_11
#define D4_GPIO_Port GPIOB
#define D5_Pin GPIO_PIN_8
#define D5_GPIO_Port GPIOB
#define D6_Pin GPIO_PIN_7
#define D6_GPIO_Port GPIOC
#define D7_Pin GPIO_PIN_6
#define D7_GPIO_Port GPIOC

#define Ard3v3_Pin GPIO_PIN_10				// ¬кл-ќткл питани§ на LED_Shield
#define Ard3v3_GPIO_Port GPIOD				// 3.3в отключить
#define Ard5v_Pin GPIO_PIN_12					// 5v и преобразователь включить
#define Ard5v_GPIO_Port GPIOD
#define LevelPower_Pin GPIO_PIN_14
#define LevelPower_GPIO_Port GPIOD

#define NRST_Pin GPIO_PIN_7						// кнопка RST на LED_Shield
#define NRST_GPIO_Port GPIOD


#define BAT_POWER GPIO_PIN_0					// опросить напр§жение батарейки
#define BAT_POWER_GPIO_Port GPIOB
*/

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

#define LED1_PIN                           HL1_Pin
#define LED1_GPIO_PORT                     HL1_GPIO_Port
#define LED1_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOE_CLK_DISABLE()

#define LED2_PIN                           HL2_Pin
#define LED2_GPIO_PORT                     HL2_GPIO_Port
#define LED2_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOE_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOE_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__LED__)      do { if((__LED__) == HL1) { LED1_GPIO_CLK_ENABLE();  } if((__LED__) == HL2) { LED2_GPIO_CLK_ENABLE();  }} while(0)
#define LEDx_GPIO_CLK_DISABLE(__LED__)     do { if((__LED__) == HL1) { LED1_GPIO_CLK_DISABLE(); } if((__LED__) == HL2) { LED2_GPIO_CLK_DISABLE(); }} while(0)

/**
  * @}
  */

/** @defgroup STM32L4XX_MU_BUTTON BUTTON
  * @{
  */
//#define BUTTONn                            1

/**
  * @brief Key push-button
  */
#define PWR_EN_PIN																GPIO_PIN_11
#define PWR_EN_PORT																GPIOA
#define PWR_EN_CLK_ENABLE()												__HAL_RCC_GPIOA_CLK_ENABLE()
#define PWR_EN_CLK_DISABLE()												__HAL_RCC_GPIOA_CLK_DISABLE()

#define BATMES_PIN																GPIO_PIN_10
#define BATMES_PORT																GPIOA
#define BATMES_CLK_ENABLE()												__HAL_RCC_GPIOA_CLK_ENABLE()
#define BATMES_CLK_DISABLE()												__HAL_RCC_GPIOA_CLK_DISABLE()

#define ADC_BAT_PIN																GPIO_PIN_1
#define ADC_BAT_PORT															GPIOA
#define ADC_BAT_CLK_ENABLE()											__HAL_RCC_GPIOA_CLK_ENABLE()
#define ADC_BAT_CLK_DISABLE()											__HAL_RCC_GPIOA_CLK_DISABLE()

#define ADC_DAT_PIN																GPIO_PIN_0
#define ADC_DAT_PORT															GPIOA
#define ADC_DAT_CLK_ENABLE()											__HAL_RCC_GPIOA_CLK_ENABLE()
#define ADC_DAT_CLK_DISABLE()											__HAL_RCC_GPIOA_CLK_DISABLE()

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
	
typedef enum
{
//  LED1,
//  LED2
		HL1,
		HL2
} Led_TypeDef;

void             MU_LED_Init(Led_TypeDef Led);
void             MU_LED_DeInit(Led_TypeDef Led);
void             MU_LED_On(Led_TypeDef Led);
void             MU_LED_Off(Led_TypeDef Led);
void             MU_LED_Toggle(Led_TypeDef Led);
void MU_Sound_Init(void);
void MU_Sound_DeInit(void);
void MU_board_LEDs_Init(void);
void MU_board_LEDs_DeInit(void);
void MU_board_USB_detect_Init(void);

/**
  * @}
  */

/** @addtogroup STM32L4XX_SHIELD_RST_Functions
  * @{
  */
//void MU_PB_Init(ButtonMode_TypeDef ButtonMode);	//Button_TypeDef Button,
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

