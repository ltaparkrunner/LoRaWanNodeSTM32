/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    lora_app.c
  * @author  MCD Application Team
  * @brief   Application of the LRWAN Middleware
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "Region.h" /* Needed for LORAWAN_DEFAULT_DATA_RATE */
#include "sys_app.h"
#include "lora_app.h"
#include "stm32_seq.h"
#include "stm32_timer.h"
#include "utilities_def.h"
#include "lora_app_version.h"
#include "lorawan_version.h"
#include "subghz_phy_version.h"
#include "lora_info.h"
#include "LmHandler.h"
#include "stm32_lpm.h"
//#include "adc_if.h"
#include "sys_conf.h"
#include "CayenneLpp.h"
#include "sys_sensors.h"
#include "stm32l4xx_mu.h"
#include "defaults.h"

/* USER CODE BEGIN Includes */
#include "usb_device.h"
#include "realise_settings.h"
/* USER CODE END Includes */

/* External variables ---------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private typedef -----------------------------------------------------------*/
/**
  * @brief LoRa State Machine states
  */
typedef enum TxEventType_e
{
  /**
    * @brief Appdata Transmission issue based on timer every TxDutyCycleTime
    */
  TX_ON_TIMER,
  /**
    * @brief Appdata Transmission external event plugged on OnSendEvent( )
    */
  TX_ON_EVENT
  /* USER CODE BEGIN TxEventType_t */

  /* USER CODE END TxEventType_t */
} TxEventType_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private function prototypes -----------------------------------------------*/
/**
  * @brief  LoRa End Node send request
  */
static void SendTxData(void);

/**
  * @brief  TX timer callback function
  * @param  context ptr of timer context
  */
static void OnTxTimerEvent(void *context);

/**
  * @brief  join event callback function
  * @param  joinParams status of join
  */
static void OnJoinRequest(LmHandlerJoinParams_t *joinParams);

/**
  * @brief  tx event callback function
  * @param  params status of last Tx
  */
static void OnTxData(LmHandlerTxParams_t *params);

/**
  * @brief callback when LoRa application has received a frame
  * @param appData data received in the last Rx
  * @param params status of last Rx
  */
static void OnRxData(LmHandlerAppData_t *appData, LmHandlerRxParams_t *params);

/*!
 * Will be called each time a Radio IRQ is handled by the MAC layer
 *
 */
static void OnMacProcessNotify(void);

/* USER CODE BEGIN PFP */

/**
  * @brief  LED Tx timer callback function
  * @param  context ptr of LED context
  */
static void OnTxTimerLedEvent(void *context);

/**
  * @brief  LED Rx timer callback function
  * @param  context ptr of LED context
  */
static void OnRxTimerLedEvent(void *context);

/**
  * @brief  LED Join timer callback function
  * @param  context ptr of LED context
  */
//static void OnJoinTimerLedEvent(void *context);
//static void OnPowerGreenLedOnEvent(void *context);
//static void OnPowerGreenLedOffEvent(void *context);
//static void OnPowerYellowLedOnEvent_USB(void *context);
//static void OnPowerYellowLedOnEvent_LoRa(void *context);
static void OnPowerYellowLedOffEvent(void *context);
static void OnLoRaRxBothLedsToggleEvent(void *context);

static void USBReset(void);
//static void ledSwitch1(void);
//static void ledSwitch2(void);
/* USER CODE END PFP */

/* Private variables ---------------------------------------------------------*/
static ActivationType_t ActivationType = LORAWAN_DEFAULT_ACTIVATION_TYPE;
int32_t ledBlinkLoRaRxCount;
/**
  * @brief LoRaWAN handler Callbacks
  */
static LmHandlerCallbacks_t LmHandlerCallbacks =
{
  .GetBatteryLevel =           GetBatteryLevel,
  .GetTemperature =            GetTemperatureLevel,
  .GetUniqueId =               GetUniqueId,
  .GetDevAddr =                GetDevAddr,
  .OnMacProcess =              OnMacProcessNotify,
  .OnJoinRequest =             OnJoinRequest,
  .OnTxData =                  OnTxData,
  .OnRxData =                  OnRxData
};

/**
  * @brief LoRaWAN handler parameters
  */
