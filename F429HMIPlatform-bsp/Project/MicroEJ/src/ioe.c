/*
 * C
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#include "microej.h"
#include "ioe.h"
#include "joystick.h"
#include "touch.h"
//#include "cpu.h" 
#if MCU == 2
#include "stm32f2xx.h"
#include "stm322xg_eval_ioe.h"
#elif MCU == 4
#include "stm32f4xx.h"
#include "stm324xg_eval_ioe.h"
#elif MCU == 42
#include "stm32f4xx.h"
#include "stm324x9i_eval_ioe.h"
#include "stm324x9i_eval_ioe16.h"
#else
#error Invalid MCU
#endif
#include "interrupts.h"
#include "tscap.h"
#include "ioe_task.h"
#include <stdio.h>

/**
 * Set this define to use the RTOS to manage the IOE interrupts in a RTOS task. 
 * Otherwise the IOE interrupts are managed in interrupt routine (deprecated)
 */ 
#define USE_RTOS

#define _I2C_SPEED 400000 

#ifdef USE_RTOS
#include <RTL.h>
#define IOE_INTERRUPT_FLAG 0x1
#endif

#define TIMEOUT_MAX    0x3000 /*<! The value of the maximal timeout for I2C waiting loops */

uint8_t IOE_initialized = MICROEJ_FALSE;

static void _delay_(__IO uint32_t nCount)
{
	__IO uint32_t index = 0;
	for(index = (100000 * nCount); index != 0; index--);
}

/**
 * @brief  Initializes the GPIO pins used by the IO expander.
 * @param  None
 * @retval None
 */
