/*!*******************************************************************************       
 * @file     
 * @brief    
 * @version  1.0
 * @version  
 * @date     
 * @author   J.Fazli
*********************************************************************************/

//===== INCLUDEs ================================================================
#include "ImplementationProtocole.h"
#include "nativePoeleSp.h"
#include <sp.h>
#include "stm32f4xx.h"
#include "stm324x9i_eval.h"
#include <stdio.h>
//===== DEFINEs  ================================================================
#define Forecast_ID 						0

#define Forecast_SP_IH_POELE 		0
//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
//===== STRUCTUREs ==============================================================
struct SpPoele
{
	uint16_t temperature;
	uint16_t puissance;
	uint16_t receptionTrame;
	uint16_t selectionMode;
	uint16_t choixFoyer;
};
struct SpPoele spRecoitC;
struct SpPoele spEnvoieJava;
//===== PROTOTYPEs ==============================================================
void copyStructure(void);
int32_t compareStructure(void);
//===== CODE ====================================================================

/*!******************************************************************************
 * @brief       
 * @param[in]   None
 * @return      None
 ********************************************************************************/
void spPoelePublication()
{
	int32_t bEnd;
	
	Java_nativeIhm_PcomNative_DataReaded(&(spEnvoieJava.temperature),&(spEnvoieJava.receptionTrame),&(spEnvoieJava.puissance),&(spEnvoieJava.selectionMode),&(spEnvoieJava.choixFoyer));
	
	ShieldedPlug database = SP_getDatabase(Forecast_ID);
	
	if(compareStructure() !=0)
	{
		bEnd=SP_write(database,Forecast_SP_IH_POELE,&spEnvoieJava);
		if(bEnd != 0)
		{
			printf("\r\n bEnd:%d",bEnd);
		}
		copyStructure();
	}
	
}

void copyStructure(void)
{
	spRecoitC.puissance				= spEnvoieJava.puissance;
	spRecoitC.receptionTrame  = spEnvoieJava.receptionTrame;
	spRecoitC.selectionMode		= spEnvoieJava.selectionMode;
	spRecoitC.temperature			= spEnvoieJava.temperature;
}

int32_t compareStructure(void)
{
	int32_t bEnd=0;
		
	if(spRecoitC.puissance != spEnvoieJava.puissance)bEnd=1;
	if(spRecoitC.receptionTrame != spEnvoieJava.receptionTrame)bEnd=1;
	if(spRecoitC.selectionMode != spEnvoieJava.selectionMode)bEnd=1;
	if(spRecoitC.temperature != spEnvoieJava.temperature)bEnd=1;
	
	return bEnd;
}