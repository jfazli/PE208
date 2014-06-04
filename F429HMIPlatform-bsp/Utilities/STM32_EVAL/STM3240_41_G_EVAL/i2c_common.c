/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
* File Name          : i2c_common.c
* Author             : MCD Tools development Team
* Date First Issued  : August 2012
* Description        : This code is used for MB1045 board test
********************************************************************************
* History:
* November 22, 2012: V0.1
********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "i2c_common.h"
#include "main.h"

#define BAUD_RATE 50000

#define Start_Bit       0x01
#define Address_Sent    0x02
#define BTF_Bit         0x04
#define Rx_Not_Empty    0x40

#define Ack_Failure     0x0400
#define Time_Out        0x4000

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

I2C_TypeDef* I2Cn;

void I2c_Workaround(void)
{
  u8 i;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  GPIO_DeInit(GPIOB);
  GPIO_SetBits(GPIOB, GPIO_Pin_6);
  // Set PB6 as OD OUT with no PullUp - I2C1_SCL
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;// standard: 100k, fast: 400k
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_SetBits(GPIOB, GPIO_Pin_6);
  delay_10us(5);
  for(i=0; i<11; i++)
  {
    GPIO_ResetBits(GPIOB, GPIO_Pin_6);
    delay_10us(5);
    GPIO_SetBits(GPIOB, GPIO_Pin_6);
    delay_10us(5);
  }
}

void I2c_Init(void)
{
  I2C_InitTypeDef  I2C_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  // Enable I2C1 and GPIOB clock
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  GPIO_DeInit(GPIOB);
  GPIO_SetBits(GPIOB, GPIO_Pin_6);
  GPIO_SetBits(GPIOB, GPIO_Pin_9);
  // Set PB6,9 as OD AF with no PullUp - I2C1_SCL, I2C1_SDA
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	// standard: 100k, fast: 400k
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  // configure AFIO
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);

  // Reset I2C1 IP (affected by other peripherals-E2p,Audio,MEMS on I2C1?)
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
  delay_10us(1);
  // Release reset signal of I2C1 IP
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
  delay_10us(1);

  // I2C1 configuration --------------------------------------//
  // I2C1 configured as follow:
  //      - Mode = I2C(not SMBus)
  //      - baudrate = (around)200k
  //      - DutyCycle: Tlow/Thigh = 2
  //      - own-address is 7-bit
  //      - Acknowledge Disabled
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_ClockSpeed = 50000;		// 50k baud
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;		// 7-bit own-address: right align to LSbit
  I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_Init(I2C1, &I2C_InitStructure);

  // Enable I2C1
  I2C_Cmd(I2C1, ENABLE);
}

//-----------------------------------------------------------------
//   I2c_Send_Start
//   send a start condition to I2C bus
//-----------------------------------------------------------------
void I2c_Send_Start(void)
{
  // send START condition
  I2C_GenerateSTART(I2C1, ENABLE);
  // wait for START condition sent
  while((I2C1->SR1 & Start_Bit) == 0)
  {
  }
}

//#define TIMEOUT 1800123
#define TIMEOUT 180123
//-----------------------------------------------------------------
//   I2c_Send_Start2
//   send a start condition to I2C bus, check timeout
//-----------------------------------------------------------------
void I2c_Send_Start2(void)
{
  u32 u;
  // send START condition
  I2C_GenerateSTART(I2C1, ENABLE);
  u = 0;
  // wait for START condition sent
  while((I2C1->SR1 & Start_Bit) == 0)
  {
    u++;
    if(u > TIMEOUT)
    {
      u = 0;
      // workaround: [send 9 cycles at SCL?]
      I2c_Workaround();
      I2c_Init();
      // send START condition
      I2C_GenerateSTART(I2C1, ENABLE);
      while((I2C1->SR1 & Start_Bit) == 0)
      {
        u++;
        if(u > (2*TIMEOUT))
          break;
      }
      I2c_Send_Stop();
    }
  }
}

//-----------------------------------------------------------------
//   I2c_Send_Stop
//   send a stop condition to I2C bus
//-----------------------------------------------------------------
void I2c_Send_Stop(void)
{
  // send STOP condition
  I2C_GenerateSTOP(I2C1, ENABLE);
}

//-----------------------------------------------------------------
//   I2C_Send_7bitAddress
//   send a 7-bit-Address with read/write direction to I2C bus
//-----------------------------------------------------------------
u8 I2C_Send_7bitAddress(u8 addr, u8 direction)
{
  // 7-bit address for E2prom
  addr = addr | direction;
  I2C1->DR = addr;

  // check whether 7-bit-Address is sent correctly 
  while( (I2C1->SR1 & I2C_FLAG_ADDR) == 0 )
  {
    if( (I2C1->SR1 & I2C_FLAG_ARLO) || (I2C1->SR1 & I2C_FLAG_AF) )
    {
      // clear I2C_FLAG_ARLO & I2C_FLAG_AF
      I2C1->SR1 &= ~(I2C_FLAG_ARLO | I2C_FLAG_AF);
      // Read SR2 to clear ADDR bit in SR1
      I2C1->SR2;      
      I2c_Send_Stop();
      return I2C_TEST_FAIL;
    }
  }
  // Read SR2 to clear ADDR bit in SR1
  I2C1->SR2;
  return I2C_TEST_PASS;
}

