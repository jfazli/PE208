/********************************************************************************
 *Fonction  : gestion couche physique de l'uart
 *********************************************************************************
 *	FICHIER	: PCOM_Uart.c
 *	PROJET	: 
 *	DEV		: JFA
 *  VERSION : 1.1	 
 *	OUTIL	: 
 *	MODIFICATION : Modification nom fonction de UART_xx en PCUART_xx
 *					version STM32F4xx
 ********************************************************************************/

//===== INCLUDEs ================================================================
#include "PCOM_Generic.h"
#include "PCOM_Config.h"
#include "stm32f4xx_usart.h"
#include "MICRO_Serial.h"
#include "PCOM_DataLink.h"
#include "ImplementationProtocole.h"
//===== DEFINEs  ===============================================================

//===== TYPEDEFs ===============================================================
//===== VARIABLEs ===============================================================
#if SERIAL_ITF == ENABLE
//static uint8_t dummy=0;
#endif
//===== STRUCTUREs ==============================================================
//===== PROTOTYPEs ==============================================================

uint8_t PCUART_Send(uint8_t Data)
{
	USART_SendData(UART4,Data);
	/* Loop until the end of transmission */
  	while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET){}
    return 0;
}



uint8_t PCUART_Interrupt(void)
{
	BOOLEAN bEnd=0;
    uint8_t Data=0;
		
	Data=USART_ReceiveData(UART4);
	if(PCDL_GestionDLE(Data,&dl_FrameInfoRecv[NUMBER_ITF_SERIAL]))
		bEnd=PCDL_ReceiveFrame(Data,&dl_FrameInfoRecv[NUMBER_ITF_SERIAL]);

	return bEnd;
}


uint8_t PCUART_Interrupt_TX(void)
{
    BOOLEAN bEnd = 0;

//    if(ENABLE_INT_TX==1 && FLAG_INT_TX==1)
//    {
//
//        bEnd=PCDL_SendFrame(&(Datas),dl_FrameInfoEm.sizeOfData);
//        BUFF_TX_UART = Datas;
//        if(bEnd)
//        {
//            PCDL_StopOfFrame();
//        }
//    }

    return bEnd;
}

