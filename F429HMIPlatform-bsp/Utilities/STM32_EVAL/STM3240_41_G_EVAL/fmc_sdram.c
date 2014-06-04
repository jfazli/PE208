/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
* File Name          : fmc_sdram.c
* Author             : MCD Tools development Team
* Date First Issued  : September 2012
* Description        : This code is used for MB1045 board test
********************************************************************************
* History:
* November 22, 2012: V0.1
********************************************************************************/

#include "fmc_sdram.h"
#include "main.h"
#include "stm32f4xx_fmc.h"

// SDRAM_MAX_COUNT can be  0x200000L(for MT48LC2M32B2B5)
//#define SDRAM_MAX_COUNT        0x200000L
//#define SDRAM_MAX_COUNT        0x400000L // test purpose for memory size

/*typedef struct
{
  vu32 Word[SDRAM_MAX_COUNT];
} EVAL_SDRAM_TypeDef;
typedef struct
{
  vu16 HalfWord[SDRAM_MAX_COUNT * 2];
} EVAL_SDRAM_HW_TypeDef;
typedef struct
{
  vu8 Byte[SDRAM_MAX_COUNT * 4];
} EVAL_SDRAM_B_TypeDef;
*/

#define FMC_SDClock_Div_1 ((uint32_t)0x00000400)
#define FMC_SDClock_Div_2 ((uint32_t)0x00000800)
#define FMC_SDClock_Div_3 ((uint32_t)0x00000C00)

// Note: SDRAM /CS is FMC_SDNE0 - 0 of 0~1
#define EVAL_SDRAM_BASE        ((u32)(0xC0000000 | 0x00000000))
#define EVAL_SDRAM             ((EVAL_SDRAM_TypeDef *) EVAL_SDRAM_BASE)
#define EVAL_SDRAM_HW          ((EVAL_SDRAM_HW_TypeDef *) EVAL_SDRAM_BASE)
#define EVAL_SDRAM_B           ((EVAL_SDRAM_B_TypeDef *) EVAL_SDRAM_BASE)


static void delay_10us(__IO uint32_t nCount)
{
	__IO uint32_t index = 0;
	for(index = (100 * nCount); index != 0; index--);
}

static void delay_ms(__IO uint32_t nCount)
{
	__IO uint32_t index = 0;
	for(index = (100000 * nCount); index != 0; index--);
}


//-----------------------------------------------------------------
//   SDRAM(MT48LC2M32B2B5-7 IT:G, marking D9GZQ) for MB1045 initialization
//   A0~A5: PF0~5, A6~A9: PF12~15, A10~A12: PG0~2 [A12 not used]
//   BA0~1[A14~15]: PG4~5
//   D0~D1: PD14~15, D2~D3: PD0~1, D4~D12: PE7~15, D13~D15: PD8~10,
//   D16~D20: PH8~12, D21~D23: PH13~15, D24~27: PI0~3,
//   D28~D29: PI6~7, D30~D31: PI9~10
//   SDNWE: PH5, SDNE0: PH3, SDCKE0: PH2
//   SDNCAS: PG15, SDNRAS: PF11, SDCLK: PG8
//   NBL3: PI5, NBL2: PI4, NBL1: PE1, NBL0: PE0
//
//   Note: FSMC can work up @60M for Manta, now improve to 84M/90MHz
//-----------------------------------------------------------------
void SDRAM_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  // SDRAM IO configuration --------------------------------------//
  // Enable FMC, GPIOD~I clock //
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE |
                         RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG |
                         RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI, ENABLE);

  // Set PD0,1, 8~10, 14,15 as PP AF - D2,3, D13~15, D0~1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
#ifdef	SILICON_CHIP
  if(GPIO_InitStructure.GPIO_Speed >= GPIO_Speed_50MHz)
  {
    // enable IO compensation cell, to limit slew rate (less EMI)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    SYSCFG_CompensationCellCmd(ENABLE);
    // wait for Compensation cell ready
    while(0 == (SYSCFG->CMPCR & 0x100))
      ;
  }
