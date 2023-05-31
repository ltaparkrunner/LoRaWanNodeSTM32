#include "flash_mem.h"
#include "stm32l4xx_mu.h"

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
uint32_t Address = 0, PAGEError = 0;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;
/* USER CODE END PV */

int32_t errplace = 0;
void write_read_flash(void)
{
	  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();
	errplace = 6;
			MU_LED_Off(LED2);
      MU_LED_On(LED2);
      HAL_Delay(700);
      MU_LED_Off(LED2);
      HAL_Delay(700);
			MU_LED_On(LED2);
      HAL_Delay(700);
      MU_LED_Off(LED2);
			
  /* Erase the user Flash area
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGAERR); 
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_PGSERR); 
  /* Get the 1st page to erase */
  FirstPage = GetPage(FLASH_USER_START_ADDR);
  /* Get the number of pages to erase from 1st page */
  NbOfPages = GetPage(FLASH_USER_END_ADDR) - FirstPage + 1;
  /* Get the bank */
  BankNumber = GetBank(FLASH_USER_START_ADDR);
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks       = BankNumber;
  EraseInitStruct.Page        = FirstPage;
  EraseInitStruct.NbPages     = NbOfPages;
	errplace = 7;
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
      MU_LED_On(LED2);
      HAL_Delay(100);
      MU_LED_Off(LED2);
      HAL_Delay(2000);
    }
  }
	errplace = 8;
  /* Program the user Flash area word by word
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  Address = FLASH_USER_START_ADDR;

  while (Address < FLASH_USER_END_ADDR)
  {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, DATA_64) == HAL_OK)
    {
      Address = Address + 8;
			HAL_Delay(20);
    }
   else
    {
      /* Error occurred while writing data in Flash memory.
         User can add here some code to deal with this error */
      while (1)
      {
        /* Make HL2 blink (100ms on, 2s off) to indicate error in Write operation */
        MU_LED_On(LED2);
        HAL_Delay(100);
        MU_LED_Off(LED2);
        HAL_Delay(2000);
      }
    }
  }
	errplace = 9;
  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  /* Check if the programmed data is OK
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/
  Address = FLASH_USER_START_ADDR;
  MemoryProgramStatus = 0x0;
	errplace = 10;
  while (Address < FLASH_USER_END_ADDR)
  {
    data32 = *(__IO uint32_t *)Address;

    if (data32 != DATA_32)
    {
      MemoryProgramStatus++;
    }
    Address = Address + 4;
  }
	errplace = 11;
  /*Check if there is an issue to program data*/
  if (MemoryProgramStatus == 0)
  {
    /* No error detected. Switch on HL2*/
		MU_LED_On(LED2);
  }
  else
  {
    /* Error detected. HL2 will blink with 1s period */
    while (1)
    {
      MU_LED_On(LED2);
			
      HAL_Delay(1000);
      MU_LED_Off(LED2);
      HAL_Delay(1000);
    }
  }

}

// read_erase_change_write_verify_repeat
int32_t readflash(uint32_t numpage, uint8_t buf[], uint32_t len)
{
	return len;
}

int32_t change_buf(uint32_t numelmt, uint8_t subst[], uint8_t buf[], uint32_t len)
{ 
	return len;
}


int32_t rewriteflash(uint32_t numpage, uint8_t buf[], uint32_t len)
{ 
	return len;
}

