/*!*******************************************************************************       
 * @file     	PID.c
 * @brief    	PID library
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


//===== STRUCTUREs ==============================================================
struct PID_Result t_Debug_result;
//===== PROTOTYPEs ==============================================================
void PID_initialisation(struct PID_Parameter *pPIDstruct,SIZE_TYPE sampleTime,SIZE_PARAM Min, SIZE_PARAM Max );
void PID_sampleTime(struct PID_Parameter *pPIDstruct,SIZE_TYPE sampleTime);
void PID_Parameter(struct PID_Parameter *pPIDstruct, SIZE_PARAM Kp,SIZE_PARAM Ki,SIZE_PARAM Kd);
void PID_Init_Saturateur(struct PID_Parameter *pPIDstruct,SIZE_PARAM Min, SIZE_PARAM Max);
void PID_Loop(struct PID_Parameter*  pPIDstruct,SIZE_PARAM input, SIZE_PARAM *pOutput, SIZE_PARAM consigne);
void PID_Init_LimitOverflow(SIZE_PARAM Min, SIZE_PARAM Max);
void PID_Debug(int32_t input, int32_t output,int32_t consigne);

//===== CODE ====================================================================
void PID_initialisation(struct PID_Parameter *pPIDstruct,SIZE_TYPE sampleTime,SIZE_PARAM Min, SIZE_PARAM Max )
{
	PID_sampleTime(pPIDstruct,sampleTime);
	PID_Init_Saturateur(pPIDstruct,Min,Max);
}

void PID_sampleTime(struct PID_Parameter *pPIDstruct,SIZE_TYPE sampleTime)
{
	pPIDstruct->time.delay = (SIZE_TYPE) sampleTime;
}

void PID_Parameter(struct PID_Parameter *pPIDstruct, SIZE_PARAM Kp,SIZE_PARAM Ki,SIZE_PARAM Kd)
{
	double value;
}

void PID_Init_Saturateur(struct PID_Parameter *pPIDstruct,SIZE_PARAM Min, SIZE_PARAM Max)
{
	pPIDstruct->saturateur.MIN = Min;
	pPIDstruct->saturateur.MAX = Max;
}


static void PID_LimitOverflow(SIZE_PARAM *pInput,SIZE_PARAM Min, SIZE_PARAM Max)
{
	if(*pInput > Max) *pInput=Max;
	else if(*pInput < Min) *pInput=Min;

}

void PID_Loop(struct PID_Parameter*  pPIDstruct,SIZE_PARAM input, SIZE_PARAM *pOutput, SIZE_PARAM consigne)
{
	int32_t error=0;	
	int32_t x1n,x2n;
	int32_t output = *pOutput;
	
	PCTIME_TempoStart(&(pPIDstruct->time),(pPIDstruct->time.delay));
	if(PCTIME_TempoIsElapsed(&(pPIDstruct->time)))
	{
		/*calcul de l'erreur à l'instant t*/
		error = consigne -  input;	
		
		// récurrence
		x1n = (pPIDstruct->coefficient.a1*pPIDstruct->last.x1n_1 + pPIDstruct->coefficient.b1*error)/COEFF_MULT_1;
		x2n = (pPIDstruct->coefficient.a2*pPIDstruct->last.x2n_1 + pPIDstruct->coefficient.b2*error)/COEFF_MULT_1;

		//calcul de la sortie
		output = (pPIDstruct->coefficient.c1*x1n + pPIDstruct->coefficient.c2*x2n)/ COEFF_MULT_2;


		// sauvegarde des états avec condition de saturation
		if(output <= 100 && output >= 0)
		{
			 pPIDstruct->last.x1n_1 = x1n;
			 pPIDstruct->last.x2n_1 = x2n;
		}

		/*On s'assure que la sortie ne dépasse pas le saturateur*/
		if(output > (pPIDstruct->saturateur.MAX))
			output = (pPIDstruct->saturateur.MAX);
		else if(output < (pPIDstruct->saturateur.MIN))
			output = (pPIDstruct->saturateur.MIN);			
		
		*pOutput=output ;
#ifdef DEBUG_PID
		PID_Debug(input,output,consigne);
#endif
	}
}

#ifdef DEBUG_PID
void PID_Debug(int32_t input, int32_t output,int32_t consigne)
{
	printf("\n\r%d,%d,%d",input,output,consigne);	
}
#endif
	


