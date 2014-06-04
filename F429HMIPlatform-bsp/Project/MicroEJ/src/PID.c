/*!*******************************************************************************       
 * @file     	PID.c
 * @brief    	PID library methode DC
 * @version  	1.0
 * @date     	15/05/2014
 * @author   	J.Fazli						
*********************************************************************************/

//===== INCLUDEs ================================================================
#include <stdint.h>
#include "PCOM_Time.h"
#include "PID.h"
//===== DEFINEs  ================================================================

//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
const int16_t ErrorSumMax=1000;
const int16_t ErrorSumMin=-1000;

#ifdef DEBUG
	int32_t error=0;	
	int32_t proportionnel=0;
	int32_t integrateur=0;
	int32_t derivateur=0;
	int32_t output =0;
#endif
//===== STRUCTUREs ==============================================================
struct PID_Result t_Debug_result;
//===== PROTOTYPEs ==============================================================
void PID_initialisation(struct PID_Parameter *pPIDstruct,SIZE_TYPE sampleTime,int32_t a0,int32_t a1,SIZE_PARAM Min, SIZE_PARAM Max);
void PID_Parameter(struct PID_Parameter *pPIDstruct,int32_t a0,int32_t a1);
void PID_Init_Saturateur(struct PID_Parameter *pPIDstruct,SIZE_PARAM Min, SIZE_PARAM Max);
void PID_Loop(struct PID_Parameter*  pPIDstruct,SIZE_PARAM input, SIZE_PARAM *pOutput, SIZE_PARAM consigne);
static void PID_LimitOverflow(int32_t *pInput,SIZE_PARAM Min, SIZE_PARAM Max);
//===== CODE ====================================================================
void PID_initialisation(struct PID_Parameter *pPIDstruct,SIZE_TYPE sampleTime,int32_t a0,int32_t a1,SIZE_PARAM Min, SIZE_PARAM Max)
{
	PID_sampleTime(pPIDstruct,sampleTime);
	PID_Parameter(pPIDstruct,a0,a1);
	PID_Init_Saturateur(pPIDstruct,Min,Max);
  (pPIDstruct->recovery.LastError)=0;
	(pPIDstruct->recovery.LastConsigne)=0;	
}

void PID_sampleTime(struct PID_Parameter *pPIDstruct,SIZE_TYPE sampleTime)
{
	pPIDstruct->time.delay = (SIZE_TYPE) sampleTime;
}

void PID_Parameter(struct PID_Parameter *pPIDstruct,int32_t a0,int32_t a1)
{
	pPIDstruct->coefficient.a0 = a0;
	pPIDstruct->coefficient.a1 = a1;
}

void PID_Init_Saturateur(struct PID_Parameter *pPIDstruct,SIZE_PARAM Min, SIZE_PARAM Max)
{
	pPIDstruct->saturateur.MIN = Min;
	pPIDstruct->saturateur.MAX = Max;
}


static void PID_LimitOverflow(int32_t *pInput,SIZE_PARAM Min, SIZE_PARAM Max)
{
	if(*pInput > Max) *pInput=Max;
	else if(*pInput < Min) *pInput=Min;

}

void PID_Loop(struct PID_Parameter*  pPIDstruct,SIZE_PARAM input, SIZE_PARAM *pOutput, SIZE_PARAM consigne)
{
#ifndef DEBUG
	int32_t error=0;	
	int32_t proportionnel=0;
	int32_t integrateur=0;
	int32_t derivateur=0;
	int32_t output = *pOutput;
#endif
	
	PCTIME_TempoStart(&(pPIDstruct->time),(pPIDstruct->time.delay));
	if(PCTIME_TempoIsElapsed(&(pPIDstruct->time)))
	{
		/*calcul de l'erreur à l'instant t*/
		error = consigne -  input;	
		PID_LimitOverflow(&error,-400, +400);
/************************************************************************************/
		output=error*(pPIDstruct->coefficient.a0);
		printf("\n\r %d",output);
		output-=(pPIDstruct->recovery.LastError)*(pPIDstruct->coefficient.a1);
		printf(";%d",output);
		output/=COEFF_MULT;
		printf(";%d",output);
		output+=(pPIDstruct->recovery.LastConsigne);
		printf(";%d",output);
		
		(pPIDstruct->recovery.LastError)=error;
		(pPIDstruct->recovery.LastConsigne)=consigne;	
		
		/*On s'assure que la sortie ne dépasse pas le saturateur*/
		if(output > (pPIDstruct->saturateur.MAX))
			output = (pPIDstruct->saturateur.MAX);
		else if(output < (pPIDstruct->saturateur.MIN))
			output = (pPIDstruct->saturateur.MIN);			
		
		*pOutput=output ;
		
		printf("\n\r %d",*pOutput);
		
	}
}

	


