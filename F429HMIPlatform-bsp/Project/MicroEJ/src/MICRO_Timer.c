/*!*******************************************************************************       
 * @file     MICRO_Timer.c
 * @brief    Configuration timer STM32F4xx
 * @version  1.0
 * @version  gcc & Eclipse
 * @date     24-09-2013
 * @author   J.Fazli
*********************************************************************************/

//===== INCLUDEs ================================================================
#include "PCOM_Generic.h"
#include "PCOM_Config.h"
#include "PCOM_Communication.h"
#include "PCOM_Gpio.h"
#include "PCOM_Time.h"
#include "stm32f4xx_tim.h"
#include "ImplementationProtocole.h"

//===== DEFINEs  ================================================================
#define MAX_TIMER_VALUE 0xFFFFFFFF
#define TC_CLOCK 1000000	// clock frequency in Hertz
//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
uint8_t _DataReceived[5] ;
uint8_t _DataSizeReceived;
uint8_t _SourceEqpOfData;
//===== STRUCTUREs ==============================================================

//===== PROTOTYPEs ==============================================================
//===== CODE ====================================================================

/*!******************************************************************************
 * @brief       .
 * @param[in]   None
 * @return      None
 ********************************************************************************/
void MICRO_TIMER_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	uint16_t PrescalerValue;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);

	/* Enable the TIM2 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	/* Time base configuration (1Khz) */
	/*
	 * Formule pour calculer la fréquence du timer
	 * FrqVoulu = (SystemCoreClock / 2) /((Prescaler-1) * (Period-1))
	 * FrqVoulu = 1Khz
	 * SystemCoreClock =168Mhz
	 * Period =1000
	 * Prescaler =  (SystemCoreClock / 2) / (1000 - 1)   ;
	 */

	/* Clock 1khz*/
//	TIM_TimeBaseStructure.TIM_Period = 1000-1 ;
//	TIM_TimeBaseStructure.TIM_Prescaler = 84-1;
//	TIM_TimeBaseStructure.TIM_ClockDivision=0;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	/* Clock 1µS */
	TIM_TimeBaseStructure.TIM_Period = 400 ;
	TIM_TimeBaseStructure.TIM_Prescaler = 1;
	TIM_TimeBaseStructure.TIM_ClockDivision=0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;


	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);

	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	/* TIM2 enable counter */
	TIM_Cmd(TIM5, ENABLE);

}

void TIM5_IRQHandler(void)
{
	static uint8_t toggle;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		PCTIME_Interrupt();
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	}
}

