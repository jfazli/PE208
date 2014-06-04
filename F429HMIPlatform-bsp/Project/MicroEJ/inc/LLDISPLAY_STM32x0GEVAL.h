/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _LLDISPLAY_STM32x0GEVAL
#define _LLDISPLAY_STM32x0GEVAL

#include "LLDISPLAY_COPY.h"

typedef struct LLDISPLAY_STM32x0GEVAL{
	struct LLDISPLAY_COPY header;
	// can add some fields
} LLDISPLAY_STM32x0GEVAL;

void LLDISPLAY_STM32x0GEVAL_new(LLDISPLAY_STM32x0GEVAL* env);

#endif
