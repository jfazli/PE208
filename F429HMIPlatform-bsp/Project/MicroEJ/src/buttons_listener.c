/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
 
#include "LLINPUT_LISTENER_BUTTONS_impl.h"
#include "LLINPUT_DEVICE.h"
#include "LLINPUT.h"
#include "microej.h"
#include <stdint.h>

// this h file is created by buildSystemMicroUI step
#include "microui_constants.h"

#include "bsp_customization.h"

//SMA test
#define MICROUI_MAP_BUTTONS_TO_COMMANDS MICROEJ_FALSE

#ifndef MICROUI_MAP_BUTTONS_TO_COMMANDS
	// By default, buttons and joystick events are mapped to MicroUI COMMANDS events
	#define MICROUI_MAP_BUTTONS_TO_COMMANDS MICROEJ_TRUE
#endif

#if MICROUI_MAP_BUTTONS_TO_COMMANDS == MICROEJ_TRUE
	// Default button to command mapping
	#ifndef BUTTON_WAKEUP_COMMAND
		#define BUTTON_WAKEUP_COMMAND LLINPUT_COMMAND_ESC
	#endif
	#ifndef BUTTON_TAMPER_COMMAND	
		#define BUTTON_TAMPER_COMMAND LLINPUT_COMMAND_CLOCKWISE
	#endif
	#ifndef BUTTON_KEY_COMMAND
		#define BUTTON_KEY_COMMAND LLINPUT_COMMAND_ANTICLOCKWISE
	#endif

	// Default joystick to command mapping
	#ifndef JOYSTICK_UP_COMMAND
		#define JOYSTICK_UP_COMMAND LLINPUT_COMMAND_UP
	#endif
	#ifndef JOYSTICK_DOWN_COMMAND
		#define JOYSTICK_DOWN_COMMAND LLINPUT_COMMAND_DOWN
	#endif
	#ifndef JOYSTICK_LEFT_COMMAND
		#define JOYSTICK_LEFT_COMMAND LLINPUT_COMMAND_LEFT
	#endif
	#ifndef JOYSTICK_RIGHT_COMMAND
		#define JOYSTICK_RIGHT_COMMAND LLINPUT_COMMAND_RIGHT
	#endif
	#ifndef JOYSTICK_SELECT_COMMAND
		#define JOYSTICK_SELECT_COMMAND LLINPUT_COMMAND_SELECT
	#endif
#endif


#if MICROUI_MAP_BUTTONS_TO_COMMANDS == MICROEJ_TRUE
// send a command event to MicroUI when buttons or joystick event occurs
static const uint8_t BUTTON_TO_COMMAND[] = {BUTTON_WAKEUP_COMMAND,BUTTON_TAMPER_COMMAND,BUTTON_KEY_COMMAND};
static const uint8_t JOYSTICK_TO_COMMAND[] = {JOYSTICK_UP_COMMAND,JOYSTICK_DOWN_COMMAND,JOYSTICK_LEFT_COMMAND,JOYSTICK_RIGHT_COMMAND,JOYSTICK_SELECT_COMMAND};
#else
// send a button event to MicroUI when buttons or joystick event occurs
#endif

#if MICROUI_MAP_BUTTONS_TO_COMMANDS == MICROEJ_TRUE
uint8_t sendCommand(LLINPUT_DEVICE* buttons, int buttonID)
{
	return LLINPUT_sendCommandEvent(MICROUI_EVENTGEN_COMMANDS, LLINPUT_DEVICE_getID(buttons) == MICROUI_EVENTGEN_BUTTONS ? BUTTON_TO_COMMAND[buttonID] : JOYSTICK_TO_COMMAND[buttonID]);
}
#endif

uint8_t LLINPUT_LISTENER_BUTTONS_IMPL_buttonPressed(LLINPUT_DEVICE* buttons, int32_t buttonId)
{
#if MICROUI_MAP_BUTTONS_TO_COMMANDS == MICROEJ_FALSE
	// send a button event
	return LLINPUT_sendButtonPressedEvent(LLINPUT_DEVICE_getID(buttons), buttonId);
#else
	// send a command event
	return sendCommand(buttons, buttonId);
#endif
}
uint8_t LLINPUT_LISTENER_BUTTONS_IMPL_buttonReleased(LLINPUT_DEVICE* buttons, int32_t buttonId)
{
#if MICROUI_MAP_BUTTONS_TO_COMMANDS == MICROEJ_FALSE
	// send a button event
	return LLINPUT_sendButtonReleasedEvent(LLINPUT_DEVICE_getID(buttons), buttonId);
#else
	// do not send a command event on release event
	return MICROEJ_TRUE;	// the event has been managed
#endif
}
uint8_t LLINPUT_LISTENER_BUTTONS_IMPL_buttonRepeated(LLINPUT_DEVICE* buttons, int32_t buttonId)
{
#if MICROUI_MAP_BUTTONS_TO_COMMANDS == MICROEJ_FALSE
	// send a button event
	return LLINPUT_sendButtonRepeatedEvent(LLINPUT_DEVICE_getID(buttons), buttonId);
#else
	// send a command event
	return sendCommand(buttons, buttonId);
#endif
}


