/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _TOUCH
#define _TOUCH

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
#else
#error Invalid MCU
#endif

typedef struct TOUCH{
	struct LLINPUT_DEVICE header;
	// can add some fields
} TOUCH;

IOE_Status_TypDef TOUCH_interrupt(void);
TOUCH* touchInitialize(void);

#endif
