/* 
 * Copyright 2012-2013 IS2T. All rights reserved.
 * Modification and distribution is permitted under certain conditions.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
/* 
 * AUTOGENERATED by COMP-ICETEA-ARMCC-ARM - 1.0.0 - DO NOT EDIT IT
 */
/**
 * The buttons listener implementation reacts on hardware buttons events.<br>
 * The hardware buttons group is represented by a {@link Device}.
 */
#ifndef _LLINPUT_LISTENER_BUTTONS
#define _LLINPUT_LISTENER_BUTTONS 
#include <intern/LLINPUT_LISTENER_BUTTONS.h>
#include <stdint.h>
#include <LLINPUT_DEVICE.h>
#ifdef __cplusplus
	extern "C" {
#endif
// --------------------------------------------------------------------------------
// -                      Functions provided by the platform                      -
// --------------------------------------------------------------------------------

/**
 * A PRESSED action has occurred on the given button of the given buttons group.
 * @param buttons the buttons group
 * @param buttonId the button ID
 * @return 1 if the button action has been managed by the listener
 */
uint8_t LLINPUT_LISTENER_BUTTONS_buttonPressed(LLINPUT_DEVICE* buttons, int32_t buttonId);

/**
 * A RELEASED action has occurred on the given button of the given buttons group.
 * @param buttons the buttons group
 * @param buttonId the button ID
 * @return 1 if the button action has been managed by the listener
 */
uint8_t LLINPUT_LISTENER_BUTTONS_buttonReleased(LLINPUT_DEVICE* buttons, int32_t buttonId);

/**
 * A REPEATED action has occurred on the given button of the given buttons group.
 * @param buttons the buttons group
 * @param buttonId the button ID
 * @return 1 if the button action has been managed by the listener
 */
uint8_t LLINPUT_LISTENER_BUTTONS_buttonRepeated(LLINPUT_DEVICE* buttons, int32_t buttonId);

#ifdef __cplusplus
	}
#endif
#endif