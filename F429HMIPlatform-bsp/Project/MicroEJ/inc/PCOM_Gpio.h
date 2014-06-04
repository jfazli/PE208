/*!*******************************************************************************       
 * @file     PCOM_Gpio.h
 * @brief    Configuration IO
 * @version  1.0
 * @version  
 * @date     29-09-2013
 * @author   J.Fazli
*********************************************************************************/
#ifndef  PCOM_GPIO_H
#define PCOM_GPIO_H
//===== INCLUDEs ================================================================
//===== DEFINEs  ================================================================
//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
//===== STRUCTUREs ==============================================================
//===== PROTOTYPEs ==============================================================
//===== CODE ====================================================================

/*!******************************************************************************
 * @brief       Definition de l'�tat de l'IO, utilis�e dans les fonctions
 * 				SetSlaveSelect() et SetSlaveTalk()
 * @param[in]   None
 * @return      None
 ********************************************************************************/
void PCGPIO_WriteBit(SIZE_ADRESS_PORT_IO *PortIo, SIZE_PIN PinIO, uint8_t state );

/*!******************************************************************************
 * @brief       Lecture de l'�tat de la pin, utils�e dans la fonction
 * 					GetSlaveTalk.
 * @param[in]   None
 * @return      None
 ********************************************************************************/
uint8_t PCGPIO_ReadBit(SIZE_ADRESS_PORT_IO *PortIo, SIZE_PIN PinIO);
#endif