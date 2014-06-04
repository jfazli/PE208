/* 
 * Copyright 2011-2013 IS2T. All rights reserved.
 * Modification and distribution is permitted under certain conditions.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
/* 
 * AUTOGENERATED by COMP-ICETEA-ARMCC-ARM - 1.0.0 - DO NOT EDIT IT
 */
/* 
 * IceTea to C mapping support. Shall not be directly included by clients.
 */
#include <stdint.h>
#define LLDISPLAY_SWITCH_CONCAT0(p,s) p ## s
#define LLDISPLAY_SWITCH_CONCAT(p,s) LLDISPLAY_SWITCH_CONCAT0(p,s)
#define __icetea__nid__LLDISPLAY_SWITCH___LLDISPLAY_SWITCH_IMPL LLDISPLAY_SWITCH_CONCAT(__icetea__nid__LLDISPLAY_SWITCH___,LLDISPLAY_SWITCH_IMPL)
#define LLDISPLAY_SWITCH_IMPL_new LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_new)
#define LLDISPLAY_SWITCH_IMPL_initialize LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_initialize)
#define LLDISPLAY_SWITCH_IMPL_getWidth LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_getWidth__Z)
#define LLDISPLAY_SWITCH_IMPL_getHeight LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_getHeight__Z)
#define LLDISPLAY_SWITCH_IMPL_convertRGBColorToDisplayColor LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_convertRGBColorToDisplayColor__I)
#define LLDISPLAY_SWITCH_IMPL_convertDisplayColorToRGBColor LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_convertDisplayColorToRGBColor__I)
#define LLDISPLAY_SWITCH_IMPL_setContrast LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_setContrast__I)
#define LLDISPLAY_SWITCH_IMPL_getContrast LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_getContrast)
#define LLDISPLAY_SWITCH_IMPL_setBacklight LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_setBacklight__I)
#define LLDISPLAY_SWITCH_IMPL_hasBackLight LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_hasBackLight)
#define LLDISPLAY_SWITCH_IMPL_getBacklight LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_getBacklight)
#define LLDISPLAY_SWITCH_IMPL_backlightOn LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_backlightOn)
#define LLDISPLAY_SWITCH_IMPL_backlightOff LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_backlightOff)
#define LLDISPLAY_SWITCH_IMPL_getFontAlphaAdjustment LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_getFontAlphaAdjustment__I)
#define LLDISPLAY_SWITCH_IMPL_getWorkingBufferStartAddress LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_getWorkingBufferStartAddress)
#define LLDISPLAY_SWITCH_IMPL_getWorkingBufferEndAddress LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_getWorkingBufferEndAddress)
#define LLDISPLAY_SWITCH_IMPL_isColor LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_isColor)
#define LLDISPLAY_SWITCH_IMPL_getNumberOfColors LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_getNumberOfColors)
#define LLDISPLAY_SWITCH_IMPL_synchronize LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_synchronize)
#define LLDISPLAY_SWITCH_IMPL_getDisplayBufferAddress LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_getDisplayBufferAddress)
#define LLDISPLAY_SWITCH_IMPL_getBackBufferAddress LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_getBackBufferAddress)
#define LLDISPLAY_SWITCH_IMPL_setDisplayBufferAddress LLDISPLAY_SWITCH_CONCAT(LLDISPLAY_SWITCH_IMPL,_setDisplayBufferAddress__IIIII)
void __icetea__nid__LLDISPLAY_SWITCH___LLDISPLAY_SWITCH_IMPL(void);
void* com_is2t_display_backend_DisplaySwitch____4init_5(void* env);

/**
 * Creates a display SWITCH stack backend driver.
 */
void LLDISPLAY_SWITCH_IMPL_new(LLDISPLAY_SWITCH_IMPL* env)
{
	((LLDISPLAY_SWITCH*)env)->__icetea__id__ = (int32_t)(&__icetea__nid__LLDISPLAY_SWITCH___LLDISPLAY_SWITCH_IMPL);
	com_is2t_display_backend_DisplaySwitch____4init_5(env);
}