void _IOE_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable IOE_I2C and IOE_I2C_GPIO_PORT & Alternate Function clocks */
	RCC_APB1PeriphClockCmd(IOE_I2C_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(IOE_I2C_SCL_GPIO_CLK | IOE_I2C_SDA_GPIO_CLK |
			IOE_IT_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Reset IOE_I2C IP */
	RCC_APB1PeriphResetCmd(IOE_I2C_CLK, ENABLE);

	/* Release reset signal of IOE_I2C IP */
	RCC_APB1PeriphResetCmd(IOE_I2C_CLK, DISABLE);

	/* Connect PXx to I2C_SCL*/
	GPIO_PinAFConfig(IOE_I2C_SCL_GPIO_PORT, IOE_I2C_SCL_SOURCE, IOE_I2C_SCL_AF);
	/* Connect PXx to I2C_SDA*/
	GPIO_PinAFConfig(IOE_I2C_SDA_GPIO_PORT, IOE_I2C_SDA_SOURCE, IOE_I2C_SDA_AF);

	/* IOE_I2C SCL and SDA pins configuration */
	GPIO_InitStructure.GPIO_Pin = IOE_I2C_SCL_PIN |IOE_I2C_SDA_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(IOE_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = IOE_I2C_SDA_PIN;
	GPIO_Init(IOE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

	/* Set EXTI pin as Input PullUp - IO_Expander_INT */
	GPIO_InitStructure.GPIO_Pin = IOE_IT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(IOE_IT_GPIO_PORT, &GPIO_InitStructure);

	/* Connect Button EXTI Line to Button GPIO Pin */
	SYSCFG_EXTILineConfig(IOE_IT_EXTI_PORT_SOURCE, IOE_IT_EXTI_PIN_SOURCE);
}

/**
 * @brief  Configure the I2C Peripheral used to communicate with IO_Expanders.
 * @param  None
 * @retval None
 */
void _IOE_I2C_Config(void)
{
	I2C_InitTypeDef I2C_InitStructure;

	/* IOE_I2C configuration */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = _I2C_SPEED;

	I2C_Init(IOE_I2C, &I2C_InitStructure);

}

/**
 * @brief  Configure the IO expander 1
 * @param  None
 * @retval IOE_OK if all initializations done correctly. Other value if error.
 */
uint8_t _IOE1_Config (void)
{

	/* Read IO Expander 1 ID  */
	if(_IOE_IsOperational(IOE_ADDR)) return IOE_NOT_OPERATIONAL;

	/* Generate IOExpander Software reset */
	if(_IOE_Reset(IOE_ADDR)) return IOE_FAILURE ;

	/* Enable the GPIO, Touch Screen and ADC functionalities */
	//if(_IOE_FnctCmd(IOE_ADDR, IOE_IO_FCT | IOE_TS_FCT | IOE_ADC_FCT, ENABLE)) return IOE_FAILURE ;


	/* Touch Screen controller configuration */
	//if(_IOE_TS_Config()) return IOE_FAILURE ;

	//if(_IOE_ITConfig(IOE_ITSRC_TSC)) return IOE_FAILURE ;
  /* Generate IO Expander Software reset */
  IOE_Reset(); 
  
  /* IO Expander configuration */
  /* Enable the GPIO, Touch Screen and ADC functionalities */
  IOE_FnctCmd(IOE_ADC_FCT, ENABLE);

  /* Touch Screen controller configuration */
  IOE_TS_Config();
	/* Configuration is OK */
	return IOE_OK;

}

/**
 * @brief  Configure the IO expander 2.
 * @param  None
 * @retval IOE_OK if all initializations done correctly. Other value if error.
 */
uint8_t _IOE2_Config(void)
{
	  /* Configure the needed pins */
  IOE16_GPIO_Config(); 
  
  /* I2C initialization */
  IOE16_I2C_Config(); 
  
  /* Read IO Expander ID  */
  if(IOE16_IsOperational())
  {
    return IOE16_NOT_OPERATIONAL;
  }
  
  /* Generate IOExpander Software reset */
  IOE16_Reset();  
	
 
	/* Extit Joystick*/
	
	IOE16_EXTI_Config ();
  /* Configuration is OK */
  return IOE16_OK; 
}

/**
 * @brief  Configures The selected interrupts on the IO Expanders.
 * @param  IOE_ITSRC_Source: the source of the interrupts. Could be one or a
 *         combination of the following parameters:
 *   @arg  IOE_ITSRC_JOYSTICK: Joystick IO intputs.
 *   @arg  IOE_ITSRC_TSC: Touch Screen interrupts.
 *   @arg  IOE_ITSRC_INMEMS: MEMS interrupt lines.
 * @retval IOE_OK: if all initializations are OK. Other value if error.
 */
uint8_t _IOE_ITConfig(uint32_t IOE_ITSRC_Source)
{   
	uint8_t Dummy;

	/* Configure the Interrupt output pin to generate low level (INT_CTRL) */

	_IOE_ITOutConfig(Polarity_Low, Type_Edge); //phv19/02 Edge!!! 
 


	/* Manage the Touch Screen Interrupts */
	if (IOE_ITSRC_Source & IOE_ITSRC_TSC)
	{

		/* Enable the Global interrupt */
		_IOE_GITCmd(IOE_ADDR, ENABLE);

		/* Enable the Global GPIO Interrupt */
		_IOE_GITConfig(IOE_ADDR, (uint8_t)(IOE_GIT_TOUCH | IOE_GIT_FTH | IOE_GIT_FOV), ENABLE);
		/* Set the register */

		/* Read the GPIO_IT_STA to clear all pending bits if any */
		_I2C_READDEVICEREGISTER(IOE_ADDR, IOE_REG_GPIO_INT_STA,&Dummy);


	}


	/* If all OK return IOE_OK */
	return IOE_OK;
}


/**
 * @brief  Checks the selected Global interrupt source pending bit
 * @param  DeviceAddr: The address of the IOExpander, could be : IOE_ADDR
 *         or IOE_2_ADDR.
 * @param  Global_IT: the Global interrupt source to be checked, could be:
 *   @arg  Global_IT_GPIO : All IOs interrupt
 *   @arg  Global_IT_ADC : ADC interrupt
 *   @arg  Global_IT_TEMP : Temperature Sensor interrupts
 *   @arg  Global_IT_FE : Touch Screen Controller FIFO Error interrupt
 *   @arg  Global_IT_FF : Touch Screen Controller FIFO Full interrupt
 *   @arg  Global_IT_FOV : Touch Screen Controller FIFO Overrun interrupt
 *   @arg  Global_IT_FTH : Touch Screen Controller FIFO Threshold interrupt
 *   @arg  Global_IT_TOUCH : Touch Screen Controller Touch Detected interrupt
 * @retval Status of the checked flag. Could be SET or RESET.
 */
uint8_t _IOE_GetGITStatus(uint8_t DeviceAddr, uint8_t Global_IT)
{
	uint8_t tmp = 0;

	/* get the Interrupt status */
	_I2C_READDEVICEREGISTER(DeviceAddr, IOE_REG_INT_STA,&tmp);

	if ((tmp & (uint8_t)Global_IT) != 0)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}


/**
 * @brief  Checks the status of the selected IO interrupt pending bit
 * @param  DeviceAddr: The address of the IOExpander, could be : IOE_ADDR
 *         or IOE_2_ADDR.
 * @param  IO_IT: the IO interrupt to be checked could be IO_ITx Where x can be
 *         from 0 to 7.
 * @retval Status of the checked flag. Could be SET or RESET.
 */
uint8_t _IOE_GetIOITStatus(uint8_t DeviceAddr, uint8_t IO_IT)
{
	uint8_t tmp = 0;

	/* get the Interrupt status */
	_I2C_READDEVICEREGISTER(DeviceAddr, IOE_REG_GPIO_INT_STA,&tmp);

	if ((tmp & (uint8_t)IO_IT) != 0)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/**
 * @brief  Clears the selected IO interrupt pending bit(s).
 * @param  DeviceAddr: The address of the IOExpander, could be : IOE_ADDR
 * @param  IO_IT: the IO interrupt to be checked could be IO_ITx Where x can be
 *         from 0 to 7.
 * @retval IOE_OK: if all initializations are OK. Other value if error.
 */
uint8_t _IOE_ClearIOITPending(uint8_t DeviceAddr, uint8_t IO_IT)
{
	/* Write 1 to the bits that have to be cleared */
	_I2C_WRITEDEVICEREGISTER(DeviceAddr, IOE_REG_GPIO_INT_STA, IO_IT);

	/* If all OK return IOE_OK */
	return IOE_OK;
}

/**
 * @brief  Checks if the selected device is correctly configured and
 *         communicates correctly on the I2C bus.
 * @param  DeviceAddr: The address of the IOExpander, could be : IOE_ADDR
 *         or IOE_2_ADDR.
 * @retval IOE_OK if IOE is operational. Other value if failure.
 */
uint8_t _IOE_IsOperational(uint8_t DeviceAddr)
{
	uint16_t ID;

	/* Return Error if the ID is not correct */

	_I2C_READDATABUFFER(DeviceAddr, IOE_REG_CHP_ID,(uint8_t*)&ID,sizeof(ID)) ; /* ID is not Correct */


	if ( ID != _STMPE811_ID)
		return IOE_FAILURE; /* ID is not Correct */

	return IOE_OK; /* ID is correct */

}

/**
 * @brief  Resets the IO Expander by Software (SYS_CTRL1, RESET bit).
 * @param  DeviceAddr: The address of the IOExpander, could be : IOE_ADDR
 *         or IOE_2_ADDR.
 * @retval IOE_OK: if all initializations are OK. Other value if error.
 */
uint8_t _IOE_Reset(uint8_t DeviceAddr)
{
	/* Power Down the IO_Expander */
	_I2C_WRITEDEVICEREGISTER(DeviceAddr, IOE_REG_SYS_CTRL1, 0x02);

	/* wait for a delay to insure registers erasing */
	_delay_(2);

	/* Power On the Codec after the power off => all registers are reinitialized*/
	_I2C_WRITEDEVICEREGISTER(DeviceAddr, IOE_REG_SYS_CTRL1, 0x00);

	/* If all OK return IOE_OK */
	return IOE_OK;
}

/**
 * @brief  Configures the selected IO Expander functionalities.
 * @param  DeviceAddr: The address of the IOExpander, could be : IOE_ADDR
 *         or IOE_2_ADDR.
 * @param  IOE_TEMPSENS_FCT: the functions to be configured. could be any
 *         combination of the following values:
 *   @arg  IOE_IO_FCT : IO function
 *   @arg  IOE_TS_FCT : Touch Screen function
 *   @arg  IOE_ADC_FCT : ADC function
 *   @arg  IOE_TEMPSENS_FCT : Temperature Sensor function
 * @retval IOE_OK: if all initializations are OK. Other value if error.
 */
uint8_t _IOE_FnctCmd(uint8_t DeviceAddr, uint8_t Fct, FunctionalState NewState)
{
	uint8_t tmp = 0;

	/* Get the register value */
	_I2C_READDEVICEREGISTER(DeviceAddr, IOE_REG_SYS_CTRL2,&tmp);

	if (NewState != DISABLE)
	{
		/* Set the Functionalities to be Enabled */
		tmp &= ~(uint8_t)Fct;
	}
	else
	{
		/* Set the Functionalities to be Disabled */
		tmp |= (uint8_t)Fct;
	}

	/* Set the register value */
	_I2C_WRITEDEVICEREGISTER(DeviceAddr, IOE_REG_SYS_CTRL2, tmp);

	/* If all OK return IOE_OK */
	return IOE_OK;
}

/**
 * @brief  Configures the selected pin direction (to be an input or an output)
 * @param  DeviceAddr: The address of the IOExpander, could be : IOE_ADDR
 *         or IOE_2_ADDR.
 * @param  IO_Pin: IO_Pin_x: Where x can be from 0 to 7.
 * @param  Direction: could be Direction_IN or Direction_OUT.
 * @retval IOE_OK: if all initializations are OK. Other value if error.
 */
uint8_t _IOE_IOPinConfig(uint8_t DeviceAddr, uint8_t IO_Pin, uint8_t Direction)
{
	uint8_t tmp = 0;

	/* Get all the Pins direction */
	_I2C_READDEVICEREGISTER(DeviceAddr, IOE_REG_GPIO_DIR,&tmp);

	if (Direction != Direction_IN)
	{
		tmp |= (uint8_t)IO_Pin;
	}
	else
	{
		tmp &= ~(uint8_t)IO_Pin;
	}

	/* Write the register new value */
	_I2C_WRITEDEVICEREGISTER(DeviceAddr, IOE_REG_GPIO_DIR, tmp);

	/* If all OK return IOE_OK */
	return IOE_OK;
}

/**
 * @brief  Enables or disables the Global interrupt.
 * @param  DeviceAddr: The address of the IOExpander, could be :I OE_1_ADDR
 *         or IOE_2_ADDR.
 * @param  NewState: could be ENABLE or DISABLE.
 * @retval IOE_OK: if all initializations are OK. Other value if error.
 */
uint8_t _IOE_GITCmd(uint8_t DeviceAddr, FunctionalState NewState)
{
	uint8_t tmp = 0;

	/* Read the Interrupt Control register  */
	_I2C_READDEVICEREGISTER(DeviceAddr, IOE_REG_INT_CTRL,&tmp);

	if (NewState != DISABLE)
	{
		/* Set the global interrupts to be Enabled */
		tmp |= (uint8_t)IOE_GIT_EN;
	}
	else
	{
		/* Set the global interrupts to be Disabled */
		tmp &= ~(uint8_t)IOE_GIT_EN;
	}

	/* Write Back the Interrupt Control register */
	_I2C_WRITEDEVICEREGISTER(DeviceAddr, IOE_REG_INT_CTRL, tmp);

	/* If all OK return IOE_OK */
	return IOE_OK;
}

/**
 * @brief  Configures the selected source to generate or not a global interrupt
 * @param DeviceAddr: The address of the IOExpander, could be : IOE_ADDR
 *        or IOE_2_ADDR.
 * @param Global_IT: the interrupt source to be configured, could be:
 *   @arg  Global_IT_GPIO : All IOs interrupt
 *   @arg  Global_IT_ADC : ADC interrupt
 *   @arg  Global_IT_TEMP : Temperature Sensor interrupts
 *   @arg  Global_IT_FE : Touch Screen Controller FIFO Error interrupt
 *   @arg  Global_IT_FF : Touch Screen Controller FIFO Full interrupt
 *   @arg  Global_IT_FOV : Touch Screen Controller FIFO Overrun interrupt
 *   @arg  Global_IT_FTH : Touch Screen Controller FIFO Threshold interrupt
 *   @arg  Global_IT_TOUCH : Touch Screen Controller Touch Detected interrupt
 * @retval IOE_OK: if all initializations are OK. Other value if error.
 */
uint8_t _IOE_GITConfig(uint8_t DeviceAddr, uint8_t Global_IT, FunctionalState NewState)
{
	uint8_t tmp = 0;

	/* Get the current value of the INT_EN register */
	_I2C_READDEVICEREGISTER(DeviceAddr, IOE_REG_INT_EN,&tmp);

	if (NewState != DISABLE)
	{
		/* Set the interrupts to be Enabled */
		tmp |= Global_IT;
	}
	else
	{
		/* Set the interrupts to be Disabled */
		tmp &= ~ Global_IT;
	}
	/* Set the register */
	_I2C_WRITEDEVICEREGISTER(DeviceAddr, IOE_REG_INT_EN, tmp);

	/* If all OK return IOE_OK */
	return IOE_OK;
}

/**
 * @brief  Configures the selected pins to generate an interrupt or not.
 * @param  DeviceAddr: The address of the IOExpander, could be : IOE_ADDR
 *         or IOE_2_ADDR.
 * @param  IO_IT: The IO interrupt to be configured. This parameter could be any
 *         combination of the following values:
 *   @arg  IO_IT_x: where x can be from 0 to 7.
 * @param  NewState: could be ENABLE or DISABLE.
 * @retval IOE_OK: if all initializations are OK. Other value if error.
 */
uint8_t _IOE_IOITConfig(uint8_t DeviceAddr, uint8_t IO_IT, FunctionalState NewState)
{
	uint8_t tmp = 0;

	_I2C_READDEVICEREGISTER(DeviceAddr, IOE_REG_GPIO_INT_EN,&tmp);

	if (NewState != DISABLE)
	{
		/* Set the interrupts to be Enabled */
		tmp |=  IO_IT;
	}
	else
	{
		/* Set the interrupts to be Disabled */
		tmp &= ~IO_IT;
	}

	/* Set the register */
	_I2C_WRITEDEVICEREGISTER(DeviceAddr, IOE_REG_GPIO_INT_EN, tmp);

	/* If all OK return IOE_OK */
	return IOE_OK;
}

/**
 * @brief  Configures the touch Screen Controller (Single point detection)
 * @param  None
 * @retval IOE_OK if all initializations are OK. Other value if error.
 */
uint8_t _IOE_TS_Config(void)
{
	uint8_t tmp = 0;

	/* Select Sample Time, bit number and ADC Reference */
	_I2C_WRITEDEVICEREGISTER(IOE_ADDR, IOE_REG_ADC_CTRL1, 0x49);

	/* Wait for ~20 ms */
	_delay_(2);

	/* Select the ADC clock speed: 3.25 MHz */
	_I2C_WRITEDEVICEREGISTER(IOE_ADDR, IOE_REG_ADC_CTRL2, 0x01);

	/* Select TSC pins in non default mode */
	_I2C_READDEVICEREGISTER(IOE_ADDR, IOE_REG_GPIO_AF,&tmp);
	tmp &= ~(uint8_t)TOUCH_IO_ALL;
	_I2C_WRITEDEVICEREGISTER(IOE_ADDR, IOE_REG_GPIO_AF, tmp);

	/* Select 2 nF filter capacitor */
	_I2C_WRITEDEVICEREGISTER(IOE_ADDR, IOE_REG_TSC_CFG, 0x9A);

	/* Select single point reading  */
	_I2C_WRITEDEVICEREGISTER(IOE_ADDR, IOE_REG_FIFO_TH, 0x0a);  //10 Samples before IT --> to reduce IT

	/* Write 0x01 to clear the FIFO memory content. */
	_I2C_WRITEDEVICEREGISTER(IOE_ADDR, IOE_REG_FIFO_STA, 0x01);

	/* Write 0x00 to put the FIFO back into operation mode  */
	_I2C_WRITEDEVICEREGISTER(IOE_ADDR, IOE_REG_FIFO_STA, 0x00);

	/* set the data format for Z value: 7 fractional part and 1 whole part */
	_I2C_WRITEDEVICEREGISTER(IOE_ADDR, IOE_REG_TSC_FRACT_XYZ, 0x01);

	/* set the driving capability of the device for TSC pins: 50mA */
	_I2C_WRITEDEVICEREGISTER(IOE_ADDR, IOE_REG_TSC_I_DRIVE, 0x01);

	/* Use no tracking index, touchscreen controller operation mode (XYZ) and
     enable the TSC */
	_I2C_WRITEDEVICEREGISTER(IOE_ADDR, IOE_REG_TSC_CTRL, 0x03); // 1 -->3 for read only XY

	/*  Clear all the status pending bits */
	_I2C_WRITEDEVICEREGISTER(IOE_ADDR, IOE_REG_INT_STA, 0xFF);

	/* Clear Touch screen FIFO */
	CLEARFIFO;

	/* All configuration done */
	return IOE_OK;
}


/**
 * @brief  Configures the selected pin to be in Alternate function or not
 * @param  DeviceAddr: The address of the IOExpander, could be : IOE_ADDR
 *         or IOE_2_ADDR.
 * @param  IO_Pin: IO_Pin_x, Where x can be from 0 to 7.
 * @param  NewState: State of the AF for the selected pin, could be
 *         ENABLE or DISABLE.
 * @retval IOE_OK: if all initializations are OK. Other value if error.
 */
uint8_t _IOE_IOAFConfig(uint8_t DeviceAddr, uint8_t IO_Pin, FunctionalState NewState)
{
	uint8_t tmp ;

	/* Get the current state of the GPIO_AF register */
	_I2C_READDEVICEREGISTER(DeviceAddr, IOE_REG_GPIO_AF,&tmp);

	if (NewState != DISABLE)
	{
		/* Enable the selected pins alternate function */
		tmp |= (uint8_t)IO_Pin;
	}
	else
	{
		/* Disable the selected pins alternate function */
		tmp &= ~(uint8_t)IO_Pin;
	}

	/* Write back the new value in GPIO_AF register */
	_I2C_WRITEDEVICEREGISTER(DeviceAddr, IOE_REG_GPIO_AF, tmp);

	/* If all OK return IOE_OK */
	return IOE_OK;
}

/**
 * @brief  Configures the Edge for which a transition is detectable for the
 *         the selected pin.
 * @param  DeviceAddr: The address of the IOExpander, could be : IOE_ADDR
 *         or IOE_2_ADDR.
 * @param  IO_Pin: IO_Pin_x, Where x can be from 0 to 7.
 * @param  Edge: The edge which will be detected. This parameter can be one or a
 *         a combination of following values: EDGE_FALLING and EDGE_RISING .
 * @retval IOE_OK: if all initializations are OK. Other value if error.
 */
uint8_t _IOE_IOEdgeConfig(uint8_t DeviceAddr, uint8_t IO_Pin, uint8_t Edge)
{
	uint8_t tmp1, tmp2 ;

	/* Get the registers values */
	_I2C_READDEVICEREGISTER(DeviceAddr, IOE_REG_GPIO_FE,&tmp1);
	_I2C_READDEVICEREGISTER(DeviceAddr, IOE_REG_GPIO_RE,&tmp2);

	/* Disable the Falling Edge */
	tmp1 &= ~(uint8_t)IO_Pin;

	/* Disable the Falling Edge */
	tmp2 &= ~(uint8_t)IO_Pin;

	/* Enable the Falling edge if selected */
	if (Edge & EDGE_FALLING)
	{
		tmp1 |= (uint8_t)IO_Pin;
	}

	/* Enable the Rising edge if selected */
	if (Edge & EDGE_RISING)
	{
		tmp2 |= (uint8_t)IO_Pin;
	}

	/* Write back the registers values */
	_I2C_WRITEDEVICEREGISTER(DeviceAddr, IOE_REG_GPIO_FE, tmp1);
	_I2C_WRITEDEVICEREGISTER(DeviceAddr, IOE_REG_GPIO_RE, tmp2);

	/* if OK return 0 */
	return IOE_OK;
}

/**
 * @brief  Configures the Interrupt line active state and format (level/edge)
 * @param  Polarity: could be
 *   @arg  Polarity_Low: Interrupt line is active Low/Falling edge
 *   @arg  Polarity_High: Interrupt line is active High/Rising edge
 * @param  Type: Interrupt line activity type, could be one of the following values
 *   @arg  Type_Level: Interrupt line is active in level model
 *   @arg  Type_Edge: Interrupt line is active in edge model
 * @retval IOE_OK: if all initializations are OK. Other value if error.
 */
uint8_t _IOE_ITOutConfig(uint8_t Polarity, uint8_t Type)
{
	uint8_t tmp ;

	/*  Get the register IOE_REG_INT_CTRL value */
	_I2C_READDEVICEREGISTER(IOE_ADDR, IOE_REG_INT_CTRL,&tmp);

	/* Mask the polarity and type bits */
	tmp &= ~(uint8_t)0x06;

	/* Modify the Interrupt Output line configuration */
	tmp |= (uint8_t)(Polarity | Type);

	/* Set the register */
	_I2C_WRITEDEVICEREGISTER(IOE_ADDR, IOE_REG_INT_CTRL, tmp);

	/* If all OK return IOE_OK */
	return IOE_OK;
}

/**
 * @brief  Writes a value in a register of the device through I2C.
 * @param  DeviceAddr: The address of the IOExpander, could be : IOE_ADDR
 *         or IOE_2_ADDR.
 * @param  RegisterAddr: The target register address
 * @param  RegisterValue: The target register value to be written
 * @retval IOE_OK: if all operations are OK. Other value if error.
 */
uint8_t _I2C_WriteDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t RegisterValue)
{
	uint32_t IOE_TimeOut;

	  /* Begin the configuration sequence */
  I2C_GenerateSTART(IOE_I2C, ENABLE);
	
  /* Test on EV5 and clear it */
  IOE_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_SB))
  {
    if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
  }

  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(IOE_I2C, IOE_ADDR, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  IOE_TimeOut = TIMEOUT_MAX;  
  while (!I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_ADDR))
  {
    if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
  }
  
  /* Read status register 2 to clear ADDR flag */
  IOE_I2C->SR2;
  
  /* Test on EV8_1 and clear it */
  IOE_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_TXE))
  {
    if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
  }
  
  /* Transmit the first address for r/w operations */
  I2C_SendData(IOE_I2C, RegisterAddr);
  
  /* Test on EV8 and clear it */
  IOE_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_TXE))
  {
    if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
  }
  
  /* Prepare the register value to be sent */
  I2C_SendData(IOE_I2C, RegisterValue);
  
  /* Test on EV8_2 and clear it */
  IOE_TimeOut = TIMEOUT_MAX;
  while ((!I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_TXE)) || (!I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_BTF)))
  {
    if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
  }
  
  /* End the configuration sequence */
  I2C_GenerateSTOP(IOE_I2C, ENABLE);


	/* Return the verifying value: 0 (Passed) */
	return IOE_OK;
}

