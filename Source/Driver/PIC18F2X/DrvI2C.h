/**********************************************************************************************************************/
/**
 * @file        DrvI2C.h
 *
 * @author      Stijn Vermeersch
 * @date        23.06.2018
 *
 * @brief       The header file for the I2C driver
 *
 * The I2C interface.\n\n
 *
 *
 * \n<hr>
 * Copyright (c) 2018-2019, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#ifndef DRV__I2C_H
#define DRV__I2C_H
/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S Y M B O L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
#define MAX_I2C_DEV_CHA			4
#define	MAX_I2C_DEV_CHB			4

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/
//I2C channel to define
typedef enum
{
	I2C_CHANNEL_A = 0,
	I2C_CHANNEL_B = 1
}
I2C_CHANNEL;

//MASTER - SLAVE select
typedef enum
{
	I2C_MASTER,
	I2C_SLAVE
}
I2C_MSTR_SLV;

typedef enum
{
    I2C_R = 0,
    I2C_W = 1
}
I2C_RW;

typedef UNSIGNED_8	DRVI2C_DEV_HNDL;

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   Initialiser for the I2C interface
 *
 * Initialises the I2C-interface.\n
 */
void DrvI2CInit(void);

/**
* @brief 	Function to  initiate a startbit to the selected I2C-bus.
*
* @param	hndl	: 	Handle to the I2C device
*
*/
void DrvI2CStart(DRVI2C_DEV_HNDL hndl);

/**
* @brief 	Function to initiate a repeated start to the selected I2C-bus
*
* @param	hndl	: 	Handle to the I2C device
*
*/
void DrvI2CRepeatedStart(DRVI2C_DEV_HNDL hndl);

/**
* @brief 	Function to initiate a stop-bit to the selected I2C-bus
*
* @param	hndl	: 	Handle to the I2C device
*
*/
void DrvI2CStop(DRVI2C_DEV_HNDL hndl);

/**
* @brief 	Function to register an I2C device and initialise its I²C-bus.
*
* @param	ch		: 	I2C channel of the device
* @param	mode	:	Define I2C_MASTER or I2C_SLAVE mode	
* @param	speed	: 	Clockfrequency in Hz
* @param	address : 	Address in case of the I2C in in slavemode
*
*/
DRVI2C_DEV_HNDL DrvI2CRegisterDevice(I2C_CHANNEL ch, UNSIGNED_8 address, UNSIGNED_32 speed, I2C_MSTR_SLV mode);

/**
* @brief 	Function to write on the I2C-bus as master
*
* @param	hndl	: 	Handle to the I2C device
* @param	data	:	Data to write
*
*/
void DrvI2CWriteMstr(DRVI2C_DEV_HNDL hndl, UNSIGNED_8* data);

/**
* @brief 	Function to write the slave address on the I2C-bus as master
*
* @param	hndl	: 	Handle to the I2C device
* @param    rw      :   Read Write transmission to start
*
*/
void DrvI2CWriteAddrMstr(DRVI2C_DEV_HNDL hndl, I2C_RW rw);


/**
* @brief 	Function to read data from the I2C-bus as master
*
* @param	hndl	: 	Handle to the I2C device
* @param	data	:	Readed data
* @param    last    :   Last data byte to read 
*/
void DrvI2CReadMstr(DRVI2C_DEV_HNDL hndl, UNSIGNED_8* data, BOOLEAN last);

/**
* @brief 	Function to start a transmission on the I2C-bus
*
* @param	hndl	: 	Handle to the I2C device
* @param    rw      :   Read Write transmission to start
*/
void DrvI2CBeginTransmission(DRVI2C_DEV_HNDL hndl, I2C_RW rw);



/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S T A T I C   I N L I N E   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

#endif /* DRV__I2C_H */