//-----------------------------------------------------------------
//   I2C_Send_7bitAddress2
//   send a 7-bit-Address with read/write direction to I2C bus,check timeout
//-----------------------------------------------------------------
u8 I2C_Send_7bitAddress2(u8 addr, u8 direction)
{
  u32 u=0;
  // 7-bit address for E2prom
  addr = addr | direction;
  I2C1->DR = addr;

  // check whether 7-bit-Address is sent correctly 
  while( (I2C1->SR1 & I2C_FLAG_ADDR) == 0 )
  {
    if( (I2C1->SR1 & I2C_FLAG_ARLO) || (I2C1->SR1 & I2C_FLAG_AF) )
    {
      // clear I2C_FLAG_ARLO & I2C_FLAG_AF
      I2C1->SR1 &= ~(I2C_FLAG_ARLO | I2C_FLAG_AF);
      // Read SR2 to clear ADDR bit in SR1
      I2C1->SR2;      
      I2c_Send_Stop();
      return I2C_TEST_FAIL;
    }
    u++;
    if(u > TIMEOUT)
    {
      I2c_Send_Stop();
      I2c_Init();
      return I2C_TEST_FAIL;
    }
  }
  // Read SR2 to clear ADDR bit in SR1
  I2C1->SR2;
  return I2C_TEST_PASS;
}

//-----------------------------------------------------------------
//   I2C_Send_Data
//   send an 8-bit data to I2C bus
//-----------------------------------------------------------------
u8 I2C_Send_Data(u8 data)
{
  u32 u;
//  while( (I2C1->SR1 & I2C_FLAG_TXE) == 0 )
//    ;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE) == RESET)
    ;
  I2C1->DR = data;
  while( (I2C1->SR1 & I2C_FLAG_TXE) == 0 )
  {
    if( (I2C1->SR2 & I2C_FLAG_ARLO) || (I2C1->SR1 & I2C_FLAG_AF) )
    {
      I2c_Send_Stop();
      return I2C_TEST_FAIL;
    }
    u++;
    if(u > TIMEOUT)
    {
      I2c_Send_Stop();
      I2c_Init();
      return I2C_TEST_FAIL;
    }
  }
  return I2C_TEST_PASS;
}

//-----------------------------------------------------------------
//   I2C_Read_Data
//   read an 8-bit data from I2C bus
//-----------------------------------------------------------------
u8 I2C_Read_Data()
{
  while( (I2C1->SR1 & I2C_FLAG_RXNE) == 0 )
    ;
  return I2C1->DR;
}

//-----------------------------------------------------------------
//   I2C_Read_exp
//
//   read a packet from I2C bus - I2Cn
//-----------------------------------------------------------------
u8 I2C_Read_exp(u8 I2c_Addr, u16 Start_Addr, u8 Addr_Cnt, u8 * data, u8 length)
{
  u8 i;

#ifdef FPGA_I2C_ISSUE
  // to avoid I2C limitation in FPGA bitstream, init I2C again
  I2C_E2prom_Init();
#endif

  if(length==0)
    return I2C_TEST_PASS;
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
  }
  while( (I2C1->SR1 & I2C_FLAG_TXE) == 0 )
    ;

  // re-send START condition
  I2c_Send_Start();
  // send 7 bit address, subsequent operation - read
  if(I2C_Send_7bitAddress(I2c_Addr, I2C_READ) != I2C_TEST_PASS)
    return I2C_TEST_FAIL;
  // enable ACK
  I2C_AcknowledgeConfig(I2C1, ENABLE);
  // read data packet
  length--;
  for(i=0; i<length; i++)
  {
    data[i] = (u8) (I2C_Read_Data());
  }
  // disable ACK
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  // send STOP before reading last data byte - special for ST I2C IP
  I2c_Send_Stop();
  data[i] = I2C_Read_Data();

  return I2C_TEST_PASS;
}

