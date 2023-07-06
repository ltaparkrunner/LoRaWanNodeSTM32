#include <stdint.h>
#include "flash_mem.h"
#include "tiny-json_extra.h"
#include "realise_settings.h"
#include "lora_app.h"
#include "LmHandler.h"

extern struct buffer_t buff;
extern struct field_json json_descr[];
#define probes_sett_head		48
#define probes_sett_tail		53

int32_t Count_Indexes(int32_t buff_i[], int32_t max_len)
{
	buff_i[0] = json_descr[DEVEUI_INDX_1].offset;
	buff_i[1] = json_descr[DEVEUI_INDX_1].offset + json_descr[DEVEUI_INDX_1].bytes - 1;
	buff_i[2] = json_descr[DEVEUI_INDX_2].offset;
	
	buff_i[3] = json_descr[APPKEY_INDX_1].offset;
	buff_i[4] = json_descr[APPKEY_INDX_1].offset + json_descr[APPKEY_INDX_1].bytes - 1;
	buff_i[5] = json_descr[APPKEY_INDX_2].offset;
	
	buff_i[6] = json_descr[FREQ_INDX].offset;
	buff_i[7] = json_descr[FR_INDX].offset;
	buff_i[8] = json_descr[UART3_INDX].offset;
	
	buff_i[9] = json_descr[HL1USB_INDX_H].offset;
	buff_i[10] = json_descr[HL1USB_INDX_T].offset;
	
	buff_i[11]  = json_descr[HL1BAT_INDX_H].offset;
	buff_i[12] = json_descr[HL1BAT_INDX_T].offset;	
	
	buff_i[13] = json_descr[HL2USB_INDX_H].offset;
	buff_i[14] = json_descr[HL2USB_INDX_T].offset;	
	
	buff_i[15] = json_descr[HL2LORA_INDX_H].offset;
	buff_i[16] = json_descr[HL2LORA_INDX_T].offset;
	
	buff_i[17] = json_descr[PERIODLORA_INDX_H].offset;
	buff_i[18] = json_descr[PERIODLORA_INDX_T].offset;
	
	buff_i[19] = json_descr[LORATEXT_INDX].offset;
	buff_i[20] = json_descr[LORATEXT_INDX].offset + json_descr[LORATEXT_INDX].bytes - 1;
	
	buff_i[21] = json_descr[LORADATA_INDX_H].offset;
	buff_i[22] = json_descr[LORADATA_INDX_T].offset;
	
	buff_i[23] = json_descr[COMMAND_INDX].offset;
	buff_i[24] = json_descr[WRTN_INDX].offset;
	
	return 24;
}

int32_t Count_TChanged(int32_t tch[], struct buffer_t* buf)
{
	int32_t i1 = 0;
	for(i1 = json_descr[HL1USB_INDX_H].offset; i1 < json_descr[HL1USB_INDX_T].offset; i1++)
	{
		if(buf->changed[i1] == 1)
		{
			tch[0] = 1;
			break;
		}
	}
	for(i1 = json_descr[HL1BAT_INDX_H].offset; i1 < json_descr[HL1BAT_INDX_T].offset; i1++)
	{
		if(buf->changed[i1] == 1)
		{
			tch[1] = 1;
			break;
		}
	}
	for(i1 = json_descr[HL2USB_INDX_H].offset; i1 < json_descr[HL2USB_INDX_T].offset; i1++)
	{
		if(buf->changed[i1] == 1)
		{
			tch[2] = 1;
			break;
		}
	}
	for(i1 = json_descr[HL2LORA_INDX_H].offset; i1 < json_descr[HL2LORA_INDX_T].offset; i1++)
	{
		if(buf->changed[i1] == 1)
		{
			tch[3] = 1;
			break;
		}
	}
	for(i1 = json_descr[PERIODLORA_INDX_H].offset; i1 < json_descr[PERIODLORA_INDX_T].offset; i1++)
	{
		if(buf->changed[i1] == 1)
		{
			tch[4] = 1;
			break;
		}
	}
	if(tch[0] || tch[1] || tch[2] || tch[3] || tch[4]) return 1;
		else return 0;
}

