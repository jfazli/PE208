/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
* File Name          : io_expander.c
* Author             : MCD Tools development Team
* Date First Issued  : September 2012
* Description        : This code is used for MB1045 board test
*                      io_expander supports extra I/O & TouchScreen feature
********************************************************************************
* History:
* November 22, 2012: V0.1
********************************************************************************/

#include "io_expander.h"
#include "i2c_common.h"
#include "main.h"

#define ENABLE_IOEXP_RESET
#define ENABLE_IOEXP_DEBUG

#define BAUD_RATE        200000

#define STMPE1600_ID   0x1600

// definitions for STMPE1600 register address
// CHIP_ID - 16 bit
#define CHIP_ID    0x00
#define ID_VER     0x02
#define SYS_CTRL1  0x03
//#define SYS_CTRL2  0x04  //reserved
//Interrupt related
#define INT_EN_LOW   0x08
#define INT_EN_HIGH  0x09
#define INT_STA_LOW  0x0A
#define INT_STA_HIGH 0x0B
//GPIO related
#define GPIO_STA_LOW  0x10
#define GPIO_STA_HIGH 0x11
#define GPIO_SET_LOW  0x12
#define GPIO_SET_HIGH 0x13
#define GPIO_DIR_LOW  0x14
#define GPIO_DIR_HIGH 0x15
#define GPIO_POL_LOW  0x16
#define GPIO_POL_HIGH 0x17

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
//   IO_Expander(STMPE1600) initialization
//-----------------------------------------------------------------
u8 IoExpander_Init(void)
{
  I2C_InitTypeDef  I2C_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  u8 data[2];

  // Enable I2C1 and GPIOB,I clocks
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOI, ENABLE);

  GPIO_DeInit(GPIOB);
  // Set PB6,9 as OD AF with no PullUp - I2C1_SCL, I2C1_SDA
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	// standard: 100k, fast: 400k
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;// better to pull-up?
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  // configure AFIO
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);

  // Set PI8 as Input no PullUp - IO_Expander_INT
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOI, &GPIO_InitStructure);

  // Reset I2C1 IP (affected by other peripherals-E2p,Audio,MEMS on I2C1?)
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
  // Release reset signal of I2C1 IP
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

  // I2C1 configuration --------------------------------------//
  // configure DigitalFilter=8 (0~0x0F)
  I2C_DigitalFilterConfig(I2C1, 8);
  // I2C1 configured as follow:
  //      - Mode = I2C(not SMBus)
  //      - baudrate = (around)200k
  //      - DutyCycle: Tlow/Thigh = 2
  //      - own-address is 7-bit
  //      - Acknowledge Disabled
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_ClockSpeed = 200000;		// 200k baud
  I2C_InitStructure.I2C_ClockSpeed = 50000;		// 50k baud
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;		// 7-bit own-address: right align to LSbit
  I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_Init(I2C1, &I2C_InitStructure);

  // Enable I2C1
  I2C_Cmd(I2C1, ENABLE);
//  delay_ms(1);

  // Init IO Expander //
  // Read IO Expander 1 IDs
//  I2C_Read(I2C_IOEXP1_ADDR, CHIP_ID, 1, data, 2);
  IO_Expander_ReadReg(CHIP_ID, data, 2);
  // little endian for STMPE1600
  if( (((u16)(data[1]) << 8) | data[0]) != STMPE1600_ID )
    return I2C_IOEXP_INIT_FAIL;
  // Read IO Expander 2 IDs

#ifdef ENABLE_IOEXP_DEBUG
  // Read IO Expander 1's reg: ID_VER
  I2C_Read(I2C_IOEXP1_ADDR, ID_VER, 1, data, 1);
#endif

  // Configure IO Expander system regs //
#ifdef ENABLE_IOEXP_RESET
  // Reset IO Expander 1
  IoExp_WriteByte(I2C_IOEXP1_ADDR, SYS_CTRL1,1, 0x80);
  // Release reset of IO Expander 1
  IoExp_WriteByte(I2C_IOEXP1_ADDR, SYS_CTRL1,1, 0x00);
