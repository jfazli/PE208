/* 
 * Copyright 2012-2013 IS2T. All rights reserved.
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
#define LLINPUT_LISTENER_TOUCH_CONCAT0(p,s) p ## s
#define LLINPUT_LISTENER_TOUCH_CONCAT(p,s) LLINPUT_LISTENER_TOUCH_CONCAT0(p,s)
#define LLINPUT_LISTENER_TOUCH_IMPL_touchPressed LLINPUT_LISTENER_TOUCH_CONCAT(LLINPUT_LISTENER_TOUCH_IMPL,_touchPressed__Lcom_is2t_inputs_listener_Device_2II)
#define LLINPUT_LISTENER_TOUCH_IMPL_touchReleased LLINPUT_LISTENER_TOUCH_CONCAT(LLINPUT_LISTENER_TOUCH_IMPL,_touchReleased__Lcom_is2t_inputs_listener_Device_2)
#define LLINPUT_LISTENER_TOUCH_IMPL_touchMoved LLINPUT_LISTENER_TOUCH_CONCAT(LLINPUT_LISTENER_TOUCH_IMPL,_touchMoved__Lcom_is2t_inputs_listener_Device_2II)
LLINPUT_LISTENER_TOUCH LLINPUT_LISTENER_TOUCH_getSingleton_instance;

/* 
 * Get the unique instance of this type within the system.
 */
LLINPUT_LISTENER_TOUCH* LLINPUT_LISTENER_TOUCH_IMPL_getSingleton(void)
{
	return &LLINPUT_LISTENER_TOUCH_getSingleton_instance;
}
