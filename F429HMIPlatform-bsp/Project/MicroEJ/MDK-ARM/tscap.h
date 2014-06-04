/*
 *  Copyright (c) 2012, Cameon,
 *  All Rights Reserved
 *
 *  The copyright notice above does not evidence any
 *  actual or intended publication of such source code.
 */

/*!
 *  \file     tscap.h
 *  \brief    Capacitive touch screen driver header
 *  \authors
 *  \date     21/08/2012
 */

#ifndef TSCAP_H_
#define TSCAP_H_

/* SSD 2531 I2C Address */
#define TSCAP_I2C_ADDR			0x70

#define MAX_POINTS	5
#define REG_EVENT_STATUS		0x02
#define REG_EVENT_STACK			0x03
#define REG_EVENT_STACK_CLR	0x81
/* Finger addresses*/
#define REG_FINGER1				0x03
#define REG_FINGER2				0x09
#define REG_FINGER3				0x0F
#define REG_FINGER4				0x15
#define REG_FINGER5				0x1B

#define FIFO_NOT_EMPTY 		0x10

//I2C speed
#define TS_I2C_SPEED  	400000
//Error code
#define TS_OK					0
#define TS_FAIL 				1
//I2C timeout
#define TS_TIMEOUT			((uint32_t)10000)
#define TS_FLAG_TIMEOUT ((uint32_t)0x1000)


/*!
 * \struct sTsRegInit
 * \brief TS initialization structure
 */
typedef struct
{
    uint8_t Reg;			/*!<Register */
    uint16_t Val;			/*!<Value to write */
    uint16_t NbVal;		/*!<Number of value */
} sTsRegInit;

/*!
 * \enum eEventFlag
 * \brief TS event flag
 */
typedef enum {evPutdown, evPutup, evContact, evReserved}eEventFlag; 

typedef struct
{
	uint16_t XCoord;
	uint16_t YCoord;
	uint8_t WeightIndex;
	uint8_t SpeedIndex;
	uint8_t TouchDirection;
	uint8_t FingerFlag;
	eEventFlag EventNo;
	
}sFinger;


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
void TsInit(void);

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
uint32_t ReadReg_Tscap(uint8_t* pBuffer, uint16_t ReadAddr, uint8_t* NumByteToRead);

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
uint32_t WriteReg_Tscap(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite);

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
sFinger TsReadFinger(uint8_t finger_nb);

/*! ------------------------------------------------------------------------------
* \fn Function :sEvent TsReadStatus()
*------------------------------------------------------------------------------
*
* Description :
* -------------
*  \brief Read the Touch status
*
* Parameter :
* ------------
*  \param [In]  :
*  \param [Out] :
*
* Return :
* --------
*  \return : if an area is touched or not (return value to be masked with 0xF)
*
*------------------------------------------------------------------------------
*/
uint8_t TsReadStatus(void);

/*! ------------------------------------------------------------------------------
* \fn Function :sEvent TsReadEvent()
*------------------------------------------------------------------------------
*
* Description :
* -------------
*  \brief Read the point number
*
* Parameter :
* ------------
*  \param [In]  :
*  \param [Out] :
*
* Return :
* --------
*  \return : detected point number
*
*------------------------------------------------------------------------------
*/
uint8_t TsReadEvent(void);

#endif /* TSCAP_H_ */
