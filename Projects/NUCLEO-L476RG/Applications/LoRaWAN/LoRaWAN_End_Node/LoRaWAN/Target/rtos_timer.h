/* The header is written by Alex_M. An attempt to change timer.h written by MCD Application Team */
#ifndef RTOS_TIMER_H
#define RTOS_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "timers.h"
#define TIMERTIME_T_MAX ( ( uint32_t )~0 )

/* Types */
#define TimerTime_t uint32_t
#define TimerEvent_t TimerHandle_t
	
/* Functions */
#define TimerInit(HANDLE, CB) do {\
                                   HANDLE = osTimerCreate( (const char *)"", 1, pdFALSE, CB, NULL);\
                                 } while(0)

																		xTimerCreateStatic((const char *)"",
                      1, // period should be filled when starting the Timer using osTimerStart
                      (type == osTimerPeriodic) ? pdTRUE : pdFALSE,
                      (void *) argument,
                      (TaskFunction_t)timer_def->ptimer,
                      (StaticTimer_t *)timer_def->controlblock);
	
																do {\
                                   UTIL_TIMER_Create( HANDLE, TIMERTIME_T_MAX, UTIL_TIMER_ONESHOT, CB, NULL);\
                                 } while(0)

#endif /* RTOS_TIMER_H */
	