uint32_t Count_interval(int32_t indx, struct buffer_t* buf)
{
	uint32_t ret = 	((uint32_t)buf->array[json_descr[indx].offset] + 
									((uint32_t)buf->array[json_descr[indx+1].offset] +
									((uint32_t)buf->array[json_descr[indx+2].offset] +
									 (uint32_t)buf->array[json_descr[indx+3].offset] * 24) * 60) * 60) * 1000;
	return ret;
}

uint32_t Count_duration(int32_t indx, struct buffer_t* buf)
{
	uint32_t ret = 	((uint32_t)buf->array[json_descr[indx].offset]<< 8) + buf->array[json_descr[indx].offset + 1];
	return ret;
}

uint32_t CountSecureChanged(struct buffer_t* buf)
{
	int32_t i1;
	for(i1 = json_descr[DEVEUI_INDX_1].offset; i1 < json_descr[FREQ_INDX].offset + json_descr[FREQ_INDX].bytes - 1; i1++)
	{
		if(buf->changed[i1] == 1)
		{
			return 1;
		}
	}
	if(buf->changed[json_descr[DEVEUI_INDX_2].offset] == 1 || buf->changed[json_descr[APPKEY_INDX_2].offset] == 1)
		return 1;
	return 0;
}

int32_t RealiseSettings(struct buffer_t* buff)
{
//	int32_t maxrec = json_descr[Json_Descript_Length - 1].offset + json_descr[Json_Descript_Length - 1].bytes -1;
	static int32_t buff_indx[flash_buff_indx];
	
	Count_Indexes(buff_indx, flash_buff_indx);
//	int32_t i1 = 0;
	int32_t tchanged[5] = {0};
	Count_TChanged(tchanged, buff);
	if(tchanged[0] == 1 && HAL_GPIO_ReadPin(USB_VBUS_Port, USB_VBUS_Pin) == GPIO_PIN_SET) {  	// USB power blink
		// blink right away, and tune the next time
		// 
		if(buff->array[json_descr[HL1USB_INDX_T].offset] == truefl)
		{
			uint32_t interval = Count_interval(HL1USB_INDX_H, buff);
			Change_power_blink(interval);
		}
		else Change_power_blink(0);
	}
	if(tchanged[1] == 1 && HAL_GPIO_ReadPin(USB_VBUS_Port, USB_VBUS_Pin) == GPIO_PIN_RESET){		// battery power blink
		if(buff->array[json_descr[HL1BAT_INDX_T].offset] == truefl)
		{
			uint32_t interval = Count_interval(HL1BAT_INDX_H, buff);
			Change_power_blink(interval);
		}
		else Change_power_blink(0);
	}
	if(tchanged[2] == 1){		// USB transmission blink
		uint32_t duration = Count_duration(HL2USB_INDX_H, buff);		
		ChangeUSB_transm_blink(duration);
	}
	if(tchanged[3] == 1){		// LORA transmission blink
		uint32_t duration = Count_duration(HL2LORA_INDX_H, buff);		
		ChangeLORA_transm_blink(duration);
	}
	if(tchanged[4] == 1){		// LORA transmission period 
		if(buff->array[json_descr[PERIODLORA_INDX_T].offset] == truefl)
		{
			uint32_t interval = Count_interval(PERIODLORA_INDX_H, buff);
			ChangeLORA_transm_period(interval);
		}
		else ChangeLORA_transm_period(0);
	}
	if(CountSecureChanged(buff)) LmHandlerReConfigure();
	return 0;
}



//int32_t ChangeProbesSettings(void)
//{
//	uint32_t addr_w;
//	uint32_t addr_r = ChooseReadFlashBank(&addr_w);
//	uint32_t offset = json_descr[probes_sett_head].offset;
//	uint32_t bf_len = json_descr[probes_sett_tail].offset + json_descr[probes_sett_tail].bytes;
//	
//	static uint8_t temp_buff[10];
//	
//	//	if(readflash(addr_r, (uint8_t*)temp_buff, FlashBuffLen) > 0) {
//	if(readflash_8b(addr_r+offset, temp_buff, bf_len) > 0) {
//		for(int32_t i1 = 0; i1 < bf_len; i1++)
//			if(temp_buff[i1] == truefl) {
//				switch(i1)
//				{
//					case 0:		//	"AD_1"
//					{

//				break;
//			}
//	}
//	return 0;
//}

