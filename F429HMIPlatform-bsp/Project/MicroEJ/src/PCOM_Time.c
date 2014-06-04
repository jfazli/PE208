/********************************************************************************
 *Fonction   :
 *********************************************************************************
 *	FICHIER	:
 *	PROJET	: PE168
 *	DEV		: JFA
 *  VERSION : 1.0
 *	OUTIL	: 
 ********************************************************************************/

//===== INCLUDEs ================================================================
#include "PCOM_generic.h"
#include "PCOM_Config.h"
#include "PCOM_Time.h"
//===== DEFINEs  ================================================================

volatile SIZE_TYPE PCTIME_Tick;

//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
//===== STRUCTUREs ==============================================================
//===== PROTOTYPEs ==============================================================
//===== CODE ====================================================================
void PCTIME_TempoStart(struct PCTIME_Tempo * pTimeStruct, SIZE_TYPE delay)
{
    if(!pTimeStruct->Request)
    {
        pTimeStruct->startTime = PCTIME_Tick;
        pTimeStruct->delay = delay;
        pTimeStruct->Request=1;
    }
}

uint8_t PCTIME_TempoIsElapsed(struct PCTIME_Tempo * pTime)
{
    BOOLEAN bRet = 0;

    if ((PCTIME_Tick - pTime->startTime) > pTime->delay)
    {
        bRet = 1;
        pTime->Request=0;
    }
    return bRet;
}

SIZE_TYPE PCTIME_TempoCount(struct PCTIME_Tempo * pTime)
{
	SIZE_TYPE CountRet=0;
    CountRet=PCTIME_Tick - pTime->startTime;
    pTime->Request=0;

    return CountRet;
}

void PCTIME_Delay(SIZE_TYPE tick)
{
    struct PCTIME_Tempo tempoDelay;
    tempoDelay.Request = 0;
    PCTIME_TempoStart(&tempoDelay, tick);
    while (!PCTIME_TempoIsElapsed(&tempoDelay));
}


void PCTIME_Interrupt(void)
{
	PCTIME_Tick++;
}

void PCTIME_InitialiseTempoStart(struct PCTIME_Tempo * pTimeStruct)
{
	pTimeStruct->Request=0;
}
