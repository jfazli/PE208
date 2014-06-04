/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#include "joystick.h"
#define LLINPUT_DEVICE_IMPL JOYSTICK
#include "LLINPUT_DEVICE_impl.h"
#include "LLINPUT_LISTENER_BUTTONS.h"
#include "cpu.h"
#if MCU == 2
#include "stm32f2xx.h"
#include "stm322xg_eval_ioe.h"
#elif MCU == 4
#include "stm32f4xx.h"
#include "stm324xg_eval_ioe.h"
#elif MCU == 42
#include "stm32f4xx.h"
#include "stm324x9i_eval_ioe.h"
#include "stm324x9i_eval_ioe16.h"
#else
#error Invalid MCU
#endif
#include "ioe.h"
#include "microej.h"

static struct JOYSTICK joystick;
static uint8_t JOYSTICK_BUTTON_PRESSED[] = {MICROEJ_FALSE, MICROEJ_FALSE, MICROEJ_FALSE, MICROEJ_FALSE, MICROEJ_FALSE};

void JOYSTICK_press_event(uint32_t id)
{
	if (JOYSTICK_BUTTON_PRESSED[id] == MICROEJ_TRUE)
	{
		// XXX repeat event never sent!!!
		// already pressed => repeat event
		// don't care about returned value: if the event has not been managed
		// the event is "just" lost
		LLINPUT_LISTENER_BUTTONS_buttonRepeated((LLINPUT_DEVICE*)&joystick, id);
	}
	else
	{
		// button was released => press event
		if (LLINPUT_LISTENER_BUTTONS_buttonPressed((LLINPUT_DEVICE*)&joystick, id) == MICROEJ_TRUE)
		{
			// the event has been managed: we can store the new button state
			// button is pressed now
			JOYSTICK_BUTTON_PRESSED[id] = MICROEJ_TRUE;
		}
		else
		{
			// The event has not been managed because the system events queue is full.
			// Two solutions:
			// - increase the size of the system events queue (see Memory page of the
			// Java application launcher
			// - limit rate of events of your drivers (for example, calibrate touch to
			// send an event at most every X ms)

			// We have not to change the button state to prevent incoherence state (to
			// not send a futur release event whereas the press event has never been
			// sent!)
		}
	}
}

void JOYSTICK_release_event(uint32_t id)
{
	if (JOYSTICK_BUTTON_PRESSED[id] == MICROEJ_TRUE)
	{
		// button was pressed => release event
		if (LLINPUT_LISTENER_BUTTONS_buttonReleased((LLINPUT_DEVICE*)&joystick, id) == MICROEJ_TRUE)
		{
			// the event has been managed: we can store the new button state
			// button is released now
			JOYSTICK_BUTTON_PRESSED[id] = MICROEJ_FALSE;
		}
		else
		{
			// The event has not been managed because the system events queue is full.
			// Two solutions:
			// - increase the size of the system events queue (see Memory page of the
			// Java application launcher
			// - limit rate of events of your drivers (for example, calibrate touch to
			// send an event at most every X ms)

			// We have not to change the button state to prevent incoherence state (to
			// not send a futur press event whereas the release event has never been
			// sent!)
		}
	}
	// else: already released
}

IOE_Status_TypDef JOYSTICK_interrupt(void)
{
	uint16_t JoyStick ;

	JoyStick = IOE16_GetITStatus() ;

	
	///phv TObe COnfirmed
	//JoyStick |= ~(JOY_IO_PINS) ;
	//JoyStick = ~JoyStick;

	switch (JoyStick)
	{
	case JOY_IO16_SEL:	// CENTER
		JOYSTICK_press_event(4);
		break;
	case JOY_IO16_DOWN: 
		JOYSTICK_press_event(1);
		break;
	case JOY_IO16_LEFT: 
		JOYSTICK_press_event(2);
		break;
	case JOY_IO16_RIGHT: 
		JOYSTICK_press_event(3);
		break;
	case JOY_IO16_UP: 
		JOYSTICK_press_event(0);
		break;
	default:
		JOYSTICK_release_event(0);
		JOYSTICK_release_event(1);
		JOYSTICK_release_event(2);
		JOYSTICK_release_event(3);
		JOYSTICK_release_event(4);
		break;
	}

	/* Clear the interrupt pending bits */
	CLEARFIFO;
	IOE_CLEAR_IT(IOE_ADDR, IOE_TS_IT);
	
	return IOE_OK;
}

JOYSTICK* joystickInitialize(void)
{
	LLINPUT_DEVICE_IMPL_new(&joystick);

	IOE16_Config();

	return (JOYSTICK*)&joystick;
}

