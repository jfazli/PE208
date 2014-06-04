/*
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#include "LLMJVM_RTX.h"
#include "LLMJVM_impl.h"
#include "cpu.h"
#if MCU == 2
#include "stm32f2xx.h"
#elif MCU == 4
#include "stm32f4xx.h"
#elif MCU == 42
#include "stm32f4xx.h"
#include "stm324xg_eval.h"
#else
#error Invalid MCU
#endif
#include <RTL.h>
#include "microej.h"
#include "interrupts.h"
#include <stdlib.h>

#if LLMJVM_RTX_TIME_IMPL == LLMJVM_RTX_TIME_HARDWARE_TIMER
#include "time_hardware_timer.h"
#endif

// The MicroJvm virtual machine task ID
OS_TID LLMJVM_RTX_taskID;
// Application time offset
int64_t LLMJVM_RTX_applicationTimeOffset = 0;
// Absolute time in milliseconds at which timer will be launched
static int64_t LLMJVM_RTX_nextWakeupTime = INT64_MAX;
// ID of the currently scheduled timer
OS_ID LLMJVM_RTX_currentTimerID = NULL;

#if LLMJVM_RTX_TIME_IMPL == LLMJVM_RTX_TIME_RTX
// This global is defined by RTX and contains the number of ticks since OS startup
extern uint16_t os_time;
#endif

int64_t LLMJVM_RTX_timeToTick(int64_t time);

int32_t LLMJVM_IMPL_initialize(void){
#if LLMJVM_RTX_TIME_IMPL == LLMJVM_RTX_TIME_HARDWARE_TIMER
	time_hardware_timer_initialize();
#endif
	return LLMJVM_OK;
}

int32_t LLMJVM_IMPL_vmTaskStarted(void){
	LLMJVM_RTX_taskID = os_tsk_self();
	return LLMJVM_OK;
}

int32_t LLMJVM_IMPL_scheduleRequest(int64_t absoluteTime){
	int64_t relativeTime;
	int64_t relativeTick;//number of ticks before absoluteTime

	//First check if absolute time is lower than current schedule time
	if(absoluteTime < LLMJVM_RTX_nextWakeupTime){
		if(LLMJVM_RTX_currentTimerID != NULL)
			os_tmr_kill(LLMJVM_RTX_currentTimerID);//kill potential previously scheduled timer
		LLMJVM_RTX_nextWakeupTime = absoluteTime;

		relativeTime = absoluteTime - LLMJVM_IMPL_getCurrentTime(MICROEJ_TRUE);
		relativeTick = LLMJVM_RTX_timeToTick(relativeTime);  //number of ticks before absoluteTime

		//Checks if absoluteTick has already been reached
		if(relativeTick > 0){
			if(relativeTick > UINT16_MAX)
				relativeTick = UINT16_MAX;//saturate relativeTick value to 16 bits
 
			LLMJVM_RTX_currentTimerID = os_tmr_create((U16)relativeTick, LLMJVM_RTX_TIMER_INFO);

			if(LLMJVM_RTX_currentTimerID == NULL)
				return LLMJVM_ERROR;
			else
				return LLMJVM_OK;
		}
		else {
			//absoluteTick has been reached, notify the vm now
			return LLMJVM_schedule();
		}
	}

	return LLMJVM_OK;
}

int32_t LLMJVM_IMPL_idleVM(void){
	os_evt_wait_or(LLMJVM_RTX_EVT_WAKEUP, 0xFFFF);
	return LLMJVM_OK; //MicroJvm virtual machine cannot be interrupted
}

int32_t LLMJVM_IMPL_wakeupVM(void){
	if(isInInterrupt())
		isr_evt_set(LLMJVM_RTX_EVT_WAKEUP, LLMJVM_RTX_taskID);
	else
		os_evt_set(LLMJVM_RTX_EVT_WAKEUP, LLMJVM_RTX_taskID);
	LLMJVM_RTX_nextWakeupTime = INT64_MAX;
	return LLMJVM_OK;
}

int32_t LLMJVM_IMPL_ackWakeup(void){
	os_evt_clr(LLMJVM_RTX_EVT_WAKEUP, LLMJVM_RTX_taskID);
	return LLMJVM_OK;
}

int32_t LLMJVM_IMPL_getCurrentTaskID(void){
	return (int32_t)os_tsk_self();
}

void LLMJVM_IMPL_setApplicationTime(int64_t t) {
	int64_t currentTime = LLMJVM_IMPL_getCurrentTime(MICROEJ_TRUE);
	LLMJVM_RTX_applicationTimeOffset = t - currentTime;
}

int64_t LLMJVM_IMPL_getCurrentTime(uint8_t system) {
	int64_t systemTime;
#if LLMJVM_RTX_TIME_IMPL == LLMJVM_RTX_TIME_RTX
	systemTime = (((int64_t)OS_TICK) * os_time)/1000;
#elif LLMJVM_RTX_TIME_IMPL == LLMJVM_RTX_TIME_HARDWARE_TIMER
	systemTime = time_hardware_timer_getCurrentTime();
#endif
	if (system)
		return systemTime;
	else
		return systemTime + LLMJVM_RTX_applicationTimeOffset;
}

int64_t LLMJVM_IMPL_getTimeNanos(void) {
#if LLMJVM_RTX_TIME_IMPL == LLMJVM_RTX_TIME_RTX
	return (((int64_t)OS_TICK) * os_time)*1000;
#elif LLMJVM_RTX_TIME_IMPL == LLMJVM_RTX_TIME_HARDWARE_TIMER
	return time_hardware_timer_getTimeNanos();
#endif
}

uint8_t LLMJVM_IMPL_isInReadOnlyMemory(void* ptr){
	if(
#if MEM_CODE_START != 0
			// avoid warning #186-D: pointless comparison of unsigned integer with zero
			(uint32_t)ptr >= MEM_CODE_START &&
#endif
			(uint32_t)ptr < MEM_CODE_END
	)
		return MICROEJ_TRUE;
	else
		return MICROEJ_FALSE;
}

// RL-RTX timer interrupt callback
void LLMJVM_RTX_tmr_call(void){
	enterInterrupt();
	LLMJVM_schedule();
	leaveInterrupt();
}

/*
 * Convert a time in milliseconds in ticks.
 * Result is round up to the next tick (ie. converting back the resulting ticks
 * to milliseconds results in a value greather than or equals to given time).
 */
int64_t LLMJVM_RTX_timeToTick(int64_t time){
	//OS_TICK is in microseconds and time in ms.
	//Convert time in microsecons and then divide by OS_TICK.
	int64_t timeus = time * 1000;
	return (timeus + (OS_TICK-1)) / OS_TICK;
}

int32_t LLMJVM_IMPL_shutdown(void){
	// nothing to do
	return LLMJVM_OK;
}