/**
 * @brief  Writes a value in a register of the device through I2C.
 * @param  DeviceAddr: The address of the IOExpander, could be : IOE_ADDR
 *         or IOE_2_ADDR.
 * @param  RegisterAddr: The target register address
 * @param  RegisterValue: The target register value to be written
 * @retval IOE_OK: if all operations are OK. Other value if error.
 */
uint8_t _I2C_WriteDeviceRegisterold(uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t RegisterValue)
{
	uint32_t IOE_TimeOut;

	/* Enable the I2C peripheral */
	I2C_GenerateSTART(IOE_I2C, ENABLE);

	/* Test on SB Flag */
	IOE_TimeOut = TIMEOUT_MAX;
	while (I2C_GetFlagStatus(IOE_I2C,I2C_FLAG_SB) == RESET)
	{
		if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
	}

	/* Transmit the slave address and enable writing operation */
	I2C_Send7bitAddress(IOE_I2C, DeviceAddr, I2C_Direction_Transmitter);

	/* Test on ADDR Flag */
	IOE_TimeOut = TIMEOUT_MAX;
	while (!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
	}

	/* Transmit the first address for r/w operations */
	I2C_SendData(IOE_I2C, RegisterAddr);

	/* Test on TXE FLag (data dent) */
	IOE_TimeOut = TIMEOUT_MAX;
	while ((!I2C_GetFlagStatus(IOE_I2C,I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(IOE_I2C,I2C_FLAG_BTF)))
	{
		if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
	}

	/* Transmit the first address for r/w operations */
	I2C_SendData(IOE_I2C, RegisterValue );

	/* Test on TXE FLag (data dent) */
	IOE_TimeOut = TIMEOUT_MAX;
	while ((!I2C_GetFlagStatus(IOE_I2C,I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(IOE_I2C,I2C_FLAG_BTF)))
	{
		if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
	}

	/* Send STOP Condition */
	I2C_GenerateSTOP(IOE_I2C, ENABLE);

	/* Return the verifying value: 0 (Passed) */
	return IOE_OK;
}


/**
 * @brief  Reads a register of the device through I2C without DMA.
 * @param  DeviceAddr: The address of the device, could be : IOE_ADDR
 *         or IOE_2_ADDR.
 * @param  RegisterAddr: The target register address (between 00x and 0x24)
 * @retval IOE_OK: if all operations are OK. Other value if error.
 */
uint8_t _I2C_ReadDeviceRegister(uint8_t DeviceAddr, uint8_t RegisterAddr,uint8_t *IOE_BufferRX )
{
	uint32_t IOE_TimeOut;

  
  /* Enable the I2C peripheral */
  I2C_GenerateSTART(IOE_I2C, ENABLE);
  
    /* Test on EV5 and clear it */
  IOE_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_SB))
  {
    if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
  }
  /* Disable Acknowledgement */
  I2C_AcknowledgeConfig(IOE_I2C, DISABLE);
  
  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(IOE_I2C, IOE_ADDR, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  IOE_TimeOut = TIMEOUT_MAX;  
  while (!I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_ADDR))
  {
    if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
  }
  
  /* Read status register 2 to clear ADDR flag */
  IOE_I2C->SR2;
  
  /* Test on EV8 and clear it */
  IOE_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_TXE))
  {
    if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
  }
  
  /* Transmit the first address for r/w operations */
  I2C_SendData(IOE_I2C, RegisterAddr);
  
  /* Test on EV8 and clear it */
  IOE_TimeOut = TIMEOUT_MAX;
  while ((!I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_TXE)) || (!I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_BTF)))
  {
    if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
  }
  
  /* Regenerate a start condition */
  I2C_GenerateSTART(IOE_I2C, ENABLE);
  
  /* Test on EV5 and clear it */
  IOE_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_SB))
  {
    if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
  }
  
  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(IOE_I2C, IOE_ADDR, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  IOE_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_ADDR))
  {
    if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
  }
  
    /* Read status register 2 to clear ADDR flag */
  IOE_I2C->SR2;
  
  /* Test on EV7 and clear it */
  IOE_TimeOut = TIMEOUT_MAX;
  while (!I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_RXNE))
  {
    if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
  }
  
  /* End the configuration sequence */
  I2C_GenerateSTOP(IOE_I2C, ENABLE);

	/*!< Read the byte received from IC2 */
	*IOE_BufferRX  = IOE_I2C->DR;

	/*!< Re-Enable Acknowledgment to be ready for another reception */
	I2C_AcknowledgeConfig(IOE_I2C, ENABLE);


	/* return OK */
	return IOE_OK; ;

}

