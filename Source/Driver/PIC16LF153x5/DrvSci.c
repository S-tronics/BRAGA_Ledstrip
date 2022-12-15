/**********************************************************************************************************************/
/**
 * @file        sci\DrvSci.c
 *
 * @author      Stijn Vermeersch
 * @date        03.08.2017
 *
 * @brief       
 *
 *
 *
 * \n<hr>
 * Copyright (c) 2017, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#define SPI__DRVSCI_C
/**********************************************************************************************************************/



/***********************************************************************************************************************
; V E R I F Y    C O N F I G U R A T I O N
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include "..\..\System\PIC16LF153x5\SysLibAll.h"

#include <stdio.h>
//DRIVER lib include section
#include "DrvSci.h"
#include "DrvGpio.h"
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   Number of the available UART ports
 */
#define SCI_PORT_COUNT              1

#define INT_SCI_TX1                 0x10    //Interrupt UART1 Transmitter

#define REC_QUEUE_LENGTH			30
#define TX_QUEUE_LENGTH             50

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    //Baudrate control register
    volatile UNSIGNED_8* baudratectrlregptr;

    //Baudrate generator low byte
    volatile UNSIGNED_8* baudrategenlow;

    //Baudrate generator high byte
    volatile UNSIGNED_8* baurdrategenhigh;

    //Interrupt enable register
    volatile UNSIGNED_8* ier;

    //Transmit register
    volatile UNSIGNED_8* txreg;

    //Receive register
    volatile UNSIGNED_8* rxreg;

    //Transmit status
    volatile UNSIGNED_8* txstatus;

    //Receive status
    volatile UNSIGNED_8* rxstatus;

    //Hook to execute when recieving new byte
    RXHOOK_NEW_BYTE rxhook_new_byte;
}
SCI_PORT;

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvSciResetPort(SCI_PORT* sci_port_ptr);
void DrvSciInitPheripheral(SCI_CHANNEL channel, SCI_SPEED speed, PARITY parity, DATA_BIT databit, STOPBIT stopbit);
void DrvSciInitRecQueue(void);
void DrvSciInitTxQueue(void);
BOOLEAN DrvSciFillRecQueue(UNSIGNED_8* queuedata);
BOOLEAN DrvSciFillTxQueue(UNSIGNED_8* queuedata);
BOOLEAN DrvSciReadRecQueue(UNSIGNED_8* queuedata);
BOOLEAN DrvSciReadTxQueue(UNSIGNED_8* queuedata);
/**********************************************************************************************************************/




/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief   Array of complete structures that allow full control of the DRVSCI port behavior.
 */
static SCI_PORT drvsci_port[SCI_PORT_COUNT];
static UNSIGNED_8 rec_data;
static UNSIGNED_8 error = 0x00;

static UNSIGNED_16 rxhead = 0;
static UNSIGNED_16 rxtail = 0;
static BOOLEAN rxfull = FALSE;
static UNSIGNED_16 rxbuf_cntr = 0;
static UNSIGNED_16 txhead = 0;
static UNSIGNED_16 txtail = 0;
static BOOLEAN txfull = FALSE;
static UNSIGNED_16 txbuf_cntr = 0;
static UNSIGNED_8 a_rec_data[REC_QUEUE_LENGTH];
static UNSIGNED_8 a_tx_data[TX_QUEUE_LENGTH];

static GPIO_PORT drive_enable_port;
static UNSIGNED_8 drive_enable_pin;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvSciResetPort(SCI_PORT* sci_port_ptr) {
    if (sci_port_ptr == &drvsci_port[0]) {
        drvsci_port[0].baudratectrlregptr = &BAUD1CON;
        drvsci_port[0].baudrategenlow = &SP1BRG;
        drvsci_port[0].baurdrategenhigh = &SP1BRGH;
        drvsci_port[0].ier = &PIE3;
        drvsci_port[0].txreg = &TX1REG;
        drvsci_port[0].rxreg = &RC1REG;
        drvsci_port[0].txstatus = &TX1STA;
        drvsci_port[0].rxstatus = &RC1STA;
    }
    //sci_port_ptr->rxhook_new_byte = NULL;
}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvSciInitPheripheral(SCI_CHANNEL channel, SCI_SPEED speed, PARITY parity, DATA_BIT databit, STOPBIT stopbit) {
    SCI_PORT* sci_port_ptr;
    UNSIGNED_16 prescaler = 0;

    if (channel == SCI_CHANNEL_A) {
        ANSELC &= 0xCF;
        TRISC |= 0x20;
        RC4PPS = 0x0F; //TX-pin declared on RC4
        RC5PPS = 0x15;
        RX1DTPPS &= 0xE0;
        RX1DTPPS |= 0x15;
        TX1CKPPS &= 0xE0;
        TX1CKPPS |= 0x14;
    }

    sci_port_ptr = &drvsci_port[channel];

    *sci_port_ptr->baudratectrlregptr |= 0x00; //16-bit Baud Rate Generator bit

    *sci_port_ptr->txstatus = 0x20; //High Baud Rate Select bit
    //Calculate baudrate generator
    //prescaler = ((Get_SysClk() / speed) >> 2) - 1;	//16-bit register used.
    prescaler = ((Get_SysClk() / speed) >> 6) - 1; //16-bit register used.
    *sci_port_ptr->baudrategenlow = (UNSIGNED_8) (prescaler & 0x00FF);
    *sci_port_ptr->baurdrategenhigh = (UNSIGNED_8) (prescaler >> 8);
    *sci_port_ptr->rxstatus |= 0x90; //Serial port enable (SPEN); Enable receiver

    //Interrupt enable register
    *sci_port_ptr->ier |= 0x20;
}

