/**********************************************************************************************************************/
/**
 * @file        gpio\DrvGpio.h
 *
 * @author      Stijn Vermeersch
 * @date        02.01.2016
 *
 * @brief       Basic GPIO manipulation functionality
 *
 * This GPIO manipulating module is designed to be used for initialisation and modifying IO ports and pins.\n
 * The implementation is <em>NOT</em> designed to be FAST!\n
 *
 *
 * \n<hr>\n
 * Copyright (c) 2016, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#ifndef DRV__GPIO_H
#define DRV__GPIO_H
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
typedef enum
{
    PORT_A = 0,			
	PORT_B = 1,			
	PORT_C = 2			
}
GPIO_PORT;


typedef enum
{
	PIN_IO_FUNC_INPUT,
	PIN_IO_FUNC_OUTPUT_LOW,
	PIN_IO_FUNC_OUTPUT_HIGH
}
GPIO_PIN_FUNC;

/**
 * @brief   Prototype of an in incoming GPIO
 *
 * @param	eint:	Number of the External interrupt
 */
//typedef void (*HOOK_GPIO_INCOMING)(UNSIGNED_8 eint);

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   Initialiser for the GPIO module
 *
 * Registers the GPIO module to the Module Manager.\n
 * This method must be called only once, which is normally done in the startup phase of the processor.\n
 */
void DrvGpioInit(void);

/**
 * @brief   Init a single PIN of an IO_PORT
 *
 * This method initialises all the concerning General Purpose IO control registers,\n
 * so that the specified pin is initialised in the given way (in/out/alternated func).
 *
 * @param   io_port :       The IO-port at which the pin is located.
 * @param   io_pin_nr :     The number of the io-pin, starting to count from zero.
 * @param   io_pin_func :   Specifies how to initialise this pin (in/out/alternated func)
 *
 * @remark  This method calculates a <i>BITMASK</i> out of the \p io_pin_nr and simply calls\n
 *          the function \ref DrvGpioInitPinMask()
 */
void DrvGpioInitPin(GPIO_PORT io_port, UNSIGNED_8 io_pin_nr, GPIO_PIN_FUNC io_pin_func);

/**
 * @brief   Sets a single IO pin high
 *
 * This method sets the specified pin to a high level.
 *
 * @param   io_port :       The IO-port at which the pin is located.
 * @param   io_pin_nr :     The number of the io-pin, starting to count from zero.
 *
 * @pre     The specified pins must be initialised as output
 * @remark  This method calculates a <i>BITMASK</i> out of the \p io_pin_nr and simply calls\n
 *          the function \ref DrvGpioSetPinMask()
 */
void DrvGpioSetPin(GPIO_PORT io_port, UNSIGNED_8 io_pin_nr);

/**
 * @brief   Sets a single IO pin low
 *
 * This method sets the specified pin to a low level.
 *
 * @param   io_port :       The IO-port at which the pin is located.
 * @param   io_pin_nr :     The number of the io-pin, starting to count from zero.
 *
 * @pre     The specified pins must be initialised as output
 * @remark  This method calculates a <i>BITMASK</i> out of the \p io_pin_nr and simply calls\n
 *          the function \ref DrvGpioClearPinMask()
 */
void DrvGpioClearPin(GPIO_PORT io_port, UNSIGNED_8 io_pin_nr);

/**
 * @brief   Toggles the level of a single IO pin
 *
 * This method toggles the level of the specified pin.
 *
 * @param   io_port :       The IO-port at which the pin is located.
 * @param   io_pin_nr :     The number of the io-pin, starting to count from zero.
 *
 * @pre     The specified pin must be initialised as output
 */
void DrvGpioTogglePin(GPIO_PORT io_port, UNSIGNED_8 io_pin_nr);

/**
 * @brief   Returns TRUE if a high level is applied to the specified pin.
 *
 * @param   io_port :       The IO-port at which the pin is located.
 * @param   io_pin_nr :     The number of the io-pin, starting to count from zero.
 *
 * @return  TRUE if a high level is applied to the specified pin, FALSE otherwise
 */
BOOLEAN DrvGpioIsPinHigh(GPIO_PORT io_port, UNSIGNED_8 io_pin_nr);

/**
 * @brief   Init several PINS of an IO_PORT
 *
 * This method initialises all the concerning General Purpose IO control registers,\n
 * so that the specified pins are initialised in the given way (in/out/alternated func).
 *
 * @param   io_port :       The IO-port at which the pins are located.
 * @param   io_pin_mask :   Specifies the IO-pins, by setting the corresponding bits
 * @param   io_pin_func :   Specifies how to initialise the pins (in/out/alternated func)
 *
 * @remark  This method throws an excpetion if no VALID pins are specified.
 */
void DrvGpioInitPinMask(GPIO_PORT io_port, UNSIGNED_8 io_pin_mask, GPIO_PIN_FUNC io_pin_func);

/**
 * @brief   Sets several IO pins high
 *
 * This method sets the specified pins to a high level.
 *
 * @param   io_port :       The IO-port at which the pin is located.
 * @param   io_pin_mask :   Specifies the IO-pins, by setting the corresponding bits
 *
 * @pre     The specified pins must be initialised as output
 */
void DrvGpioSetPinMask(GPIO_PORT io_port, UNSIGNED_8 io_pin_mask);

/**
 * @brief   Sets several IO pins low
 *
 * This method sets the specified pins to a low level.
 *
 * @param   io_port :       The IO-port at which the pin is located.
 * @param   io_pin_mask :   Specifies the IO-pins, by setting the corresponding bits
 *
 * @pre     The specified pins must be initialised as output
 */
void DrvGpioClearPinMask(GPIO_PORT io_port, UNSIGNED_8 io_pin_mask);

/**
 * @brief   Function to read data from an IO port
 *
 * This method returns the data on an I/O port
 *
 * @param   io_port :       The IO-port at which the pin is located.
 */
UNSIGNED_8 DrvGpioReadPortData(GPIO_PORT io_port);

/**
 * @brief   Fucntion to Enable GPIO-interrupt
 *
 */
void DrvGpioIntEnable(GPIO_PORT port, UNSIGNED_8 io_pin_nr);

/**
 * @brief   Fucntion to Disable GPIO-interrupt
 *
 */
void DrvGpioIntDisable(GPIO_PORT port, UNSIGNED_8 io_pin_nr);

/**
 * @brief   Interrupt subroutine when an input is changed
 *
 */
void DrvGpioIncomingIsr(void);

/**
 * @brief   Registers the Handler when a GPIO is incomming
 *
 * @param   io_port : 			The port of the GPIO
 * @param	io_pin_number : 	The pin number of the GPIO
 * @param	mode :				Defines on which level/edge the interrupt occures
 * @param 	hook_gpio_income: 	Pointer to function when an incoming interrupt occures
 *
 * @remark	Only use GPIO's who could be connected to External Interrupts (EINT_x) (EINT0 - 3)
 *
 */
//void DrvGpioRegisterIncomingIsr(GPIO_PORT port, UNSIGNED_8 io_pin_nr, SYS_INT_GPIO_MODE mode, HOOK_GPIO_INCOMING hook_gpio_income);

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S T A T I C   I N L I N E   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/

#endif /* DRV__GPIO_H */
