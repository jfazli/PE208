/*!*******************************************************************************       
 * @file     usart_hardware.c
 * @brief    Configuration usart STM32F4xx
 * @version  1.0
 * @version  XC8 (Vx.x, MPLAB X (Vx.x)
 * @date     26-07-2013
 * @author   J.Fazli
*********************************************************************************/
#ifndef  MICRO_SERIAL_H
#define MICRO_SERIAL_H
//===== INCLUDEs ================================================================
//===== DEFINEs  ================================================================
//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
//===== STRUCTUREs ==============================================================
//===== PROTOTYPEs ==============================================================
/*!******************************************************************************
 * @brief       Init Uart 4 to communicate with the stove through BLE at 115200bd
 * @param[in]   None
 * @return      None
 ********************************************************************************/
void MICRO_SERIAL_Init(void);
#endif