void DrvSciInitDriveEnable(GPIO_PORT port, UNSIGNED_8 pin) {
    drive_enable_port = port;
    drive_enable_pin = pin;
}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvSciInitRecQueue(void) {
    rxhead = 0;
    rxtail = 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvSciInitTxQueue(void) {
    txhead = 0;
    txtail = 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/
BOOLEAN DrvSciFillRecQueue(UNSIGNED_8* queuedata) {
    SIGNED_16 diff = 0;
    SIGNED_16 length = 0;
    BOOLEAN cmdfull = FALSE;

    length = REC_QUEUE_LENGTH;

    if (rxfull == TRUE) {
        return FALSE;
    }
    diff = rxhead - rxtail;
    if (diff < 0) {
        diff += length;
    }
    if (diff >= length) {
        return FALSE;
    }
    a_rec_data[rxhead] = *queuedata;
    rxhead++;

    if (rxhead == length) {
        rxhead = 0;
    }
    if (rxhead == rxtail) {
        rxfull = TRUE;
    }
    return TRUE;
}

/*--------------------------------------------------------------------------------------------------------------------*/
BOOLEAN DrvSciFillTxQueue(UNSIGNED_8* queuedata) {
    SIGNED_16 diff = 0;
    SIGNED_16 length = 0;

    length = TX_QUEUE_LENGTH;

    if (txfull == TRUE) {
        return FALSE;
    }
    diff = txhead - txtail;
    if (diff < 0) {
        diff += length;
    }
    if (diff >= length) {
        return FALSE;
    }
    a_tx_data[txhead] = *queuedata;
    txhead++;
    txbuf_cntr++;
    if (txhead == length) {
        txhead = 0;
    }
    if (txbuf_cntr == length) {
        txfull = TRUE;
    }
    return TRUE;
}

/*--------------------------------------------------------------------------------------------------------------------*/
BOOLEAN DrvSciReadRecQueue(UNSIGNED_8* queuedata) {
    UNSIGNED_16 length = 0;

    length = REC_QUEUE_LENGTH;

    if ((rxhead == rxtail) && !rxfull) {
        return FALSE;
    }
    *queuedata = a_rec_data[rxtail];
    rxtail++;
    if (rxtail == length) {
        rxtail = 0;
    }
    return TRUE;
}

/*--------------------------------------------------------------------------------------------------------------------*/
BOOLEAN DrvSciReadTxQueue(UNSIGNED_8* queuedata) {
    UNSIGNED_16 length = 0;

    length = TX_QUEUE_LENGTH;

    if ((txhead == txtail) && !txfull) {
        return FALSE;
    }
    *queuedata = a_tx_data[txtail];
    txtail++;
    txbuf_cntr--;
    if (txtail == length) {
        txtail = 0;
    }
    return TRUE;

}
/**********************************************************************************************************************/

/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvSciInit(void) {
    DrvSciResetPort(&drvsci_port[0]);
}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvSciInitChannel(SCI_CHANNEL channel, SCI_SPEED speed, PARITY parity, DATA_BIT databit, STOPBIT stopbit) {
    DrvSciInitPheripheral(channel, speed, parity, databit, stopbit);
}

/*--------------------------------------------------------------------------------------------------------------------*/
BOOLEAN DrvSciRegisterRxHandler(SCI_CHANNEL channel, RXHOOK_NEW_BYTE new_byte) {
    SCI_PORT* sci_port_ptr;

    sci_port_ptr = &drvsci_port[channel];

    if (sci_port_ptr->rxhook_new_byte == NULL) {
        sci_port_ptr->rxhook_new_byte = new_byte;
        return TRUE;
    }
    return FALSE;
}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvSciWriteData(UNSIGNED_8* wdata, UNSIGNED_8 length) {
    UNSIGNED_8 i = 0;

    for (i = 0; i < length; i++) {
        if (DrvSciFillTxQueue(wdata + i)) {
            error = 0x00;
        } else {
            error = 0x01;
        }
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/
BOOLEAN DrvSciReadData(UNSIGNED_8* wdata) {
    return DrvSciReadRecQueue(wdata);
}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvTxHandler(void) {
    static UNSIGNED_8 data;
    SCI_PORT* sci_port_ptr;
    sci_port_ptr = &drvsci_port[0];

    while (DrvSciReadTxQueue(&data)) {
        DrvGpioSetPin(drive_enable_port, drive_enable_pin);
        TX1STA = 0x20;

        *sci_port_ptr->txreg = data;
        while (!TX1IF) continue;
        TX1IF = 0;
        //TRMT bit 0 when transmit shift tergister is empty
        while (!(TXSTA1 & 0x02)) {
        };
    }
    DrvGpioClearPin(drive_enable_port, drive_enable_pin);
}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvSciIntRec1(void) {
    SCI_PORT* sci_port_ptr;

    sci_port_ptr = &drvsci_port[0];
    //rec_data = *sci_port_ptr->rxreg;
    if (DrvSciFillRecQueue(sci_port_ptr->rxreg)) {
        error = 0x00;
    } else {
        error = 0x01;
    }

}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvSciIntTx1(void) {

}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvSciIntRec2(void) {
    SCI_PORT* sci_port_ptr;
    static UNSIGNED_8 recdata;

    sci_port_ptr = &drvsci_port[1];
    //rec_data = *sci_port_ptr->rxreg;
    //*sci_port_ptr->txreg = rec_data;
    //sci_port_ptr->rxhook_new_byte(rec_data);
    recdata = *sci_port_ptr->rxreg;

    if (DrvSciFillRecQueue(&recdata)) {
        error = 0x00;
    } else {
        error = 0x01;
    }

}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvSciIntTx2(void) {

}
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************/