#endif
  // configure AFIO
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FMC);
  // Set PE0~1, PE7~15 as PP AF - NBL0,1, D4~12
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |
                                GPIO_Pin_7 | GPIO_Pin_8 |
                                GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 |
                                GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  // configure AFIO
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FMC);
  // Set PF0~5, 12~15, 11 as PP AF - A0~5, A6~9, SDNRAS
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                                GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_11 |
                                GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  // configure AFIO
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource0, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource2, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource3, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource4, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource5, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource11, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource12, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource13, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource14, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource15, GPIO_AF_FMC);

  // Set PG0~1/2, 4~5, 8, 15 as PP AF - A10~11/12, BA0~1, SDCLK, SDNCAS
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
                                GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_8 | GPIO_Pin_15;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  // configure AFIO
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource0, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource1, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource2, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource4, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource5, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource8, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource15, GPIO_AF_FMC);

  // Set PH2,3,5, 8~15 as PP AF - SDCKE0,SDNE0,SDNWE, D16~23
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5 |
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
                                GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOH, &GPIO_InitStructure);
  // configure AFIO
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource2, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource3, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource5, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource8, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource9, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource10, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource11, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource12, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource13, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource14, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource15, GPIO_AF_FMC);

  // Set PI0~3, 4~5, 6~7, 9~10 as PP AF - D24~27, NBL2~3, D28~29, D30~31
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                                GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_6 | GPIO_Pin_7 |
                                GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_Init(GPIOI, &GPIO_InitStructure);
  // configure AFIO
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource0, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource1, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource2, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource3, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource4, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource5, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource6, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource7, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource9, GPIO_AF_FMC);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource10, GPIO_AF_FMC);

  // SDRAM configuration -----------------------------------------//
  // SDRAM configured as follow:
  //      - Column bits = 8-bit[256 columns]
  //      - Row bits = 11-bit[2048 rows]
  //      - Data Width = 32-bit
  //      - Bank Number = 4
  //      - CAS Latency = 2 (CAS=2 to test higher CLK, CAS can be 1~3)
  //      - Write Operation = Enable
  //      - SDCLK clock = FMC clock/2[silicon,FMC_CLK=HCLK up to 168M/180M]
  //      - single read requests are always managed as bursts
  //      - Read Pipe delay = 2 cycles (to test higher CLK?)
  FMC_Bank5_6->SDCR[0] = FMC_ColumnBits_Number_8b | FMC_RowBits_Number_11b |
                         FMC_SDMemory_Width_32b | FMC_InternalBank_Number_4|
                         FMC_CAS_Latency_2 | FMC_Write_Protection_Disable |
#ifdef	SILICON_CHIP
                         FMC_SDClock_Div_2 | FMC_Read_Burst_Enable |
#elif EMULATOR
//                         FMC_SDClock_Div_1 | FMC_Read_Burst_enable |
  // note: STice2+ (related to FMC) can work up to 40M, FMC can work up to 20M
//                         FMC_SDClock_Div_2 | FMC_Read_Burst_enable |
                         FMC_SDClock_Div_2 | FMC_Read_Burst_disable |
#else
#error	"either SILICON_CHIP or EMULATOR should be defined"
#endif
                         // FMC_ReadPipe_Delay_3 means 2-cycle delay in fact
                         FMC_ReadPipe_Delay_2;

  // and as follow - for both read & write operations:
  //      - Load Mode Register to Active = 0x01[2 cycles]
  //      - Exit Self Refresh[tMRD] = 0x06?[7 cycles]
  //      - Self Refresh Time[tXSR] = 0x03?[4 cycles]
  //      - Row Cycle Delay[tRAS] = 0x07[8 cycles?], it also affects tRFC
  //      - Write Recovery Delay[tWR] = 0x01[2 cycles]
  //      - Row Precharge Delay[tRP] = 0x01[2 cycles]
  //      - Row to Column Delay[tRCD] = 0x01[2 cycles]
  // timing OK for 60/168?/180?MHz Fcpu
  FMC_Bank5_6->SDTR[0] = 0x00000361 | (0x07 << 12) |
                       (0x01 << 16) | (0x01 << 20) |
                       (0x01 << 24);
  // and as follow - for both read & write operations:
  //      - Clear Refresh Error Flag
  //      - Refresh Timer Count = 0x01[2 cycles]
  //        64ms / 4096 = 15.625us, 15.625 * 60M = 937.5,-20=>917 [Emulator]
  //        15.625 * 84M = 1312.5,-20=>1292 [Silicon]
