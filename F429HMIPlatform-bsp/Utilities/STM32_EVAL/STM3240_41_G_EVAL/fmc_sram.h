
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FMC_SRAM_H
#define __FMC_SRAM_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"


/* Exported macros -----------------------------------------------------------*/
// Note: SRAM /CS is FSMC_NE2 - Bank 1 of 0~3
#define EVAL_SRAM_BASE        ((u32)(0x60000000 | 0x04000000))
//#define EVAL_SRAM_BASE_LCD    ((u32)(0x60000000 | 0x08000000))

#define SRAM_TEST_PASS      0
// fail bitmap
#define SRAM_TEST_FAIL_0_0  0x01
#define SRAM_TEST_FAIL_0_F  0x02
#define SRAM_TEST_FAIL_0_5  0x04
#define SRAM_TEST_FAIL_F_0  0x08
#define SRAM_TEST_FAIL_F_F  0x10
#define SRAM_TEST_FAIL_F_5  0x20
// fail items
#define SRAM_TEST_FAIL_64   64
#define SRAM_TEST_FAIL_65   65
#define SRAM_TEST_FAIL_66   66
#define SRAM_TEST_FAIL_67   67


/* Exported functions ------------------------------------------------------- */
void SRAM_Test_Init(void);
u8 SRAM_Test(u32 address);

#endif /* __FMC_SRAM_H */
