/*!*******************************************************************************       
 * @file     PCOM_Gpio.c
 * @brief    Configuration IO couche basse STM32
 * @version  1.0
 * @version  XC8 (Vx.x, MPLAB X (Vx.x)
 * @date     26-07-2013
 * @author   J.Fazli
*********************************************************************************/

//===== INCLUDEs ================================================================
#include "PCOM_Generic.h"
#include "PCOM_Config.h"
#include "PCOM_Gpio.h"
//===== DEFINEs  ================================================================
//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
//===== STRUCTUREs ==============================================================
//===== PROTOTYPEs ==============================================================
//===== CODE ====================================================================
void PCGPIO_WriteBit(SIZE_ADRESS_PORT_IO *PortIo, SIZE_PIN PinIO, uint8_t state )
{
	GPIO_WriteBit(PortIo, PinIO,state);
}

uint8_t PCGPIO_ReadBit(SIZE_ADRESS_PORT_IO *PortIo, SIZE_PIN PinIO)
{
	uint8_t StateOfPin=0;

	StateOfPin=GPIO_ReadInputDataBit(PortIo,PinIO);

	return StateOfPin;
}
