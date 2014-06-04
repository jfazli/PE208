/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#include "LLLEDS_impl.h"
#include "cpu.h"
#if MCU == 2
#include "stm32f2xx.h"
#include "stm322xg_eval.h"
#elif MCU == 4
#include "stm32f4xx.h"
#include "stm324xg_eval.h"
#elif MCU == 42
#include "stm32f4xx.h"
#include "stm324x9i_eval.h"
#else
#error Invalid MCU
#endif
/**************************************************************
 * LLLEDS implementation functions
 **************************************************************/

int32_t LLLEDS_IMPL_getIntensity(int32_t ledID)
{
	// not available yet
	return LLLEDS_MIN_INTENSITY;
}

int32_t LLLEDS_IMPL_initialize(void)
{
	/* Initialize LEDs on board */
	STM_EVAL_LEDInit(LED1);
	STM_EVAL_LEDInit(LED2);
	return 2;
}
  
void LLLEDS_IMPL_setIntensity(int32_t ledID, int32_t intensity)
{
	Led_TypeDef led;
	switch(ledID)
	{
	case 0: led = LED1;	break;
	default: case 1: led = LED2;	break;
	}

	switch(intensity)
	{
	case LLLEDS_MIN_INTENSITY:
		STM_EVAL_LEDOff(led);
		break;
	default:
		STM_EVAL_LEDOn(led);
		break;
	}
}
