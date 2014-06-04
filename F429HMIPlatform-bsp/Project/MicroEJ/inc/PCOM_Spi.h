/********************************************************************************
 *Fonction  :
 *********************************************************************************
 *	FICHIER	: Communication.h
 *	PROJET	: 
 *	DEV		: JFA
 *  VERSION : 1.1
 *	OUTIL	: 
 *  MODIFICATION : Modifications fonctions de SPI_xx en PCSPI_xxx
 ********************************************************************************/
#ifndef PCOM_SPI_H
#define PCOM_SPI_H
//===== INCLUDEs ================================================================
//===== DEFINEs  ===============================================================
#if SPI_MASTER_ITF == ENABLE
//SPI Mode Definitions
#define	SPI_IDLE	0
#define	RX		1
#define	TX		2
#endif
//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
#if SPI_MASTER_ITF == ENABLE
extern volatile uint8_t SPI_MODE_MASTER_INT;
#endif


//===== STRUCTUREs ==============================================================
//===== PROTOTYPEs ==============================================================
/******************************************************************************/
/**
 * Write & Read the slave SPI
 * @param Data: The data to send at the slave
 *
 * @return the data received from the Slave
 ******************************************************************************/
uint8_t PCSPI_Write_Master(uint8_t Data);
/******************************************************************************/
/**
 *Is used for the interrupt for the SPI slave
 *IN: void
 * @return 1 if a complete frame is Sended.
 ******************************************************************************/
BOOLEAN PCSPI_MasterInterrupt(void);
/******************************************************************************/
/**
 *Is used for the interrupt for the SPI slave
 * IN: void
 * @return 1 if a complete frame is Sended.
 ******************************************************************************/
BOOLEAN PCSPI_SlaveInterrupt(void);
/******************************************************************************/
/**
 *Is used for the interrupt for the SPI slave
 * IN: void
 * OUT:return 1 if a complete frame is received.
 ******************************************************************************/
BOOLEAN PCSPI_Cycle(void);

#endif