/**
 * @brief  Reads a register of the device through I2C without DMA.
 * @param  DeviceAddr: The address of the device, could be : IOE_ADDR
 *         or IOE_2_ADDR.
 * @param  RegisterAddr: The target register address (between 00x and 0x24)
 * @retval IOE_OK: if all operations are OK. Other value if error.
 */
uint8_t _I2C_ReadDeviceRegisterold(uint8_t DeviceAddr, uint8_t RegisterAddr,uint8_t *IOE_BufferRX )
{
	uint32_t IOE_TimeOut;

	/*!< While the bus is busy */
	IOE_TimeOut = TIMEOUT_MAX;
	while(I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_BUSY))
	{
		if((IOE_TimeOut--) == 0) return(IOE_TimeoutUserCallback());
	}
	/* Enable the I2C peripheral */
	I2C_GenerateSTART(IOE_I2C, ENABLE);

	/* Test on SB Flag */
	IOE_TimeOut = TIMEOUT_MAX;
	while (!I2C_GetFlagStatus(IOE_I2C,I2C_FLAG_SB))
	{
		if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
	}

	/* Send device address for write */
	I2C_Send7bitAddress(IOE_I2C, DeviceAddr, I2C_Direction_Transmitter);

	/* Test on ADDR Flag */
	IOE_TimeOut = TIMEOUT_MAX;
	while (!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
	}

	/* Send the device's internal address to write to */
	I2C_SendData(IOE_I2C, RegisterAddr);

	/* Test on TXE FLag (data dent) */
	IOE_TimeOut = TIMEOUT_MAX;
	while ((!I2C_GetFlagStatus(IOE_I2C,I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(IOE_I2C,I2C_FLAG_BTF)))
	{
		if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
	}

	/* Send START condition a second time */
	I2C_GenerateSTART(IOE_I2C, ENABLE);

	/* Test on SB Flag */
	IOE_TimeOut = TIMEOUT_MAX;
	while (!I2C_GetFlagStatus(IOE_I2C,I2C_FLAG_SB))
	{
		if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
	}

	/* Send IOExpander address for read */
	I2C_Send7bitAddress(IOE_I2C, DeviceAddr, I2C_Direction_Receiver);

	/* Test on ADDR Flag */
	IOE_TimeOut = TIMEOUT_MAX;
	while (!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
	}

	/*!< Disable Acknowledgment */
	I2C_AcknowledgeConfig(IOE_I2C, DISABLE);

	/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
	(void)IOE_I2C->SR2;

	/*!< Send STOP Condition */
	I2C_GenerateSTOP(IOE_I2C, ENABLE);

	/* Wait for the byte to be received */
	IOE_TimeOut = TIMEOUT_MAX;
	while(I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_RXNE ) == RESET)
	{
		if((IOE_TimeOut--) == 0) return(IOE_TimeoutUserCallback());
	}

	/*!< Read the byte received from IC2 */
	*IOE_BufferRX  = IOE_I2C->DR;

	/* Wait to make sure that STOP control bit has been cleared */
	IOE_TimeOut = TIMEOUT_MAX;
	while(IOE_I2C->CR1 & I2C_CR1_STOP)
	{
		if((IOE_TimeOut--) == 0) return(IOE_TimeoutUserCallback());
	}

	/*!< Re-Enable Acknowledgment to be ready for another reception */
	I2C_AcknowledgeConfig(IOE_I2C, ENABLE);


	/* return OK */
	return IOE_OK; ;

}

