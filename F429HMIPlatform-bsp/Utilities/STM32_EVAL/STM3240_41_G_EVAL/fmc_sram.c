/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
* File Name          : fmc_sram.c
* Author             : MCD Tools development Team
* Date First Issued  : September 2012
* Description        : This code is used for MB1045 board test
********************************************************************************
* History:
* November 22, 2012: V0.1
********************************************************************************/
#include <stm32f4xx.h>

#include "fmc_sram.h"
#include "main.h"


//-----------------------------------------------------------------
//   SRAM(IS61WV102416BLL-10) for MB1045 initialization
//-----------------------------------------------------------------
void SRAM_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
  // SRAM IO configuration --------------------------------------//
  // Enable FMC, GPIOD~G clock //
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE);
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE |
                         RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);

  // Set PD0,1, 4~5, 8~10,11~13, 14,15 as PP AF - D2,3, /OE,/WE, D13~15, A16~18, D0~1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 |
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  if(GPIO_InitStructure.GPIO_Speed >= GPIO_Speed_50MHz)
  {
    // enable IO compensation cell, to limit slew rate (less EMI)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_CompensationCellCmd(ENABLE);
    // wait for Compensation cell ready
    while(0 == (SYSCFG->CMPCR & 0x100))
      ;
  }
  // configure AFIO
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FMC);
  // Set PE0~1, PE3~4, PE7~15 as PP AF - BLN0,1, A19~20, D4~12
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |
                                GPIO_Pin_3 | GPIO_Pin_4 |
                                GPIO_Pin_7 | GPIO_Pin_8 |
                                GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 |
                                GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  // configure AFIO
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource3, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FMC);
  // Set PF0~5, 12~15 as PP AF - A0~5, A6~9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                                GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  // configure AFIO
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource2, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource3, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource4, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource5, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource12, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource13, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource14, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource15, GPIO_AF_FMC);

  // Set PG0~5, PG9 as PP AF - A10~15, NE2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                                GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_9;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  // configure AFIO
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource0, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource1, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource2, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource3, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource4, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource5, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_FMC);

  // SRAM configuration -----------------------------------------//
  // SRAM configured as follow:
  //      - Data/Address MUX = Disable
  //      - Memory Type = SRAM
  //      - Data Width = 16bit
  //      - Write Operation = Enable
  //      - Extended Mode = Disable
  //      - Asynchronous Wait = Disable
  FMC_Bank1->BTCR[2] = FSMC_DataAddressMux_Disable | FSMC_MemoryType_SRAM |
                        FSMC_MemoryDataWidth_16b | FSMC_WriteOperation_Enable |
                        FSMC_ExtendedMode_Disable | FSMC_AsynchronousWait_Disable;
  FMC_Bank1->BTCR[2] = FSMC_DataAddressMux_Disable | FSMC_MemoryType_SRAM |
                        FSMC_MemoryDataWidth_16b | FSMC_WriteOperation_Enable |
                        FSMC_ExtendedMode_Disable | FSMC_AsynchronousWait_Disable;
  //      - Extended Mode = Enable
  FMC_Bank1->BTCR[2] = FSMC_DataAddressMux_Disable | FSMC_MemoryType_SRAM |
                        FSMC_MemoryDataWidth_16b | FSMC_WriteOperation_Enable |
                        FSMC_ExtendedMode_Enable | FSMC_AsynchronousWait_Disable;

  // and as follow - for both read & write operations:
  //      - Address Setup Time = 0x02
  //      - Address Hold Time = 0x00
  //      - Data Setup Time = 0x03
  //      - Bus Turn around Duration = 0x00
//  FSMC_Bank1->BTCR[3] = 0x00000101;	// timing FAIL for 48M Hz Fcpu
//  FSMC_Bank1->BTCR[3] = 0x00000202;	// timing FAIL for 48M Hz Fcpu - err 64
  FMC_Bank1->BTCR[3] = 0x00000303;	// timing OK for 48M Hz Fcpu
  FMC_Bank1->BTCR[3] = 0x00000302;	// timing OK for 48M Hz Fcpu
//  FMC_Bank1->BTCR[3] = 0x00000604;	// timing OK for 48M Hz Fcpu
//  FMC_Bank1->BTCR[3] = 0x00FFffff;	// timing OK for 48M Hz Fcpu

  FMC_Bank1->BTCR[3] = 0x00000201;// follow SRAM 10ns timing, fastest possible(16.7ns) @120M
  FMC_Bank1->BTCR[3] = 0x00000201;// follow SRAM 10ns timing, fastest possible(13.3ns) @150M
