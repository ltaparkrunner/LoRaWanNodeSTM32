#ifndef FLASH_MEM_H
#define FLASH_MEM_H

//#include "stm32l4xx_hal_def.h"
#include "stm32l4xx_mu.h"
#include "settings_json.h"
//#include <stdint.h>
//

#define ADDR_FLASH_PAGE_0    	((uint32_t)0x08000000) /* Base @ of Page 0, 2 Kbytes */
#define ADDR_FLASH_PAGE_16    ((uint32_t)0x08008000) /* Base @ of Page 16, 2 Kbytes */
#define ADDR_FLASH_PAGE_17    ((uint32_t)0x08008800) /* Base @ of Page 17, 2 Kbytes */
#define ADDR_FLASH_PAGE_18    ((uint32_t)0x08009000) /* Base @ of Page 18, 2 Kbytes */
#define ADDR_FLASH_PAGE_19    ((uint32_t)0x08009800) /* Base @ of Page 19, 2 Kbytes */
#define ADDR_FLASH_PAGE_20    ((uint32_t)0x0800A000) /* Base @ of Page 20, 2 Kbytes */
#define ADDR_FLASH_PAGE_21    ((uint32_t)0x0800A800) /* Base @ of Page 21, 2 Kbytes */
#define ADDR_FLASH_PAGE_98    ((uint32_t)0x08031000) /* Base @ of Page 98, 2 Kbytes */
#define ADDR_FLASH_PAGE_99    ((uint32_t)0x08031800) /* Base @ of Page 99, 2 Kbytes */
#define ADDR_FLASH_PAGE_220   ((uint32_t)0x0806e000) /* Base @ of Page 220, 2 Kbytes */
#define ADDR_FLASH_PAGE_221   ((uint32_t)0x0806e800) /* Base @ of Page 221, 2 Kbytes */
#define ADDR_FLASH_PAGE_255   ((uint32_t)0x0807f800) /* Base @ of Page 255, 2 Kbytes */
#define ADDR_FLASH_PAGE_256   ((uint32_t)0x08080000) /* Base @ of Page 256, 2 Kbytes */
#define ADDR_FLASH_PAGE_384   ((uint32_t)0x080c0000) /* Base @ of Page 384, 2 Kbytes */
#define ADDR_FLASH_PAGE_385   ((uint32_t)0x080c0800) /* Base @ of Page 385, 2 Kbytes */
#define ADDR_FLASH_PAGE_386   ((uint32_t)0x080c1000) /* Base @ of Page 386, 2 Kbytes */
#define ADDR_FLASH_PAGE_389   ((uint32_t)0x080c2800) /* Base @ of Page 389, 2 Kbytes */
#define ADDR_FLASH_PAGE_390   ((uint32_t)0x080c3000) /* Base @ of Page 390, 2 Kbytes */
#define ADDR_FLASH_PAGE_391   ((uint32_t)0x080c3800) /* Base @ of Page 391, 2 Kbytes */
#define ADDR_FLASH_PAGE_392   ((uint32_t)0x080c4000) /* Base @ of Page 392, 2 Kbytes */

#define ADDR_FLASH_PAGE_401   ((uint32_t)0x080c8800) /* Base @ of Page 392, 2 Kbytes */
#define ADDR_FLASH_PAGE_402   ((uint32_t)0x080c9000) /* Base @ of Page 392, 2 Kbytes */
#define ADDR_FLASH_PAGE_403   ((uint32_t)0x080c9800) /* Base @ of Page 392, 2 Kbytes */

#define ADDR_FLASH_PAGE_511   ((uint32_t)0x080ff800) /* Base @ of Page 511, 2 Kbytes */

#define DATA_32                 ((uint32_t)0x12345678)
#define DATA_64                 ((uint64_t)0x1234567812345678)

#define FLASH_ROW_SIZE          32
#define FLASH_PAGE_FOR_SETTINGS	220

/* !!! Be careful the user area should be in another bank than the code !!! */
//#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_384   /* Start @ of user Flash area */
//#define FLASH_USER_END_ADDR     ADDR_FLASH_PAGE_386   /* End @ of user Flash area */

