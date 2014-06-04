/*!*******************************************************************************       
 * @file     MICRO_Spi.h
 * @brief    Configuration usart STM32F4xx
 * @version  1.0
 * @version  XC8 (Vx.x, MPLAB X (Vx.x)
 * @date     26-07-2013
 * @author   J.Fazli
*********************************************************************************/
#ifndef  MICRO_SPI_H
#define MICRO_SPI_H
//===== INCLUDEs ================================================================
//#include <stm32f4xx.h>
//#include "stm32f4xxg_eval.h"
//#include "stm32f4xx_spi.h"
//===== DEFINEs  ================================================================
#define PORT_SPI SPI3
//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
//===== STRUCTUREs ==============================================================
//===== PROTOTYPEs ==============================================================
//===== CODE ====================================================================

/*!******************************************************************************
 * @brief       Init SPI to communicate with the both generator board
 * @param[in]   None
 * @return      None
 ********************************************************************************/
void MICRO_SPI_Init(void);
#endif