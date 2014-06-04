/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#include "cpu.h"
#if MCU == 2
#include "stm32f2xx.h"
#include "stm322xg_eval.h"
#elif MCU == 4
#include "stm32f4xx.h"
#include "stm324xg_eval.h"
#elif MCU == 42
#include "stm32f4xx.h"
#include "stm324x9i_eval.h"
#include "usart_1.h"
#else
#error Invalid MCU
#endif
#include <stdio.h>

#define PUTCHAR_COM_PORT  COM1
#define PUTCHAR_USART	  EVAL_COM1

int putchar_initialized = 0;

USART_InitTypeDef USART_InitStructure;

extern int getkey   (void){
 	return 0;
}

void uart_init(COM_TypeDef port){
/* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM_EVAL_COMInit(port, &USART_InitStructure);
}

int fputc(int ch, FILE *f){

	if(!putchar_initialized){
		uart_init(PUTCHAR_COM_PORT);
		putchar_initialized = 1;
		while (USART_GetFlagStatus(PUTCHAR_USART, USART_FLAG_TC) == RESET)
  		{}
	}
	
	USART_SendData(PUTCHAR_USART, (uint8_t) ch);
	
	/* Loop until the end of transmission */
  	while (USART_GetFlagStatus(PUTCHAR_USART, USART_FLAG_TC) == RESET)
  	{}
		
	return ch;
}






