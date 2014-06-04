/*
 * C
 *
 * Copyright 2013 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _LCD_TEST
#define _LCD_TEST

#include <stdint.h>

/**====================================================================================================================
 * Defines
 * ==================================================================================================================*/

/*
 * Set the define LCD_TYPE to fix the LCD double buffer type
 * (SWITCH for switch mode, COPY for copy mode).
 *
 * You have to set too the define LLDISPLAY_XX_IMPL, where
 * XX is SWITCH or COPY, with the same value you have set
 * in your LCD driver
 * When LCD_TYPE == LCD_TYPE_SWITCH, please set LLDISPLAY_SWITCH_IMPL
 * When LCD_TYPE == LCD_TYPE_COPY, please set LLDISPLAY_COPY_IMPL
 */
#define LCD_TYPE_SWITCH 1
#define LCD_TYPE_COPY 2


/*
 * Set the define LCD_BPP to fix the number of bits per pixels
 */
#define LCD_BPP_16 16
#define LCD_BPP_32 32


/*
 * Set the define LCD_LAYOUT to fix the LCD memory layout
 */
#define LCD_LAYOUT_XY 0
#define LCD_LAYOUT_YX 1



/*
 * The lcdTest can be launched instead of launching the MicroJvm
 * virtual machine, keeping the same BSP linker configuration
 * (libraries mjvm, display, javaapp are linked with the application).
 *
 * The lcdTest can be launched too as a standalone application.
 * No MicroEJ library is linked with.
 *
 * You have to set the define LCD_LINK to fix methods to link.
 */
#define LCD_LINK_WITHOUT_DISPLAY_STACK 0
#define LCD_LINK_AGAINST_DISPLAY_STACK 1


/**====================================================================================================================
 * Public methods
 * ==================================================================================================================*/

void lcdTest_pixels1(void);
void lcdTest_pixels2(void);
void lcdTest_fillRect1(void);
void lcdTest_fillRect2(void);
void lcdTest_framerateFlush(uint32_t nbLoops);
void lcdTest_framerateFillAndFlush(uint32_t nbLoops) ;
void lcdTest_framerateRandomRect(uint32_t nbLoops);

#endif	// _LCD_TEST
