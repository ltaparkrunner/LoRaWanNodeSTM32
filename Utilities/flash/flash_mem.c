//#include "stm32l4xx_mu.h"
#include "settings_json.h"
#include "tiny-json.h"
#include "tiny-json_extra.h"
#include "flash_mem.h"

void Error_Handler(void);
//int32_t rewriteflash(uint32_t numpage, uint8_t buf[], uint32_t len);
/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
uint32_t GetPage(uint32_t Addr)
{
  uint32_t page = 0;
  
  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }
  
  return page;
}

/**
  * @brief  Gets the bank of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The bank of a given address
  */
uint32_t GetBank(uint32_t Addr)
{
  uint32_t bank = 0;
  
  if (READ_BIT(SYSCFG->MEMRMP, SYSCFG_MEMRMP_FB_MODE) == 0)
  {
  	/* No Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_1;
    }
    else
    {
      bank = FLASH_BANK_2;
    }
  }
  else
  {
  	/* Bank swap */
    if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_2;
    }
    else
    {
      bank = FLASH_BANK_1;
    }
  }
  
  return bank;
}

HAL_StatusTypeDef HAL_Init_Flash()
{
	  HAL_StatusTypeDef  status = HAL_OK;

  /* Configure Flash prefetch, Instruction cache, Data cache */
  /* Default configuration at reset is:                      */
  /* - Prefetch disabled                                     */
  /* - Instruction cache enabled                             */
  /* - Data cache enabled                                    */
#if (INSTRUCTION_CACHE_ENABLE == 0)
   __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();
#endif /* INSTRUCTION_CACHE_ENABLE */

#if (DATA_CACHE_ENABLE == 0)
   __HAL_FLASH_DATA_CACHE_DISABLE();
#endif /* DATA_CACHE_ENABLE */

#if (PREFETCH_ENABLE != 0)
  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
#endif /* PREFETCH_ENABLE */
	  /* Return function status */
  return status;
}

/* USER CODE BEGIN PV */
uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
uint32_t /*Address = 0,*/ PAGEError = 0;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;
/* USER CODE END PV */


void write_read_flash(void)
{
//	int32_t errplace = 0;
	uint32_t Address = 0;
	  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();
//	errplace = 6;
			MU_LED_Off(HL2);
//      MU_LED_On(HL2);
//      HAL_Delay(700);
//      MU_LED_Off(HL2);
      HAL_Delay(300);
			MU_LED_On(HL2);
//      HAL_Delay(700);
//      MU_LED_Off(HL2);
//			
  /* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
	//__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGAERR); 
	//__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGSERR); 
  /* Get the 1st page to erase */
  FirstPage = GetPage(FLASH_USER_START_ADDR2);
  /* Get the number of pages to erase from 1st page */
  NbOfPages = GetPage(FLASH_USER_END_ADDR2) - FirstPage + 1;
  /* Get the bank */
  BankNumber = GetBank(FLASH_USER_START_ADDR2);
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;//FLASH_TYPEERASE_MASSERASE;//FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks       = BankNumber;
  EraseInitStruct.Page        = FirstPage;
  EraseInitStruct.NbPages     = NbOfPages;
//	errplace = 7;
  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
		 
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
  {
    /*
      Error occurred while page erase.
      User can add here some code to deal with this error.
      PAGEError will contain the faulty page and then to know the code error on this page,
      user can call function 'HAL_FLASH_GetError()'
    */
    /* Infinite loop */
    while (1)
    {
      /* Make HL2 blink (100ms on, 2s off) to indicate error in Erase operation */
//      MU_LED_On(HL2);
//      HAL_Delay(100);
//      MU_LED_Off(HL2);
//      HAL_Delay(2000);
			Error_Handler();
    }
  }
//	errplace = 8;
  /* Program the user Flash area word by word
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  Address = FLASH_USER_START_ADDR2;

  while (Address < FLASH_USER_END_ADDR2)
  {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, DATA_64) == HAL_OK)
    {
      Address = Address + 8;
			HAL_Delay(2);
    }
   else
    {
			Error_Handler();
    }
  }
//	errplace = 9;
  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  /* Check if the programmed data is OK
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/
  Address = FLASH_USER_START_ADDR2;
  MemoryProgramStatus = 0x0;
//	errplace = 10;
  while (Address < FLASH_USER_END_ADDR)
  {
    data32 = *(__IO uint32_t *)Address;

    if (data32 != DATA_32)
    {
      MemoryProgramStatus++;
    }
    Address = Address + 4;
  }
//	errplace = 11;
  /*Check if there is an issue to program data*/
  if (MemoryProgramStatus == 0)
  {
    /* No error detected. Switch on HL2*/
		MU_LED_On(HL2);
  }
  else
  {
    Error_Handler();
  }

}

// read_erase_change_write_verify_repeat
//Flash Bank2 mapped at 0x08000000 (and aliased @0x00000000)
//*         and Flash Bank1 mapped at 0x08100000 (and aliased at 0x00100000)
extern json_t pool[ Num_Field ];
extern char sets_JSON[];
//uint8_t buff[Buff_Len];
struct buffer_t buff;
struct json_arr *jsonarrflash_1, *jsonarrflash_2;//*jsonarrmem ;