#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_389   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ADDR_FLASH_PAGE_392 + FLASH_PAGE_SIZE - 1   /* End @ of user Flash area */

#define FLASH_USER_START_ADDR1   ADDR_FLASH_PAGE_401   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR1     ADDR_FLASH_PAGE_401 + FLASH_PAGE_SIZE - 1   /* End @ of user Flash area */
#define FLASH_USER_START_ADDR2   ADDR_FLASH_PAGE_402   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR2     ADDR_FLASH_PAGE_402 + FLASH_PAGE_SIZE - 1   /* End @ of user Flash area */


typedef enum
{
  WRITTEN_FLASH_OK = 20U,
	WRITTEN_FLASH_FAIL,
	ERASED_FLASH,
	WRITTEN_FLASH_1_OK = 30U,
	WRITTEN_FLASH_1_FAIL,
	ERASED_FLASH_1,
	WRITTEN_FLASH_2_OK = 40U,
	WRITTEN_FLASH_2_FAIL,
	ERASED_FLASH_2,
	NOTHING_TO_WRITE
}WrittenFlash_StatusTypeDef;


struct buffer_t{
	uint8_t array[Buff_Len];
	int8_t changed[Buff_Len];
	int32_t written;
};

uint32_t GetPage(uint32_t Addr);
uint32_t GetBank(uint32_t Address);
HAL_StatusTypeDef HAL_Init_Flash(void);
void write_read_flash(void);
int32_t init_flash(uint32_t numpage, uint8_t buf[], uint32_t len);
int32_t readflash(uint32_t numpage, uint32_t buf[], uint32_t len);
int32_t change_buf(uint32_t numelmt, uint32_t subst[], uint32_t buf[], uint32_t len);
int32_t rewriteflash(uint32_t numpage, uint8_t buf[], uint32_t len);

uint8_t WriteBufferToFlash_wrap(uint8_t json_res);
int32_t WriteBufferToFlash(struct buffer_t* buff);
int32_t FinilizeWriteBufferToFlash(struct buffer_t* buff);
int32_t CancelJSONChanges(struct buffer_t* buff);


#define WRTN_CHECK 0x99
#define WRTN_OFFSET 96


struct json_arr{
	struct LoRa_settings{
		uint64_t deveui;
		uint64_t appkey;
		uint32_t freq;
		uint8_t fr;
	}LoRa_settings_t;
	uint8_t uart3;
	struct led2{
		struct usb{
			uint8_t sec;
			uint8_t min;
			uint8_t hour;
			uint8_t days;
			uint8_t	work;
		}period_t;
		struct battery{
			uint8_t sec;
			uint8_t min;
			uint8_t hour;
			uint8_t days;
			uint8_t	work;
		}period2_t;
	}led2_t;
	struct led1{
		struct usb1{
			struct blinks{
				uint8_t msec;
				uint8_t times;
			}blinks_t;
			struct period{
				uint8_t sec;
				uint8_t min;
				uint8_t hour;
				uint8_t days;
				uint8_t	work;
			}period_t;
		}usb1_t;
	}led1_t;
		struct LoRa{
			struct blinksL{
				uint8_t msec;
				uint8_t times;
			}blinks_t;
			struct periodL{
				uint8_t sec;
				uint8_t min;
				uint8_t hour;
				uint8_t days;
				uint8_t	work;
			}period_t;
		}LoRa_t;
		struct period_LoRa{
			struct periodLoRa{
				uint8_t sec;
				uint8_t min;
				uint8_t hour;
				uint8_t days;
				uint8_t	work;
			}period_t; 
		}period_LoRa_t;
		uint8_t LoRa_text[34];
		struct LoRa_Data{
			uint8_t ad_1;
			uint8_t inp1;
			uint8_t inp3;
			uint8_t ad_4;
			uint8_t text;
		}LoRa_Data_t;
		uint8_t command;
		uint8_t wrtn;
		uint8_t devEuiVal;
		uint8_t appKeyVal;
};

#endif /* FLASH_MEM_H */
