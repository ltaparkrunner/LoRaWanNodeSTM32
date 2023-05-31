#include "stm32l4xx_hal.h"

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
#define ADDR_FLASH_PAGE_511   ((uint32_t)0x080ff800) /* Base @ of Page 511, 2 Kbytes */

#define DATA_32                 ((uint32_t)0x12345678)
#define DATA_64                 ((uint64_t)0x1234567812345678)

#define FLASH_ROW_SIZE          32

/* !!! Be careful the user area should be in another bank than the code !!! */
//#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_384   /* Start @ of user Flash area */
//#define FLASH_USER_END_ADDR     ADDR_FLASH_PAGE_386   /* End @ of user Flash area */

#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_389   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ADDR_FLASH_PAGE_392 + FLASH_PAGE_SIZE - 1   /* End @ of user Flash area */

uint32_t GetPage(uint32_t Addr);
uint32_t GetBank(uint32_t Address);
HAL_StatusTypeDef HAL_Init_Flash(void);
void write_read_flash(void);
