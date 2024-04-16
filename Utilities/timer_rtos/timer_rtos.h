#ifndef __TIMER_RTOS_H__
#define __TIMER_RTOS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "timers.h"

/* The definition of the timers themselves. */
/*typedef*/ struct tmrTimerControl /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
	const char				*pcTimerName;		/*<< Text name.  This is not used by the kernel, it is included simply to make debugging easier. */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
	ListItem_t				xTimerListItem;		/*<< Standard linked list item as used by all kernel features for event management. */
	TickType_t				xTimerPeriodInTicks;/*<< How quickly and often the timer expires. */
	void 					*pvTimerID;			/*<< An ID to identify the timer.  This allows the timer to be identified when the same callback is used for multiple timers. */
	TimerCallbackFunction_t	pxCallbackFunction;	/*<< The function that will be called when the timer expires. */
	#if( configUSE_TRACE_FACILITY == 1 )
		UBaseType_t			uxTimerNumber;		/*<< An ID assigned by trace tools such as FreeRTOS+Trace */
	#endif
	uint8_t 				ucStatus;			/*<< Holds bits to say if the timer was statically allocated or not, and if it is active or not. */
};

#define NUM_TIMERS 20
#define TIMERTIME_T_MAX ( ( uint32_t )~0 )
#define BLOCK_TIME 0

#define TimerTime_t uint32_t /* UTIL_TIMER_Time_t */
//#define TimerEvent_t TimerHandle_t /* UTIL_TIMER_Object_t */
#define TimerEvent_t struct tmrTimerControl

//void TimerInit(TimerHandle_t HANDLE, void ( *CB )( void *));
void TimerInit(TimerHandle_t HANDLE, void ( *CB )( void *));
	
#define TimerSetValue(HANDLE, TIMEOUT) do{ \
                                           xTimerChangePeriod(HANDLE, TIMEOUT / portTICK_PERIOD_MS, BLOCK_TIME);\
                                         } while(0)

#define TimerStart(HANDLE) do {\
                                  xTimerStart(HANDLE, BLOCK_TIME);\
                              } while(0)

#define TimerStop(HANDLE) do {\
                                 xTimerStop(HANDLE, BLOCK_TIME);\
                               } while(0)

																	 
TimerTime_t TimerGetCurrentTime(void);

TimerTime_t TimerGetElapsedTime(TimerTime_t);

#ifdef __cplusplus
}
#endif

#endif /* __TIMER_RTOS_H__ */
