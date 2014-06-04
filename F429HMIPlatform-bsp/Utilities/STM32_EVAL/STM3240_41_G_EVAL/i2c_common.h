
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_COMMON_H
#define __I2C_COMMON_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported variables --------------------------------------------------------*/
extern I2C_TypeDef* I2Cn;


/* Exported macros -----------------------------------------------------------*/
#define I2C_WRITE 0
#define I2C_READ  1

#define I2C_TEST_PASS    0
#define I2C_READ_FAIL    1
#define I2C_WRITE_FAIL   2
#define I2C_TEST_FAIL    3

/* Exported functions ------------------------------------------------------- */

u8 I2C_Read_exp(u8 I2c_Addr, u16 Start_Addr, u8 Addr_Cnt, u8 * data, u8 length);
u8 I2C_Read(u8 I2c_Addr, u16 Start_Addr, u8 Addr_Cnt, u8 * data, u8 length);
u8 I2C_Read2(u8 I2c_Addr, u16 Start_Addr, u8 Addr_Cnt, u8 * data, u8 length);
u8 I2C_Write(u8 I2c_Addr, u16 Start_Addr, u8 Addr_Cnt, u8* data, u8 length);

void I2c_Send_Start(void);
void I2c_Send_Start2(void);
void I2c_Send_Stop(void);
u8 I2C_Send_7bitAddress(u8 addr, u8 direction);
u8 I2C_Send_7bitAddress2(u8 addr, u8 direction);
u8 I2C_Send_Data(u8 data);
u8 I2C_Read_Data(void);

#endif /* __I2C_COMMON_H */

