/*!*******************************************************************************       
 * @file     MICRO_Serial.c
 * @brief    Configuration usart STM32F4xx
 * @version  1.0
 * @version  
 * @date     23-09-2013
 * @author   J.Fazli
*********************************************************************************/

//===== INCLUDEs ================================================================
#include "PCOM_Generic.h"
#include "stm32f4xx_usart.h"
#include "MICRO_Serial.h"

//===== DEFINEs  ================================================================

//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
//===== STRUCTUREs ==============================================================
//===== PROTOTYPEs ==============================================================
//===== CODE ====================================================================

/*!******************************************************************************
 * @brief       Initialize uart 4 for communicate with BLE
 * @param[in]   None
 * @return      None
 ********************************************************************************/
void MICRO_SERIAL_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure; // this is used to configure the NVIC

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  /*TX*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//Init à 0
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	//disable pullup or pull down
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  /*RX */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//Init à 0
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;	//disable pullup or pull down
	GPIO_Init(GPIOA,&GPIO_InitStructure);

#ifdef DEBIT_BLE_9600
	USART_InitStructure.USART_BaudRate = 9600;
#else
	USART_InitStructure.USART_BaudRate = 115200;
#endif
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);

	//Interruption
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;        // we want to configure the UART4 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// this sets the priority group of the USART3 interrupts
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;       // this sets the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          // the UART4 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);

	USART_Cmd(UART4, ENABLE);

}

void UART4_IRQHandler(void){	//fonction d'interruption pour lire uart 2, voir fichier "startup_stm32f4xx.s"

	if(USART_GetITStatus(UART4,USART_IT_RXNE))
	{
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);
		PCUART_Interrupt();
	}

}