//#ifdef	SILICON_CHIP
//  FMC_Bank5_6->SDRTR = 0x01 | (1292 << 1);
////  FMC_Bank1->SDRTR = 0x01 | (917 << 1);// in case only 60MHz is achieved
//#elif EMULATOR
//  FMC_Bank5_6->SDRTR = 0x01 | (917 << 1);
//#endif

}

//-----------------------------------------------------------------
// Function Name  : SDRAM_ActiveCmd
// Description    : send Active command to SDRAM
// Input          : address: 32-bit address to be sent in Active command
// Output         : None
// Return         : None
//-----------------------------------------------------------------
void SDRAM_ActiveCmd(u32 address)
{
}

//-----------------------------------------------------------------
// Function Name  : SDRAM_ReadProc
// Description    : send Read related commands(activate,read) to SDRAM
// Input          : address: 32-bit address to be sent in Read command
// Output         : None
// Return         : None
//-----------------------------------------------------------------
void SDRAM_ReadProc(u32 address)
{
  // Activate command
  FMC_Bank5_6->SDCMR = 0x02 | (0x01 << 4) | (0x00);
  // wait till SDRAM Controller is ready to accept a new request
  while(FMC_Bank5_6->SDSR & 0x20)
    ;
}

//-----------------------------------------------------------------
// Function Name  : SDRAM_WriteCmd
// Description    : send Write command to SDRAM
// Input          : address: 32-bit address to be sent in Write command
// Output         : None
// Return         : None
//-----------------------------------------------------------------
void SDRAM_WriteCmd(u32 address)
{
}

//-----------------------------------------------------------------
// Function Name  : SDRAM_PrechargeCmd
// Description    : send Precharge command to SDRAM
// Input          : address: 32-bit address to be sent in Precharge command
// Output         : None
// Return         : None
//-----------------------------------------------------------------
void SDRAM_PrechargeCmd(u32 address)
{
}

//-----------------------------------------------------------------
// Function Name  : SDRAM_Init
// Description    : send Initialization process to SDRAM(MT48LC2M32B2B5)
// Input          : None
// Output         : None
// Return         : None
//-----------------------------------------------------------------
void SDRAM_Init(void)
{
	
	SDRAM_GPIO_Init();
	
  // provide CLK [Clock Configuration Enable] to bank 1
  FMC_Bank5_6->SDCMR = 0x01 | (0x01 << 4) | (0x00);
  // delay 100us
  delay_10us(10);
  // Precharge All command
  FMC_Bank5_6->SDCMR = 0x02 | (0x01 << 4) | (0x00);
  // wait till SDRAM Controller is ready to accept a new request
  while(FMC_Bank5_6->SDSR & 0x20)
    ;
  // Auto-refresh command, 2 Auto-refresh cycles
  // tRFC will be followed (according to tRAS)
  FMC_Bank5_6->SDCMR = 0x03 | (0x01 << 4) | (0x01 << 5);
  // wait till SDRAM Controller is ready to accept a new request
  while(FMC_Bank5_6->SDSR & 0x20)
    ;
  // Load Mode Register: CAS Latency=2, Sequential, BurstLength=1
  FMC_Bank5_6->SDCMR = 0x04 | (0x01 << 4) | (0x20 << 9);
  // wait till SDRAM Controller is ready to accept a new request
  while(FMC_Bank5_6->SDSR & 0x20)
    ;
  // Normal mode
  FMC_Bank5_6->SDCMR = 0x00 | (0x01 << 4);
  // wait till SDRAM Controller is ready to accept a new request
  while(FMC_Bank5_6->SDSR & 0x20)
    ;

  // and as follow - for both read & write operations:
  //      - Clear Refresh Error Flag
  //      - Refresh Timer Count = 0x01[2 cycles]
  //        64ms / 4096 = 15.625us, 15.625 * 60M = 937.5,-20=>917 [Emulator]
  //        15.625 * 20M = 312.5,-20=>292 [Emulator]
  //        15.625 * 84M = 1312.5,-20=>1292 [Silicon]
  //        15.625 * 60M = 937.5,-20=>917 [Silicon]
#ifdef	SILICON_CHIP
//  FMC_Bank5_6->SDRTR = 0x01 | (1292 << 1);
  FMC_Bank5_6->SDRTR = 0x01 | (917 << 1);// in case only 60MHz is achieved
//  FMC_Bank5_6->SDRTR = 0x01 | (300 << 1);// in case TFT LCD is used
#elif EMULATOR
  FMC_Bank5_6->SDRTR = 0x01 | (292 << 1);
#endif

	
}
