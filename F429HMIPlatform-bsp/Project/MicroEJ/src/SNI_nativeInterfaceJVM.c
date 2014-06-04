/*!*******************************************************************************       
 * @file     usart_hardware.c
 * @brief    Configuration usart STM32F4xx
 * @version  1.0
 * @version  XC8 (Vx.x, MPLAB X (Vx.x)
 * @version 	PICXXFXX
 * @date     26-07-2013
 * @author   J.Fazli
*********************************************************************************/

//===== INCLUDEs ================================================================
#include "ImplementationProtocole.h"
#include "APPLI_InteractiveCookware.h"
#include "SNI_nativeInterfaceJVM.h"
//===== DEFINEs  ================================================================
//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
//===== STRUCTUREs ==============================================================
//===== PROTOTYPEs ==============================================================
//===== CODE ====================================================================
/***********************************************************************
 * @brief       Initialisation et declaration des ?quipements et des
 * @brief       interfaces du protocoles. Mettre cette fonction au debut
 * @brief       de la fonction principale  : "Main()"
 * @param[in]   None
 * @return      None
 **********************************************************************/
void Java_nativeIhm_PcomNative_init(void)
{
   IP_Initialisation();
	 APPLI_StartTask();		
	APPLI_InitPID();	
//	os_tsk_delete_self (); /* Goes away and task1 starts */	
	
}

void Java_nativeIhm_PcomNative_stateButton(jint state)
{
	stateAppli=state;
}

/*******************************************************************************
 * @brief       Reception des donnees du protocole.Fonction a mettre dans
 *              la boucle principale.
 * @param[in]   None
 * @return      None
 ******************************************************************************/
void Java_nativeIhm_PcomNative_DataReaded(jint *pTemperature,jchar *pReceptionTrameUart, jchar *pMemorisationPuissance,jchar *pSelectMode,jchar *pChoixFoyer,jchar *pStartAppli)
{
	*pTemperature = FramePoeleRecu.TemperatureMesure;
	*pReceptionTrameUart= ReceptionTrameUart;
	*pMemorisationPuissance= MemorisationPuissance;
	*pSelectMode = SelectMode;
	*pChoixFoyer = ChoixFoyer;
	
	if((ReceptionTrameUart >=1) && (FrameTabletteRecu.NumeroEtape > 0))
		*pStartAppli = 1;
	else 
		*pStartAppli = 0;
}