#endif
#ifdef ENABLE_IOEXP_DEBUG
  // Read IO Expander 1's reg: SYS_CTRL1
  I2C_Read(I2C_IOEXP1_ADDR, SYS_CTRL1, 1, data, 1);
#endif

#ifdef ENABLE_IOEXP_DEBUG
  // Read IO Expander 1's reg: INT_EN
  I2C_Read(I2C_IOEXP1_ADDR, INT_EN_LOW, 1, data, 1);
  I2C_Read(I2C_IOEXP1_ADDR, INT_EN_HIGH, 1, data, 1);
  // Read IO Expander 1's reg: INT_STA
  I2C_Read(I2C_IOEXP1_ADDR, INT_STA_LOW, 1, data, 1);
  I2C_Read(I2C_IOEXP1_ADDR, INT_STA_HIGH, 1, data, 1);

  // Read IO Expander 1's reg: GPIO_STA
  I2C_Read(I2C_IOEXP1_ADDR, GPIO_STA_LOW, 1, data, 1);
  I2C_Read(I2C_IOEXP1_ADDR, GPIO_STA_HIGH, 1, data, 1);
  // Read IO Expander 1's reg: GPIO_SET
  I2C_Read(I2C_IOEXP1_ADDR, GPIO_SET_LOW, 1, data, 1);
  I2C_Read(I2C_IOEXP1_ADDR, GPIO_SET_HIGH, 1, data, 1);
  // Read IO Expander 1's reg: GPIO_DIR
  I2C_Read(I2C_IOEXP1_ADDR, GPIO_DIR_LOW, 1, data, 1);
  I2C_Read(I2C_IOEXP1_ADDR, GPIO_DIR_HIGH, 1, data, 1);
  // Read IO Expander 1's reg: GPIO_POL
  I2C_Read(I2C_IOEXP1_ADDR, GPIO_POL_LOW, 1, data, 1);
  I2C_Read(I2C_IOEXP1_ADDR, GPIO_POL_HIGH, 1, data, 1);
#endif

  // Configure IO Expander GPIO regs //
  // Configure IO Expander 1's reg: GPIO_DIR
  IoExp_WriteByte(I2C_IOEXP1_ADDR, GPIO_DIR_LOW,1, 0x85);
  IoExp_WriteByte(I2C_IOEXP1_ADDR, GPIO_DIR_HIGH,1, 0x02);
  // Configure IO Expander 1's reg: GPIO_SET
  IoExp_WriteByte(I2C_IOEXP1_ADDR, GPIO_SET_LOW,1, 0x00);//LOW for IO2&IO0?
  IoExp_WriteByte(I2C_IOEXP1_ADDR, GPIO_SET_HIGH,1, 0x00);
  // Configure IO Expander 1's reg: GPIO_POL
  IoExp_WriteByte(I2C_IOEXP1_ADDR, GPIO_POL_LOW,1, 0x00);
  IoExp_WriteByte(I2C_IOEXP1_ADDR, GPIO_POL_HIGH,1, 0x00);

  // Read IO Expander 1's reg: GPIO_SET
  I2C_Read(I2C_IOEXP1_ADDR, GPIO_SET_LOW, 1, data, 1);
  I2C_Read(I2C_IOEXP1_ADDR, GPIO_SET_HIGH, 1, data, 1);
  // Read IO Expander 1's reg: GPIO_DIR
  I2C_Read(I2C_IOEXP1_ADDR, GPIO_DIR_LOW, 1, data, 1);
  I2C_Read(I2C_IOEXP1_ADDR, GPIO_DIR_HIGH, 1, data, 1);
  // Read IO Expander 1's reg: GPIO_POL
  I2C_Read(I2C_IOEXP1_ADDR, GPIO_POL_LOW, 1, data, 1);
  I2C_Read(I2C_IOEXP1_ADDR, GPIO_POL_HIGH, 1, data, 1);

  // Configure IO Expander Interrupt regs //
  // Enable IO Expander 1's global interrupt, Polarity: Active Low
  IoExp_WriteByte(I2C_IOEXP1_ADDR, SYS_CTRL1,1, 0x04);
  // Disable IO Expander 1's GPIO interrup
  IoExp_WriteByte(I2C_IOEXP1_ADDR, INT_EN_LOW,1, 0x00);
  IoExp_WriteByte(I2C_IOEXP1_ADDR, INT_EN_HIGH,1, 0x00);
  // Clear IO Expander 1's GPIO interrupt status
  I2C_Read(I2C_IOEXP1_ADDR, INT_STA_LOW, 1, data, 1);
  I2C_Read(I2C_IOEXP1_ADDR, INT_STA_HIGH, 1, data, 1);

  return I2C_IOEXP_TEST_PASS;
}

