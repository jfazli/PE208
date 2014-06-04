/*
 *  Copyright (c) 2012, Cameon,
 *  All Rights Reserved
 *
 *  The copyright notice above does not evidence any
 *  actual or intended publication of such source code.
 */

/*!
 *  \file     tscap.c
 *  \brief    Capacitive touch screen driver code + other driver intialization
 *  \authors
 *  \date     21/08/2012
 */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_i2c.h"
#include "stm324x9i_eval_lcd.h"
#include "tscap.h"
#include "touch.h"
#include <stdio.h>
#include "ioe.h"
#include "ioe_task.h"
#include "interrupts.h"

#define Y_COORD_OFFSET 0xB2//B2//8A//0xDC

/* Private variables ---------------------------------------------------------*/
__IO uint16_t  gTsAddress = 0;
__IO uint32_t  gTsTimeout = TS_TIMEOUT;


sFinger gFinger[MAX_POINTS];


//Initialization table
/*!
 * \ TS table
 * \brief TS Setting table
 */
static const sTsRegInit FT5216_REG_Setting[] =
{
	//Wake up
	//Done before init
	//{0x23,0x0000,0x01},	/*!<Exit sleep mode */
	//{0x2B,0x0003,0x01},		/*!<Enable DSP clock */

	//Touch Panel configuration table
	{0x00,0x0000,0x01},		/*!<Operating mode 0 = normal operating mode */
	{0xA4,0x0000,0x01},		/*!<IT enable */
	{0xA5,0x0000,0x01},		/*!<Power mode = active */
	

	//TS Analog setting table
	

	//Touch detection setting
	


	//Finger recognition criteria
	

	//Scaling factor
	

	//
	
};


/* Private functions ---------------------------------------------------------*/
/*! ------------------------------------------------------------------------------
* \fn Function :uint32_t Ts_Timeout_UserCallback(void)
*------------------------------------------------------------------------------
*
* Description :
* -------------
*  \brief Time out i2c callback
*
* Parameter :
* ------------
*  \param [In]  :
*  \param [Out] :
*
* Return :
* --------
*  \return : Error code
*
*------------------------------------------------------------------------------
*/
uint32_t Ts_Timeout_UserCallback(void)
{
  /* Return with error code */
  return TS_FAIL;
}

/*! ------------------------------------------------------------------------------
* \fn Function :void I2c1LowLevelInit(void)
*------------------------------------------------------------------------------
*
* Description :
* -------------
*  \brief Low I2C1 low level initialization
*
* Parameter :
* ------------
*  \param [In]  :
*  \param [Out] :
*
* Return :
* --------
*  \return :
*
*------------------------------------------------------------------------------
*/
void I2c1LowLevelInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

  /*!< I2C1 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

  /*!< GPIO Periph clock enable */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/*!< GPIO Periph clock enable */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);


  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Reset I2C1 IP */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);

  /* Release reset signal of I2C1 IP */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);

  /*!< GPIO configuration */
  /*!< Configure I2C1 pins: SCL */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*!< Configure I2C1 pins: SDA */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_SetBits(GPIOB,GPIO_Pin_6);
  GPIO_SetBits(GPIOB,GPIO_Pin_7);

  GPIO_ResetBits(GPIOB,GPIO_Pin_6);
  GPIO_ResetBits(GPIOB,GPIO_Pin_7);

  /*!< GPIO configuration */
  /*!< Configure I2C1 pins: SCL */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*!< Configure I2C1 pins: SDA */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Connect PB6 to I2C_SCL*/
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);

  /* Connect PB7 to I2C_SDA*/
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);
	
	/* Set EXTI pin as Input PullUp */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
  
  /* Connect Button EXTI Line to Button GPIO Pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);  
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line13 ;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Configure and enable IMPULSE interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


}

/*! ------------------------------------------------------------------------------
* \fn Function :void tsReset(void)
*------------------------------------------------------------------------------
*
* Description :
* -------------
*  \brief TS reset
*
* Parameter :
* ------------
*  \param [In]  :
*  \param [Out] :
*
* Return :
* --------
*  \return :
*
*------------------------------------------------------------------------------
*/
void tsReset(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /*!< GPIO Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);

  /*!< GPIO configuration */
  /*!< Configure I2C1 pins: SCL */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  //GPIO_Init(GPIOH, &GPIO_InitStructure);

  //GPIO_SetBits(GPIOH,GPIO_Pin_14);

  //Delay(100);
  //GPIO_ResetBits(GPIOH,GPIO_Pin_14);

  //Delay(1000);
  //GPIO_SetBits(GPIOH,GPIO_Pin_14);


  //TODO
  /*!< Configure I2C1 pins: SDA */
  /*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOB, &GPIO_InitStructure);*/
}

