/*!*****************************************************************************
 * @file     PCOM_Uart.h
 * @brief    Couche basse uart, doit etre modifie selon le uC utilise
 * @version  1.1
 * @version  
 * @date     
 * @author   J.Fazli
 ******************************************************************************/
/* VERSION	| DATE     	| AUTEUR       	| DESCRIPTION
*----------------------------------------------------------------------------------------------------------------
*	V1.2.1	| 23/10/2013| J.FAZLI		| 	Ajout fonction getSlaveSelect pour s'assurer lors du PCSPI_Cycle n'utilise
											pas plusieurs slave select en même temps
*----------------------------------------------------------------------------------------------------------------
*	V1.2	| 27/09/2013| J.FAZLI		| 	Modif fct SlaveSelect,SlaveTalk avec type modifiable dans fichier PCOM_Config.h
											Modif variable et struct PCOM_Time avec type modifiable réglable ds fichier PCOM_Config.h	
											Création fichier PCOM_Gpio pour gestion IO par utilisateur.
											Variable utilisé dans boucle for intialisé au début des fonctions
											Variables initialisées ds fichier "xx.c" et mis en extern ds "xx.h".
											Correction warning mineur.
											Erreur sur fonction GetSlaveTalk pin slave select au lieu de slave talk
*----------------------------------------------------------------------------------------------------------------
*	V1.1	| 13/09/2013| J.FAZLI		| Modification noms des fonctions, ajout PC au début de la fonction
*											Compatibilité avec Stm32F4xx		
*----------------------------------------------------------------------------------------------------------------
*	V1.0	| 05/06/2012| J.FAZLI		| Création soft
*----------------------------------------------------------------------------------------------------------------

*****************************************************************************************************************/
