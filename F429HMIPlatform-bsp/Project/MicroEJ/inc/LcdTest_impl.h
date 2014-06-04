/*
 * C
 *
 * Copyright 2013 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _LCD_TEST_IMPL
#define _LCD_TEST_IMPL

#include <stdint.h>

/*
 * Initialize srand. This function can only call srand() function
 * without argument. In this way the randomized values are always
 * the same between two launches
 */
void lcdTest_srandInit(void);

/*
 * Get time in milliseconds. This function can return 1. In this
 * way the framerate calculations will be wrong.
 */
uint64_t lcdTest_getTime(void);

/*
 * Print the framerate
 */
void lcdTest_printFramerate(uint32_t time, uint32_t frequency);

#endif	// _LCD_TEST_IMPL
