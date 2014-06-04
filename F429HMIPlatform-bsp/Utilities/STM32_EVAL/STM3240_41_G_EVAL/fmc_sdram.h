
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FMC_SDRAM_H
#define __FMC_SDRAM_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"


/* Exported macros -----------------------------------------------------------*/
#define EVAL_SDRAM_BASE        ((u32)(0xC0000000 | 0x00000000))

#define SDRAM_TEST_PASS      0
// fail bitmap
#define SDRAM_TEST_FAIL_0_0  0x01
#define SDRAM_TEST_FAIL_0_F  0x02
#define SDRAM_TEST_FAIL_0_5  0x04
#define SDRAM_TEST_FAIL_F_0  0x08
#define SDRAM_TEST_FAIL_F_F  0x10
#define SDRAM_TEST_FAIL_F_5  0x20
// fail items
#define SDRAM_TEST_FAIL_64   64
#define SDRAM_TEST_FAIL_65   65
#define SDRAM_TEST_FAIL_66   66
#define SDRAM_TEST_FAIL_67   67


/* Exported functions ------------------------------------------------------- */
void SDRAM_Test_Init(void);
void SDRAM_Init(void);
u8 SDRAM_Test(u32 address);

#endif /* __FMC_SDRAM_H */
