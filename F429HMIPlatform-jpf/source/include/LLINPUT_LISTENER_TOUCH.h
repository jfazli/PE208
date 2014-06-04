/* 
 * Copyright 2012-2013 IS2T. All rights reserved.
 * Modification and distribution is permitted under certain conditions.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
/* 
 * AUTOGENERATED by COMP-ICETEA-ARMCC-ARM - 1.0.0 - DO NOT EDIT IT
 */
/**
 * The touch listener implementation reacts on hardware touch events.<br>
 * The touch device is represented by a {@link Device}.
 */
#ifndef _LLINPUT_LISTENER_TOUCH
#define _LLINPUT_LISTENER_TOUCH 
#include <intern/LLINPUT_LISTENER_TOUCH.h>
#include <stdint.h>
#include <LLINPUT_DEVICE.h>
#ifdef __cplusplus
	extern "C" {
#endif
// --------------------------------------------------------------------------------
// -                      Functions provided by the platform                      -
// --------------------------------------------------------------------------------

/**
 * A PRESSED action has occurred on the given touch. 
 * @param touch the touch device instance
 * @param x the x-coordinate where the touch action has occurred
 * @param y the y-coordinate where the touch action has occurred
 * @return 1 if the touch action has been managed by the listener
 */
uint8_t LLINPUT_LISTENER_TOUCH_touchPressed(LLINPUT_DEVICE* touch, int32_t x, int32_t y);

/**
 * A RELEASED action has occurred on the given touch. 
 * @param touch the touch device instance
 * @return 1 if the touch action has been managed by the listener
 */
uint8_t LLINPUT_LISTENER_TOUCH_touchReleased(LLINPUT_DEVICE* touch);

/**
 * A MOVE action has occurred on the given touch. 
 * @param touch the touch device instance
 * @param x the new touch x-coordinate
 * @param y the new touch y-coordinate 
 * @return 1 if the touch action has been managed by the listener
 */
uint8_t LLINPUT_LISTENER_TOUCH_touchMoved(LLINPUT_DEVICE* touch, int32_t x, int32_t y);

#ifdef __cplusplus
	}
#endif
#endif