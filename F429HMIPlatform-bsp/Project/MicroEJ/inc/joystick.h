/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _JOYSTICK
#define _JOYSTICK

#include "LLINPUT_DEVICE.h"
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

typedef struct JOYSTICK{
	struct LLINPUT_DEVICE header;
	// can add some fields
} JOYSTICK;

IOE_Status_TypDef JOYSTICK_interrupt(void);
JOYSTICK* joystickInitialize(void);

#endif
