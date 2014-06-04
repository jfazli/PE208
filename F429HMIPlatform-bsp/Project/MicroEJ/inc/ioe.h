/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

#ifndef _IOEXPANDER
#define _IOEXPANDER

#include "cpu.h"
#if MCU == 2
#include "stm32f2xx.h"
#include "stm322xg_eval_ioe.h"
#elif MCU == 4
#include "stm32f4xx.h"
#include "stm324xg_eval_ioe.h"
#elif MCU == 42
#include "stm32f4xx.h"
#include "stm324x9i_eval_ioe.h"
#else
#error Invalid MCU
#endif

#define CLEARFIFO    _I2C_WriteDeviceRegister(IOE_ADDR, IOE_REG_FIFO_STA, 0x01);\
    _I2C_WriteDeviceRegister(IOE_ADDR, IOE_REG_FIFO_STA, 0x00);

#define IOE_CLEAR_IT(a,b) _I2C_WriteDeviceRegister(a, IOE_REG_INT_STA, b)

#define _I2C_READDEVICEREGISTER(a,b,c) if(_I2C_ReadDeviceRegister(a,b,c)) return IOE_FAILURE
#define _I2C_READDATABUFFER(a,b,c,d) if(_I2C_ReadDataBuffer(a,b,c,d)) return IOE_FAILURE
#define _I2C_WRITEDEVICEREGISTER(a,b,c) if(_I2C_WriteDeviceRegister(a,b,c)) return IOE_FAILURE

/*
ID 0x0811
*/
#define _STMPE811_ID                (uint16_t)0x1108

void IOExpander_interrupt(void);
/**
  * @brief  Configuration and initialization functions
  */
uint8_t IOE_Config(void);
uint8_t _IOE_ITConfig(uint32_t IOE_ITSRC_Source);
static void _IOE_EXTI_Config(void);

uint8_t _IOExpander_init(void);

/**
  * @brief  Timeout user callback function. This function is called when a timeout
  *         condition occurs during communication with IO Expander. Only protoype
  *         of this function is decalred in IO Expander driver. Its implementation
  *         may be done into user application. This function may typically stop
  *         current operations and reset the I2C peripheral and IO Expander.
  *         To enable this function use uncomment the define USE_TIMEOUT_USER_CALLBACK
  *         at the top of this file.
  */


#define 	USE_TIMEOUT_USER_CALLBACK
#ifdef USE_TIMEOUT_USER_CALLBACK
 uint8_t IOE_TimeoutUserCallback(void);
#else
 #define IOE_TimeoutUserCallback()  IOE_TIMEOUT
#endif /* USE_TIMEOUT_USER_CALLBACK */

/**
  * @brief IO pins control functions
  */
uint8_t _IOE_WriteIOPin(uint8_t IO_Pin, IOE_BitValue_TypeDef BitVal);
uint8_t _IOE_ReadIOPin(uint32_t IO_Pin);

/**
  * @brief Touch Screen controller functions
  */


uint8_t IOE_TS_GetStateXY(uint16_t* , uint16_t*  );
/**
  * @brief Interrupts Mangement functions
  */
uint8_t _IOE_GetGITStatus(uint8_t DeviceAddr, uint8_t Global_IT);

uint8_t _IOE_ClearIOITPending(uint8_t DeviceAddr, uint8_t IO_IT);

uint8_t _IOE_GetIOITStatus(uint8_t DeviceAddr, uint8_t IO_IT);

/**
  * @brief IO-Expander Control functions
  */
uint8_t _IOE_IsOperational(uint8_t DeviceAddr);
uint8_t _IOE_FnctCmd(uint8_t DeviceAddr, uint8_t Fct, FunctionalState NewState);
uint8_t _IOE_IOPinConfig(uint8_t DeviceAddr, uint8_t IO_Pin, uint8_t Direction);
uint8_t _IOE_GITCmd(uint8_t DeviceAddr, FunctionalState NewState);
uint8_t _IOE_GITConfig(uint8_t DeviceAddr, uint8_t Global_IT, FunctionalState NewState);
uint8_t _IOE_IOITConfig(uint8_t DeviceAddr, uint8_t IO_IT, FunctionalState NewState);

/**
  * @brief Low Layer functions
  */
uint8_t _IOE_Reset(uint8_t);
uint8_t _IOE_Config(void);
uint8_t _IOE_TS_Config(void);
uint8_t _IOE_IOAFConfig(uint8_t DeviceAddr, uint8_t IO_Pin, FunctionalState NewState);
uint8_t _IOE_IOEdgeConfig(uint8_t DeviceAddr, uint8_t IO_Pin, uint8_t Edge);
uint8_t _IOE_ITOutConfig(uint8_t Polarity, uint8_t Type);
uint8_t _I2C_ReadDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr,uint8_t *IOE_BufferRX );
uint8_t _I2C_WriteDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t RegisterValue);
uint8_t _I2C_ReadDataBuffer(uint8_t DeviceAddr, uint8_t RegisterAddr,uint8_t* IOE_BufferRX, uint16_t sizebuffer);

#endif
