/*
 * sensors.c
 *
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */


#include "cpu.h"
#if MCU == 2
#include "stm32f2xx.h"
#include "stm32f2xx_gpio.h"
#include "stm32f2xx_rcc.h"
#include "stm32f2xx_adc.h"
#elif MCU == 4
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"
#elif MCU == 42
#include "stm32f4xx.h"

#else
#error Invalid MCU
#endif
#include "microej.h"
#include "ioe.h"

// --------------------------------------------------------------------------------
// -                                  Constants                                   -
// --------------------------------------------------------------------------------

#define CONV_NOK 0xffffffff
#define CONV_OK 0

#define NOK 0xffffffff
#define OK 0

// --------------------------------------------------------------------------------
// -                              Potentiometer                                   -
// --------------------------------------------------------------------------------

/* You can choose between 8 or 12bits conversions */
#define ADC_RESOLUTION_HI 1

#ifdef ADC_RESOLUTION_HI
#define RESOLUTION ADC_Resolution_12b
#define MAX_VALUE 0xfff
#else
#define RESOLUTION ADC_Resolution_8b
#define MAX_VALUE 0xff
#endif

/**
 * @brief  To initialize the Output and ADC for Potentiometer
 * The potentiometer is connected to PF9
 * @param  None
 * @retval None
 */
void potentiometer_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	/* GPIOF and ADC CLOCK */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	/* Configure potentiometer pin as input ANALOG*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;

	GPIO_Init(GPIOF, &GPIO_InitStructure);

	ADC_InitStructure.ADC_Resolution = RESOLUTION;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_Ext_IT11;// not actually used
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC3, &ADC_InitStructure);

	ADC_Cmd(ADC3, DISABLE);

	ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 1, ADC_SampleTime_56Cycles);

	ADC_Cmd(ADC3, ENABLE);

}

/**
 * @brief  To return ADC converter value Potentiometer on PF9 ADC3 channel 1
 * @param  None
 * @retval Potentiometer value (8 or 12 bits) or -1 if error
 */

uint32_t get_potentiometer_value(void)
{
	uint8_t i;
	uint32_t timeout ;

	uint32_t value = 0;

	/* 4 acquisitions */
	for ( i = 0 ; i < 4; i++)
	{
		ADC_SoftwareStartConv(ADC3);
		// wait until end-of-conversion
		timeout = 0xffff;
		while( ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == 0 )
		{
			if( timeout-- == 0 ) return CONV_NOK ;
		}

		// read ADC conversion result
		value += ADC_GetConversionValue(ADC3);

	}

	return value >> 2;
}

/**
 * @brief  To return he minimum value from potentiometer
 * @param  None
 * @retval 0
 */
uint32_t get_potentiometer_min_value(void)
{
	return 0;
}

/**
 * @brief  To initialize the Mems
 * @param  None
 * @retval max value
 */
uint32_t get_potentiometer_max_value(void)
{
	return MAX_VALUE;
}

// --------------------------------------------------------------------------------
// -                                     MEMS                                     -
// --------------------------------------------------------------------------------

// 7-bit address(left aligned) of MEMS: LIS302DL
#define I2C_MEMS_ADDR  0x38
#define LIS302DL_ID   0x3B
#define MEMS_ID    0x0F

// registers
#define CTRL_REG1  0x20
#define CTRL_REG2  0x21
#define CTRL_REG3  0x22
#define CTRL_REG4  0x23
#define STATUS_REG 0x27
#define OUT_X      0x29
#define OUT_Y      0x2B
#define OUT_Z      0x2D

// free falling/wake up regs
#define FF_WU_CFG_1      0x30
#define FF_WU_SRC_1      0x31
#define FF_WU_THS_1      0x32
#define FF_WU_DURATION_1 0x33
#define FF_WU_CFG_2      0x34
#define FF_WU_SRC_2      0x35
#define FF_WU_THS_2      0x36
#define FF_WU_DURATION_2 0x37

/**
 * @brief  To initialize Mems
 * @param  None
 * @retval OK or NoK if ID not correct
 */
uint32_t mems_init(void)
{
	uint8_t data;
	_I2C_READDEVICEREGISTER(I2C_MEMS_ADDR, MEMS_ID, &data);

	if( data != LIS302DL_ID )
		return NOK;

	// Configure CTRL_REG2 (reload timing values from internal flash memory): 0x40
	_I2C_WRITEDEVICEREGISTER(I2C_MEMS_ADDR, CTRL_REG2,0x40);

	// Configure CTRL_REG1: 0x07 - power down, 2g, X/Y/Z axis enabled
	_I2C_WRITEDEVICEREGISTER(I2C_MEMS_ADDR, CTRL_REG1,0x07);

	// Configure CTRL_REG2: 0x00
	_I2C_WRITEDEVICEREGISTER(I2C_MEMS_ADDR, CTRL_REG2, 0x00);

	// Configure CTRL_REG3: 0x80 - INT active low,PP, INT2: FF_WU_1, INT1: Click
	_I2C_WRITEDEVICEREGISTER(I2C_MEMS_ADDR, CTRL_REG3, 0x80|(1<<3)|7);

	// Configure FF_WU_CFG_1: 0x0A - enable X/Y high,disable Z interrupts
	_I2C_WRITEDEVICEREGISTER(I2C_MEMS_ADDR, FF_WU_CFG_1, 0x0A);

	// Configure FF_WU_CFG_2: 0x00 - disable X/Y/Z interrupts
	_I2C_WRITEDEVICEREGISTER(I2C_MEMS_ADDR, FF_WU_CFG_2, 0x00);

	// Read FF_WU_SRC_1 to clear it
	_I2C_READDEVICEREGISTER(I2C_MEMS_ADDR, FF_WU_SRC_1, &data);

	// Configure FF_WU_THS_1: 0x2D - counter reseted when interrupt inactive,threshold is 45
	_I2C_WRITEDEVICEREGISTER(I2C_MEMS_ADDR, FF_WU_THS_1, 0x2D);

	// Configure FF_WU_DURATION_1: 10 - 0.1s duration
	_I2C_WRITEDEVICEREGISTER(I2C_MEMS_ADDR, FF_WU_DURATION_1, 10);

	// Configure CTRL_REG1: 0x47 - 100Hz data rate,make MEMS active, (2g, X/Y/Z axis enabled)
	_I2C_WRITEDEVICEREGISTER(I2C_MEMS_ADDR, CTRL_REG1, 0x47);

	return OK;
}

