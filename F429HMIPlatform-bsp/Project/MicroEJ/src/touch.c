/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#include "touch.h"
#define LLINPUT_DEVICE_IMPL TOUCH
#include "LLINPUT_DEVICE_impl.h"
#include "LLINPUT_LISTENER_TOUCH.h"
#include "cpu.h"
#if MCU == 2
#include "stm32f2xx.h"
#include "stm322xg_eval_ioe.h"
#elif MCU == 4
#include "stm32f4xx.h"
#include "stm324xg_eval_ioe.h"
#elif MCU == 42
#include "stm32f4xx.h"
#include "stm324x9i_eval_ioe.h"
#include "stm324x9i_eval_ioe16.h"
#else
#error Invalid MCU
#endif
#include "ioe.h"
#include "microej.h"
#include "interrupts.h"
#include <rtl.h>
#ifdef CAP_TOUCH
#include "tscap.h"
extern sFinger gFinger[MAX_POINTS];
#endif
#include <stdio.h>

static struct TOUCH touch;

static uint8_t touch_pressed = MICROEJ_FALSE;

#define TOUCH_FILTER_COUNTER	0xF

static uint8_t IOE_TS_GetStateXY(uint16_t* x, uint16_t* y)
{
	int32_t x_tmp,y_tmp ;
	static uint16_t  _x = 0, _y = 0;
	uint16_t xDiff, yDiff	;

	uint8_t DataXY[3];

	_I2C_READDATABUFFER(IOE_ADDR, IOE_REG_TSC_DATA_XYZ,DataXY,sizeof(DataXY)) ;

	if (DataXY[0] == 0)
	{
		_x =  _y = *x = *y =0 ;
		return IOE_OK;
	}
	x_tmp = DataXY[0] << 4;
	x_tmp |= (DataXY[1] & 0xF0) >> 4;

	y_tmp = (DataXY[ 1] & 0x0F) << 8;
	y_tmp |= DataXY[ 2];

	/* first correction */

	x_tmp =  (x_tmp * LCD_PIXEL_WIDTH) >> 12 ;
	x_tmp = ((x_tmp * 48)/45) - 10;

	if(x_tmp <= 0)
		x_tmp = 0;

	//	y_tmp = (y_tmp * 240) >> 12 ;
	//	y_tmp = ((y_tmp * 240) / 217) - 12;
	y_tmp = (y_tmp * LCD_PIXEL_HEIGHT) >> 12 ;
	y_tmp = ((y_tmp * LCD_PIXEL_HEIGHT) / 265) - 12;
	y_tmp = LCD_PIXEL_HEIGHT - y_tmp ;
	if(y_tmp <= 0)
		y_tmp = 0;

	xDiff = x_tmp > _x? (x_tmp - _x): (_x - x_tmp);
	yDiff = y_tmp > _y? (y_tmp - _y): (_y - y_tmp);

	if (xDiff + yDiff > 5)
	{
		_x = x_tmp;
		_y = y_tmp;

	}

	*x = x_tmp;
	*y = y_tmp;

	/* Return pointer to the updated structure */
	return IOE_OK;
}

/**************************************/
IOE_Status_TypDef TOUCH_interrupt(void)
{
	uint8_t tstatus=0;
	tstatus = TsReadStatus() & 0x0F;	// read if touch is pressed

	while (tstatus > 0) // as long as touch is pressed
	{
		gFinger[0] = TsReadFinger(1); // no multi touch

		if (touch_pressed == MICROEJ_TRUE)
		{
			// pen was down => move event
			// don't care about returned value: if the event has not been managed
			// the event is "just" lost
			LLINPUT_LISTENER_TOUCH_touchMoved((LLINPUT_DEVICE*)&touch, (int32_t) gFinger[0].XCoord, (int32_t) gFinger[0].YCoord);
		}
		else
		{
			// pen was up => press event
			if (LLINPUT_LISTENER_TOUCH_touchPressed((LLINPUT_DEVICE*)&touch, (int32_t)gFinger[0].XCoord, (int32_t)gFinger[0].YCoord) == MICROEJ_TRUE)
			{
				// the event has been managed: we can store the new touch state
				// touch is pressed now
				touch_pressed = MICROEJ_TRUE;
				//printf("***touch pressed true***\n");
			}
			else
			{
				printf("system events queue is full!\n");
			}
		}
		os_dly_wait(TOUCH_FILTER_COUNTER);
		tstatus = TsReadStatus() & 0x0F;	// check if touch is still pressed or not
	}

	if (touch_pressed == MICROEJ_TRUE)
	{
		if (LLINPUT_LISTENER_TOUCH_touchReleased((LLINPUT_DEVICE*)&touch) == MICROEJ_TRUE)
		{
			// the event has been managed: we can store the new touch state
			// touch is released now
			//printf("***touch is released***\n");
			touch_pressed = MICROEJ_FALSE;
		}

	}
	return IOE_OK;
}

TOUCH* touchInitialize(void)
{
	LLINPUT_DEVICE_IMPL_new(&touch);
	/*
	#ifdef CAP_TOUCH // interrupt driven cap touch
		TsInit();
	#else
		_IOExpander_init();
	#endif
	 */

	// test touch with a separate task in polling mode
	TsInit();
	_IOExpander_init();


	return (TOUCH*)&touch;
}