/**
 * @brief  Reads a register of the device through I2C without dma.
 * @param  DeviceAddr: The address of the device, could be : IOE_ADDR
 *         or IOE_2_ADDR.
 * @param  RegisterAddr: The target register address (between 00x and 0x24)
 * @retval IOE_OK: if all operations are OK. Other value if error.
 */

uint8_t _I2C_ReadDataBuffer(uint8_t DeviceAddr, uint8_t RegisterAddr,uint8_t *IOE_BufferRX,uint16_t nb_car)
{
	uint16_t i ;
	uint32_t IOE_TimeOut;


	/*!< While the bus is busy */
	IOE_TimeOut = TIMEOUT_MAX;
	while(I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_BUSY))
	{
		if((IOE_TimeOut--) == 0) return(IOE_TimeoutUserCallback());
	}

	/* Enable the I2C peripheral */
	I2C_GenerateSTART(IOE_I2C, ENABLE);

	/* Test on SB Flag */
	IOE_TimeOut = TIMEOUT_MAX;
	while (!I2C_GetFlagStatus(IOE_I2C,I2C_FLAG_SB))
	{
		if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
	}

	/* Send device address for write */
	I2C_Send7bitAddress(IOE_I2C, DeviceAddr, I2C_Direction_Transmitter);

	/* Test on ADDR Flag */
	IOE_TimeOut = TIMEOUT_MAX;
	while (!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
	}

	/* Send the device's internal address to write to */
	I2C_SendData(IOE_I2C, RegisterAddr);

	/* Test on TXE FLag (data dent) */
	IOE_TimeOut = TIMEOUT_MAX;
	while ((!I2C_GetFlagStatus(IOE_I2C,I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(IOE_I2C,I2C_FLAG_BTF)))
	{
		if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
	}

	/* Send START condition a second time */
	I2C_GenerateSTART(IOE_I2C, ENABLE);

	/* Test on SB Flag */
	IOE_TimeOut = TIMEOUT_MAX;
	while (!I2C_GetFlagStatus(IOE_I2C,I2C_FLAG_SB))
	{
		if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
	}

	/* Send IOExpander address for read */
	I2C_Send7bitAddress(IOE_I2C, DeviceAddr, I2C_Direction_Receiver);

	/* Test on ADDR Flag */
	IOE_TimeOut = TIMEOUT_MAX;
	while (!I2C_CheckEvent(IOE_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if (IOE_TimeOut-- == 0) return(IOE_TimeoutUserCallback());
	}

	/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
	(void)IOE_I2C->SR2;

	i = 0;
	while (nb_car > 1 )
	{
		/* Wait for the byte to be received */
		IOE_TimeOut = TIMEOUT_MAX ;
		while(I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_RXNE) == RESET)
		{
			if((IOE_TimeOut--) == 0) {
				return(IOE_TimeoutUserCallback());
			}
		}

		/*!< Read the byte received from the I2C */
		IOE_BufferRX [i++] = IOE_I2C->DR;
		nb_car--;
	}

	/*!< Disable Acknowledgment to be ready for another reception */
	I2C_AcknowledgeConfig(IOE_I2C, DISABLE);

	/*!< Send STOP Condition */
	I2C_GenerateSTOP(IOE_I2C, ENABLE);

	/* Wait to make sure that STOP control bit has been cleared */
	IOE_TimeOut = TIMEOUT_MAX;
	while(IOE_I2C->CR1 & I2C_CR1_STOP)
	{
		if((IOE_TimeOut--) == 0) return(IOE_TimeoutUserCallback());
	}

	/* Wait for the byte to be received */
	IOE_TimeOut = TIMEOUT_MAX ;
	while(I2C_GetFlagStatus(IOE_I2C, I2C_FLAG_RXNE) == RESET)
	{
		if((IOE_TimeOut--) == 0) return(IOE_TimeoutUserCallback());
	}

	/*!< Read the last byte received from the I2C */
	IOE_BufferRX [i] = IOE_I2C->DR;

	/*!< Re-Enable Acknowledgment to be ready for another reception */
	I2C_AcknowledgeConfig(IOE_I2C, ENABLE);

	/* return a pointer to the IOE_Buffer */
	return IOE_OK;;

}

