/*!*****************************************************************************
 * @file     PCOM_Config.h
 * @brief    Definitions des configurations du protocole
 * @version  1.0
 * @version  XC8(V1.10), MPLAB X(V1.70)
 * @date     04-09-2013
 * @author   J.Fazli
 ******************************************************************************/
#ifndef PCOM_CONFIG_H
#define PCOM_CONFIG_H
//===== INCLUDEs ===============================================================
#include "PCOM_Generic.h"
//===== DEFINEs  ===============================================================
/*!******************************************************************************
 * @brief       Definitions des identifiants des equipements.
 *              Equipement 1 slave<
 ******************************************************************************/
#define ID_EQP_SL_1         0x04//REF CARTE PUISSANCE:002
/*!******************************************************************************
 * @brief       Definitions des identifiants des equipements.
 *              Equipement 2 slave
 ******************************************************************************/
#define ID_EQP_SL_2         0x05 //REF CARTE PUISSANCE:003
/*!******************************************************************************
 * @brief       Definitions des identifiants des equipements.
 *              Equipement master spi
 ******************************************************************************/
#define ID_EQP_IHM          0x02    

#define ID_EQP_POELE				0x03
#define ID_EQP_TABLETTE			0x01
/*!******************************************************************************
 * @brief       Definitions du numero de l'equipement
 *              ne definir que les equipements qui sont sur le reseau et toujours
 *              commence par 0.
 *              Ne pas change le nom car est utilise dans le fichier
 *              "PCOM_Spi.c"
 ******************************************************************************/
#define EQP_1             0   // slave spi 1
#define EQP_2             1   // slave spi 2
#define EQP_POELE		 			2   // poele
#define EQP_TABLETTE			3		// Tablette
/*!******************************************************************************
 * @brief       Definition du nombre d'equipement qui doit etre connu
 * @brief       Pour ce projet l'equipement slave n'a pas besoin de connaitre
 * @brief       les autres equipement car il ne  recoit des ordres que par l'
 * @brief       equipement SPI master.
 *              Une carte est un equipement
 ******************************************************************************/
#define NUMBER_EQP          4
/*!******************************************************************************
 * @brief       Nombre de donnee max que l'on peut emettre au max dans une trame
 ******************************************************************************/
#define MAX_SIZE_DATA       8       //Nombre de donnée max à envoyée
/*!******************************************************************************
 * @brief       Definition de l'identifiant de l'equipement que l'on est en train
 *              de configurer.
 ******************************************************************************/
#define NET_SRC                 ID_EQP_IHM
/*!******************************************************************************
 * @brief       Selectionner les types d'interfaces utilisees par cette equipement
 *              pour communiquer avec d'autre equipement.
 *              On ne peut etre a la fois Master et slave pour le spi.
 ******************************************************************************/
#define SPI_MASTER_ITF          ENABLE
/*!******************************************************************************
 * @brief       Selectionner les types d'interfaces utilisees par cette equipement
 *              pour communiquer avec d'autre equipement.
 *              On ne peut etre a la fois Master et slave pour le spi.
 ******************************************************************************/
#define SPI_SLAVE_ITF           DISABLE
/*!******************************************************************************
 * @brief       Selectionner les types d'interfaces utilisees par cette equipement
 *              pour communiquer avec d'autre equipement.
 ******************************************************************************/
#define SERIAL_ITF              ENABLE

#if SERIAL_ITF == ENABLE
#define UART_POLLING_INTERRUPT_ITF DISABLE //0 POLLING 1 INTERRUPT
#endif
/*!******************************************************************************
 * @brief       Taille de l'adresse du Port du µC et valeur de la pin.
 ******************************************************************************/
#define SIZE_ADRESS_PORT_IO		GPIO_TypeDef
#define SIZE_PIN				uint16_t
/*!******************************************************************************
 * @brief       Adresse du port du slave Talk
 ******************************************************************************/
#if SPI_SLAVE_ITF == ENABLE
#define PORT_SLAVE_TALK     &PORTB
/*!******************************************************************************
 * @brief       pin du slave Talk
 ******************************************************************************/
#define PIN_SLAVE_TALK      4
#endif
/*!******************************************************************************
 * @brief       Activation de l'acquittement des trames.
 ******************************************************************************/
#define ENABLE_ACQ_FRAME        DISABLE
/*!******************************************************************************
 * @brief       Activation de la repetitions des trames. Il est interdit de le
 *              le desactiver lorsque l'on active l'acquittement des trames. Si
 *              on ne veut pas l'utiliser, definir "NUMB_SEND_SAME_FRAME" a 0.
 ******************************************************************************/
#define ENABLE_REPEAT_FRAME     ENABLE        //Activation ou desactivation(mettre en commentaire) repetition de trame
/*!******************************************************************************
 * @brief       Si l'on a activer la repetition de trame, configurer le temps
 *              d'attente avant l'envoie d'une autre trame. Si toutes les trames
 *              de repetitions on ete envoye et que l'on a pas recu d'acquittement
 *              alors on pars en erreur
 *              (voir fontion COM_Cycle()).
 ******************************************************************************/
#if ENABLE_REPEAT_FRAME ==      ENABLE
/*!******************************************************************************
 * @brief       Si l'on a activer la repetition de trame, configurer le temps
 *              d'attente avant l'envoie d'une autre trame. Si toutes les trames
 *              de repetitions on ete envoye et que l'on a pas recu d'acquittement
 *              alors on pars en erreur
 *              (voir fontion COM_Cycle()).
 ******************************************************************************/
#define TIME_ACQ_FRAME          2000 // en 10µS
/*!******************************************************************************
 * @brief       Si l'on a activer la repetition de trame, configurer le temps
 *              d'attente avant l'envoie d'une autre trame. Si toutes les trames
 *              de repetitions on ete envoye et que l'on a pas recu d'acquittement
 *              alors on pars en erreur
 *              (voir fontion COM_Cycle()).
 ******************************************************************************/
#define NUMB_SEND_SAME_FRAME    0
#endif

/*!******************************************************************************
 * @brief       Nombre d'interface maximum present. Si on definie 1 seule
 *              interface, il faut definir l'interface a 0.
 ******************************************************************************/
#define NUMBER_ITF              2       //Definie le nombre d'interface utilisée sur la carte que l'on configure(uart,spi sont des interfaces)
#define NUMBER_ITF_SERIAL       0
#define NUMBER_ITF_SPI          1

/*!******************************************************************************
 * @brief       Timer PCOM_Time.h : remplir dans ce fichier SIZE_TYPE
 * 							
 ******************************************************************************/
#define SIZE_TYPE	uint32_t
//#define PCTIME_Tick PCTIME_Tick
/*!******************************************************************************
 * @brief       Activation ou non le mode passerelle.
 ******************************************************************************/
#define GATEWAY                 DISABLE
/*!******************************************************************************
 * @brief       Definition des octets de la trame. 
 ******************************************************************************/
#define STX 0X02
/*!******************************************************************************
 * @brief       Definition des octets de la trame.
 ******************************************************************************/
#define ETX 0X03
/*!******************************************************************************
 * @brief       Definition des octets de la trame.
 ******************************************************************************/
#define DLE 0X10
/*!******************************************************************************
 * @brief       Definition des octets de la trame.
 ******************************************************************************/
#define TYP 0

#endif
