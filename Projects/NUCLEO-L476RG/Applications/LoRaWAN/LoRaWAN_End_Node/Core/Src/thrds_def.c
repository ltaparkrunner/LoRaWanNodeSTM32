#include "stm32_lpm.h"
#include "stm32l4xx_mu.h"
#include "thrds_def.h"
#include "main.h"
#include "cmsis_os.h"

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"


extern osThreadId_t USBPlugInTaskHandle;
extern osThreadId_t USBPlugOffTaskHandle;

static void USB_Init(void);
static void USB_DeInit(void);

extern USBD_HandleTypeDef hUsbDeviceFS;
extern USBD_DescriptorsTypeDef FS_Desc;

void Idle_Task(void* argument)
{
  /* USER CODE BEGIN UTIL_SEQ_Idle_1 */
	(void) argument;
  /* USER CODE END UTIL_SEQ_Idle_1 */
	if(	HAL_GPIO_ReadPin(USB_VBUS_Port, USB_VBUS_Pin) == GPIO_PIN_RESET )
		UTIL_LPM_EnterLowPower();
  /* USER CODE BEGIN UTIL_SEQ_Idle_2 */

  /* USER CODE END UTIL_SEQ_Idle_2 */
}

void onUSBPlugIn(void)
{
	//UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_USBPlugInEvent), CFG_SEQ_Prio_0);
	osThreadFlagsSet( USBPlugInTaskHandle, USBPlugInSignal);
}

void onUSBPlugOff(void)
{
	//UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_USBPlugOffEvent), CFG_SEQ_Prio_0);
	osThreadFlagsSet( USBPlugOffTaskHandle, USBPlugOffSignal);
}

void wrap_USB_Init(void *argument)
{
	(void) argument;
//  osEvent event;
	uint32_t flag;
  
  for(;;)
  {
//    event = osSignalWait( 4, osWaitForever);
		flag = osThreadFlagsWait(USBPlugInSignal, osFlagsWaitAny, osWaitForever);
    if(flag == USBPlugInSignal)
    {      
      USB_Init();
    }
		else if(flag > 0x7ffffff) Error_Handler();
			
  }
}

void wrap_USB_DeInit(void *argument)
{
	(void) argument;
//  osEvent event;
	uint32_t flag;
  
  for(;;)
  {
//    event = osSignalWait( 2, osWaitForever);
		flag = osThreadFlagsWait(USBPlugOffSignal, osFlagsWaitAny, osWaitForever);
    if(flag == USBPlugOffSignal)
    {      
      USB_DeInit();
    }
		else if(flag > 0x7ffffff) Error_Handler();
  }
}

static void USB_Init(void) {
	
	/* Init Device Library, add supported class and start the library. */
  if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
  {
    Error_Handler();
  }
}

static void USB_DeInit(void) {
		if(hUsbDeviceFS.pData != NULL) {
		if(USBD_Stop(&hUsbDeviceFS)!= USBD_OK)
		{
			Error_Handler();
		}
		if (USBD_DeInit(&hUsbDeviceFS) != USBD_OK)
		{
			Error_Handler();
		}
	}
}