/*! ------------------------------------------------------------------------------
* \fn Function :void Init_Tscap_I2c1(void)
*------------------------------------------------------------------------------
*
* Description :
* -------------
*  \brief Low I2C1 initialization
*
* Parameter :
* ------------
*  \param [In]  :
*  \param [Out] :
*
* Return :
* --------
*  \return :
*
*------------------------------------------------------------------------------
*/
void Init_Tscap_I2c1(void)
{
	I2C_InitTypeDef  I2C_InitStructure;

	I2c1LowLevelInit();
	/*!< I2C configuration */
	/* I2C1 configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = TSCAP_I2C_ADDR;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = TS_I2C_SPEED;

	/* I2C1 Peripheral Enable */
	I2C_Cmd(I2C1, ENABLE);
	/* Apply I2C1 configuration after enabling it */
	I2C_Init(I2C1, &I2C_InitStructure);

	gTsAddress = TSCAP_I2C_ADDR;
}

/*! ------------------------------------------------------------------------------
* \fn Function :uint32_t ReadReg_Tscap(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead)
*------------------------------------------------------------------------------
*
* Description :
* -------------
*  \brief Read TS register
*
* Parameter :
* ------------
*  \param [In]  : pBuffer destination data
*  \param [In]  : ReadAddr register to read
*  \param [In]  : NumByteToRead number bytes to read
*  \param [Out] :
*
* Return :
* --------
*  \return : Error code
*
*------------------------------------------------------------------------------
*/
uint32_t ReadReg_Tscap(uint8_t* pBuffer, uint16_t ReadAddr, uint8_t* NumByteToRead)
{
	uint16_t i = 0;
  /*!< While the bus is busy */
  gTsTimeout = TS_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
  {
    if((gTsTimeout--) == 0) return Ts_Timeout_UserCallback();
  }

  /*!< Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
  gTsTimeout = TS_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((gTsTimeout--) == 0) return Ts_Timeout_UserCallback();
  }

  /*!< Send TS address for write */
  I2C_Send7bitAddress(I2C1, gTsAddress, I2C_Direction_Transmitter);

  /*!< Test on EV6 and clear it */
  gTsTimeout = TS_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((gTsTimeout--) == 0) return Ts_Timeout_UserCallback();
  }

  /*!< Send the TS register to read */
  I2C_SendData(I2C1, (uint8_t)(ReadAddr & 0x00FF));

  /*!< Test on EV8 and clear it */
	gTsTimeout = TS_FLAG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF) == RESET)
	{
		if((gTsTimeout--) == 0) return Ts_Timeout_UserCallback();
	}

	/*!< Send STRAT condition a second time */
	I2C_GenerateSTART(I2C1, ENABLE);

	/*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
	gTsTimeout = TS_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((gTsTimeout--) == 0) return Ts_Timeout_UserCallback();
	}

	/*!< Send TS address for read */
	I2C_Send7bitAddress(I2C1, gTsAddress, I2C_Direction_Receiver);


	/* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
	gTsTimeout = TS_FLAG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
	{
		if((gTsTimeout--) == 0) return Ts_Timeout_UserCallback();
	}

	i = 0;
	while(*NumByteToRead != 0)
	{
		/* Wait for the byte to be received */
		gTsTimeout = TS_FLAG_TIMEOUT;
		//while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET)
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			if((gTsTimeout--) == 0) return Ts_Timeout_UserCallback();
		}

		/*!< Read the byte received from TS */
		pBuffer[i] = I2C_ReceiveData(I2C1);
		//Increment data index
		i++;
		/*!< Decrement the read bytes counter */
		(uint16_t)(*NumByteToRead)--;
	}

	/*!< Disable Acknowledgment */
	I2C_AcknowledgeConfig(I2C1, DISABLE);

	/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
	(void)I2C1->SR2;

	/*!< Send STOP Condition */
	I2C_GenerateSTOP(I2C1, ENABLE);
	/* Wait to make sure that STOP control bit has been cleared */
	gTsTimeout = TS_FLAG_TIMEOUT;
	while(I2C1->CR1 & I2C_CR1_STOP)
	{
		if((gTsTimeout--) == 0) return Ts_Timeout_UserCallback();
	}

	/*!< Re-Enable Acknowledgment to be ready for another reception */
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	/* If all operations OK, return TS_OK (0) */
	return TS_OK;
}
/*! ------------------------------------------------------------------------------
* \fn Function :uint32_t WriteReg_Tscap(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t* NumByteToWrite)
*------------------------------------------------------------------------------
*
* Description :
* -------------
*  \brief Write TS register
*
* Parameter :
* ------------
*  \param [In]  : pBuffer data to write
*  \param [In]  : WriteAddr register to write
*  \param [In]  : NumByteToWrite number bytes to write
*  \param [Out] :
*
* Return :
* --------
*  \return : Error code
*
*------------------------------------------------------------------------------
*/
uint32_t WriteReg_Tscap(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite)
{
	uint16_t i;
  /*!< While the bus is busy */
  gTsTimeout = TS_TIMEOUT;
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
  {
    if((gTsTimeout--) == 0) return Ts_Timeout_UserCallback();
  }

  /*!< Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
  gTsTimeout = TS_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((gTsTimeout--) == 0) return Ts_Timeout_UserCallback();
  }

  /*!< Send TS address for write */
  I2C_Send7bitAddress(I2C1, gTsAddress, I2C_Direction_Transmitter);

  /*!< Test on EV6 and clear it */
  gTsTimeout = TS_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((gTsTimeout--) == 0) return Ts_Timeout_UserCallback();
  }

  /*!< Send the TS register to write */
  I2C_SendData(I2C1, (uint8_t)(WriteAddr & 0x00FF));

  /*!< Test on EV8 and clear it */
	gTsTimeout = TS_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
	{
		if((gTsTimeout--) == 0) return Ts_Timeout_UserCallback();
	}
	i = 0;
	while( *NumByteToWrite != 0)
	{
		/*!< Send the data to write */
		I2C_SendData(I2C1, pBuffer[i]);

		/*!< Test on EV8 and clear it */
		gTsTimeout = TS_FLAG_TIMEOUT;
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF) == RESET)
		{
			if((gTsTimeout--) == 0) return Ts_Timeout_UserCallback();
		}
		/*!< Decrement the write bytes counter */
		(uint16_t)(*NumByteToWrite)--;
		i++;
	}
	/*!< Disable Acknowledgment */
	I2C_AcknowledgeConfig(I2C1, DISABLE);

	/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
	(void)I2C1->SR2;

	/*!< Send STOP Condition */
	I2C_GenerateSTOP(I2C1, ENABLE);


	/*!< Re-Enable Acknowledgment to be ready for another reception */
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	/* If all operations OK, return TS_OK (0) */
	return TS_OK;
}

