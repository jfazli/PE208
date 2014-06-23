/*!*******************************************************************************       
 * @file			PID.h
 * @brief    	PID library
 * @version  	V1.0
 * @brief			Ajout 
 * @date     	19/05/2014
 * @author   	J.Fazli
 * @todo			Regarder si besoin de rajouter un coefficient multiplicateur aux	
								coefficient.
 						  Methode de calcul de l'integrateur à validé entre solution 1 et 
								solution 2(voir fichier excel).
 * @details	  Pour utiliser cette librairie il faut:
								1/Creation d'une variable de type  "PID_Parameter"
									struct PID_Parameter 	variableExemple							
								2/Définir SIZE_PARAM et SIZE_TYPE(dans fichier PCOM_Time.h) 
								3/Initialisation du PID
								4/Mise en place de la fonction "PID_Loop()" dans la boucle principale
								
*********************************************************************************/
#ifndef  PID_H
#define PID_H
//===== INCLUDEs ================================================================
#include <stdint.h>
//===== DEFINEs  ================================================================
//#define DEBUG_PID
#define SIZE_PARAM	int16_t
#define COEFF_MULT_1	4096
#define COEFF_MULT_2	131072

#define BASE_TEMPS_SEC_INTERRUPTION	(uint32_t)  (TIME_1S) //Interruption du timer toutes les 10µS
//===== TYPEDEFs ================================================================
//===== VARIABLEs ===============================================================

//===== STRUCTUREs ==============================================================
struct PID_Saturateur
{
	SIZE_PARAM MAX;
	SIZE_PARAM MIN;
};

struct PID_coefficient
{
	int32_t a1;
	int32_t a2;
	int32_t b1;
	int32_t b2;
	int32_t c1;
	int32_t c2;
};

struct PID_Last
{
	int32_t LastError;
	int32_t x1n_1 ;
	int32_t x2n_1;
};

struct PID_Result
{
	int32_t output; 
	int32_t error;	
};

struct PID_Parameter
{
	struct PCTIME_Tempo time;
	struct PID_Saturateur saturateur;
	struct PID_coefficient coefficient;
	struct PID_Last last;
#ifdef DEBUG_PID
	struct PID_Result debug;
#endif
};

//===== PROTOTYPEs ==============================================================
/*!*************************************************************************************************
 * @brief       Initialisation du PID à implémenter au début.	le type "SIZE_TYPE" est à définir au début de PID.h,
								il
								
 * @param[in]   *pPIDstruct : Pointeur de la structure que l'on a créé
 *							sampleTime : Dépend de la librairie "PCOM_Time"
								
 * @return      None
 ***************************************************************************************************/
extern void PID_initialisation(struct PID_Parameter *pPIDstruct,SIZE_TYPE sampleTime,SIZE_PARAM Min, SIZE_PARAM Max );
/*!*************************************************************************************************
 * @brief       Choisis le taux d'échantillonnage. Le timing est gérer par la librairie "PCOM_Time"
 *							Il faut donc que la fonction "PCTIME_Interrupt" soit placer dans une interruption.
 *							La base de temps sera basé sur l'interruption.							
 * @param[in]   *pPIDstruct : Pointeur de la structure que l'on a créé
 * @return      None
 ********************************************************************************/
extern void PID_sampleTime(struct PID_Parameter *pPIDstruct,SIZE_TYPE sampleTime);
/*!******************************************************************************
 * @brief       Initialisation des parametres de la consigne et des coefficients, les valeurs serons des 
								des entiers de taille choisis par l'utilisateur (SIZE_PARAM).		
								Utilisé lorsque l'on veut changer les parametres pendant le fonctionnement aussi.
								Ajout temps échantilonnage aux coefficient pour supprimer un calcul dans les calculs en boucle

 * @param[in]   None
 * @return      None
 ********************************************************************************/
extern void PID_Parameter(struct PID_Parameter *pPIDstruct, SIZE_PARAM Kp,SIZE_PARAM Ki,SIZE_PARAM Kd);
/*!******************************************************************************
 * @brief       Mise en place de valeur de seuils à ne pas dépasser
 * @param[in]   None
 * @return      None
 ********************************************************************************/
extern void PID_Saturateur(struct PID_Parameter *pPIDstruct,SIZE_PARAM Min, SIZE_PARAM Max);
/*!******************************************************************************
 * @brief       Boucle de mesure à placer dans la boucle principale qui 
 * @brief			se chargera des calculs.
 * @param[in]   None
 * @return      None
 ********************************************************************************/
extern void PID_Loop(struct PID_Parameter*  pPIDstruct,SIZE_PARAM input, SIZE_PARAM *pOutput, SIZE_PARAM consigne);
extern void PID_initialisationRegistre(void);
extern void PID_Debug(int32_t input, int32_t output,int32_t consigne);

#endif