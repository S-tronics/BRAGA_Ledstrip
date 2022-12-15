/**********************************************************************************************************************/
/**
 * @file        DrvTimer.h
 *
 * @author      Stijn Vermeersch
 * @date        18.07.2021
 *
 * @brief       Functionality for Non-Volatile Memory
 *
 *
 * \n<hr>\n
 * Copyright (c) 2021, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
#ifndef DRV__NVM_H
#define DRV__NVM_H


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
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/

/*
 * Only use these functions for config registers like DIA, User ID, etc..
 */
UNSIGNED_16 DrvNVM_read_config(UNSIGNED_16 address);

//Last 2 bits (MSB) won't be written to memory
void DrvNVM_write_config(UNSIGNED_16 address, UNSIGNED_16 data);
void DrvNVM_erase_config(UNSIGNED_16 address);
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S T A T I C   I N L I N E   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/

#endif /* DRV__NVM_H */