static LmHandlerParams_t LmHandlerParams =
{
  .ActiveRegion =             ACTIVE_REGION,
  .DefaultClass =             LORAWAN_DEFAULT_CLASS,
  .AdrEnable =                LORAWAN_ADR_STATE,
  .TxDatarate =               LORAWAN_DEFAULT_DATA_RATE,
  .PingPeriodicity =          LORAWAN_DEFAULT_PING_SLOT_PERIODICITY
};

/**
  * @brief Type of Event to generate application Tx
  */
static TxEventType_t EventType = TX_ON_TIMER;

/**
  * @brief Timer to handle the application Tx
  */
static UTIL_TIMER_Object_t TxTimer;

/* USER CODE BEGIN PV */
/**
  * @brief User application buffer
  */
static uint8_t AppDataBuffer[LORAWAN_APP_DATA_BUFFER_MAX_SIZE];

/**
  * @brief User application data structure
  */
static LmHandlerAppData_t AppData = { 0, 0, AppDataBuffer };

/**
  * @brief Specifies the state of the application LED
  */
static uint8_t AppLedStateOn = RESET;

/**
  * @brief Timer to handle the application Tx Led to toggle
  */
static UTIL_TIMER_Object_t TxLedTimer;

/**
  * @brief Timer to handle the application Rx Led to toggle
  */
static UTIL_TIMER_Object_t RxLedTimer;

/**
  * @brief Timer to handle the application Join Led to toggle
  */
static UTIL_TIMER_Object_t JoinLedTimer;
//** static UTIL_TIMER_Object_t PowerGreenLedOnTimer;
//** static UTIL_TIMER_Object_t PowerGreenLedOffTimer;
//static UTIL_TIMER_Object_t PowerYellowLedOnTimer;
static UTIL_TIMER_Object_t YellowLedOffTimer_USB;
static UTIL_TIMER_Object_t YellowLedOffTimer_LORA;
static UTIL_TIMER_Object_t BothLedsBlinkLoRaRx;

/* USER CODE END PV */

/* Exported functions ---------------------------------------------------------*/
/* USER CODE BEGIN EF */

/* USER CODE END EF */