int32_t init_flash(uint8_t buffer[], uint32_t len)
{
//	uint32_t addr_w;
//	uint32_t addr_r = ChooseReadFlashBank(&addr_w);
	static uint8_t bf[Buff_Len];
	uint32_t addr = FLASH_USER_START_ADDR1;//FLASH_USER_START_ADDR2;// ADDR_FLASH_PAGE_0 + FLASH_PAGE_SIZE * numpage;
//	uint32_t i;
	int32_t wrtn1 = *(__IO uint8_t*)(FLASH_USER_START_ADDR1 + WRTN_OFFSET);
	int32_t wrtn2 = *(__IO uint8_t*)(FLASH_USER_START_ADDR2 + WRTN_OFFSET);
//	jsonarrflash_1 = (struct json_arr*)addr; 
//	jsonarrflash_2 = (struct json_arr*)FLASH_USER_START_ADDR2;
	if(wrtn1 > 0x7f && wrtn2 > 0x7f )//WRTN_CHECK)
	{
		uint32_t lenR = json_to_buffer(sets_JSON, pool, Num_Field, bf/*buffer*/, Buff_Len);
		rewriteflash(addr, bf/*buffer*/, lenR);
		rewriteflash(FLASH_USER_START_ADDR2, bf /*buffer*/, lenR);
	}
	return 0;
}

int32_t readflash(uint32_t adr, uint8_t buf[], uint32_t len)
{
	//uint32_t addr = ADDR_FLASH_PAGE_0 + FLASH_PAGE_SIZE * numpage;
	uint32_t addr = adr;
	uint32_t addrEnd = adr + len;	

  while (addr < addrEnd)
  {
		*(__IO uint64_t *)buf = *(__IO uint64_t *)addr;
      addr = addr + 8;
			buf +=8;
  }
	return len;
}

int32_t readflash_8b(uint32_t adr, uint8_t buf[], uint32_t len)
{
	//uint32_t addr = ADDR_FLASH_PAGE_0 + FLASH_PAGE_SIZE * numpage;
	uint32_t addr = adr;
	uint32_t addrEnd = adr + len;	

  while (addr < addrEnd)
  {
		*(__IO uint8_t *)buf = *(__IO uint8_t *)addr;
      addr = addr + 1;
			buf +=1;
  }
	return len;
}

int32_t change_buf(uint32_t adr, uint32_t subst[], uint32_t buf[], uint32_t len)
{ 
	uint32_t i;
	for(i = 0; i < len; i++)
	{
		buf[i] = subst[i];
	}
	return i;
}


int32_t rewriteflash(uint32_t adr, uint8_t buf[], uint32_t len)
{ 
	uint32_t addr = adr;//FLASH_USER_START_ADDR2;	//ADDR_FLASH_PAGE_0 + FLASH_PAGE_SIZE * numpage;
	uint8_t *buf_p = buf;
	HAL_FLASH_Unlock();
	  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGAERR); 
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGSERR); 
  /* Get the 1st page to erase */
  FirstPage = GetPage(addr);
  /* Get the number of pages to erase from 1st page */
  NbOfPages = GetPage(addr + len) - FirstPage + 1;
  /* Get the bank */
  BankNumber = GetBank(addr);
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks       = BankNumber;
  EraseInitStruct.Page        = FirstPage;
  EraseInitStruct.NbPages     = NbOfPages;
	//errplace = 7;
  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
		 
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
  {
    Error_Handler();
  }
	
	  /* Program the user Flash area word by word
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  addr = adr; // FLASH_USER_START_ADDR2;//ADDR_FLASH_PAGE_0 + FLASH_PAGE_SIZE * numpage;
	uint32_t addrEnd = adr + len; //FLASH_USER_START_ADDR2 + len; //ADDR_FLASH_PAGE_0 + FLASH_PAGE_SIZE * numpage + len;
	//uint32_t i = 0;
	static uint64_t tmp;
  while (addr < addrEnd)
  {
		tmp = *(__IO uint64_t *)buf;
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, tmp) == HAL_OK)
    {
      addr = addr + 8;
			buf +=8;
			HAL_Delay(2);
    }
   else
    {
			Error_Handler();
    }
  }
  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();
	
	
			addr = adr; // FLASH_USER_START_ADDR2;//ADDR_FLASH_PAGE_0 + FLASH_PAGE_SIZE * numpage;
	addrEnd = adr + len; //FLASH_USER_START_ADDR2 + len; //ADDR_FLASH_PAGE_0 + FLASH_PAGE_SIZE * numpage + len;
	buf = buf_p;
  for(int32_t i2 = 0; i2 < len ; i2+= 8)
  {
		*(__IO uint64_t *)buf = 0;
			buf +=8;
  }
	
	addr = adr; // FLASH_USER_START_ADDR2;//ADDR_FLASH_PAGE_0 + FLASH_PAGE_SIZE * numpage;
	addrEnd = adr + len; //FLASH_USER_START_ADDR2 + len; //ADDR_FLASH_PAGE_0 + FLASH_PAGE_SIZE * numpage + len;
	buf = buf_p;
  while (addr < addrEnd)
  {
		*(__IO uint64_t *)buf = *(__IO uint64_t *)addr;
      addr = addr + 8;
			buf +=8;
  }

	return len;
}

struct json_arr* GetJsonFlash(void)
{
	uint32_t addr_w;
	uint32_t addr_r = ChooseReadFlashBank(&addr_w);
	return (struct json_arr*)addr_r;
}
