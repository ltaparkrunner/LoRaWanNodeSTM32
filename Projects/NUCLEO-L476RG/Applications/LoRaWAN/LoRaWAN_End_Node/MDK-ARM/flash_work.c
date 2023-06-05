#include "stm32l4xx_mu.h"

#include "tiny-json.h"
#include "tiny-json_extra.h"
#include <string.h>
#include "settings_json.h"

#include "flash_mem.h"

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