uint32_t ReadBlinkPeriod(int32_t ch)
{
	uint32_t addr_w;
	uint32_t addr_r = ChooseReadFlashBank(&addr_w);
	uint32_t offset;
	int32_t len;
	uint8_t buf[10] ={0};
	uint32_t ret = 0;
	if(ch == 0) {
		offset = json_descr[HL2USB_INDX_H].offset;
		len = json_descr[HL2USB_INDX_T].offset - json_descr[HL2USB_INDX_H].offset + 1;
	}
	else {
		offset = json_descr[HL2LORA_INDX_H].offset;
		len = json_descr[HL2LORA_INDX_T].offset - json_descr[HL2LORA_INDX_H].offset + 1;
	}
	readflash_8b(addr_r + offset , buf, len);
	if(buf[2] > 0)
		ret = 	((uint32_t)buf[0]<<8) + buf[1];
	
	return ret;
}

//uint32_t ReadDuration(int32_t ch)
uint32_t ReadPeriod(int32_t ch)
{
	uint32_t addr_w;
	uint32_t addr_r = ChooseReadFlashBank(&addr_w);
	uint32_t offset;
	int32_t len;
	uint8_t buf[10] = {0};
	uint32_t ret = 0;
	if(ch == 0) {
		offset = json_descr[HL1USB_INDX_H].offset;
		len = json_descr[HL1USB_INDX_T].offset - json_descr[HL1USB_INDX_H].offset + 1;
	}
	else if(ch == 1){
		offset = json_descr[HL1BAT_INDX_H].offset;
		len =json_descr[HL1BAT_INDX_T].offset -  json_descr[HL1BAT_INDX_H].offset + 1;
	}
	else {
		offset = json_descr[PERIODLORA_INDX_H].offset;
		len =json_descr[PERIODLORA_INDX_T].offset -  json_descr[PERIODLORA_INDX_H].offset + 1;
	}
	readflash_8b(addr_r + offset , buf, len);
	if(buf[4] == truefl)
		ret = 	((uint32_t)buf[0] + ((uint32_t)buf[1] + ((uint32_t)buf[2] +
									 (uint32_t)buf[3] * 24) * 60) * 60) * 1000;
	return ret;
}

int32_t ReadAppKey(uint8_t *buf, int32_t ln, uint8_t* bl)
{
	uint32_t addr_w;
	uint32_t addr_r = ChooseReadFlashBank(&addr_w);
	uint32_t offset;
	int32_t len;
//	uint8_t buf[10];
//	uint32_t ret = 0;
	
	offset = json_descr[APPKEY_INDX_1].offset;
	len = json_descr[APPKEY_INDX_1].bytes;
	if(len >= ln) {
		readflash_8b(addr_r + offset , buf, len);
		
		offset = json_descr[APPKEY_INDX_2].offset;
		readflash_8b(addr_r + offset , bl, 1);
		return len + 1;
	}
	else return 0;
}

int32_t ChooseChannelFreq(uint8_t enabledChannels[], uint8_t nbEnabledChannels)
{
	uint32_t addr_w;
	uint32_t addr_r = ChooseReadFlashBank(&addr_w);
	uint32_t offset;
	int32_t len;
	
	offset = json_descr[FREQ_INDX].offset;
	len = json_descr[FREQ_INDX].bytes;
	
	uint8_t buf[4];
	if(len <= 4)
		readflash_8b(addr_r + offset , buf, len);
	
	uint32_t tmpfreq = (((((buf[0] << 8) + buf[1]) << 8) + buf[2]) << 8) + buf[3];
	
	if(tmpfreq == 869100000 && nbEnabledChannels == 2) {
		return enabledChannels[1];
	}
	else return enabledChannels[0];
		
}

int32_t DevEuimem(void)
{
	uint32_t addr_w;
	uint32_t addr_r = ChooseReadFlashBank(&addr_w);
	uint32_t offset = json_descr[DEVEUI_INDX_2].offset;
	uint8_t bl;
	readflash_8b(addr_r + offset , &bl, 1);
	if(bl == truefl) return 1;
	else return 0;
}

int32_t readDevEui(uint8_t* tmp)
{
	uint32_t addr_w;
	uint32_t addr_r = ChooseReadFlashBank(&addr_w);
	uint32_t offset = json_descr[DEVEUI_INDX_1].offset;
	uint32_t ln = json_descr[DEVEUI_INDX_1].bytes;

	readflash_8b(addr_r + offset , tmp, ln);
	return ln;
}
