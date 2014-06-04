/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
* File Name          : usart_1.c
* Author             : MCD Tools development Team
* Date First Issued  : September 2012
* Description        : This code is used for MB1045 board test
********************************************************************************
* History:
* November 22, 2012: V0.1
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usart_1.h"
#include "main.h"

#define USART_A   USART1
#define USARTATX_PORT   GPIOA
#define USARTATX_PIN	GPIO_Pin_9
#define USARTARX_PORT   GPIOA
#define USARTARX_PIN	GPIO_Pin_10

const u32 USART_BAUD[] =
{
  1200,//Low
  9600,//used often
  115200,//High
  250000,//Highest: at least 250kbps guranteed for ST3241E
  300000,//Highest: should be OK and with some margin for ST3241E
//  500000,//Highest: OK with short loop-back although out-of-spec for ST3241E
//  800000,//Highest: OK with short loop-back although out-of-spec for ST3241E
//  900000,//Highest: FAIL & is out-of-spec for ST3241E
};
const u8 USART_TX_PACKET_2[] = "USARTx Test Data";

//#define TX_PACKET_1_SIZE (sizeof(TX_PACKET_1))
//#define USART_TX_PACKET_2_SIZE (sizeof(USART_TX_PACKET_2))
#define USART_TX_PACKET_SIZE   (TX_PACKET_1_SIZE + USART_TX_PACKET_2_SIZE)

u32 Usart_Baud;

//-----------------------------------------------------------------
//   USART 1 initialization
//   USART 1 on ev board Corresponds to USART 1(1 of 1~3) of MCU
//-----------------------------------------------------------------
void USARTA_Test_Init(u8 i)
{
  USART_InitTypeDef USART_InitStructure;

  // USART1 IO configuration ------------------------------------//
  // Enable GPIOA clock
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  // Set PA9/PA10 as PP AF - USART_TXD,RXD
  GPIO_InitStructure.GPIO_Pin = USARTATX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(USARTATX_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = USARTARX_PIN;  
  GPIO_Init(USARTARX_PORT, &GPIO_InitStructure);
  // SET PA9/PA10 for USART
  GPIO_PinAFConfig(USARTATX_PORT, GPIO_PinSource9, GPIO_AF_USART1);
  GPIO_PinAFConfig(USARTARX_PORT, GPIO_PinSource10,GPIO_AF_USART1);

  // DeInit USART
  USART_DeInit(USART_A);
  // USART configuration --------------------------------------//
  // USART configured as follow:
  //      - BaudRate = 115200 or other baud  
  //      - Word Length = 8 Bits
  //      - 1 Stop Bit
  //      - No parity
  //      - Hardware flow control disabled (RTS and CTS signals)
  //      - Receive and transmit enabled
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  // de-init USART1 to avoid the impact of USART3_IrDA
  USART_DeInit(USART1);
  
  Usart_Baud = USART_BAUD[i % (sizeof(USART_BAUD)/sizeof(u32))];
  USART_InitStructure.USART_BaudRate = Usart_Baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;// DANA change
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART_A, &USART_InitStructure);

  // Enable USART_A
  USART_Cmd(USART_A, ENABLE);
  // short delay seems mandatory before sending 1st byte
//  delay_10us(10);
}

//-----------------------------------------------------------------
//   USART 1 on ev board Corresponds to USART 1(1 of 1~3) of MCU
//   USART1_Test method:
//   the output pin is loop-back to the input pin for the USART socket
//   USART1 test is passed when below 2 conditions are met:
//   USART1 sends a data-packet successfully, meanwhile
//   USART1 receives a data-packet correctly
//-----------------------------------------------------------------
u8 USARTA_Test(u8 random)
{
  u32 i, timeout;

  timeout = 100000uL*10 / Usart_Baud;//with 10us unit,10-bit per data byte
  if(timeout == 0)
    timeout = 1;
  // USART1 TX RX test - prepare a packet //
  for(i=0; i<sizeof(TX_PACKET_1); i++)
    Tx_Packet[i] = TX_PACKET_1[i];
  for(i=0; i<USART_TX_PACKET_2_SIZE-1; i++)
  {
    Rx_Packet[i] = USART_TX_PACKET_2[i];
    Tx_Packet[i+TX_PACKET_1_SIZE] = USART_TX_PACKET_2[i];
  }
  // change it to "...""USART1 Test Data"
  Tx_Packet[TX_PACKET_1_SIZE + 5] = '1';
  // append 1byte of random data
  Tx_Packet[TX_PACKET_1_SIZE + USART_TX_PACKET_2_SIZE - 1] = random;

  // dummy Tx&Rx acitons needed before normal operation (reason unkown)
  USART_SendData(USART_A, Tx_Packet[0]);
  delay_10us(timeout*3);
  i = USART_A->SR;			// dummy read-status
  USART_ReceiveData(USART_A);		// dummy read-data
  delay_10us(timeout*2); ///????
  // USART TX RX test - send & receive a packet
  for(i=0; i<USART_TX_PACKET_SIZE; i++)
  {
    USART_SendData(USART_A, Tx_Packet[i]);
    // check whether a byte can be sent in specified time
    delay_10us(timeout);
    if( ((USART_A->SR) & USART_FLAG_TC) == 0 )
    {
      delay_10us(timeout);
      if( ((USART_A->SR) & USART_FLAG_TC) == 0 )
        return USART_SEND_FAIL;
    }
    // check whether a byte can be received in specified time
    if( ((USART_A->SR) & USART_FLAG_RXNE) == 0 )
    {
      delay_10us(timeout*2);
      if( ((USART_A->SR) & USART_FLAG_RXNE) == 0 )
        return USART_RECEIVE_FAIL;
    }
    Rx_Packet[i] = USART_ReceiveData(USART_A);
  }
  if( BufferCmp(Tx_Packet, Rx_Packet, USART_TX_PACKET_SIZE) == FAILED )
  {
     for(i=0; i<USART_TX_PACKET_SIZE; i++)
     Rx_Packet[i] = 0; 
    return USART_TEST_FAIL;
  } 
  else
  {
     for(i=0; i<USART_TX_PACKET_SIZE; i++)    
     Rx_Packet[i] = 0; 
     return USART_TEST_PASS;
  } 
}
