/********************************************************************************
 *Fonction   : couche physique spi du protocole compatible stm32f4xx
 *********************************************************************************
 *	FICHIER	: PCOM_Spi.c
 *	PROJET	: 
 *	DEV	: JFA
 *  VERSION : 1.1
 *	OUTIL	:
 *  MODIFICATION : Modification noms des fonctions SPI_xxx en PCSPI_xxx		
 *	COMMENT :
 ********************************************************************************/

//===== INCLUDEs ================================================================
#include "PCOM_Generic.h"
#include "stm32f4xx_spi.h"
#include "MICRO_Spi.h"
#include "PCOM_Config.h"
#include "PCOM_CouchePhysique.h"
#include "PCOM_DataLink.h"
#include "PCOM_Time.h"
#include "PCOM_Gpio.h"
//#ifndef _PCOM_SPI_C
//#define _PCOM_SPI_C
#include "PCOM_Spi.h"
//#endif
//===== DEFINEs  ================================================================

//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
#if SPI_MASTER_ITF == ENABLE
volatile uint8_t SPI_MODE_MASTER_INT=SPI_IDLE;
#endif
//===== STRUCTUREs ==============================================================
//===== PROTOTYPEs ==============================================================
//===== CODE ====================================================================
//Configuration pour STM23F4xx
uint8_t PCSPI_Write_Master(uint8_t Data)
{
	while (SPI_I2S_GetFlagStatus(PORT_SPI, SPI_FLAG_TXE) == RESET){}
	SPI_I2S_SendData(PORT_SPI, Data);
	while (SPI_I2S_GetFlagStatus(PORT_SPI, SPI_FLAG_RXNE) == RESET){}
	Data =SPI_I2S_ReceiveData(PORT_SPI);

	PCTIME_Delay(3);

	return Data;
}

BOOLEAN PCSPI_Cycle(void)
{
    BOOLEAN bEnd=0;
    uint8_t Data;


    Data=PCSPI_Write_Master(0);
    if(PCDL_GestionDLE(Data,&dl_FrameInfoRecv[NUMBER_ITF_SPI]))
		bEnd=PCDL_ReceiveFrame(Data,&dl_FrameInfoRecv[NUMBER_ITF_SPI]);       //NUMBER_ITF_SPI_MASTER

    return bEnd;
}

