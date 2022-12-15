/**********************************************************************************************************************/
/**
 * @file        eeprom\Eeprom.h
 *
 * @author      Stijn Vermeersch
 * @date        03/05/2017
 *
 * @brief       Basic Eeprom functionality
 *
 *
 * \n<hr>\n
 * Copyright (c) 2017, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/

//DRIVER lib include section

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S Y M B O L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   Initialisation of the eeprom functionality of the �C
 *
 * UNIGNED_16 values were used for compatibilty reasons with 16-bit �C's.
 */
void DrvEepromInit(void);
/**
 * @brief   Function to read from eeprom
 *
 * Function reads 1 byte wide data from de selected address.\n
 *
 * @param   eepromaddrh : MSB address section in case of 10 bit address selection
 * @param   eepromaddrl : LSB address section
 * 
 * @return  returns the 1 byte wide data selected by the address.
 */
UNSIGNED_16 DrvEepromRead(UNSIGNED_16 address);

/**
 * @brief   Function to write to the eeprom
 *
 * Function reads 1 byte wide data from de selected address.\n
 *
 * @param   eepromaddrh : MSB address section in case of 10 bit address selection
 * @param   eepromaddrl : LSB address section
 * @param   wdata       : Data to write to the selected address.  
 */
void DrvEepromWrite(UNSIGNED_16 address, UNSIGNED_16 wdata);
/**
 * @brief   Function that checks if write to eeprom is finished
 *
 * Function that reads the Eeprom Interrupt Flag to check if an een interrupt for
 * writing to the eeprom had occured.\n
 *  
 */
void DrvEepromInt(void);

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S T A T I C   I N L I N E   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/




