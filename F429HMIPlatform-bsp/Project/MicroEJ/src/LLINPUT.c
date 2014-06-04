/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
 
#include "LLINPUT_impl.h"
#include "buttons.h"
#include "joystick.h"
#include "touch.h"
#include "interrupts.h"
#include "microej.h"
#include <stdint.h>
#include <RTL.h>
 
// this h file is created by buildSystemMicroUI step
#include "microui_constants.h"

uint8_t LLINPUT_IMPL_initialize(void)
{

	TOUCH* touch = touchInitialize();

	LLINPUT_DEVICE_setID((LLINPUT_DEVICE*)touch, MICROUI_EVENTGEN_TOUCH);
	
	/*Init IT Touch and Joystick*/
#ifndef CAP_TOUCH
	IOE16_ITConfig(IOE16_TS_IT );
	IOE16_ITConfig( IOE16_JOY_IT);

  IOE16_I2C_ReadDeviceRegister(IOE16_REG_GPMR_LSB);
  IOE16_I2C_ReadDeviceRegister(IOE16_REG_GPMR_MSB);
	IOE_TS_GetState();
#endif
	return MICROEJ_TRUE;
	
}

int32_t LLINPUT_IMPL_getInitialStateValue(int32_t stateMachinesID, int32_t stateID){
	// no state on this BSP
	return 0;
}

void LLINPUT_IMPL_enterCriticalSection()
{
	if (isInInterrupt() == MICROEJ_FALSE)
	{
		tsk_lock();
	}
}

void LLINPUT_IMPL_leaveCriticalSection()
{
	if (isInInterrupt() == MICROEJ_FALSE)
	{
		tsk_unlock();
	}
}
