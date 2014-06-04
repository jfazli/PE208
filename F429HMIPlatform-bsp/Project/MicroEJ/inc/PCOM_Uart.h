/********************************************************************************
 *	FONCTION   	: Couche physique uart
 *********************************************************************************
 *	FICHIER		: PCOM_Uart.h
 *	PROJET		: 
 *	DEV			: JFA
 *  VERSION 	: 1.1
 *	OUTIL		: 
 * 	MODIF		: Modification noms des fonctions, ajout PC au début de la fonction	
 ********************************************************************************/
#ifndef PCOM_UART_H
#define PCOM_UART_H
//===== INCLUDEs ================================================================
//===== DEFINEs  ===============================================================
//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
//===== STRUCTUREs ==============================================================
//===== PROTOTYPEs ==============================================================
uint8_t PCUART_Send(uint8_t Data);
uint8_t PCUART_Interrupt(void);
uint8_t PCUART_Interrupt_TX(void);
uint8_t PCUART_Interrupt_TX(void);
#endif