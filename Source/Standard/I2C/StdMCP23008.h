/**********************************************************************************************************************/
/**
 * @file        StdMCP280003.h
 *
 * @author      Stijn Vermeersch
 * @date        01.12.2020
 *
 * @brief       Basic GPIO expander
 *
 *
 *
 * \n<hr>\n
 * Copyright (c) 2020, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#ifndef STDMCP28003_H
#define	STDMCP28003_H
/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include "..\..\Driver\PIC18F2X\DrvGpio.h"
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
    OUTPUT = 0,
    INPUT = 1
}
MCP_DIR;

typedef struct
{
    UNSIGNED_8  nr;
    MCP_DIR     dir;
    BOOLEAN     interruptenable;  
    BOOLEAN     pullup;                 //If set and pin is declared as input, an internal pull-up is set
}
MCP_GPIO;

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   Initialiser for the IO-expander
 *
 * Registers the GPIO module to the Module Manager.\n
 * This method must be called only once, which is normally done in the startup phase of the processor.\n
 */
void StdMCP23008Init(GPIO_PORT intport, UNSIGNED_8 intpin, GPIO_PORT resport, UNSIGNED_8 respin);

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
void StdMCP23008Config(MCP_GPIO gpio);

BOOLEAN StdMCP23008Set(UNSIGNED_8 ch, BOOLEAN value);

BOOLEAN StdMCP23008Get(UNSIGNED_8 ch);

void StdMCP23008GetInt(UNSIGNED_8 ch);
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S T A T I C   I N L I N E   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/


#endif	/* STDMCP28003_H */