void LoRaWAN_Init(void)
{
  /* USER CODE BEGIN LoRaWAN_Init_1 */


	MU_LED_Init(HL1);
	MU_LED_Off(HL1);
	MU_LED_Init(HL2);
	MU_LED_Off(HL2);	
	
	MU_Sound_Init();

  /* Get LoRa APP version*/
  APP_LOG(TS_OFF, VLEVEL_M, "APP_VERSION:        V%X.%X.%X\r\n",
          (uint8_t)(__LORA_APP_VERSION >> __APP_VERSION_MAIN_SHIFT),
          (uint8_t)(__LORA_APP_VERSION >> __APP_VERSION_SUB1_SHIFT),
          (uint8_t)(__LORA_APP_VERSION >> __APP_VERSION_SUB2_SHIFT));

  /* Get MW LoraWAN info */
  APP_LOG(TS_OFF, VLEVEL_M, "MW_LORAWAN_VERSION: V%X.%X.%X\r\n",
          (uint8_t)(__LORAWAN_VERSION >> __APP_VERSION_MAIN_SHIFT),
          (uint8_t)(__LORAWAN_VERSION >> __APP_VERSION_SUB1_SHIFT),
          (uint8_t)(__LORAWAN_VERSION >> __APP_VERSION_SUB2_SHIFT));

  /* Get MW SubGhz_Phy info */
  APP_LOG(TS_OFF, VLEVEL_M, "MW_RADIO_VERSION:   V%X.%X.%X\r\n",
          (uint8_t)(__SUBGHZ_PHY_VERSION >> __APP_VERSION_MAIN_SHIFT),
          (uint8_t)(__SUBGHZ_PHY_VERSION >> __APP_VERSION_SUB1_SHIFT),
          (uint8_t)(__SUBGHZ_PHY_VERSION >> __APP_VERSION_SUB2_SHIFT));

  UTIL_TIMER_Create(&TxLedTimer, 0xFFFFFFFFU, UTIL_TIMER_ONESHOT, OnTxTimerLedEvent, NULL);
  UTIL_TIMER_Create(&RxLedTimer, 0xFFFFFFFFU, UTIL_TIMER_ONESHOT, OnRxTimerLedEvent, NULL);
//  UTIL_TIMER_Create(&JoinLedTimer, 0xFFFFFFFFU, UTIL_TIMER_PERIODIC, OnJoinTimerLedEvent, NULL);
//	UTIL_TIMER_Create(&PowerGreenLedOnTimer, 0xFFFFFFFFU, UTIL_TIMER_PERIODIC, OnPowerGreenLedOnEvent, NULL);
//	UTIL_TIMER_Create(&PowerGreenLedOffTimer, 0xFFFFFFFFU, UTIL_TIMER_PERIODIC, OnPowerGreenLedOffEvent, NULL);
//	UTIL_TIMER_Create(&YellowLedOffTimer_LORA, 0xFFFFFFFFU, UTIL_TIMER_PERIODIC, OnPowerGreenLedOffEvent, NULL);
	UTIL_TIMER_Create(&BothLedsBlinkLoRaRx, 0xFFFFFFFFU, UTIL_TIMER_PERIODIC, OnLoRaRxBothLedsToggleEvent, NULL);	
//	UTIL_TIMER_Create(&PowerYellowLedOnTimer, 0xFFFFFFFFU, UTIL_TIMER_PERIODIC, OnPowerYellowLedOnEvent, NULL);
	UTIL_TIMER_Create(&YellowLedOffTimer_USB, 0xFFFFFFFFU, UTIL_TIMER_PERIODIC, OnPowerYellowLedOffEvent, NULL);
	UTIL_TIMER_Create(&YellowLedOffTimer_LORA, 0xFFFFFFFFU, UTIL_TIMER_PERIODIC, OnPowerYellowLedOffEvent, NULL);
  UTIL_TIMER_SetPeriod(&TxLedTimer, 1500);
  UTIL_TIMER_SetPeriod(&RxLedTimer, 500);
	UTIL_TIMER_SetPeriod(&BothLedsBlinkLoRaRx, 300);
//  UTIL_TIMER_SetPeriod(&JoinLedTimer, 30000);
//	UTIL_TIMER_SetPeriod(&PowerGreenLedOnTimer, 30000);
//	UTIL_TIMER_SetPeriod(&PowerGreenLedOffTimer, 100);
//	UTIL_TIMER_SetPeriod(&PowerYellowLedOnTimer, 20000);
//	UTIL_TIMER_SetPeriod(&YellowLedOffTimer_USB, 500);
//	UTIL_TIMER_SetPeriod(&YellowLedOffTimer_LORA, 200);
	
  /* USER CODE END LoRaWAN_Init_1 */

	UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_LmHandlerProcess), UTIL_SEQ_RFU, LmHandlerProcess);
	UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_LoRaSendOnTxTimerEvent), UTIL_SEQ_RFU, SendTxData);
	UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_USBPlugInEvent), UTIL_SEQ_RFU, USBReset);
	
	//UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_LmHandlerProcess), UTIL_SEQ_RFU, ledSwitch2);
	//UTIL_SEQ_RegTask((1 << CFG_SEQ_Task_LoRaSendOnTxTimerEvent), UTIL_SEQ_RFU, ledSwitch1);
  /* Init Info table used by LmHandler*/
  LoraInfo_Init();

  /* Init the Lora Stack*/
  LmHandlerInit(&LmHandlerCallbacks);

  LmHandlerConfigure(&LmHandlerParams);

  /* USER CODE BEGIN LoRaWAN_Init_2 */
	Set_Green_Led_Period();
	Set_yellow_blink(0);
	Set_yellow_blink(1);

	//UTIL_TIMER_Start(&TxLedTimer);
//  UTIL_TIMER_Start(&JoinLedTimer);
//	UTIL_TIMER_Start(&PowerGreenLedOnTimer);
//	UTIL_TIMER_Start(&PowerYellowLedOnTimer);
  /* USER CODE END LoRaWAN_Init_2 */

  LmHandlerJoin(ActivationType);

  if (EventType == TX_ON_TIMER)
  {
    /* send every time timer elapses */
    UTIL_TIMER_Create(&TxTimer,  0xFFFFFFFFU, UTIL_TIMER_ONESHOT, OnTxTimerEvent, NULL);
//    UTIL_TIMER_SetPeriod(&TxTimer,  APP_TX_DUTYCYCLE);
//    UTIL_TIMER_Start(&TxTimer);
			Set_LoRaWAN_Period();
  }