#ifdef HIGHER_SYSCLK
  FMC_Bank1->BTCR[3] = 0x00000200;// follow SRAM 10ns timing, fast(26.7ns) @75M
  FMC_Bank1E->BWTR[2] = 0x00000200;// timing OK for 150MHz Fcpu & 75M AHB with LCD
  FMC_Bank1->BTCR[3] = 0x00000101;// follow SRAM 10ns timing, fastest possible(13.3ns) @75M
  FMC_Bank1E->BWTR[2] = 0x00000101;// timing OK for 150MHz Fcpu & 75M AHB with LCD
//  FMC_Bank1->BTCR[3] = 0x00000100;// follow SRAM 10ns timing, fastest possible(13.3ns) @75M,OK? FAIL
  FMC_Bank1E->BWTR[2] = 0x00000100;// timing OK for 150MHz Fcpu & 75M AHB with LCD

  // follow SRAM 10ns timing, fastest possible(???ns) @180M
  FMC_Bank1->BTCR[3] = 0x00000301;
 
  // timing OK for 180MHz Fcpu & 180M AHB with LCD
  FMC_Bank1E->BWTR[2] = 0x00000201;
//  FMC_Bank1->BTCR[3] = 0x00000201;// FAIL @180M AHB
  // timing OK for 180MHz Fcpu & 180M AHB with LCD
	
  FMC_Bank1E->BWTR[2] = 0x00000101;
#else
  FMC_Bank1->BTCR[3] = 0x00000100;// follow SRAM 10ns timing, fastest possible(16.7ns) @60M
  FMC_Bank1E->BWTR[2] = 0x00000100;// timing OK for 120MHz Fcpu & 60M AHB with LCD?
#endif

  //      - BANK 2 (of NOR/SRAM Bank 1~4) is enabled
  FMC_Bank1->BTCR[2] |= 0x0001;
}

#define SRAM_MAX_COUNT        0x100000L
typedef struct
{
  vu16 HalfWord[SRAM_MAX_COUNT];
} EVAL_SRAM_TypeDef;
typedef struct
{
  vu8 Byte[SRAM_MAX_COUNT * 2];
} EVAL_SRAM_B_TypeDef;

#define EVAL_SRAM             ((EVAL_SRAM_TypeDef *) EVAL_SRAM_BASE)
#define EVAL_SRAM_B           ((EVAL_SRAM_B_TypeDef *) EVAL_SRAM_BASE)


const u16 SRAM_TEST_DATA[] =
{
  0xffff, 0x0000, 0x0001, 0x0002,  0x0004, 0x0008, 0x0010, 0x0020,
  0x0040, 0x0080, 0x0100, 0x0200,  0x0400, 0x0800, 0x1000, 0x2000,
  0x4000, 0x8000, 0x5555, 0xaaaa,  0xfffe, 0xfffd, 0xfffb, 0xfff7,
  0xffef, 0xffdf, 0xffbf, 0xff7f,  0xfeff, 0xfdff, 0xfbff, 0xf7ff,
  0xefff, 0xdfff, 0xbfff, 0x7fff,  0x00ff, 0xff00, 0x1234, 0x5678
};
const u8 SRAM_PACKET_2[] = "SRAM Test Data";

// append 1 half word(2-byte) random data
#define SRAM_BUF_SIZE (((sizeof(SRAM_TEST_DATA) / sizeof(u16)) + (sizeof(SRAM_PACKET_2) / 2)) + 1)
//#define SRAM_BUF_SIZE (((sizeof(SRAM_TEST_DATA) / sizeof(u16)) + (sizeof(SRAM_PACKET_2) / 2)) + 1 -9)
u16 Sram_WriteBuf[SRAM_BUF_SIZE];
u16 Sram_ReadBuf[SRAM_BUF_SIZE];
u32 Sram_ReadBuf2[16];

