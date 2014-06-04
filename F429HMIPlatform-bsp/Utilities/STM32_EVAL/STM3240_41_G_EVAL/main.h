#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported constants --------------------------------------------------------*/
extern const u8 TX_PACKET_1[24];

/* Exported macros -----------------------------------------------------------*/

#define HIGHER_SYSCLK

//#define  PLL_MUL_X  8
#ifdef HIGHER_SYSCLK
#define  SYS_CLK  180000L
#else
#define  SYS_CLK  120000L
#endif

//Note: TX_PACKET_1_SIZE = 24
#define TX_PACKET_1_SIZE (sizeof(TX_PACKET_1))
#define TX_PACKET_SIZE (2 * sizeof(TX_PACKET_1))

/* Exported variables --------------------------------------------------------*/
extern GPIO_InitTypeDef GPIO_InitStructure;
extern u8 Tx_Packet[TX_PACKET_SIZE];
extern u8 Rx_Packet[128];
extern u8 Backup_Packet[128];

/* typedef -------------------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* function prototypes -------------------------------------------------------*/
TestStatus BufferCmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength);
void delay(u32 ms_time);
void delay_ms(u32 n_ms);
void delay_10us(u32 n_10us);
void Enable_Usb_1k5_Pullup(void);

#endif
