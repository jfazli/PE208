
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_1_H
#define __USART_1_H


#include "stm32f4xx.h"

/* Exported constants --------------------------------------------------------*/
extern const u8 USART_TX_PACKET_2[16];

/* Exported macros -----------------------------------------------------------*/
#define USART_TX_PACKET_2_SIZE (sizeof(USART_TX_PACKET_2))

#define USART_TEST_PASS		0
#define USART_RTS_CTS_FAIL	1
#define USART_SEND_FAIL		2
#define USART_RECEIVE_FAIL	3
#define USART_TEST_FAIL		4


/* Exported functions ------------------------------------------------------- */
void USARTA_Test_Init(u8 i);
u8 USARTA_Test(u8 random);

#endif /* __USART_1_H */