/**
 * @brief  To return a value from Mems
 * @param  mask the status register mask
 * @param  registerAddr the register address
 * @retval Current mems value
 */
int32_t get_mems_value(uint8_t mask, uint32_t registerAddr)
{
	uint8_t  value;

	// read status to check whether xyz data is available
	_I2C_READDEVICEREGISTER(I2C_MEMS_ADDR, STATUS_REG, &value);

	/*check if data ready */
	if ((value & mask))
	{
		_I2C_READDEVICEREGISTER(I2C_MEMS_ADDR, registerAddr, &value);
		return (int8_t)value;	// sign extend
	}

	return CONV_NOK ;
}

/**
 * @brief  To return the X value from Mems
 * @param  None
 * @retval Current X value
 */
int32_t get_mems_value_X()
{
	return get_mems_value(0x01, OUT_X);
}
/**
 * @brief  To return the Y value from Mems
 * @param  None
 * @retval Current Y value
 */
int32_t get_mems_value_Y()
{
	return get_mems_value(0x02, OUT_Y);
}
/**
 * @brief  To return the Z value from Mems
 * @param  None
 * @retval Current Z value
 */
int32_t get_mems_value_Z()
{
	return get_mems_value(0x04, OUT_Z);
}

/**
 * @brief  To return the X min MEMs value
 * @param  None
 * @retval None
 */
int32_t get_mems_min_value_X(void)
{
	return -75;	// see LIS302DL datasheet
}

/**
 * @brief  To return the Y min MEMs value
 * @param  None
 * @retval None
 */
int32_t get_mems_min_value_Y(void)
{
	return -75;	// see LIS302DL datasheet
}

/**
 * @brief  To return the Z min MEMs value
 * @param  None
 * @retval None
 */
int32_t get_mems_min_value_Z(void)
{
	return -100;	// see LIS302DL datasheet
}

/**
 * @brief  To return the X max MEMs value
 * @param  None
 * @retval None
 */
int32_t get_mems_max_value_X(void)
{
	return 75;	// see LIS302DL datasheet
}

/**
 * @brief  To return the Y max MEMs value
 * @param  None
 * @retval None
 */
int32_t get_mems_max_value_Y(void)
{
	return 75;	// see LIS302DL datasheet
}

/**
 * @brief  To return the Z max MEMs value
 * @param  None
 * @retval None
 */
int32_t get_mems_max_value_Z(void)
{
	return 100;	// see LIS302DL datasheet
}

// --------------------------------------------------------------------------------
// -                com.is2t.demos.support.Sensor native methods                  -
// --------------------------------------------------------------------------------

#define POTENTIOMETER_ID 0
#define ACCELEROMETER_X_AXIS_ID 1
#define ACCELEROMETER_Y_AXIS_ID 2
#define ACCELEROMETER_Z_AXIS_ID 3

void Java_com_is2t_demos_support_Sensor_hardwareInitialize(void)
{
	potentiometer_init();
	mems_init();
}

int Java_com_is2t_demos_support_Sensor_getHardwareValue(int sensorID)
{
	switch (sensorID) {
		case POTENTIOMETER_ID:	return get_potentiometer_value();
		case ACCELEROMETER_X_AXIS_ID:	return get_mems_value_X();
		case ACCELEROMETER_Y_AXIS_ID:	return -get_mems_value_Y();	// same orientation as lcd
		case ACCELEROMETER_Z_AXIS_ID:	return get_mems_value_Z();
		default: return CONV_NOK;
	}
}

int Java_com_is2t_demos_support_Sensor_getMaxValue(int sensorID)
{
	switch (sensorID) {
		case POTENTIOMETER_ID:	return get_potentiometer_max_value();
		case ACCELEROMETER_X_AXIS_ID:	return get_mems_max_value_X();
		case ACCELEROMETER_Y_AXIS_ID:	return get_mems_max_value_Y();
		case ACCELEROMETER_Z_AXIS_ID:	return get_mems_max_value_Z();
		default: return CONV_NOK;
	}
}

int Java_com_is2t_demos_support_Sensor_getMinValue(int sensorID)
{
	switch (sensorID) {
		case POTENTIOMETER_ID:	return get_potentiometer_min_value();
		case ACCELEROMETER_X_AXIS_ID:	return get_mems_min_value_X();
		case ACCELEROMETER_Y_AXIS_ID:	return get_mems_min_value_Y();
		case ACCELEROMETER_Z_AXIS_ID:	return get_mems_min_value_Z();
		default: return CONV_NOK;
	}
}

