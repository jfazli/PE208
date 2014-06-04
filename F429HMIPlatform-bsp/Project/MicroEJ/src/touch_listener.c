/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
 
#include "LLINPUT_LISTENER_TOUCH_impl.h"
#include "LLINPUT_DEVICE.h"
#include "LLINPUT.h"
#include "microej.h"
#include <stdint.h>

uint8_t LLINPUT_LISTENER_TOUCH_IMPL_touchMoved(LLINPUT_DEVICE* touch, int32_t x, int32_t y)
{
	return LLINPUT_sendTouchMovedEvent(LLINPUT_DEVICE_getID(touch), x, y);
}
uint8_t LLINPUT_LISTENER_TOUCH_IMPL_touchPressed(LLINPUT_DEVICE* touch, int32_t x, int32_t y)
{
	uint8_t ret = LLINPUT_sendTouchPressedEvent(LLINPUT_DEVICE_getID(touch), x, y);
// 	printf("id = %d\n", LLINPUT_DEVICE_getID(touch));
// 	printf("ret = %d\n", ret);
	return ret;
	//return LLINPUT_sendTouchPressedEvent(LLINPUT_DEVICE_getID(touch), x, y);
}
uint8_t LLINPUT_LISTENER_TOUCH_IMPL_touchReleased(LLINPUT_DEVICE* touch)
{
	return LLINPUT_sendTouchReleasedEvent(LLINPUT_DEVICE_getID(touch));
}

