/*
 * File:   Appmain.c
 * Author: stronics
 *
 * Created on 30 maart 2021, 13:11
 */

/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/


#include "..\Source\System\PIC16LF153x5\SysLibAll.h"

//Include Driver Function Libraries
#include "..\Source\Driver\PIC16LF153x5\DrvI2C.h"
//#include "..\Source\Driver\PIC16LF153x5\DrvTimerTick.h"
#include "..\Source\Driver\PIC16LF153x5\DrvSci.h"
#include "..\Source\Driver\PIC16LF153x5\DrvGpio.h"
#include "..\Source\Driver\PIC16LF153x5\DrvPwm.h"
#include "../Source/Driver/PIC16LF153x5/DrvTimer.h"
//Include Standard Function Libraries
#include "..\Source\Standard\I2C\StdVcnl4200.h"
//Include Application Function Library
#include "AppIrq.h"
#include "AppStrip.h"


/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
void AppMainInitAllObjects(void);
void AppMainBackGroundLoop(void);
/**********************************************************************************************************************/

// PIC16(L)F153x5 Configuration Bit Settings
// CONFIG1
#pragma config FEXTOSC = OFF
#pragma config RSTOSC = HFINT32 // Power-up default value for COSC bits (HFINTOSC with OSCFRQ= 32 MHz and CDIV = 1:1)
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; i/o or oscillator function on OSC2)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (FSCM timer enabled)

// CONFIG2
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR pin is Master Clear function)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config LPBOREN = OFF    // Low-Power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = ON       // Brown-out reset enable bits (Brown-out Reset Enabled, SBOREN bit is ignored)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) set to 1.9V on LF, and 2.45V on F Devices)
#pragma config ZCD = OFF        // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR.)
#pragma config PPS1WAY = ON     // Peripheral Pin Select one-way control (The PPSLOCK bit can be cleared and set only once in software)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a reset)

// CONFIG3
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep; SWDTEN ignored)
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4
#pragma config BBSIZE = BB512   // Boot Block Size Selection bits (512 words boot block size)
#pragma config BBEN = OFF       // Boot Block Enable bit (Boot Block disabled)
#pragma config SAFEN = OFF      // SAF Enable bit (SAF enabled)
#pragma config WRTAPP = OFF     // Application Block Write Protection bit (Application Block not write protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block not write protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration Register not write protected)
#pragma config WRTSAF = OFF     // Storage Area Flash Write Protection bit (SAF not write protected)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low Voltage programming enabled. MCLR/Vpp pin function is MCLR.)

// CONFIG5
#pragma config CP = OFF         // UserNVM Program memory code protection bit (UserNVM code protection disabled)

/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/

void AppMainInitAllObjects() {
    AppStripClearGPIO();
    DrvI2CInit();

    DrvSciInit();
    DrvSciInitChannel(SCI_CHANNEL_A, SCI_SPEED_38400_bps, NO_PARITY, DATA_BITS_8, STOP_BIT_1);
    DrvSciInitDriveEnable(PORT_A, 2);

    DrvPwmInit();
    AppIrqInit();

    AppStripInit();

    StdVcnl4200Init();

    DrvTimerInit(100, &timer_tick);
}

/**********************************************************************************************************************/
void AppMainBackGroundLoop(void) {

    /*Tick functions the need constant ticks*/
    AppStripTimerTick();
    AppStripHandler();
    AppDrvTxHandler();
    /**/
}

/**********************************************************************************************************************/
void main(void) {

    AppMainInitAllObjects();

    for (;;) {
        AppMainBackGroundLoop();
    }
    return;
}
/**********************************************************************************************************************/