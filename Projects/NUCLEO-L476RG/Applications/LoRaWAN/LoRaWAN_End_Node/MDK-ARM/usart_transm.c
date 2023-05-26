#include "usart.h"
#include "dma.h"
#include "settings_json.h"
#include <stdint.h>
#include <string.h>


extern DMA_HandleTypeDef hdma_usart3_rx;
extern uint8_t aRxBuffer[RXBUFFERSIZE];

int32_t Message_parsing(uint32_t length);
	
void USART_transm_Init(void)
{
	
}

void USART_receive_func(void)
{
	if(hdma_usart3_rx.Instance->CNDTR < RXBUFFERSIZE-3)
	{
		// Запускаем или выполняем обработку данных
		Message_parsing(hdma_usart3_rx.Instance->CNDTR);
		//	Запускаем здесь или отдельным потоком??
		// Здесь разбор сообщения
		// Запускаем запись в память
		// Запускаем ответ на сообщение
		// запускаем RESET устройства
	}
}
#include "tiny-json.h"
int32_t Message_parsing(uint32_t length)
{
	json_t pool[ Num_Field ];
	json_t const *settings = json_create((char*)aRxBuffer, pool, Num_Field);
	if (settings == NULL) return -1;
	
	//////////////////
	json_t const* namefield = json_getProperty(settings, "LoRa_text");
	if (namefield == NULL) return -1;
	if (json_getType(namefield) != JSON_TEXT) return -1;
	
	//////////////////
	char const* namevalue = json_getValue(namefield);
	static char name2[150];

  strcpy(name2, namevalue);
	
	return 0;
}

int32_t parseStruct(void)
{
	return 0;
}
