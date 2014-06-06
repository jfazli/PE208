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
const int16_t ErrorSumMax=+400;
const int16_t ErrorSumMin=-400;


//===== STRUCTUREs ==============================================================
struct PID_Result t_Debug_result;
//===== PROTOTYPEs ==============================================================
void PID_initialisation(struct PID_Parameter *pPIDstruct,SIZE_TYPE sampleTime,SIZE_PARAM Kp,SIZE_PARAM Ki,SIZE_PARAM Kd,SIZE_PARAM Min, SIZE_PARAM Max );
void PID_sampleTime(struct PID_Parameter *pPIDstruct,SIZE_TYPE sampleTime);
void PID_Parameter(struct PID_Parameter *pPIDstruct, SIZE_PARAM Kp,SIZE_PARAM Ki,SIZE_PARAM Kd);
void PID_Init_Saturateur(struct PID_Parameter *pPIDstruct,SIZE_PARAM Min, SIZE_PARAM Max);
void PID_Loop(struct PID_Parameter*  pPIDstruct,SIZE_PARAM input, SIZE_PARAM *pOutput, SIZE_PARAM consigne);
void PID_Init_LimitOverflow(SIZE_PARAM Min, SIZE_PARAM Max);
static void PID_Debug(struct PID_Parameter*  pPIDstruct,int32_t error, int32_t proportionnel, int32_t integrateur, int32_t derivateur, int16_t output);

//===== CODE ====================================================================
void PID_initialisation(struct PID_Parameter *pPIDstruct,SIZE_TYPE sampleTime,SIZE_PARAM Kp,SIZE_PARAM Ki,SIZE_PARAM Kd,SIZE_PARAM Min, SIZE_PARAM Max )
{
	PID_sampleTime(pPIDstruct,sampleTime);
	PID_Parameter(pPIDstruct,Kp,Ki,Kd);
	PID_Init_Saturateur(pPIDstruct,Min,Max);
}

void PID_sampleTime(struct PID_Parameter *pPIDstruct,SIZE_TYPE sampleTime)
{
	pPIDstruct->time.delay = (SIZE_TYPE) sampleTime;
}

void PID_Parameter(struct PID_Parameter *pPIDstruct, SIZE_PARAM Kp,SIZE_PARAM Ki,SIZE_PARAM Kd)
{
	double value;
	pPIDstruct->coefficient.Kp = Kp;
	pPIDstruct->coefficient.Ki = Ki/**pPIDstruct->time.delay/BASE_TEMPS_SEC_INTERRUPTION*/;
	pPIDstruct->coefficient.Kd = Kd/* /(pPIDstruct->time.delay/BASE_TEMPS_SEC_INTERRUPTION)*/;
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
	int32_t proportionnel=0;
	int32_t integrateur=0;
	int32_t derivateur=0;
	int32_t output = *pOutput;
	
	PCTIME_TempoStart(&(pPIDstruct->time),(pPIDstruct->time.delay));
	if(PCTIME_TempoIsElapsed(&(pPIDstruct->time)))
	{
		/*calcul de l'erreur à l'instant t*/
		error = consigne -  input;	
/************************************************************************************/
		

												
		/* Calcul integrateur*/	


		integrateur	= (pPIDstruct->coefficient.Ki)* ((pPIDstruct->error.ErrorSum)+error);
		/*ajout base de temps=> il est possible pour économiser 2 operations de l'integrer directement dans les coefficients*/
//		integrateur *= pPIDstruct->time.delay;
//		integrateur /=BASE_TEMPS_SEC_INTERRUPTION;	
		
/************************************************************************************/			
		/*	Calcul du dérivateur	= (erreur - lastError) * kD*/ 	
		if((pPIDstruct->coefficient.Kd)!= 0)
		{
			derivateur = (pPIDstruct->coefficient.Kd) *((pPIDstruct->error.ErrorSum)-error);	//solution 1
			(pPIDstruct->error.LastError)= error;	//recupération de l'erreur
		}
		else
			derivateur=0;
/*************************************************************************************/		
		/*	Calcul du proportionnel*/
		proportionnel= (pPIDstruct->coefficient.Kp) * error;

/************************************************************************************/
		(pPIDstruct->error.ErrorSum) = error;
		/*Calcul de la sortie avec PID*/
		output= proportionnel +  integrateur + derivateur;
//		output /=10; // les coefficients sont à 1/10emes



		/*On s'assure que la sortie ne dépasse pas le saturateur*/
		if(output > (pPIDstruct->saturateur.MAX))
			output = (pPIDstruct->saturateur.MAX);
		else if(output < (pPIDstruct->saturateur.MIN))
			output = (pPIDstruct->saturateur.MIN);			
		
		*pOutput=output ;
#ifdef DEBUG_PID
		PID_Debug(pPIDstruct,error,proportionnel,integrateur,derivateur,output);
#endif
	}
}

#ifdef DEBUG_PID
static void PID_Debug(struct PID_Parameter*  pPIDstruct,int32_t error, int32_t proportionnel, int32_t integrateur, int32_t derivateur, int16_t output)
{

	pPIDstruct->debug.proportionnel=proportionnel;
	pPIDstruct->debug.integrateur=integrateur;
	pPIDstruct->debug.derivateur=derivateur;
	pPIDstruct->debug.error=error;
	pPIDstruct->debug.output=output;
	
	printf("\n\r int:%d;error:%d;output:%d",integrateur,error,output);
	
}
#endif
	