//-----------------------------------------------------------------
//   I2C Write to other devices except E2prom
//
//   wrtie a packet into I2C bus - I2Cn
//-----------------------------------------------------------------
u8 I2C_Write_exp(u8 I2c_Addr, u16 Start_Addr, u8 Addr_Cnt, u8* data, u8 length)
{
  u8 i;
#ifdef FPGA_I2C_ISSUE
  // to avoid I2C limitation in FPGA bitstream, init I2C again
  I2C_E2prom_Init();
#endif

  if(length == 0)
    return I2C_TEST_PASS;
  for(i=0; i<length; i++)
  {
    I2c_Send_Start();
    // send 7 bit address, subsequent operation - write
    if(I2C_Send_7bitAddress(I2c_Addr, I2C_WRITE) != I2C_TEST_PASS)
      return I2C_TEST_FAIL;

    // Send 1/2-byte data address(es)
    while( (I2Cn->SR1 & I2C_FLAG_TXE) == 0 )
      ;
    if(Addr_Cnt == 1)
    {
      I2Cn->DR = (Start_Addr & 0xff) + i;
    }
    else
    {
      I2Cn->DR = Start_Addr >> 8;
      I2Cn->DR = (Start_Addr & 0xff) + i;
    }
    while( (I2Cn->SR1 & I2C_FLAG_TXE) == 0 )
      ;
    // write data packet - 1 byte per time (byte write mode)
    if(I2C_Send_Data(data[i]) != I2C_TEST_PASS)
      return I2C_TEST_FAIL;
    I2c_Send_Stop();
#ifdef FPGA_I2C_ISSUE
    // to avoid I2C limitation in FPGA bitstream, init I2C again
    I2C_E2prom_Init();
#endif
  }

  return I2C_TEST_PASS;
}


//-----------------------------------------------------------------
//   I2C Read
//
//   read a packet from I2C bus - I2C1
//-----------------------------------------------------------------
u8 I2C_Read(u8 I2c_Addr, u16 Start_Addr, u8 Addr_Cnt, u8 * data, u8 length)
{
  u8 i;

  // init I2C again to avoid I2C instability
//  I2c_Init();

  if(length==0)
    return I2C_TEST_PASS;
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
  }
  while( (I2C1->SR1 & I2C_FLAG_TXE) == 0 )
    ;
  // re-send START condition
  I2c_Send_Start();
  // send 7 bit address, subsequent operation - read
  if(I2C_Send_7bitAddress(I2c_Addr, I2C_READ) != I2C_TEST_PASS)
    return I2C_TEST_FAIL;
  // enable ACK
  I2C_AcknowledgeConfig(I2C1, ENABLE);
  // read data packet
  length--;
  for(i=0; i<length; i++)
  {
    data[i] = (u8) (I2C_Read_Data());
  }
  // disable ACK
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  // send STOP before reading last data byte - special for ST I2C IP
  I2c_Send_Stop();
  data[i] = I2C_Read_Data();

  return I2C_TEST_PASS;
}

//-----------------------------------------------------------------
//   I2C Read 2
//
//   read a packet from I2C bus - I2C1, with timeout
//-----------------------------------------------------------------
u8 I2C_Read2(u8 I2c_Addr, u16 Start_Addr, u8 Addr_Cnt, u8 * data, u8 length)
{
  u8 i;

  if(length==0)
    return I2C_TEST_PASS;
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
  // enable ACK
  I2C_AcknowledgeConfig(I2C1, ENABLE);
  // read data packet
  length--;
  for(i=0; i<length; i++)
  {
    data[i] = (u8) (I2C_Read_Data());
  }
  // disable ACK
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  // send STOP before reading last data byte - special for ST I2C IP
  I2c_Send_Stop();
  data[i] = I2C_Read_Data();

  return I2C_TEST_PASS;
}

//-----------------------------------------------------------------
//   I2C Write to other devices except E2prom
//
//   wrtie a packet into I2C bus - I2C1
//-----------------------------------------------------------------
u8 I2C_Write(u8 I2c_Addr, u16 Start_Addr, u8 Addr_Cnt, u8* data, u8 length)
{
  u8 i;
  // init I2C again to avoid I2C instability
  I2c_Init();

  if(length == 0)
    return I2C_TEST_PASS;
  for(i=0; i<length; i++)
  {
    I2c_Send_Start();
    // send 7 bit address, subsequent operation - write
    if(I2C_Send_7bitAddress(I2c_Addr, I2C_WRITE) != I2C_TEST_PASS)
      return I2C_TEST_FAIL;

    // Send 1/2-byte data address(es)
    while( (I2C1->SR1 & I2C_FLAG_TXE) == 0 )
      ;
    if(Addr_Cnt == 1)
    {
      I2C1->DR = (Start_Addr & 0xff) + i;
    }
    else
    {
      I2C1->DR = Start_Addr >> 8;
      I2C1->DR = (Start_Addr & 0xff) + i;
    }
    while( (I2C1->SR1 & I2C_FLAG_TXE) == 0 )
      ;
    // write data packet - 1 byte per time (byte write mode)
    if(I2C_Send_Data(data[i]) != I2C_TEST_PASS)
      return I2C_TEST_FAIL;
    I2c_Send_Stop();
    __NOP();
#ifdef FPGA_I2C_ISSUE
    // to avoid I2C limitation in FPGA bitstream, init I2C again
    I2C_E2prom_Init();
#endif
  }

  return I2C_TEST_PASS;
}

