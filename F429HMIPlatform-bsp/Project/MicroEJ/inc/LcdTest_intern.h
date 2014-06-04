/*
 * C
 *
 * Copyright 2013 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _LCD_TEST_INTERN
#define _LCD_TEST_INTERN

/* ====================================================================================================================
 * Includes
 * ==================================================================================================================*/

#include "lcdTest.h"
#include "lcdTest_conf.h"
#include "lcdTest_impl.h"
#include <stdint.h>

/* ====================================================================================================================
 * Check user defines
 * ==================================================================================================================*/

#ifndef LCD_TYPE
#error Please set LCD_TYPE (see lcdTest.h)
#elif LCD_TYPE != LCD_TYPE_COPY && LCD_TYPE != LCD_TYPE_SWITCH
#error Please set a valid value for LCD_TYPE (see lcdTest.h)
#elif LCD_TYPE == LCD_TYPE_COPY
#ifndef LLDISPLAY_COPY_IMPL
#warning "Implementation tag 'LLDISPLAY_COPY_IMPL' has not been defined. Use the tag defined in your driver"
#endif
#else // LCD_TYPE == LCD_TYPE_SWITCH
#ifndef LLDISPLAY_SWITCH_IMPL
#warning "Implementation tag 'LLDISPLAY_SWITCH_IMPL' has not been defined. Use the tag defined in your driver"
#endif
#endif

#ifndef LCD_BPP
#error Please set LCD_BPP (see lcdTest.h)
#elif LCD_BPP != LCD_BPP_16 && LCD_BPP != LCD_BPP_32
#error Please set a valid value for LCD_BPP (see lcdTest.h)
#endif

#ifndef LCD_LAYOUT
#error Please set LCD_LAYOUT (see lcdTest.h)
#elif LCD_LAYOUT != LCD_LAYOUT_XY && LCD_LAYOUT != LCD_LAYOUT_YX
#error Please set a valid value for LCD_LAYOUT (see lcdTest.h)
#endif

#ifndef LCD_LINK
#error Please set LCD_LINK (see lcdTest.h)
#elif LCD_LINK != LCD_LINK_WITHOUT_DISPLAY_STACK && LCD_LINK != LCD_LINK_AGAINST_DISPLAY_STACK
#error Please set a valid value for LCD_LINK (see lcdTest.h)
#endif

/* ====================================================================================================================
 * Types
 * ==================================================================================================================*/

#if LCD_BPP == LCD_BPP_16
#define lcdMem uint16_t
#elif LCD_BPP == LCD_BPP_32
#define lcdMem uint32_t
#endif

/* ====================================================================================================================
 * Functions "redirections"
 * ==================================================================================================================*/

#define LCDTEST_CONCAT0(p,s) p ## s
#define LCDTEST_CONCAT(p,s) LCDTEST_CONCAT0(p,s)

