
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IO_EXPANDER_H
#define __IO_EXPANDER_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"


/* Exported macros -----------------------------------------------------------*/
#define I2C_IOEXP_TEST_PASS    0
#define I2C_IOEXP_INIT_FAIL    1
#define I2C_IOEXP_READ_FAIL    2
#define I2C_IOEXP_WRITE_FAIL   3
#define I2C_IOEXP_TEST_FAIL    4

// 7-bit address of IO_Expander 1: STMPE1600
#define I2C_IOEXP1_ADDR  0x84
//#define I2C_IOEXP2_ADDR  0x84

#define IN0   0
#define IN1   1
#define IN2   2
#define IN3   3
#define IN4   4
#define IN5   5
#define IN6   6
#define IN7   7

#define OUT0   0
#define OUT1   1
#define OUT2   2
#define OUT3   3
#define OUT4   4
#define OUT5   5
#define OUT6   6
#define OUT7   7

/* Exported functions ------------------------------------------------------- */
u8 IoExpander_Init(void);

u8 IoExp_WriteByte(u8 I2c_Addr, u16 Start_Addr, u8 Addr_Cnt, u8 data);
u8 IoExp_ReadByte(u8 I2c_Addr, u16 Start_Addr, u8 Addr_Cnt, u8 *data);
u16 IO_Expander_ReadIO(u8 INx);
u8 IO_Expander_WriteIO(u8 Outx, u8 state);
u8 IO_Expander_GpioIntCmd(u16 bits, FunctionalState NewState);
u8 IO_Expander_ReadReg(u8 Reg, u8 * data, u8 length);
u8 IO_Expander_WriteReg(u8 Reg, u8 * data, u8 length);

#endif /* __IO_EXPANDER_H */
