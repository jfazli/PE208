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
											pas plusieurs slave select en m�me temps
*----------------------------------------------------------------------------------------------------------------
*	V1.2	| 27/09/2013| J.FAZLI		| 	Modif fct SlaveSelect,SlaveTalk avec type modifiable dans fichier PCOM_Config.h
											Modif variable et struct PCOM_Time avec type modifiable r�glable ds fichier PCOM_Config.h	
											Cr�ation fichier PCOM_Gpio pour gestion IO par utilisateur.
											Variable utilis� dans boucle for intialis� au d�but des fonctions
											Variables initialis�es ds fichier "xx.c" et mis en extern ds "xx.h".
											Correction warning mineur.
											Erreur sur fonction GetSlaveTalk pin slave select au lieu de slave talk
*----------------------------------------------------------------------------------------------------------------
*	V1.1	| 13/09/2013| J.FAZLI		| Modification noms des fonctions, ajout PC au d�but de la fonction
*											Compatibilit� avec Stm32F4xx		
*----------------------------------------------------------------------------------------------------------------
*	V1.0	| 05/06/2012| J.FAZLI		| Cr�ation soft
*----------------------------------------------------------------------------------------------------------------

*****************************************************************************************************************/