/*! ------------------------------------------------------------------------------
* \fn Function :void TsInitialSettings(void)
*------------------------------------------------------------------------------
*
* Description :
* -------------
*  \brief TS initial settings
*
* Parameter :
* ------------
*  \param [In]  :
*  \param [Out] :
*
* Return :
* --------
*  \return :
*
*------------------------------------------------------------------------------
*/
void TsInitialSettings(void)
{
	uint32_t i;
	uint8_t buf[2],nb_val;

	//FT5216_REG_Setting table contains all TS FT5216 controller settings
	for(i = 0; i < (sizeof(FT5216_REG_Setting) / sizeof(sTsRegInit)) ; i++)
	{
		buf[0] = (uint8_t)FT5216_REG_Setting[i].Val & 0x00FF;
		buf[1] = (uint8_t)(FT5216_REG_Setting[i].Val & 0xFF00) >> 8;
		nb_val = FT5216_REG_Setting[i].NbVal;
		WriteReg_Tscap(buf, FT5216_REG_Setting[i].Reg, &nb_val);

	}
}

/*! ------------------------------------------------------------------------------
* \fn Function :void TsInit(void)
*------------------------------------------------------------------------------
*
* Description :
* -------------
*  \brief TS init
*
* Parameter :
* ------------
*  \param [In]  :
*  \param [Out] :
*
* Return :
* --------
*  \return :
*
*------------------------------------------------------------------------------
*/
void TsInit(void)
{
	uint16_t id = 0;
	uint8_t buf[2],i = 0;

#ifdef STM3240G
	//tsReset();
#endif
	Init_Tscap_I2c1();
	//Soft reset
	//WriteReg_Tscap(buf, 0x01, &i);
	//Exit sleep mode
	//WriteReg_Tscap(buf, 0x23, &i);
	//Read device ID
	
	i = 1;
	buf[0] = 0;
	ReadReg_Tscap(buf, 0xA0, &i);
	
	i = 1;
	buf[0] = 0;
	ReadReg_Tscap(buf, 0xA4, &i);
	
	i = 1;
	buf[0] = 00;										//Enable auto calibration
	WriteReg_Tscap(buf, 0xA0, &i);
	
	i = 1;
	buf[0] = 0;
	ReadReg_Tscap(buf, 0xA8, &i);
	id = (uint16_t)buf[0];
	//if(id == 0x2531)
	{
		TsInitialSettings();
	}
	
	i = 1;
	buf[0] = 0;
	ReadReg_Tscap(buf, 0xA4, &i);
	i = 1;
	buf[0] = 4;										//Work = 1  configure = 0 calibration = 2 factory = 3  auto calibration = 4
	WriteReg_Tscap(buf, 0xA7, &i);
	
	i = 1;
	buf[0] = 0;
	ReadReg_Tscap(buf, 0xA7, &i);
	
	i = 1;
	buf[0] = 1;										//Work = 1  configure = 0 calibration = 2 factory = 3  auto calibration = 4
	WriteReg_Tscap(buf, 0xA7, &i);
	
	//Check error code register
	buf[0] = 0;
	i = 1;
	ReadReg_Tscap(buf, 0xA9, &i);
	if(buf[0] != 0)
	{
		printf("TS error code : %X\n",(int)buf[0]);
	}
}

