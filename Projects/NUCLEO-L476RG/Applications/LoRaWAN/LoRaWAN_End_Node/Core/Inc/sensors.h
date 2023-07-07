#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include "LmHandlerTypes.h"
#include "tiny-json_extra.h"

int32_t CollectData(struct LmHandlerAppData_s* appData);
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc);
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc);
static void MX_ADC2_Init(uint32_t channel);


int32_t ADCtoBuff(struct LmHandlerAppData_s* appData, int32_t ch);
int32_t MX_DIG_Init(void);
int32_t MX_Dig_DeInit(void);
int32_t DigtoBuff(struct LmHandlerAppData_s* appData);
int32_t TexttoBuff(struct LmHandlerAppData_s* appData);

#define P0_PIN GPIO_PIN_1
#define P0123567__Port GPIOA
#define P1_PIN GPIO_PIN_0
#define P1__Port GPIOA
#define P2_PIN GPIO_PIN_3
#define P2__Port GPIOA
#define P3_PIN GPIO_PIN_2
#define P3__Port GPIOA
#define P4_PIN GPIO_PIN_0
#define P4__Port GPIOB
#define P5_PIN GPIO_PIN_5
#define P5__Port GPIOA
#define P6_PIN GPIO_PIN_6
#define P6__Port GPIOA
#define P7_PIN GPIO_PIN_7
#define P7__Port GPIOA

#endif /* SENSOR_H */
