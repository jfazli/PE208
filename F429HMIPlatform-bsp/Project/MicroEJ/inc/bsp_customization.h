/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef _BSP_CUSTOMIZATION
#define _BSP_CUSTOMIZATION

/**
 * BSP customization header file.
 * This file may be overridden with an application specific one.
 */

#include "microej.h"

/**
 *	Specifies the buffer size of the PNG decoder (in bytes). The PNG decoder needs several
 *	intermediate buffers to decode a PNG image. Furthermore the final decoded image is stored into
 *	this buffer. If the buffer is full an OutOfMemory error is thrown.
 * 	By default, size is set to 64Kb.
 */
//#define MICROUI_PNG_BUFFER_SIZE	1600000
#define MICROUI_PNG_BUFFER_SIZE	1048576
 

/**
 * When enabled, buttons and joystick events are mapped to MicroUI COMMANDS events.
 * By default, it is enabled.
 */
//	#define MICROUI_MAP_BUTTONS_TO_COMMANDS [MICROEJ_TRUE|MICROEJ_FALSE]

//	#if MICROUI_MAP_BUTTONS_TO_COMMANDS == MICROEJ_TRUE
	/**
	 * Button to command mapping.
	 */
	//	#define BUTTON_[WAKEUP|TAMPER|KEY]_COMMAND COMMANDS_EVENTGENERATOR_[ESC|BACK|UP|DOWN|LEFT|RIGHT|SELECT|CANCEL|HELP|MENU|EXIT|START|STOP|PAUSE|RESUME|COPY|CUT|PASTE|CLOCKWISE|ANTICLOCKWISE|PREVIOUS|NEXT|DISPLAY]
	/**
	 * Joystick to command mapping.
	 */
	//	#define JOYSTICK_[UP|DOWN|LEFT|RIGHT|SELECT]_COMMAND COMMANDS_EVENTGENERATOR_[ESC|BACK|UP|DOWN|LEFT|RIGHT|SELECT|CANCEL|HELP|MENU|EXIT|START|STOP|PAUSE|RESUME|COPY|CUT|PASTE|CLOCKWISE|ANTICLOCKWISE|PREVIOUS|NEXT|DISPLAY]

//#endif

#endif