//#define FPGA_I2C_ISSUE
//-----------------------------------------------------------------
//   I2C WriteByte
//   Input   - I2c_Addr: is address of I2C device
//           - Start_Addr: is start address of I2C device to write the data
//           - Addr_Cnt: is start address bytes, should be 1 or 2
//           - data: is the data byte to be written
//   Output      : none
//   Return      : PASS/FAIL result
//
//   wrtie a byte into I2C bus - I2C1
//-----------------------------------------------------------------
u8 IoExp_WriteByte(u8 I2c_Addr, u16 Start_Addr, u8 Addr_Cnt, u8 data)
{
#ifdef FPGA_I2C_ISSUE
  // to avoid I2C limitation in FPGA bitstream, init I2C again
  I2C_E2prom_Init();
#endif

  I2c_Send_Start();
  // send 7 bit address, subsequent operation - write
  if(I2C_Send_7bitAddress(I2c_Addr, I2C_WRITE) != I2C_TEST_PASS)
    return I2C_TEST_FAIL;

  // Send 1/2-byte data address(es)
  while( (I2C1->SR1 & I2C_FLAG_TXE) == 0 )
    ;
  if(Addr_Cnt == 1)
  {
    I2C1->DR = Start_Addr & 0xff;
  }
  else
  {
    I2C1->DR = Start_Addr >> 8;
    I2C1->DR = Start_Addr & 0xff;
    while( (I2C1->SR1 & I2C_FLAG_TXE) == 0 )
      ;
  }

  // write 1 byte data
  if(I2C_Send_Data(data) != I2C_TEST_PASS)
    return I2C_TEST_FAIL;
  I2c_Send_Stop();
  // delay seems needed
  // due to I2C limitation in FPGA bitstream?
  // or due to io_expander limitation?
  delay_ms(1);

  return I2C_TEST_PASS;
}

//-----------------------------------------------------------------
//   I2C ReadByte
//   Input   - I2c_Addr: is address of I2C device
//           - Start_Addr: is start address of I2C device to read the data
//   Output      : data is the read back result
//   Return      : PASS/FAIL result
//
//   read a byte from I2C bus - I2C1
//-----------------------------------------------------------------
u8 IoExp_ReadByte(u8 I2c_Addr, u16 Start_Addr, u8 Addr_Cnt, u8 *data)
{
#ifdef FPGA_I2C_ISSUE
  // to avoid I2C limitation in FPGA bitstream, init I2C again
  I2C_E2prom_Init();
#endif

  I2c_Send_Start2();
  // send 7 bit address, subsequent operation - write
  if(I2C_Send_7bitAddress2(I2c_Addr, I2C_WRITE) != I2C_TEST_PASS)
    return I2C_TEST_FAIL;

  // Send 1/2-byte data address(es)
  while( (I2C1->SR1 & I2C_FLAG_TXE) == 0 )
    ;
  if(Addr_Cnt == 1)
  {
    I2C1->DR = Start_Addr & 0xff;
  }
  else
  {
    I2C1->DR = Start_Addr >> 8;
    while( (I2C1->SR1 & I2C_FLAG_TXE) == 0 )
      ;
    I2C1->DR = Start_Addr & 0xff;
  }
  while( (I2C1->SR1 & I2C_FLAG_TXE) == 0 )
    ;

  // re-send START condition
  I2c_Send_Start2();
  // send 7 bit address, subsequent operation - read
  if(I2C_Send_7bitAddress2(I2c_Addr, I2C_READ) != I2C_TEST_PASS)
    return I2C_TEST_FAIL;
  // disable ACK
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  // send STOP before reading last data byte - special for ST I2C IP
  I2c_Send_Stop();
  // read data packet - 1 byte
  data[0] = I2C_Read_Data();

  return I2C_TEST_PASS;
}

