/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _LLDISPLAY_STM32x0DISCO
#define _LLDISPLAY_STM32x0DISCO

#include "LLDISPLAY_SWITCH.h"
 
typedef struct LLDISPLAY_STM32x0DISCO{
	struct LLDISPLAY_SWITCH header;
	// can add some fields
} LLDISPLAY_STM32x0DISCO;

void LLDISPLAY_STM32x0DISCO_new(LLDISPLAY_STM32x0DISCO* env);

#endif