//  else
  {
    /* USER CODE BEGIN LoRaWAN_Init_3 */

    /* send every time button is pushed */
    //MU_PB_Init(BUTTON_MODE_EXTI); //BUTTON_USER, 
    /* USER CODE END LoRaWAN_Init_3 */
  }

	//HAL_Delay(100);
  /* USER CODE BEGIN LoRaWAN_Init_Last */

  /* USER CODE END LoRaWAN_Init_Last */
}

/* USER CODE BEGIN PB_Callbacks */
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//  switch (GPIO_Pin)
//  {
////    case  USER_BUTTON_PIN:
////      UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_LoRaSendOnTxTimerOrButtonEvent), CFG_SEQ_Prio_0);
////      break;
//    default:
//      break;
//  }
//}
//void CallbackRSTButton(void)//(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
//{
//	if (HAL_GPIO_ReadPin(SHIELD_RST_GPIO_PORT, SHIELD_RST_PIN) == GPIO_PIN_RESET)
//	{
//		HAL_Delay(100);
//	}
//}
/* USER CODE END PB_Callbacks */

/* Private functions ---------------------------------------------------------*/
/* USER CODE BEGIN PrFD */

/* USER CODE END PrFD */

static void OnRxData(LmHandlerAppData_t *appData, LmHandlerRxParams_t *params)
{
  /* USER CODE BEGIN OnRxData_1 */
  if ((appData != NULL) || (params != NULL))
  {
    //LED_On(LED_BLUE);

    UTIL_TIMER_Start(&RxLedTimer);

    static const char *slotStrings[] = { "1", "2", "C", "C Multicast", "B Ping-Slot", "B Multicast Ping-Slot" };

    APP_LOG(TS_OFF, VLEVEL_M, "\r\n###### ========== MCPS-Indication ==========\r\n");
    APP_LOG(TS_OFF, VLEVEL_H, "###### D/L FRAME:%04d | SLOT:%s | PORT:%d | DR:%d | RSSI:%d | SNR:%d\r\n",
            params->DownlinkCounter, slotStrings[params->RxSlot], appData->Port, params->Datarate, params->Rssi, params->Snr);
    switch (appData->Port)
    {
      case LORAWAN_SWITCH_CLASS_PORT:
        /*this port switches the class*/
        if (appData->BufferSize == 1)
        {
          switch (appData->Buffer[0])
          {
            case 0:
            {
              LmHandlerRequestClass(CLASS_A);
              break;
            }
            case 1:
            {
              LmHandlerRequestClass(CLASS_B);
              break;
            }
            case 2:
            {
              LmHandlerRequestClass(CLASS_C);
              break;
            }
            default:
              break;
          }
        }
        break;
      case LORAWAN_USER_APP_PORT:
        if (appData->BufferSize == 1)
        {
          AppLedStateOn = appData->Buffer[0] & 0x01;
          if (AppLedStateOn == RESET)
          {
            APP_LOG(TS_OFF, VLEVEL_H,   "LED OFF\r\n");

            //LED_Off(LED_RED1);
          }
          else
          {
            APP_LOG(TS_OFF, VLEVEL_H, "LED ON\r\n");

            //LED_On(LED_RED1);
          }
        }
        break;

      default:

        break;
    }
  }
  /* USER CODE END OnRxData_1 */
}

#include "settings_json.h"
#include "sensors.h"
static void SendTxData(void)
{
  /* USER CODE BEGIN SendTxData_1 */
	YellowLedOn_LoRa(NULL);
  UTIL_TIMER_Time_t nextTxIn = 0;

  AppData.Port = LORAWAN_USER_APP_PORT;
	AppData.BufferSize = CollectData(&AppData);

  //AppData.BufferSize = i;


  if (LORAMAC_HANDLER_SUCCESS == LmHandlerSend(&AppData, LORAWAN_DEFAULT_CONFIRMED_MSG_STATE, &nextTxIn, false))
  {
    APP_LOG(TS_ON, VLEVEL_L, "SEND REQUEST\r\n");
  }
  else if (nextTxIn > 0)
  {
    APP_LOG(TS_ON, VLEVEL_L, "Next Tx in  : ~%d second(s)\r\n", (nextTxIn / 1000));
  }

  /* USER CODE END SendTxData_1 */
}

void onUSBPlugIn(void)
{
	UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_USBPlugInEvent), CFG_SEQ_Prio_0);
}