//-----------------------------------------------------------------
//   IO_Expander Read IO
//   Input   - IoExp_Addr: is address of IO_Expander (I2C_IOEXP1_ADDR/0x82)
//           - INx: may be 0~15 or, defines which IN pin of IO_Expander
//           - INx: may be 0xff, defines all IN pins of IO_Expander
//   Output      : None
//   Return      : input state of the pin (0 or 1)
//
//   read IO status from IO_Expander via I2C bus - I2C1
//-----------------------------------------------------------------
u16 IO_Expander_ReadIO(u8 INx)
{
  u8 ResultLow,ResultHigh, ret;

  // read from GPIO_STA reg from IO_Expander
  if(INx < 8)
    ret = IoExp_ReadByte(I2C_IOEXP1_ADDR, GPIO_STA_LOW, 1, &ResultLow);
  else if(INx < 16)
  {
    INx -= 8;
    ret = IoExp_ReadByte(I2C_IOEXP1_ADDR, GPIO_STA_HIGH, 1, &ResultLow);
  }
  else if(INx == 0xFF)
  {
    ret = IoExp_ReadByte(I2C_IOEXP1_ADDR, GPIO_STA_LOW, 1, &ResultLow);
    ret |= IoExp_ReadByte(I2C_IOEXP1_ADDR, GPIO_STA_HIGH, 1, &ResultHigh);
    if(ret != I2C_TEST_PASS)
      return 0xFFFF;
    return (u16)ResultHigh << 8 | ResultLow;
  }
  else
    ret = IoExp_ReadByte(I2C_IOEXP1_ADDR, GPIO_STA_LOW, 1, &ResultLow);
  if(ret != I2C_TEST_PASS)
    return 0xFFFF;
  INx &= 7;
  // get the bit @ correct bit position
  ResultLow &= 1 << INx;
  // shift the bit to bit0
  ResultLow >>= INx;

  return ResultLow;
}

//-----------------------------------------------------------------
//   IO_Expander Write IO
//   Input   - IoExp_Addr: is address of IO_Expander (I2C_IOEXP1_ADDR/0x82 or 0x88)
//           - Outx: is 0~15, defines which pin of IO_Expander
//           - state: is LOW/HIGH (0/1)
//   Output      : None
//   Return      : PASS/FAIL (0 / non-0)
//
//   read IO status from IO_Expander via I2C bus - I2C1
//-----------------------------------------------------------------
u8 IO_Expander_WriteIO(u8 Outx, u8 state)
{
  u8 result;

  // get the bit @ correct bit position
  if(Outx < 8)
  {
    I2C_Read(I2C_IOEXP1_ADDR, GPIO_STA_LOW, 1, &result, 1);
    Outx = 1 << Outx;
    if(state)
      result |= Outx;
    else
      result &= ~Outx;
    result = IoExp_WriteByte(I2C_IOEXP1_ADDR, GPIO_SET_LOW,1, result);
  }
  else if(Outx < 16)
  {
    I2C_Read(I2C_IOEXP1_ADDR, GPIO_STA_HIGH, 1, &result, 1);
    Outx = 1 << (Outx-8);
    if(state)
      result |= Outx;
    else
      result &= ~Outx;
    result = IoExp_WriteByte(I2C_IOEXP1_ADDR, GPIO_SET_HIGH,1, result);
  }
  else
    return I2C_IOEXP_WRITE_FAIL;

  if(result == I2C_TEST_PASS)
    return I2C_IOEXP_TEST_PASS;
  else
    return I2C_IOEXP_WRITE_FAIL;
}

