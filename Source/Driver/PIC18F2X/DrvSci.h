/**********************************************************************************************************************/
/**
 * @file        DrvSci.h
 *
 * @author      Stijn Vermeersch
 * @date        03.08.2017
 *
 * @brief       The header file for the Serial Communication Interface
 *
 * \n<hr>
 * Copyright (c) 2017-2018, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/

/**********************************************************************************************************************/
#ifndef SCI__DRVSCI_H
#define SCI__DRVSCI_H


/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
//DRIVER lib include section
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S Y M B O L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   INVALID SPI-device HANDLE definition
 */

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   SCI baudrate enumerator
 */
typedef enum
{
    SCI_SPEED_9600_bps       = 9600,
    SCI_SPEED_19200_bps      = 19200,
    SCI_SPEED_38400_bps      = 38400,
    SCI_SPEED_57600_bps      = 57600,
    SCI_SPEED_115200_bps     = 115200,
    SCI_SPEED_128000_bps     = 128000,
    SCI_SPEED_230400_bps     = 230400,
    SCI_SPEED_460800_bps     = 460800
}
SCI_SPEED;

/**
 * @brief   SCI predefined number of parity bits enumeration
 */
typedef enum
{
    NO_PARITY               = 0,
    EVEN_PARITY             = 1,
    ODD_PARITY              = 2,
    FORCED_1_STICK_PARITY   = 3,
    FORCED_0_STICK_PARITY   = 4
}
PARITY;

/**
 * @brief   SCI predefined SCI channel enumeration
 */
typedef enum
{
    SCI_CHANNEL_A   = 0,
    SCI_CHANNEL_B   = 1
}
SCI_CHANNEL;

/**
 * @brief   SCI predefined number of databits enumeration
 */
typedef enum
{
    DATA_BITS_5     = 0,
    DATA_BITS_6     = 1,
    DATA_BITS_7     = 2,
    DATA_BITS_8     = 3
}
DATA_BIT;

/**
 * @brief   SCI predefined number of stopbits enumeration
 */
typedef enum
{
    STOP_BIT_1      = 0,
    STOP_BITS_2     = 1
}
STOPBIT;

/**
 * @brief   Prototype of a <em>Receive a new byte Routine</em>
 */
typedef void (*RXHOOK_NEW_BYTE)(UNSIGNED_8 byte);

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   Initialiser for the SPI master driver
 *
 * Initialises the SPI master driver and registers this module to the Module Manager.\n
 */
void DrvSciInit(void);
/**
 * @brief   Init a single SCI PORT
 *
 * This method initialises all the concerning SCI registers,\n
 * so that the specified channel is initialised in the given way (channel/speed/parity/stopbit).
 *
 * @param   channel :             the SCI channel
 * @param   speed :               the SCI baudrate
 * @param   parity :              a binary digit is added to the data to make the sum of all the digits of the binary
 *                                data either always even (even parity) or always odd (odd parity).
 * @param   stopbit :             number of stopbits (=bits that indicate that a byte has just been transmitted)
 * @param   databit:              number of databits for each transfer
 *
 * @return  TRUE if \p channel is a valid Module channel, otherwise returns FALSE
 */
void DrvSciInitChannel(SCI_CHANNEL channel, SCI_SPEED speed, PARITY parity, DATA_BIT databit, STOPBIT stopbit);
/**
 * @brief Interrupt service routine for UART 1 Receiver
 *
 */
BOOLEAN DrvSciRegisterRxHandler(SCI_CHANNEL channel, RXHOOK_NEW_BYTE new_byte);

/**
 * @brief Function to Read data from a serial port. All data is stored in queue
 * 
 * @param	*wdata :		pointer to the data to read
 *
 */
BOOLEAN DrvSciReadData(UNSIGNED_8* wdata);
/**
 * @brief Function to write data to a serial port
 * 
 * @param	*wdata :		pointer to the data to write
 *
 */ 
void DrvSciWriteData(UNSIGNED_8* wdata, UNSIGNED_8 length);
/**
 * @brief Handler to handle to data to transmit
 *
 */ 
void DrvTxHandler(void);
/**
 * @brief Interrupt service routine for UART 1 Receiver
 *
 */ 
void DrvSciIntRec1(void);
/**
 * @brief Interrupt service routine for UART 1 Transmitter
 *
 */ 
void DrvSciIntTx1(void);
/**
 * @brief Interrupt service routine for UART 1 Receiver
 *
 */ 
void DrvSciIntRec2(void);
/**
 * @brief Interrupt service routine for UART 1 Transmitter
 *
 */ 
void DrvSciIntTx2(void);

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S T A T I C   I N L I N E   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/

#endif /* SPI__DRVSPI_H */
