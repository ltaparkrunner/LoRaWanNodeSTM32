#include "timer_rtos.h"
#include "rtc_if.h"

//void TimerInit(TimerTime_t HANDLE, TimerCBFunc_t CB) {
void TimerInit(TimerHandle_t HANDLE, void ( *CB )( void *)) {
static int num = 0;
char name[] = "Timer_00";
	
	if(num > NUM_TIMERS - 1 || num < 0) { HANDLE = NULL; return; }
	else {
		if(num > 10) { name[6] += 1; name[7] += num - 10; }
		else name[7] += num;
	}
	
	HANDLE = xTimerCreate( name, TIMERTIME_T_MAX, pdFALSE, ( void * ) 0, (TimerCallbackFunction_t)CB);
	num++;
}

TimerTime_t TimerGetCurrentTime(void) {
	uint32_t now = RTC_IF_GetTimerValue();
	return  RTC_IF_Convert_Tick2ms(now);
}

TimerTime_t TimerGetElapsedTime(TimerTime_t  past) {
	uint32_t nowInTicks = RTC_IF_GetTimerValue( );
  uint32_t pastInTicks = RTC_IF_Convert_Tick2ms( past );
  /* intentional wrap around. Works Ok if tick duation below 1ms */
  return RTC_IF_Convert_Tick2ms( nowInTicks- pastInTicks );
}