/*! ------------------------------------------------------------------------------
* \fn Function :sFinger TsReadFinger(uint8_t finger_nb)
*------------------------------------------------------------------------------
*
* Description :
* -------------
*  \brief Read the finger number finger_nb
*
* Parameter :
* ------------
*  \param [In]  : finger number
*  \param [Out] :
*
* Return :
* --------
*  \return : sFinger
*
*------------------------------------------------------------------------------
*/
sFinger TsReadFinger(uint8_t finger_id)
{
	uint8_t buf[4];
	uint8_t reg,i = 0;

	sFinger finger;

	//memset(buf,0,4);
	finger.XCoord=0;
	finger.YCoord=0;
	finger.WeightIndex=0;
	finger.SpeedIndex=0;

	switch(finger_id)
	{
		case 1 :	reg=REG_FINGER1;	//Read finger 1
		break;
		case 2 :	reg=REG_FINGER2;	//Read finger 2
		break;
		case 3 :	reg=REG_FINGER3;	//Read finger 3
		break;
		case 4 :	reg=REG_FINGER4;	//Read finger 4
		break;
		case 5 :	reg=REG_FINGER5;	//Read finger 5
		break;
		default :return finger;
	}

	i=1;
	ReadReg_Tscap(buf, reg, &i);							//TOUCHn_XH
	finger.FingerFlag = (buf[0] & 0xC0) >> 6;	//7:6 Event flag    0:3 X MSB
	

	i=1;
	reg = reg + 1;														//TOUCHn_XL
	ReadReg_Tscap(&buf[1], reg, &i);
	finger.XCoord = ((buf[0] & 0x0F) << 8) | (buf[1] & 0xFF);  //buf[0] = X MSB   buf[1] = X LSB
	
	i=1;																			//TOUCHn_YH
	reg = reg + 1;
	ReadReg_Tscap(&buf[0], reg, &i);					//0:3 Y MSB   7:4 Touch ID
	finger.EventNo = (buf[0] & 0xF0) >> 4;
	
	i=1;																			//TOUCHn_YL
	reg = reg + 1;
	ReadReg_Tscap(&buf[1], reg, &i);
	finger.YCoord = ((buf[0] & 0x0F) << 8) | (buf[1] & 0xFF);  //buf[0] = Y MSB   buf[1] = Y LSB
	
	
	if (finger.XCoord > LCD_PIXEL_WIDTH)
	{
		finger.XCoord = LCD_PIXEL_WIDTH;
	}
	
	if (finger.YCoord > LCD_PIXEL_HEIGHT)
	{
		finger.YCoord = LCD_PIXEL_HEIGHT;
	}
	
	i=1;																			//TOUCHn_WEIGHT
	reg = reg + 1;
	ReadReg_Tscap(&buf[0], reg, &i);
	finger.SpeedIndex = buf[0] & 0x03;
	finger.TouchDirection = (buf[0] & 0x0C) >> 2;
	
	i=1;																			//TOUCHn_MISC
	reg = reg + 1;
	ReadReg_Tscap(&buf[0], reg, &i);
	finger.WeightIndex = buf[0];
	

	return (finger);
}

