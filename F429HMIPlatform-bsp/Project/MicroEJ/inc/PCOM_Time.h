/*!*****************************************************************************
 * @file     PCOM_Time.h
 * @brief    Gestion des timings du protocole
 * @detail   Les fonctions ci-dessous peuvent etre utilise pour cadence le soft
 *           ou si besoin de timer. A chaque utilisation il faut creer une
 *           structure 'struct PCTIME_Tempo t_toto' dans le fichier ou l'on a
 *           besoin et inclure le fichier  .h
 * @version  1.0
 * @version  XC8(V1.10), MPLAB X(V1.70)
 * @date     04-09-2013
 * @author   J.Fazli
 ******************************************************************************/
#ifndef PCOM_TIME_H
#define PCOM_TIME_H
//===== INCLUDEs ================================================================
#include "PCOM_Config.h"
//===== DEFINEs  ================================================================
#ifndef SIZE_TYPE
#define SIZE_TYPE	uint32_t
#endif

/*temps */
#define PERIODE_TIMER 1UL//µS	
#define MS 100*PERIODE_TIMER

#define TIME_10MS (10*MS)
#define TIME_100MS (100*MS)

#define TIME_1S (1000*MS)
#define TIME_6S (6000*MS)
#define TIME_30S (30000*MS)

#define TIME_1MIN (TIME_1S * 60)
#define TIME_2MIN (uint32_t)(TIME_1MIN *2)

//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================
extern volatile SIZE_TYPE PCTIME_Tick;

//===== STRUCTUREs ==============================================================
struct PCTIME_Tempo
{
    uint8_t Request:1;
    SIZE_TYPE  startTime;
    SIZE_TYPE delay;
};
//===== PROTOTYPEs ==============================================================
/*!******************************************************************************
 * @brief       Demarrage d'une tempo. Au premier passage dans cette fonction
 *              elle se verrouille tant que l'on est pas passe par la fonction
 *              'PCTIME_TempoIsElapsed'.
 * @param   *pTimeStruct : Pointeur de la structure que l'on a cree.
 * @param   delay : delay du timer.
 ******************************************************************************/
void PCTIME_TempoStart(struct PCTIME_Tempo * pTimeStruct, SIZE_TYPE delay);
/*!******************************************************************************
 * @brief       Lorsque la tempo que l'on configurer auparavant par la fonction
 *              'PCTIME_TempoStart' est ecoulee, la fonction retourne 1.
 * @param   *pTimeStruct : Pointeur de la structure que l'on a cree.
 * @return      Retourne lorsque la tempo est ecoulee.
 ******************************************************************************/
uint8_t PCTIME_TempoIsElapsed(struct PCTIME_Tempo * pTime);
/*!******************************************************************************
 * @brief       Utilise pour connaitre un temps ecoulee. On démarre une tempo
 *              avec la fonction 'PCTIME_TempoStart' avec comme delay 0. puis
 *              cette fonction retournera le delta temps entre elle et la fonction
 *              de demarrage.
 * @param   *pTimeStruct : Pointeur de la structure que l'on a cree.
 * @return      Temp ecoule
 ******************************************************************************/
SIZE_TYPE PCTIME_TempoCount(struct PCTIME_Tempo * pTime);
/*!******************************************************************************
 * @brief       Fontion pour avoir un delay bloquee, le soft tourne en boucle
 *              dans cette fonction jusqu'a que le delay soit ecoulee.
 * @param   tick : Temps en ms
 ******************************************************************************/
void PCTIME_Delay(SIZE_TYPE tick);
/*!******************************************************************************
 * @brief       Cadencement du timer a placer obligatoirement dans 1 une
 *              interruption toutes les ms.
 * @param   tick : Temps en ms
 ******************************************************************************/
void PCTIME_Interrupt(void);

void PCTIME_InitialiseTempoStart(struct PCTIME_Tempo * pTimeStruct);


#endif