//-----------------------------------------------------------------
//   IO_Expander Read IO
//   Input   - IoExp_Addr: is address of IO_Expander (I2C_IOEXP1_ADDR/0x82)
//           - bits: some bits to change
//           - NewState: ENABLE or DISABLE
//   Output      : None
//   Return      : PASS/FAIL (0 or non-0)
//
//   Enable or disable IO_Expander GPIO interrupt via I2C bus - I2C1
//-----------------------------------------------------------------
u8 IO_Expander_GpioIntCmd(u16 bits, FunctionalState NewState)
{
  u16 state;
  u8 result;

  // read current state of GPIO_INT_EN
  IoExp_ReadByte(I2C_IOEXP1_ADDR, INT_EN_LOW, 1, (u8 *)(&state));
  IoExp_ReadByte(I2C_IOEXP1_ADDR, INT_EN_HIGH, 1, (u8 *)(&state)+1);
  if(NewState)
    state |= bits;
  else
    state &= ~bits;
  // configure GPIO_INT_EN according to bits & NewState
  result = IoExp_WriteByte(I2C_IOEXP1_ADDR, INT_EN_LOW,1, state);
  result |= IoExp_WriteByte(I2C_IOEXP1_ADDR, INT_EN_HIGH, 1, state>>8);

  if(result == I2C_TEST_PASS)
    return I2C_IOEXP_TEST_PASS;
  else
    return I2C_IOEXP_WRITE_FAIL;
}

//-----------------------------------------------------------------
//   IO_Expander Read Register
//   This function will be used to access regs: INT_STA,GPIO_INT_STA
//   Input   - IoExp_Addr: is address of IO_Expander (I2C_IOEXP1_ADDR/0x82 or 0x88)
//           - Reg: is the register to read
//           - length: is how many bytes to read
//   Output      : data
//   Return      : PASS/FAIL status of the command
//
//   Read Register from IO_Expander via I2C bus - I2C1
//-----------------------------------------------------------------
u8 IO_Expander_ReadReg(u8 Reg, u8 * data, u8 length)
{
  u8 i, result=0;

  for(i=0; i< length; i++)
  {
    result |= IoExp_ReadByte(I2C_IOEXP1_ADDR, Reg+i, 1, data+i);
  }

  if(result == I2C_TEST_PASS)
    return I2C_IOEXP_TEST_PASS;
  else
    return I2C_IOEXP_READ_FAIL;
}

//-----------------------------------------------------------------
//   IO_Expander Write Register
//   This function will be used to access regs: INT_EN,GPIO_INT_EN
//   Input   - IoExp_Addr: is address of IO_Expander (I2C_IOEXP1_ADDR/0x82 or 0x88)
//           - Reg: is the register to write
//           - data: is the data to write
//           - length: is how many bytes to write
//   Output      : data
//   Return      : PASS/FAIL status of the command
//
//   Write Register to IO_Expander via I2C bus - I2C1
//-----------------------------------------------------------------
u8 IO_Expander_WriteReg(u8 Reg, u8 * data, u8 length)
{
  u8 i, result=0;

//  result = I2C_Write(IoExp_Addr, Reg, 1, data, length);
  for(i=0; i<length; i++)
  {
    result |= IoExp_WriteByte(I2C_IOEXP1_ADDR, Reg,1, data[i]);
  }

  if(result == I2C_TEST_PASS)
    return I2C_IOEXP_TEST_PASS;
  else
    return I2C_IOEXP_WRITE_FAIL;
}
