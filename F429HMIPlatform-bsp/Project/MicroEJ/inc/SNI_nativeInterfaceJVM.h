/*!*******************************************************************************       
 * @file		SNI_nativeInterfaceJVM.h
 * @brief    	Configuration usart STM32F4xx
 * @version  	V1.0
 * @version  	XC8 (Vx.x, MPLAB X (Vx.x)
 * @version 	PICXXFXX
 * @date     	26-07-2013
 * @author   	J.Fazli
*********************************************************************************/
#ifndef  SNI_NATIVEINTERFACEJVM_H
#define SNI_NATIVEINTERFACEJVM_H
//===== INCLUDEs ================================================================
#include "sni.h"
//===== DEFINEs  ================================================================
//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
//===== STRUCTUREs ==============================================================
//===== PROTOTYPEs ==============================================================
/*********************************************************************//**
 * @brief      Gestion de l'acquittement. A mettre dans la boucle principale
 * @param[in]   None
 * @return      None
 **********************************************************************/
//jchar Java_nativeIhm_PcomNative_acqProtocole(void);
/*******************************************************************************
 * @brief       Fonction d'émission de donnée.
 * @param[in]   None
 * @return      None
 ******************************************************************************/
//void Java_nativeIhm_PcomNative_setDataSending(jchar Eqp,jchar * pDataSending,jchar SizeOfData);

/*******************************************************************************
 * @brief       Liberation de la trame d'envoie
 * @param[in]   None
 * @return      None
 ******************************************************************************/
//void Java_nativeIhm_PcomNative_FreeFrame(void);
/*******************************************************************************
 * @brief       Lecture par la JVM des data gérés dans la couche basse
 *							
 * @param[in]   None
 * @return      None
 ******************************************************************************/
void Java_nativeIhm_PcomNative_DataReaded(jint *pTemperature,jchar *pReceptionTrameUart, jchar *pMemorisationPuissance,jchar *pSelectMode,jchar *pChoixFoyer,jchar *pStartAppli);
void Java_nativeIhm_PcomNative_stateButton(jint state);
void Java_nativeIhm_PcomNative_init(void);

/*!******************************************************************************
 * @brief       Enable Touch Screen Controller.
 * @param[in]   None
 * @return      None
 ********************************************************************************/
#endif