/*! ------------------------------------------------------------------------------
* \fn Function :sEvent TsReadEvent()
*------------------------------------------------------------------------------
*
* Description :
* -------------
*  \brief Read the event in the event stack
*
* Parameter :
* ------------
*  \param [In]  :
*  \param [Out] :
*
* Return :
* --------
*  \return : sEvent
*
*------------------------------------------------------------------------------
*/
uint8_t TsReadEvent(void)
{
	uint8_t buf;
	uint8_t reg,i = 0;

	reg = REG_EVENT_STACK; //Read Event number
	i=1;
	ReadReg_Tscap(&buf, reg, &i);
	return buf;

}

/*! ------------------------------------------------------------------------------
* \fn Function :sEvent TsReadEvent()
*------------------------------------------------------------------------------
*
* Description :
* -------------
*  \brief Read the status
*
* Parameter :
* ------------
*  \param [In]  :
*  \param [Out] :
*
* Return :
* --------
*  \return : sEvent
*
*------------------------------------------------------------------------------
*/
uint8_t TsReadStatus(void)
{
	uint8_t buf;
	uint8_t reg,i = 0;

	reg = REG_EVENT_STATUS; //Read status
	i=1;
	ReadReg_Tscap(&buf, reg, &i);
	return buf;
}

void TS_interrupt(void)
{
    TOUCH_interrupt() ;
}

/**
  * @brief  This function handles External line  interrupt request.
  * @param  None
  * @retval None
  */
void IT_Touch_Sense(void)
{
  if(EXTI_GetITStatus(EXTI_Line13) != RESET)
  {
			#define IOE_USE_TASK // TODO: remove test force
      #ifdef IOE_USE_TASK
				//interrupt = TRUE;
				IOE_TASK_handle_interrupt();
      #else
        TS_interrupt();
      #endif
      EXTI_ClearITPendingBit(EXTI_Line13);
  }
}


/**
  * @brief  This function handles External line 2 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
	enterInterrupt();
	IT_Touch_Sense();
	leaveInterrupt();
}

/*
void TS_task(void)
{
	#ifdef USE_RTOS
		if (interrupt == TRUE)
		{
			TS_interrupt();
			interrupt = FALSE;
		}
	#endif
}
*/