//#include "usb_device.h"
//#include "usbd_core.h"
//extern USBD_HandleTypeDef hUsbDeviceFS;
//extern USBD_DescriptorsTypeDef FS_Desc;
static void USBReset(void)
{
	//MX_USB_DEVICE_Init();
//	if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
//  {
//    Error_Handler();
//  }
}

static void OnTxTimerEvent(void *context)
{
  /* USER CODE BEGIN OnTxTimerEvent_1 */

  /* USER CODE END OnTxTimerEvent_1 */
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_LoRaSendOnTxTimerEvent), CFG_SEQ_Prio_0);

  /*Wait for next tx slot*/
  UTIL_TIMER_Start(&TxTimer);
  /* USER CODE BEGIN OnTxTimerEvent_2 */

  /* USER CODE END OnTxTimerEvent_2 */
}

/* USER CODE BEGIN PrFD_LedEvents */
static void OnTxTimerLedEvent(void *context)
{
//  LED_Off(LED_RED2);
//	MU_LED_Toggle(LED1);
	
}

static void OnRxTimerLedEvent(void *context)
{
//  LED_Off(LED_BLUE) ;
}

//static void OnJoinTimerLedEvent(void *context)
//{
//  LED_Toggle(LED_RED1) ;
//	MU_LED_Toggle(LED2);
//}

int32_t Set_Green_Led_Period(void)
{
	uint32_t interval = 0;
	//MU_board_USB_detect_Init();
//	static GPIO_PinState res;
//	res = HAL_GPIO_ReadPin(USB_VBUS_Port, USB_VBUS_Pin);
	if( HAL_GPIO_ReadPin(USB_VBUS_Port, USB_VBUS_Pin) == GPIO_PIN_SET) 
		interval = ReadPeriod(0);
	else interval = ReadPeriod(1);
	Change_power_blink(interval);
	return interval;
}

int32_t Set_LoRaWAN_Period(void)
{
	uint32_t interval = ReadPeriod(2);
	ChangeLORA_transm_period(interval);
	return interval;
}

uint32_t Set_yellow_blink(int32_t ch)
{
	uint32_t dur = ReadBlinkPeriod(ch);
	if(ch == 0) ChangeUSB_transm_blink(dur);
	else ChangeLORA_transm_blink(dur);
	return dur;
}
//////////////////****
//static void OnPowerGreenLedOnEvent(void *context)
//{
//	MU_LED_On(HL1);
//	UTIL_TIMER_Start(&PowerGreenLedOffTimer);
//}

//static void OnPowerGreenLedOffEvent(void *context)
//{
//	MU_LED_Off(HL1);
//	UTIL_TIMER_Stop(&PowerGreenLedOffTimer);
//}

void YellowLedOn_USB(void *context)
{
	MU_LED_On(HL2);
	UTIL_TIMER_Start(&YellowLedOffTimer_USB);
}

void YellowLedOn_LoRa(void *context)
{
	MU_LED_On(HL2);
	UTIL_TIMER_Start(&YellowLedOffTimer_LORA);
}

static void OnPowerYellowLedOffEvent(void *context)
{
	MU_LED_Off(HL2);
	UTIL_TIMER_Stop(&YellowLedOffTimer_USB);
	UTIL_TIMER_Stop(&YellowLedOffTimer_LORA);
}
//****
int32_t Change_power_blink(uint32_t interval)
{
//	MU_LED_On(HL1);
//	UTIL_TIMER_Start(&PowerGreenLedOffTimer);
//	
//	if(interval > 0) {
//		UTIL_TIMER_Stop(&PowerGreenLedOnTimer);
//		UTIL_TIMER_SetPeriod(&PowerGreenLedOnTimer,  interval);
//		UTIL_TIMER_Start(&PowerGreenLedOnTimer);
//	}
//	else UTIL_TIMER_Stop(&PowerGreenLedOnTimer);
//	return interval;
}

//int32_t ChangeBat_power_blink(uint32_t interval)
//{
//	return 0;
//}
int32_t ChangeUSB_transm_blink(uint32_t interval)
{
		UTIL_TIMER_SetPeriod(&YellowLedOffTimer_USB,  interval);
	return interval;
}
int32_t ChangeLORA_transm_blink(uint32_t interval)
{
		UTIL_TIMER_SetPeriod(&YellowLedOffTimer_LORA,  interval);
	return interval;
}

