#include <stdint.h>
#include <string.h>
//#include <ctype.h>
#include "tiny-json.h"
#include "settings_json.h"

extern char sets_JSON[];
extern json_t pool[ Num_Field ];
extern json_t const *settings;
extern json_t const* namefield;
extern char const* namevalue;
static char name2[] = "Soon as early Dawn appeared";
int32_t fetch_text()
{
	settings = json_create(sets_JSON, pool, Num_Field);
	if (settings == NULL) return -1;
	
	///////////////
	
	namefield = json_getProperty(settings, "LoRa_text");
	if (namefield == NULL) return -1;
	if (json_getType(namefield) != JSON_TEXT) return -1;
	
	//////////////////
	namevalue = json_getValue(namefield);
	

  strcpy(name2, namevalue);
	//HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pHAL_UART_Transmit(&huart3, (uint8_t *)name2, 100, 800);
	return 0;
}

uint32_t fillBuff(uint8_t* buff)
{
	//fetch_text();
	uint32_t i;
	for(i = 0; name2[i] != 0; i++)
	{
		buff[i] = name2[i];
		if(i>200) break; 
	}
	return i;
}
