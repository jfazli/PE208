/********************************************************************************
 *  FONCTION    :
 ********************************************************************************
 *  FICHIER     : 
 *  PROJET		:
 *  DEV         : JFA
 *  VERSION     : 1.0
 *  OUTIL		: 
 ********************************************************************************/
#ifndef PCOM_GENERIC_H
#define PCOM_GENERIC_H


#include <stm32f4xx.h>
#include <sni.h>
#include <stdio.h>
#include "stm32f4xx_gpio.h"
//#include <stdint.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>


#define ENABLE      1
#define DISABLE     0



typedef uint8_t BOOLEAN;


/////////////////Paramétre utilisateur////////////////////////////////////////////
/*Pilotage de la plaque sans tablette et sans régulation, directement par la poele*/
//#define TOUCH_COOK
/*La poele fonctionne en 9600baud, lorsque on la branche sans BLE mettre se debit à 9600*/
#define DEBIT_BLE_9600
#ifndef TOUCH_COOK
/* Simulation d'une recette implémenter à l'init de l'appli*/
#define SIMULATION_TABLETTE
#endif

#define DEBUG
//#define DEBUG_OUT //test avec une pin
//#define DEBUG_PRINTF //utilisé pour ecrire sur l'uart
//-------------------------------------------------------------------------------
//Definitions Identifiant
//-------------------------------------------------------------------------------
#define ID_SLAVE_SPI_EQP1			0x04
#define ID_SLAVE_SPI_EQP2 		0x05
#define ID_MASTER_SPI 				0x02

#ifdef DEBUG_OUT 
#define IO_DEBUG 		GPIO_Pin_9
#define PORT_DEBUG 		GPIOG
#endif

#endif
