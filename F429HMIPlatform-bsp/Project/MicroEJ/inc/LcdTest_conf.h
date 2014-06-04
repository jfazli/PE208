/*
 * C
 *
 * Copyright 2013 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _LCD_TEST_CONF
#define _LCD_TEST_CONF

#include "lcdTest.h"

/**====================================================================================================================
 * Defines
 * ==================================================================================================================*/

// FIXME Default configuration file. Please set your own defines
#define LCD_TYPE LCD_TYPE_SWITCH
#define LCD_BPP LCD_BPP_16
#define LCD_LAYOUT LCD_LAYOUT_XY
#define LCD_LINK LCD_LINK_AGAINST_DISPLAY_STACK
#define LLDISPLAY_SWITCH_IMPL LLDISPLAY_STM32x0DISCO

#endif	// _LCD_TEST_CONF
