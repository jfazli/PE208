/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#include "buttons.h"
#define LLINPUT_DEVICE_IMPL BUTTONS
#include "LLINPUT_DEVICE_impl.h"
#include "LLINPUT_LISTENER_BUTTONS.h"
#include "cpu.h"
#if MCU == 2
#include "stm32f2xx.h"
#include "stm322xg_eval.h"
#elif MCU == 4
#include "stm32f4xx.h"
#include "stm324xg_eval.h"
#elif MCU == 42
#include "stm32f4xx.h"
#include "fmc_sram.h"
#include "fmc_sdram.h"
#include "stm32f4xx_fmc.h"
#include "stm324x9i_eval.h"

#else
#error Invalid MCU
#endif
#include "microej.h"
#include "interrupts.h"

static GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {WAKEUP_BUTTON_GPIO_PORT, TAMPER_BUTTON_GPIO_PORT};

static const uint16_t BUTTON_PIN[BUTTONn] = {WAKEUP_BUTTON_PIN, TAMPER_BUTTON_PIN};

static const uint32_t BUTTON_CLK[BUTTONn] = {WAKEUP_BUTTON_GPIO_CLK, TAMPER_BUTTON_GPIO_CLK};

static const uint16_t BUTTON_EXTI_LINE[BUTTONn] = {WAKEUP_BUTTON_EXTI_LINE,
		TAMPER_BUTTON_EXTI_LINE};

static const uint8_t BUTTON_PORT_SOURCE[BUTTONn] = {WAKEUP_BUTTON_EXTI_PORT_SOURCE,
		TAMPER_BUTTON_EXTI_PORT_SOURCE};

static const uint8_t BUTTON_PIN_SOURCE[BUTTONn] = {WAKEUP_BUTTON_EXTI_PIN_SOURCE,
		TAMPER_BUTTON_EXTI_PIN_SOURCE};
static const uint8_t BUTTON_IRQn[BUTTONn] = {WAKEUP_BUTTON_EXTI_IRQn, TAMPER_BUTTON_EXTI_IRQn};

static const uint8_t BUTTON_ID[BUTTONn] = {0, 1};
static const uint8_t BUTTON_REVERSE[BUTTONn] = {MICROEJ_FALSE, MICROEJ_TRUE};
static uint8_t BUTTON_PRESSED[BUTTONn] = {MICROEJ_FALSE, MICROEJ_FALSE};

static struct BUTTONS buttons;

void BUTTONS_event(Button_TypeDef Button)
{
	uint8_t button_pressed;

	if (GPIO_ReadInputDataBit(BUTTON_PORT[Button], BUTTON_PIN[Button]) == Bit_SET)
	{
		// GPIO == 1

		if (BUTTON_REVERSE[Button] == MICROEJ_TRUE)
		{
			// GPIO == 1 means "released"
			button_pressed = MICROEJ_FALSE;
		}
		else
		{
			// GPIO == 1 means "pressed"
			button_pressed = MICROEJ_TRUE;
		}
	}
	else
	{
		// GPIO == 0

		if (BUTTON_REVERSE[Button] == MICROEJ_TRUE)
		{
			// GPIO == 0 means "pressed"
			button_pressed = MICROEJ_TRUE;
		}
		else
		{
			// GPIO == 0 means "released"
			button_pressed = MICROEJ_FALSE;
		}
	}

	if (button_pressed == MICROEJ_TRUE)
	{
		// button is currently pressed

		if (BUTTON_PRESSED[Button] == MICROEJ_TRUE)
		{
			// button was pressed => repeat event
			// don't care about returned value: if the event has not been managed
			// the event is "just" lost
			LLINPUT_LISTENER_BUTTONS_buttonRepeated((LLINPUT_DEVICE*)&buttons, BUTTON_ID[Button]);
		}
		else
		{
			// button was released => press event
			if (LLINPUT_LISTENER_BUTTONS_buttonPressed((LLINPUT_DEVICE*)&buttons, BUTTON_ID[Button]) == MICROEJ_TRUE)
			{
				// the event has been managed: we can store the new button state
				// button is pressed now
				BUTTON_PRESSED[Button] = MICROEJ_TRUE;
			}
			else
			{
				// The event has not been managed because the system events queue is full.
				// Two solutions:
				// - increase the size of the system events queue (see Memory page of the
				// Java application launcher
				// - limit rate of events of your drivers (for example, calibrate touch to
				// send an event at most every X ms)

				// We have not to change the button state to prevent incoherence state (to
				// not send a futur release event whereas the press event has never been
				// sent!)
			}
		}
	}
	else
	{
		// button is now released

		if (BUTTON_PRESSED[Button] == MICROEJ_TRUE)
		{
			// button was pressed => release event
			if (LLINPUT_LISTENER_BUTTONS_buttonReleased((LLINPUT_DEVICE*)&buttons, BUTTON_ID[Button]) == MICROEJ_TRUE)
			{
				// the event has been managed: we can store the new button state
				// button is released now
				BUTTON_PRESSED[Button] = MICROEJ_FALSE;
			}
			else
			{
				// The event has not been managed because the system events queue is full.
				// Two solutions:
				// - increase the size of the system events queue (see Memory page of the
				// Java application launcher
				// - limit rate of events of your drivers (for example, calibrate touch to
				// send an event at most every X ms)

				// We have not to change the button state to prevent incoherence state (to
				// not send a futur press event whereas the release event has never been
				// sent!)
			}
		}
		// else: already released
	}
}

void BUTTONS_interrupt(Button_TypeDef Button)
{
	if(EXTI_GetITStatus(BUTTON_EXTI_LINE[Button]) != RESET)
	{
		BUTTONS_event(Button);
		EXTI_ClearITPendingBit(BUTTON_EXTI_LINE[Button]);
	}
}

void BUTTONS_init(Button_TypeDef Button)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the BUTTON Clock */
	RCC_AHB1PeriphClockCmd(BUTTON_CLK[Button], ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure Button pin as input */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[Button];
	GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);

	/* Connect Button EXTI Line to Button GPIO Pin */
	SYSCFG_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);

	/* Configure Button EXTI line */
	EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set Button EXTI Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  This function handles External line 0 interrupt request.
 * @param  None
 * @retval None
 */
/*void EXTI0_IRQHandler(void)
{
	enterInterrupt();
	// XXX missing release event !!!
	BUTTONS_interrupt(BUTTON_WAKEUP);
	leaveInterrupt();
}*/

/**
 * @brief  This function handles External lines 15 to 10 interrupt request.
 * @param  None
 * @retval None
 */
//void EXTI15_10_IRQHandler(void)
//{
//	enterInterrupt();
//	// XXX missing release event !!!
//	BUTTONS_interrupt(BUTTON_TAMPER);
//	leaveInterrupt();
//}

void setButtonsInterrupts(FunctionalState state)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[BUTTON_WAKEUP] | BUTTON_EXTI_LINE[BUTTON_TAMPER] ;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = state;
	EXTI_Init(&EXTI_InitStructure);
}

void enableButtonsInterrupts(void)
{
	setButtonsInterrupts(ENABLE);
}

void disableButtonsInterrupts(void)
{
	setButtonsInterrupts(DISABLE);
}

BUTTONS* buttonsInitialize(void)
{
	LLINPUT_DEVICE_IMPL_new(&buttons);
	//BUTTONS_init(BUTTON_WAKEUP);
	//BUTTONS_init(BUTTON_TAMPER);
	return (BUTTONS*)&buttons;
}
