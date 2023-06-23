#include "tiny-json.h"
#include "tiny-json_extra.h"
#include <string.h>
#include "settings_json.h"

#include "flash_mem.h"
#include "parseMessage.h"

struct json_arr json_buff, *pjson_buff = &json_buff;

//int32_t init_settings(struct json_arr* p_json_buffS)
//{
//	// read first bite of flash, to understand either flash have been written before or not

//	uint32_t addr = ADDR_FLASH_PAGE_0 + FLASH_PAGE_SIZE * FLASH_PAGE_FOR_SETTINGS;
//	uint8_t wrtn = *(__IO uint8_t *)addr;
//	if(wrtn = ){
//		json_to_buffer
//	}
//}

extern struct buffer_t buff;
uint8_t WriteBufferToFlash_wrap(uint8_t json_res)
{
	static int32_t res = 1;
	if(json_res == PARSED_JSON_OK){
		res = WriteBufferToFlash(&buff);
		if(res > 0) {
			CancelJSONChanges(&buff);
			return WRITTEN_FLASH_OK;
		}
		else return WRITTEN_FLASH_FAIL;
	}
	else if(json_res == NO_PARSED_JSON && res <= 0){
		res = FinilizeWriteBufferToFlash(&buff);
		if(res > 0){
			CancelJSONChanges(&buff);
			return WRITTEN_FLASH_OK;
		}
		else return WRITTEN_FLASH_FAIL;
	}
	else return NOTHING_TO_WRITE;
	
}

uint8_t buffer2[Buff_Len];
extern struct json_arr *jsonarrflash_1, *jsonarrflash_2;

int32_t WriteBufferToFlash(struct buffer_t* buff)
{
	int32_t changed = 0;
	
	// Read the number WRTN to understand what the record on flash have been made later? 
//	uint32_t Address_w = FLASH_USER_START_ADDR1;
//	uint8_t wrtn1 = *(__IO uint8_t *)(Address_w + WRTN_OFFSET);
//	uint32_t Address_r = FLASH_USER_START_ADDR2;
//	uint8_t wrtn2 = *(__IO uint8_t *)(Address_r + WRTN_OFFSET);
//	if(wrtn2 != 0xff && wrtn2 < wrtn1) {
//		Address_w = FLASH_USER_START_ADDR2;
//		Address_r = FLASH_USER_START_ADDR1;
//	}
	uint32_t Address_w;
	uint32_t Address_r = ChooseReadFlashBank(&Address_w);
	struct json_arr *jsonarrflash_r, *jsonarrflash_w;
	//////////////////////////
	//jsonarrflash_r = (struct json_arr*)Address_r; 
	//jsonarrflash_w = (struct json_arr*)Address_w;

	/////////////////
	for(int32_t i1 = 0; i1 < Buff_Len; i1++) {
		if(buff->changed[i1] == 1) {
			buffer2[i1] = buff->array[i1];
			changed = 1;
		}
		else {
			buffer2[i1] = *(__IO uint8_t *)(Address_r++);
		}
	}
	
	if(changed == 1){
		if(buff->array[WRTN_OFFSET] > 0x7F || buff->array[WRTN_OFFSET] < 0)
			buffer2[WRTN_OFFSET] = 0;
		else  buffer2[WRTN_OFFSET] = buff->array[WRTN_OFFSET] + 1;
		return rewriteflash(Address_w, buffer2, Buff_Len);
	}
	else return 0;
}

int32_t FinilizeWriteBufferToFlash(struct buffer_t* buff)
{
	uint32_t Address_w = FLASH_USER_START_ADDR1;
	uint8_t wrtn1 = *(__IO uint8_t *)(Address_w + WRTN_OFFSET);
	Address_w = FLASH_USER_START_ADDR2;
	uint8_t wrtn2 = *(__IO uint8_t *)(Address_w + WRTN_OFFSET);
	if(wrtn2 < wrtn1 || wrtn2 == 0xff) {
		Address_w = FLASH_USER_START_ADDR1;
	}
	return rewriteflash(Address_w, buffer2, Buff_Len);
}

int32_t CancelJSONChanges(struct buffer_t* buff)
{
	for(int32_t i1 = 0; i1 < Buff_Len; i1++) {
		buff->changed[i1] = 0;
	}
	return 0;
}

uint32_t ChooseReadFlashBank(uint32_t* Addr_w)
{
	uint32_t Address_w = FLASH_USER_START_ADDR1;
	uint8_t wrtn1 = *(__IO uint8_t *)(Address_w + WRTN_OFFSET);
	uint32_t Address_r = FLASH_USER_START_ADDR2;
	uint8_t wrtn2 = *(__IO uint8_t *)(Address_r + WRTN_OFFSET);
	if(wrtn1 == 0xff || wrtn2 > wrtn1) {
		if(Addr_w) *Addr_w = FLASH_USER_START_ADDR1;
		return (uint32_t) FLASH_USER_START_ADDR2;
	}
	else{
		if(Addr_w) *Addr_w = FLASH_USER_START_ADDR2;
		return (uint32_t) FLASH_USER_START_ADDR1;
	}
			
}
