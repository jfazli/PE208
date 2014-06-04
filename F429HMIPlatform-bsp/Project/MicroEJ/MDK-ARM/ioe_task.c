/*
 * C
 *
 * Copyright 2012-2013 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#include "ioe_task.h"
#include "ioe.h"

#include <RTL.h>

#define IOE_INTERRUPT_FLAG 0x1
static U64 ioeTaskStack[(512/8)]; // that is 4096 bits
OS_TID ioeTaskStackID;

__task void IOExpander_task(void)
{
	while(1)
	{
		if (os_evt_wait_and(IOE_INTERRUPT_FLAG, 0xffff) == OS_R_EVT)
		{
			IOExpander_interrupt();
		}
	}
}

uint8_t IOE_TASK_create_task(void)
{
	ioeTaskStackID = os_tsk_create_user(IOExpander_task, 10, &ioeTaskStack, sizeof(ioeTaskStack));
  //printf("IOExpander_task created with ID: ioeTaskStackID=%X\n", ioeTaskStackID );
	return (uint8_t)(ioeTaskStackID != 0);
}

uint8_t IOE_TASK_handle_interrupt(void)
{
	// Send an event to wake up the IOE interrupt management task
	//printf("***Send evt to %X task\n", ioeTaskStackID);
	isr_evt_set(IOE_INTERRUPT_FLAG,ioeTaskStackID);
	return MICROEJ_TRUE;
}
