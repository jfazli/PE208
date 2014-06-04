/*!*****************************************************************************
 * @file     PCOM_CouchePhysique.h
 * @brief    Definitions des registres du uC
 * @details  Configuration de la couche physique du SPI.
 *           Ce fichier fait la liaison avec le fichier 'PCOM_Spi.c'.
 *           La selection des interfaces se fait dans le fichier 'PCOM_Config.h'
 * @version  1.0
 * @version  XC8(V1.10), MPLAB X(V1.70)
 * @date     04-09-2013
 * @author   J.Fazli
 ******************************************************************************/
#ifndef PCOM_COUCHEPHYSIQUE_H
#define PCOM_COUCHEPHYSIQUE_H

#include "PCOM_Generic.h"
#include "PCOM_Config.h"


#if SPI_MASTER_ITF == ENABLE && SPI_SLAVE_ITF == ENABLE
#error Impossible activation spi mode Master & Slave
#endif

//-------------------------------------------------------------------------------
//Configuration de la couche SPI master
//-------------------------------------------------------------------------------
#if SPI_MASTER_ITF == ENABLE
/*!******************************************************************************
 * @brief       Couche SPI MASTER
 * @brief       Selection du mode master en interruption ou en polling,
 *              Mettre à 0 pour etre en polling et à 1 pour etre en interruption
 *              Attention l'interruption pour le master n'a pas ete teste.
 ******************************************************************************/
#define SPI_POLLING_INTERRUPT_ITF   0
/*!******************************************************************************
 * @brief       Couche SPI MASTER
 * @brief       Definition du registre du buffer spi qui fonctionne dans les 2
 *              sens.
 ******************************************************************************/
#define BUFFER_SPI                  SSPBUF
/*!*****************************************************************************
 * @brief       Couche SPI MASTER
 * @brief       Pin slave talk utilise dans l'interruption SPI car la fonction
 *              'COM_SetSlaveTalk' ne fonctionne pas en interruption.
 ******************************************************************************/
#define IO_SLAVETALK                PORTBbits.RB4//PORTEbits.RE1

#if  SPI_POLLING_INTERRUPT_ITF == 0
/*!*****************************************************************************
 * @brief       Couche SPI MASTER
 * @brief      Bit de status du buffer Spi, vaut 0 si le buffer spi est vide et
 *             retourne 1 si il y a une trame complete.
 ******************************************************************************/
#define STATUS_BUFFER_SPI           SSPSTATbits.BF

#elif SPI_POLLING_INTERRUPT_ITF ==  1
/*!*****************************************************************************
 * @brief       Couche SPI MASTER
 * @brief       bit de status si il y a une interruption  ou non.
 ******************************************************************************/
#define FLAG_INTERRUPT_SPI          PIR1bits.SSPIF //PIC16F1937
//#define FLAG_INTERRUPT_SPI        PIR1bits.SSP1IF //PIC16F1828
#define ENABLE_INTERRUPT_SPI        PIE1bits.SSPIE
#endif

//-------------------------------------------------------------------------------
//Configuration de la couche SPI slave
//-------------------------------------------------------------------------------
#elif SPI_SLAVE_ITF == ENABLE
/*!*****************************************************************************
 * @brief       Couche SPI SLAVE
 * @brief      Bit de status du buffer Spi, vaut 0 si le buffer spi est vide et
 *             retourne 1 si il y a une trame complete.
 ******************************************************************************/
#define STATUS_BUFFER_SPI       SSPSTATbits.BF
/*!*****************************************************************************
 * @brief       Couche SPI SLAVE
 * @brief       Definition du registre du buffer spi qui fonctionne dans les 2
 *              sens.
 ******************************************************************************/
#define BUFFER_SPI              SSPBUF
/*!*****************************************************************************
 * @brief       Couche SPI SLAVE
 * @brief       bit de status si il y a une interruption  ou non.
 ******************************************************************************/
#define FLAG_INTERRUPT_SPI      SSPIF //
/*!*****************************************************************************
 * @brief       Couche SPI SLAVE
 * @brief       activation ou non des interruptions du spi..
 ******************************************************************************/
#define ENABLE_INTERRUPT_SPI    SSPIE
/*!*****************************************************************************
 * @brief       Couche SPI SLAVE
 * @brief       Pin slave talk utilise dans l'interruption SPI car la fonction
 *              'COM_SetSlaveTalk' ne fonctionne pas en interruption.
 ******************************************************************************/
#define IO_SLAVETALK            LATBbits.LATB4
#endif

//-------------------------------------------------------------------------------
//Configuration de la couche UART
//-------------------------------------------------------------------------------
//Couche Uart
#if SERIAL_ITF == ENABLE
/*!*****************************************************************************
 * @brief       Couche UART
 * @brief       Activation/desactivation ou verification interruption reception
 ******************************************************************************/
#define ENABLE_INT_RX       RCIE
/*!*****************************************************************************
 * @brief       Couche UART
 * @brief       Flag d'interruption de reception qui retourne 1
 *              si il y a une interruption.
 ******************************************************************************/
#define FLAG_INT_RX         RCIF
/*!*****************************************************************************
 * @brief       Couche UART
 * @brief       Activation/desactivation ou verification interruption emission
 ******************************************************************************/
#define ENABLE_INT_TX       TXIE
/*!*****************************************************************************
 * @brief       Couche UART
 * @brief       Flag d'interruption d'émission qui retourne 1
 *              si il y a une interruption.
 ******************************************************************************/
#define FLAG_INT_TX         TXIF
/*!*****************************************************************************
 * @brief       Couche UART
 * @brief       Buffer d'emission des donnees
 ******************************************************************************/
#define BUFF_TX_UART        TXREG
/*!*****************************************************************************
 * @brief       Couche UART
 * @brief       Buffer de reception des donnees
 ******************************************************************************/
#define BUFF_RX_UART        RCREG
/*!*****************************************************************************
 * @brief       Couche UART
 * @brief       Flag de verification si il y a une erreur de reception de donnée
 *              superieur à la place du buffer
 ******************************************************************************/
#define OVERRUN_ERROR       OERR
/*!*****************************************************************************
 * @brief       Couche UART
 * @brief       Retourne une erreur si il n'y a pas de bit de stop suite au
 *              caractere recu
 ******************************************************************************/
#define RX_FRAME_ERROR      RCSTAbits.FERR
/*!*****************************************************************************
 * @brief       Couche UART
 * @brief       Activation/desactivation de la reception Uart
 ******************************************************************************/
#define ENABLE_RX           CREN
/*!*****************************************************************************
 * @brief       Couche UART
 * @brief       Activation/desactivation de l'emission Uart
 ******************************************************************************/
#define ENABLE_TX           TXEN
#endif



#endif