//-----------------------------------------------------------------
//   SRAM(IS61WV102416BLL-10) for MB1045 test
//   SRAM test is done in 3 steps:
//   simple test - write 0x0, 0xffff & 0x5555 @ all '0' & all '1'
//   complex test - various data from start address 0
//   robust test - various data from random address
//   return SRAM_TEST_FAIL_xx - Fail
//   return SRAM_TEST_PASS - PASS
//-----------------------------------------------------------------
u8 SRAM_Test(u32 address)
{
  u32 Sram_StartAddr, Added_Addr;
  vu16 * p_SramData;
  u32 i,j;
  u8 Fail_Code;

  // start addres is 0x6000_0000
//  p_SramData = (vu16 *) (EVAL_SRAM_BASE / 2);
  p_SramData = (vu16 *) (EVAL_SRAM_BASE);
  Fail_Code = 0;
  // SRAM simple tests - write 0x0, 0xffff & 0x5555 @ all '0' & all '1' - A0~A18 //
  // write 0x0 @ all '0' - A0~A18
  EVAL_SRAM->HalfWord[0] = 0x0000;
  Sram_ReadBuf[0] = EVAL_SRAM->HalfWord[0];
  if(Sram_ReadBuf[0] != 0)
    Fail_Code |= SRAM_TEST_FAIL_0_0;
  // write 0x0001 @ all '0' - A0~A18
  EVAL_SRAM->HalfWord[0] = 0x0001;
  // check BLN0 & BLN1
  Sram_ReadBuf[0] = EVAL_SRAM_B->Byte[0];
  Sram_ReadBuf[0] |= EVAL_SRAM_B->Byte[1] << 8;
  if(Sram_ReadBuf[0] != 0x0001)
    Fail_Code |= SRAM_TEST_FAIL_0_0;
  // write 0xffff @ all '0' - A0~A18
  p_SramData[0] = 0xffff;
  Sram_ReadBuf[0] = p_SramData[0];
  if(Sram_ReadBuf[0] != 0xffff)
    Fail_Code |= SRAM_TEST_FAIL_0_F;
  // write 0xefff @ all '0' - A0~A18
  p_SramData[0] = 0xefff;
  // check BLN0 & BLN1
  Sram_ReadBuf[0] = EVAL_SRAM_B->Byte[0];
  Sram_ReadBuf[0] |= EVAL_SRAM_B->Byte[1] << 8;
  if(Sram_ReadBuf[0] != 0xefff)
    Fail_Code |= SRAM_TEST_FAIL_0_F;
  // write 0x5555 @ all '0' - A0~A18
  EVAL_SRAM->HalfWord[0] = 0x5555;
  Sram_ReadBuf[0] = EVAL_SRAM->HalfWord[0];
  if(Sram_ReadBuf[0] != 0x5555)
    Fail_Code |= SRAM_TEST_FAIL_0_5;
  // write 0x5678 @ all '0' - A0~A18
  EVAL_SRAM->HalfWord[0] = 0x5678;
  // check BLN0 & BLN1
  Sram_ReadBuf[0] = EVAL_SRAM_B->Byte[0];
  Sram_ReadBuf[0] |= (EVAL_SRAM_B->Byte[1] << 8);
  if(Sram_ReadBuf[0] != 0x5678)
    Fail_Code |= SRAM_TEST_FAIL_0_5;

  // write 0x0 @ all '1' - A0~A18
  EVAL_SRAM->HalfWord[SRAM_MAX_COUNT-1] = 0x0000;
  Sram_ReadBuf[0] = EVAL_SRAM->HalfWord[SRAM_MAX_COUNT-1];
  if(Sram_ReadBuf[0] != 0)
    Fail_Code |= SRAM_TEST_FAIL_F_0;
  // write 0x5678 @ all '0' - A0~A18
  EVAL_SRAM->HalfWord[SRAM_MAX_COUNT-1] = 0x0001;
  // check BLN0 & BLN1
  Sram_ReadBuf[0] = EVAL_SRAM_B->Byte[SRAM_MAX_COUNT*2-2];
  Sram_ReadBuf[0] |= EVAL_SRAM_B->Byte[SRAM_MAX_COUNT*2-1] << 8;
  if(Sram_ReadBuf[0] != 0x0001)
    Fail_Code |= SRAM_TEST_FAIL_F_0;
  // write 0xffff @ all '1' - A0~A18
  EVAL_SRAM->HalfWord[SRAM_MAX_COUNT-1] = 0xffff;
  Sram_ReadBuf[0] = EVAL_SRAM->HalfWord[SRAM_MAX_COUNT-1];
  if(Sram_ReadBuf[0] != 0xffff)
    Fail_Code |= SRAM_TEST_FAIL_F_F;
  // write 0x5678 @ all '0' - A0~A18
  EVAL_SRAM->HalfWord[SRAM_MAX_COUNT-1] = 0xefff;
  // check BLN0 & BLN1
  Sram_ReadBuf[0] = EVAL_SRAM_B->Byte[SRAM_MAX_COUNT*2-2];
  Sram_ReadBuf[0] |= EVAL_SRAM_B->Byte[SRAM_MAX_COUNT*2-1] << 8;
  if(Sram_ReadBuf[0] != 0xefff)
    Fail_Code |= SRAM_TEST_FAIL_F_F;
  // write 0x5555 @ all '1' - A0~A18
  EVAL_SRAM->HalfWord[SRAM_MAX_COUNT-1] = 0x5555;
  Sram_ReadBuf[0] = EVAL_SRAM->HalfWord[SRAM_MAX_COUNT-1];
  if(Sram_ReadBuf[0] != 0x5555)
    Fail_Code |= SRAM_TEST_FAIL_F_5;
  // write 0x5678 @ all '0' - A0~A18
  EVAL_SRAM->HalfWord[SRAM_MAX_COUNT-1] = 0x5678;
  // check BLN0 & BLN1
  Sram_ReadBuf[0] = EVAL_SRAM_B->Byte[SRAM_MAX_COUNT*2-2];
  Sram_ReadBuf[0] |= EVAL_SRAM_B->Byte[SRAM_MAX_COUNT*2-1] << 8;
  if(Sram_ReadBuf[0] != 0x5678)
    Fail_Code |= SRAM_TEST_FAIL_F_5;

  // check SRAM simple tests result
  if(Fail_Code)
    return Fail_Code;

  // start addres is 0x00000
  // SRAM complex & robust test - prepare buf with various data / pattern
  for(i=0; i<sizeof(SRAM_TEST_DATA)/sizeof(u16); i++)
    Sram_WriteBuf[i] = SRAM_TEST_DATA[i];
  for(i=0; i<sizeof(SRAM_PACKET_2)/2; i++)
  {
    Sram_WriteBuf[i + sizeof(SRAM_TEST_DATA)/sizeof(u16)] =
      SRAM_PACKET_2[i*2] | (SRAM_PACKET_2[i*2+1] << 8);
  }
  Sram_WriteBuf[SRAM_BUF_SIZE - 1] = (address >> 16) ^ (address & 0xffff);

  // SRAM complex test - test a block of SRAM area from SRAM address 0x00000
  // SRAM_MAX_COUNT = 0x40000
  for(Added_Addr=0; Added_Addr<SRAM_MAX_COUNT; )
  {
    for(i=0; i<0x80; i++)
    {
      extern void delay_10us(u32 n_10us);
      // write data to SRAM
      for(j=0; j<SRAM_BUF_SIZE; j++)
      {
        EVAL_SRAM->HalfWord[i+j + Added_Addr] = Sram_WriteBuf[j];
//        delay_10us(1);
      }
      // read data from SRAM
      for(j=0; j<SRAM_BUF_SIZE; j++)
        Sram_ReadBuf[j] = EVAL_SRAM->HalfWord[i+j + Added_Addr];
      // compare read-data with written-data
      for(j=0; j<SRAM_BUF_SIZE; j++)
        if(Sram_ReadBuf[j] != Sram_WriteBuf[j])
          return SRAM_TEST_FAIL_64;
      // clear buffer - specific value
      for(j=0; j<SRAM_BUF_SIZE; j++)
        Sram_ReadBuf[j] = 0x8888;
    }
    if(Added_Addr==0)
      Added_Addr = 0x100;
    else
      Added_Addr <<= 1;
  }

  // start addres is 0x???00
  Sram_StartAddr = 0x7ff0f & address;

  // SRAM robust test - test a block of SRAM area @ random address
  // SRAM_MAX_COUNT = 0x40000
  for(Added_Addr=0x100; Added_Addr<SRAM_MAX_COUNT; )
  {
    for(i=0; i<0x80; i++)
    {
      // write data to SRAM
      for(j=0; j<SRAM_BUF_SIZE; j++)
        EVAL_SRAM->HalfWord[i+j + (Sram_StartAddr ^ Added_Addr)] = Sram_WriteBuf[j];
      // read data from SRAM
      for(j=0; j<SRAM_BUF_SIZE; j++)
        Sram_ReadBuf[j] = EVAL_SRAM->HalfWord[i+j + (Sram_StartAddr ^ Added_Addr)];
      // compare read-data with written-data
      for(j=0; j<SRAM_BUF_SIZE; j++)
        if(Sram_ReadBuf[j] != Sram_WriteBuf[j])
          return SRAM_TEST_FAIL_65;
      // clear buffer - specific value
      for(j=0; j<SRAM_BUF_SIZE; j++)
        Sram_ReadBuf[j] = 0x8888;
    }
    if(Added_Addr==0)
      Added_Addr = 0x100;
    else
      Added_Addr <<= 1;
    // adjust Sram_StartAddr if needed
//    if((Sram_StartAddr+Added_Addr) > 0x80000)
//      Sram_StartAddr = Sram_StartAddr+Added_Addr - 0x80000;
  }

  // SRAM_TEST_FAIL_66 - to check memory size
  j = 0;
  for(i=0xFF; i<SRAM_MAX_COUNT; i=i<<1 | 1)
  {
    EVAL_SRAM->HalfWord[i-1] = i & 0xFFFF;
    EVAL_SRAM->HalfWord[i] = i >> 16;
    Sram_ReadBuf2[j++] = 0x8888;
  }
  j = 0;
  for(i=0xFF; i<SRAM_MAX_COUNT; i=i<<1 | 1)
  {
    Sram_ReadBuf2[j] = EVAL_SRAM->HalfWord[i-1];
    Sram_ReadBuf2[j] |= (u32)EVAL_SRAM->HalfWord[i] << 16;
    if(Sram_ReadBuf2[j] != i)
      return SRAM_TEST_FAIL_66;
    j++;
  }

  // SRAM_TEST_FAIL_67 - to check read&write via DMA

  return SRAM_TEST_PASS;
}
