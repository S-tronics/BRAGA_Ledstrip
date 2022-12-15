/**********************************************************************************************************************/
/**
 * @file        DrvPwm.h
 *
 * @author      Stijn Vermeersch
 * @date        13.04.2021
 *
 * @brief       The header file for the I2C driver
 *
 * The I2C interface.\n\n
 *
 *
 * \n<hr>
 * Copyright (c) 2021-2022, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#ifndef DRV__PWM_H
#define DRV__PWM_H
/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S Y M B O L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/
//PWM channel to define
typedef enum
{
	PWM3 = 0,
	PWM4 = 1,
	PWM5 = 2,
	PWM6 = 3
}
PWM_CH;

typedef UNSIGNED_8	DRVPWM_DRV_HNDL;

typedef enum
{
    PWM_ACT_H = 0,
    PWM_ACT_L = 1
}
PWM_POL;

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   Initialiser for the PWM driver
 *
 * Initialises the PWM-driver.\n
 */
void DrvPwmInit(void);

/**
* @brief 	Function to  initiate a startbit to the selected I2C-bus.
*
* @param	hndl	: 	Handle to the I2C device
*
*/
void DrvPwmStart(DRVPWM_DRV_HNDL hndl, UNSIGNED_8 dc);

/**
* @brief 	Function to initiate a stop-bit to the selected I2C-bus
*
* @param	hndl	: 	Handle to the I2C device
*
*/
void DrvPwmStop(DRVPWM_DRV_HNDL hndl);

/**
* @brief 	Function to register an I2C device and initialise its I²C-bus.
*
* @param	ch		: 	PWM channel of the driver
* @param	period	:	Period of the PWM channel	
* @param	dc	: 	Duty-cycle of the PWM channel
*
*/
DRVPWM_DRV_HNDL DrvPWMRegisterChannel(PWM_CH ch, PWM_POL pol);

/**
* @brief 	Function to update the period of the PWM-driver.
*
* @param	ch		: 	PWM channel of the driver
* @param	period	:	Period of the PWM channel
*
*/
void DrvPwmPeriodUpdate(PWM_CH ch, UNSIGNED_16 period);

/**
* @brief 	Function to update the dutycycle of the PWM-driver.
*
* @param	ch		: 	PWM channel of the driver
* @param	dc		:	Dutycycle of the PWM-driver
*
*/
void DrvPwmDutycycleUpdate(DRVPWM_DRV_HNDL hndl, UNSIGNED_16 dc);

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S T A T I C   I N L I N E   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

#endif /* DRV__I2C_H */