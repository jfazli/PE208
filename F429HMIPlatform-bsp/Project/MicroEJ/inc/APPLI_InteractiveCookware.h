/*!*******************************************************************************       
 * @file		usart_hardware.c
 * @brief    	Configuration usart STM32F4xx
 * @version  	V1.0
 * @version  	XC8 (Vx.x, MPLAB X (Vx.x)
 * @version 	PICXXFXX
 * @date     	26-07-2013
 * @author   	J.Fazli
*********************************************************************************/
#ifndef  APPLI_InteractiveCookware_H
#define APPLI_InteractiveCookware_H
//===== INCLUDEs ================================================================
#include <stdint.h>
//===== DEFINEs  ================================================================
#define MODE_INIT 						0
#define MODE_SCRUTATION				1
#define MODE_FONCTIONNEMENT		2

#define FOYER_FOYER2					EQP_2
#define FOYER_FOYER1					EQP_1

#define STATE_APPLI_WAIT		(int16_t)0
#define STATE_APPLI_START		(int16_t)1
#define STATE_APPLI_STOP		(int16_t)2

#define TIME_TOGGLE_SCRUTATION	(500*MS)
#define TIME_FIN_SCRUTATION			(5000*MS)
#define TIME_SEND_FRAME_TABLETTE (1000*MS)

//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
extern volatile int32_t stateAppli;
extern uint8_t SelectMode;
extern uint8_t ChoixFoyer;
extern uint8_t MemorisationPuissance;

//===== STRUCTUREs ==============================================================
extern struct PCTIME_Tempo t_tempsNonreceptionTrameUart;
//===== PROTOTYPEs ==============================================================
void APPLI_AppliLogiciel(void);
void APPLI_StartTask(void);
void APPLI_ReceptionTrameUART(void);
extern void APPLI_InitPID(void);
#endif