/**
 * @brief  Configures the IO expander Interrupt line and GPIO in EXTI mode.
 * @param  None
 * @retval None
 */
static void _IOE_EXTI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(IOE_IT_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PI2 IOexpander as input floating */
	GPIO_InitStructure.GPIO_Pin = IOE_IT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(IOE_IT_GPIO_PORT, &GPIO_InitStructure);

	/* Connect Button EXTI Line to Button GPIO Pin */
	SYSCFG_EXTILineConfig(IOE_IT_EXTI_PORT_SOURCE, IOE_IT_EXTI_PIN_SOURCE);

	/* Configure Button EXTI line */
	EXTI_InitStructure.EXTI_Line = IOE_IT_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling ;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set Button EXTI Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = IOE_IT_EXTI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


uint8_t _IOExpander_init(void)
{
	if (IOE_initialized == MICROEJ_TRUE)
	{
		// ioexpander already initialized
		return MICROEJ_TRUE;	// no error
	}

	IOE_Config(); // TODO: check if we need this
	IOE_TSITConfig();
	//IOE_ITConfig(IOE_ITSRC_TSC );
	
	#define IOE_USE_TASK // now use IO task	// TODO remove forced definition
	#ifdef IOE_USE_TASK
	IOE_TASK_create_task();
	#endif
	
	/* Configure the Interrupt line as EXTI source for IOE1 and IOE2 it's the same IT line */
	_IOE_EXTI_Config();

	IOE_initialized = MICROEJ_TRUE;
	return MICROEJ_TRUE;	// no error
}

uint8_t IOExpander_checkState(IOE_Status_TypDef state)
{
	if (state == IOE_FAILURE)
	{
		//pb in IO expander
		//--->reinit
		printf("IOE failure, reinit IOE...\n"); 
		IOE_initialized = MICROEJ_FALSE ;
		_IOExpander_init(); // TODO: check if everything in this is required
		TsInit();
		return MICROEJ_FALSE;
	}

	return MICROEJ_TRUE;	// no error
}

void IOExpander_interrupt(void)
{
	//printf("IOExpander_interrupt\n");
	// check touch events
	IOExpander_checkState(TOUCH_interrupt());
/* // TODO remove this code if no joystick or other I2C comm
	// check joystick events
	if (IOExpander_checkState(JOYSTICK_interrupt()) == MICROEJ_TRUE)
	{
		// force to clean the touch screen interrupt to prevent I2C error
		CLEARFIFO ;
	}
		// clear interrupt status - INT_STA_LOW(0x0A), Io_Expander's INT pin output also cleared
	IOE16_I2C_ReadDeviceRegister(0x0A );	 

	// Read IOs state to let IO interrupt occur 
  IOE16_I2C_ReadDeviceRegister(IOE16_REG_GPMR_LSB);
  IOE16_I2C_ReadDeviceRegister(IOE16_REG_GPMR_MSB);
	
	//phv 8/3/13 ToBecheckEXTI_ClearITPendingBit(IOE16_IT_EXTI_LINE);
*/	
}
