/*
 * File:   AppLed.c
 * Author: stronics
 *
 * Created on 14 februari 2017, 13:11
 */

/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
//#include <xc.h>
#include "SysLibAll.h"
#include "System.h"
/**********************************************************************************************************************/

/***********************************************************************************************************************
; V E R I F Y    C O N F I G U R A T I O N
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
//#define SYS_OSC_CLOCK_IN_HZ     1000000
#define SYS_OSC_CLOCK_IN_HZ     32000000
/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void SysPCLKInit(void)
{
    UNSIGNED_32 sysclk = SYS_OSC_CLOCK_IN_HZ;
    
//    if(sysclk >= 16000000)
//    {
//        OSCCON  &= ~0x03;                //Internal Oscillator block
//        OSCTUNE |= 0x40;                //Frequency Multiplier 4xPLL for HFINTOSC Enable bit
//        OSCCON  |= 0x70;                //16MHz
//        OSCCON2 &= ~0x04;
//    }
//    while(!(OSCCON & 0x08) == 0x08)
//    {
//        
//    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
UNSIGNED_32 Get_SysClk(void)
{
    return SYS_OSC_CLOCK_IN_HZ;
}
/*--------------------------------------------------------------------------------------------------------------------*/
UNSIGNED_32 Get_PCLK(void)
{
    return Get_SysClk();
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/