int32_t ChangeLORA_transm_period(uint32_t interval)
{
	// transmit the data
	UTIL_TIMER_Stop(&TxTimer);
	UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_LoRaSendOnTxTimerEvent), CFG_SEQ_Prio_0);
	
	if(interval > 0) {
		UTIL_TIMER_SetPeriod(&TxTimer,  interval);
		UTIL_TIMER_Start(&TxTimer);
	}
	else UTIL_TIMER_Stop(&TxTimer);
	return interval;
}


//		UTIL_TIMER_Create(&TxTimer,  0xFFFFFFFFU, UTIL_TIMER_ONESHOT, OnTxTimerEvent, NULL);
//    UTIL_TIMER_SetPeriod(&TxTimer,  APP_TX_DUTYCYCLE);
//    UTIL_TIMER_Start(&TxTimer);
/* USER CODE END PrFD_LedEvents */

static void OnTxData(LmHandlerTxParams_t *params)
{
  /* USER CODE BEGIN OnTxData_1 */
  if ((params != NULL))
  {
    /* Process Tx event only if its a mcps response to prevent some internal events (mlme) */
    if (params->IsMcpsConfirm != 0)
    {
      //LED_On(LED_RED2) ;
      UTIL_TIMER_Start(&TxLedTimer);

      APP_LOG(TS_OFF, VLEVEL_M, "\r\n###### ========== MCPS-Confirm =============\r\n");
      APP_LOG(TS_OFF, VLEVEL_H, "###### U/L FRAME:%04d | PORT:%d | DR:%d | PWR:%d", params->UplinkCounter,
              params->AppData.Port, params->Datarate, params->TxPower);

      APP_LOG(TS_OFF, VLEVEL_H, " | MSG TYPE:");
      if (params->MsgType == LORAMAC_HANDLER_CONFIRMED_MSG)
      {
        APP_LOG(TS_OFF, VLEVEL_H, "CONFIRMED [%s]\r\n", (params->AckReceived != 0) ? "ACK" : "NACK");
      }
      else
      {
        APP_LOG(TS_OFF, VLEVEL_H, "UNCONFIRMED\r\n");
      }
    }
  }
  /* USER CODE END OnTxData_1 */
}

static void OnJoinRequest(LmHandlerJoinParams_t *joinParams)
{
  /* USER CODE BEGIN OnJoinRequest_1 */
  if (joinParams != NULL)
  {
    if (joinParams->Status == LORAMAC_HANDLER_SUCCESS)
    {
      UTIL_TIMER_Stop(&JoinLedTimer);
			//****
			ledBlinkLoRaRxCount = 6;
			MU_LED_On(HL1);
			MU_LED_On(HL2);
			UTIL_TIMER_Start(&BothLedsBlinkLoRaRx);
      //LED_Off(LED_RED1) ;

      APP_LOG(TS_OFF, VLEVEL_M, "\r\n###### = JOINED = ");
      if (joinParams->Mode == ACTIVATION_TYPE_ABP)
      {
        APP_LOG(TS_OFF, VLEVEL_M, "ABP ======================\r\n");
      }
      else
      {
        APP_LOG(TS_OFF, VLEVEL_M, "OTAA =====================\r\n");
      }
    }
    else
    {
      APP_LOG(TS_OFF, VLEVEL_M, "\r\n###### = JOIN FAILED\r\n");
			ledBlinkLoRaRxCount = 2;
			MU_LED_On(HL1);
			MU_LED_On(HL2);
			UTIL_TIMER_Start(&BothLedsBlinkLoRaRx);
    }
  }
  /* USER CODE END OnJoinRequest_1 */
}

static void OnMacProcessNotify(void)
{
  /* USER CODE BEGIN OnMacProcessNotify_1 */

  /* USER CODE END OnMacProcessNotify_1 */
  UTIL_SEQ_SetTask((1 << CFG_SEQ_Task_LmHandlerProcess), CFG_SEQ_Prio_0);

  /* USER CODE BEGIN OnMacProcessNotify_2 */

  /* USER CODE END OnMacProcessNotify_2 */
}


static void OnLoRaRxBothLedsToggleEvent(void *context)
{
	if(ledBlinkLoRaRxCount > 0) 
	{
		MU_LED_Toggle(HL1);
		MU_LED_Toggle(HL2);
		ledBlinkLoRaRxCount--;
	}		
	else
	{
		MU_LED_Off(HL1);
		MU_LED_Off(HL2);
		UTIL_TIMER_Stop(&BothLedsBlinkLoRaRx);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
