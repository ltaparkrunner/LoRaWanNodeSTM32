#ifndef REALISE_SETTINGS_H
#define REALISE_SETTINGS_H

#include <stdint.h>
//typedef enum
//{
//  RFSW_OFF = 0,
//  RFSW_RX,
//  RFSW_RFO_LP,
//  RFSW_RFO_HP,
//  RFSW_RFO_LF,
//} RfSw_TypeDef;

typedef enum
{
  DEVEUI_INDX_1 = 1,
	DEVEUI_INDX_2 = 55,
  APPKEY_INDX_1 = 2,
	APPKEY_INDX_2 = 56,	
	FREQ_INDX	= 		3,
	FR_INDX = 			4,
	UART3_INDX =		5,
	
	HL1USB_INDX_H = 8, //6,
	HL1USB_INDX_T = 12,
	HL1BAT_INDX_H = 14,	//13,
	HL1BAT_INDX_T = 18,
	
	HL2USB_INDX_H = 19,
	HL2USB_INDX_T = 29,
	HL2LORA_INDX_H = 30,
	HL2LORA_INDX_T = 39,
	
	PERIODLORA_INDX_H = 41,	//40,
	PERIODLORA_INDX_T = 45,
	
	LORATEXT_INDX = 	46,
	LORADATA_INDX_H	=		47,
	LORADATA_INDX_T	=		52,
	
	COMMAND_INDX = 		53,
	WRTN_INDX	= 			54
} bl_indxs_t;

#define flash_buff_indx		32
int32_t RealiseSettings(struct buffer_t* buff);
uint32_t ReadPeriod(int32_t ch);
uint32_t ReadBlinkPeriod(int32_t ch);
int32_t ReadAppKey(uint8_t *buf, int32_t ln, uint8_t* bl);
int32_t ChooseChannelFreq(uint8_t enabledChannels[], uint8_t nbEnabledChannels);
int32_t DevEuimem(void);
int32_t readDevEui(uint8_t* tmp);
#endif