#if LCD_TYPE == LCD_TYPE_COPY
#define LLDISPLAY_internal0 LCDTEST_CONCAT(__icetea__nid__LLDISPLAY_COPY___, LLDISPLAY_COPY_IMPL)
#define LLDISPLAY_internal1 com_is2t_display_backend_DisplayCopy____4init_5
#define LLDISPLAY_initialize() LCDTEST_CONCAT(LLDISPLAY_COPY_IMPL,_initialize)(NULL)
#define LLDISPLAY_getWidth(logical) LCDTEST_CONCAT(LLDISPLAY_COPY_IMPL,_getWidth__Z)(NULL, logical)
#define LLDISPLAY_getHeight(logical) LCDTEST_CONCAT(LLDISPLAY_COPY_IMPL,_getHeight__Z)(NULL, logical)
#define LLDISPLAY_convertRGBColorToDisplayColor(color) LCDTEST_CONCAT(LLDISPLAY_COPY_IMPL,_convertRGBColorToDisplayColor__I)(NULL, color)
#define LLDISPLAY_convertDisplayColorToRGBColor(color) LCDTEST_CONCAT(LLDISPLAY_COPY_IMPL,_convertDisplayColorToRGBColor__I)(NULL, color)
#define LLDISPLAY_setContrast(contrast) LCDTEST_CONCAT(LLDISPLAY_COPY_IMPL,_setContrast__I)(NULL, contrast)
#define LLDISPLAY_getContrast() LCDTEST_CONCAT(LLDISPLAY_COPY_IMPL,_getContrast)(NULL)
#define LLDISPLAY_setBacklight(backLight) LCDTEST_CONCAT(LLDISPLAY_COPY_IMPL,_setBacklight__I)(NULL, backLight)
#define LLDISPLAY_hasBackLight() LCDTEST_CONCAT(LLDISPLAY_COPY_IMPL,_hasBackLight)(NULL)
#define LLDISPLAY_getBacklight() LCDTEST_CONCAT(LLDISPLAY_COPY_IMPL,_getBacklight)(NULL)
#define LLDISPLAY_backlightOn() LCDTEST_CONCAT(LLDISPLAY_COPY_IMPL,_backlightOn)(NULL)
#define LLDISPLAY_backlightOff() LCDTEST_CONCAT(LLDISPLAY_COPY_IMPL,_backlightOff)(NULL)
#define LLDISPLAY_getWorkingBufferStartAddress() LCDTEST_CONCAT(LLDISPLAY_COPY_IMPL,_getWorkingBufferStartAddress)(NULL)
#define LLDISPLAY_getWorkingBufferEndAddress() LCDTEST_CONCAT(LLDISPLAY_COPY_IMPL,_getWorkingBufferEndAddress)(NULL)
#define LLDISPLAY_synchronize() LCDTEST_CONCAT(LLDISPLAY_COPY_IMPL,_synchronize)(NULL)
#define LLDISPLAY_getBackBufferAddress() LCDTEST_CONCAT(LLDISPLAY_COPY_IMPL,_getBackBufferAddress)(NULL)
#define LLDISPLAY_copyBuffer(xmin, ymin, xmax, ymax) LCDTEST_CONCAT(LLDISPLAY_COPY_IMPL,_copyBuffer__IIII)(NULL, xmin, ymin, xmax, ymax)
#elif LCD_TYPE == LCD_TYPE_SWITCH
#define LLDISPLAY_internal0 LCDTEST_CONCAT(__icetea__nid__LLDISPLAY_SWITCH___, LLDISPLAY_SWITCH_IMPL)
#define LLDISPLAY_internal1 com_is2t_display_backend_DisplaySwitch____4init_5
#define LLDISPLAY_initialize() LCDTEST_CONCAT(LLDISPLAY_SWITCH_IMPL,_initialize)(NULL)
#define LLDISPLAY_getWidth(logical)  LCDTEST_CONCAT(LLDISPLAY_SWITCH_IMPL,_getWidth__Z)(NULL, logical)
#define LLDISPLAY_getHeight(logical)  LCDTEST_CONCAT(LLDISPLAY_SWITCH_IMPL,_getHeight__Z)(NULL, logical)
#define LLDISPLAY_convertRGBColorToDisplayColor(color)  LCDTEST_CONCAT(LLDISPLAY_SWITCH_IMPL,_convertRGBColorToDisplayColor__I)(NULL, color)
#define LLDISPLAY_convertDisplayColorToRGBColor(color)  LCDTEST_CONCAT(LLDISPLAY_SWITCH_IMPL,_convertDisplayColorToRGBColor__I)(NULL, color)
#define LLDISPLAY_setContrast(contrast)  LCDTEST_CONCAT(LLDISPLAY_SWITCH_IMPL,_setContrast__I)(NULL, contrast)
#define LLDISPLAY_getContrast()  LCDTEST_CONCAT(LLDISPLAY_SWITCH_IMPL,_getContrast)(NULL)
#define LLDISPLAY_setBacklight(backLight)  LCDTEST_CONCAT(LLDISPLAY_SWITCH_IMPL,_setBacklight__I)(NULL, backLight)
#define LLDISPLAY_hasBackLight()  LCDTEST_CONCAT(LLDISPLAY_SWITCH_IMPL,_hasBackLight)(NULL)
#define LLDISPLAY_getBacklight()  LCDTEST_CONCAT(LLDISPLAY_SWITCH_IMPL,_getBacklight)(NULL)
#define LLDISPLAY_backlightOn()  LCDTEST_CONCAT(LLDISPLAY_SWITCH_IMPL,_backlightOn)(NULL)
#define LLDISPLAY_backlightOff()  LCDTEST_CONCAT(LLDISPLAY_SWITCH_IMPL,_backlightOff)(NULL)
#define LLDISPLAY_getWorkingBufferStartAddress()  LCDTEST_CONCAT(LLDISPLAY_SWITCH_IMPL,_getWorkingBufferStartAddress)(NULL)
#define LLDISPLAY_getWorkingBufferEndAddress()  LCDTEST_CONCAT(LLDISPLAY_SWITCH_IMPL,_getWorkingBufferEndAddress)(NULL)
#define LLDISPLAY_synchronize()  LCDTEST_CONCAT(LLDISPLAY_SWITCH_IMPL,_synchronize)(NULL)
#define LLDISPLAY_getDisplayBufferAddress()  LCDTEST_CONCAT(LLDISPLAY_SWITCH_IMPL,_getDisplayBufferAddress)(NULL)
#define LLDISPLAY_getBackBufferAddress()  LCDTEST_CONCAT(LLDISPLAY_SWITCH_IMPL,_getBackBufferAddress)(NULL)
#define LLDISPLAY_setDisplayBufferAddress(addr)  LCDTEST_CONCAT(LLDISPLAY_SWITCH_IMPL,_setDisplayBufferAddress__I)(NULL, addr)

#endif

#if LCD_LINK == LCD_LINK_WITHOUT_DISPLAY_STACK
/*
 * Internal display stack methods (never called)
 */
void LLDISPLAY_internal0(void){}
void* LLDISPLAY_internal1(void* env){}
#endif

#endif	// _LCD_TEST_INTERN
