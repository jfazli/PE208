/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#include "LLSP_RTX.h"
#include "LLSP_impl.h"
#include "microej.h"
#include "interrupts.h"
#include <RTL.h>

OS_MUT LLSP_mutex;
int8_t LLSP_mutex_initialized = 0;

void LLSP_IMPL_initialize(void){
	//OPTIM: first check if mutex is initialized without locking the OS
	if(!LLSP_mutex_initialized){
		tsk_lock();
		// Check again if mutex is initialized because another task may have initialized it before locking the OS
		if(!LLSP_mutex_initialized){
			os_mut_init(LLSP_mutex);
			LLSP_mutex_initialized = 1;
		}
		tsk_unlock();	
	}
}

void LLSP_IMPL_syncWriteBlockEnter(int32_t databaseID, int32_t blockID){
	//Don't manage block specific synchronization. 
	//Only one block can be accessed at the same time. 
	os_mut_wait(LLSP_mutex, 0xFFFF);
}

void LLSP_IMPL_syncWriteBlockExit(int32_t databaseID, int32_t blockID){
	os_mut_release(LLSP_mutex);
}

void LLSP_IMPL_syncReadBlockEnter(int32_t databaseID, int32_t blockID){
	//Read synchronization not specific
	LLSP_IMPL_syncWriteBlockEnter(databaseID, blockID);
}

void LLSP_IMPL_syncReadBlockExit(int32_t databaseID, int32_t blockID){
	//Read synchronization not specific
	LLSP_IMPL_syncWriteBlockExit(databaseID, blockID);
}

int32_t LLSP_IMPL_wait(void){
	U16 receivedEvents;
	//Wait for wake up from the Shielded Plug or interruption from another task
	os_evt_wait_or(LLSP_EVT_WAKEUP | LLSP_EVT_INTERRUPT, 0xFFFF);
	receivedEvents = os_evt_get();
	if((receivedEvents&LLSP_EVT_WAKEUP) != 0)
		return LLSP_OK;
	else
		return LLSP_INTERRUPTED;
}

void LLSP_IMPL_wakeup(int32_t taskID){
	os_evt_set(LLSP_EVT_WAKEUP, (OS_TID)taskID);
}
