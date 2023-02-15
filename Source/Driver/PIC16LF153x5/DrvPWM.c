/**********************************************************************************************************************/
/**
 * @file        pwm\DrvPWM.c
 *
 * @author      Stijn Vermeersch
 * @date        30.03.2021
 *
 * @brief       This file contains the implementation of the PWM-driver modules
 *
 *
 *
 * \n<hr>
 * Copyright (c) 2021, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#define PWM__DRVPWM_C
/**********************************************************************************************************************/

/***********************************************************************************************************************
; V E R I F Y    C O N F I G U R A T I O N
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include "..\..\..\Source\System\PIC16LF153x5\SysLibAll.h"

//DRIVER lib include section
#include "DrvGpio.h"
#include "DrvPWM.h"
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
#define PWM_CH_COUNT	4   
#define F_BASE          1200        

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/
typedef struct {
    volatile UNSIGNED_8* pwm_ctrl_reg;
    volatile UNSIGNED_8* pwm_dc_high;
    volatile UNSIGNED_8* pwm_dc_low;
    volatile UNSIGNED_8* pwm_tmr_clk;
    volatile UNSIGNED_8* pwm_tmr_cont;
}
PWM_DRV;

typedef struct {
    PWM_CH ch;
    UNSIGNED_16 period;
    UNSIGNED_16 dc;
}
DRVPWM_DRV_PROPS;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvPwmChannelInit(PWM_CH ch, PWM_POL pol);
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static PWM_DRV drvpwmdrv[PWM_CH_COUNT];
//static DRVI2C_DEV_PROPS		drvi2cdevice[MAX_I2C_DEV_CHA + MAX_I2C_DEV_CHA];
static DRVPWM_DRV_PROPS drvpwmchannel[PWM_CH_COUNT];

static DRVPWM_DRV_HNDL drvpwm_registered_drvcount;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvPwmChannelInit(PWM_CH ch, PWM_POL pol) {
    PWM_DRV* pwmdrvptr;
    DOUBLE periodreg = 0;
    switch (ch) {
        case PWM3:
            RA5PPS &= 0x00; //Connect RA5 with PWM3 (White)
            RA5PPS |= 0x0B;
            ANSELA &= ~0x20;
            TRISA &= ~0x20;
            pwmdrvptr = &drvpwmdrv[PWM3];
            drvpwmdrv[PWM3].pwm_ctrl_reg = &PWM3CON;
            drvpwmdrv[PWM3].pwm_dc_high = &PWM3DCH;
            drvpwmdrv[PWM3].pwm_dc_low = &PWM3DCL;
            drvpwmdrv[PWM3].pwm_tmr_clk = &T2CLKCON;
            drvpwmdrv[PWM3].pwm_tmr_cont = &T2CON;
            break;
        case PWM4: //Connect RC3 with PWM4 (Red))
            RC3PPS &= 0x00;
            RC3PPS |= 0x0C;
            ANSELC &= ~0x08;
            TRISC &= ~0x08;
            pwmdrvptr = &drvpwmdrv[PWM4];
            drvpwmdrv[PWM4].pwm_ctrl_reg = &PWM4CON;
            drvpwmdrv[PWM4].pwm_dc_high = &PWM4DCH;
            drvpwmdrv[PWM4].pwm_dc_low = &PWM4DCL;
            drvpwmdrv[PWM4].pwm_tmr_clk = &T2CLKCON;
            drvpwmdrv[PWM4].pwm_tmr_cont = &T2CON;
            break;
        case PWM5: //Connect RC2 with PWM5 (Green))
            RC2PPS &= 0x00;
            RC2PPS |= 0x0D;
            ANSELC &= ~0x04;
            TRISC &= ~0x04;
            pwmdrvptr = &drvpwmdrv[PWM5];
            drvpwmdrv[PWM5].pwm_ctrl_reg = &PWM5CON;
            drvpwmdrv[PWM5].pwm_dc_high = &PWM5DCH;
            drvpwmdrv[PWM5].pwm_dc_low = &PWM5DCL;
            drvpwmdrv[PWM5].pwm_tmr_clk = &T2CLKCON;
            drvpwmdrv[PWM5].pwm_tmr_cont = &T2CON;
            break;
        case PWM6: //Connect RA4 with PWM6 (Blue)
            RA4PPS &= 0x00;
            RA4PPS |= 0x0E;
            ANSELA &= ~0x10;
            TRISA &= ~0x10;
            pwmdrvptr = &drvpwmdrv[PWM6];
            drvpwmdrv[PWM6].pwm_ctrl_reg = &PWM6CON;
            drvpwmdrv[PWM6].pwm_dc_high = &PWM6DCH;
            drvpwmdrv[PWM6].pwm_dc_low = &PWM6DCL;
            drvpwmdrv[PWM6].pwm_tmr_clk = &T2CLKCON;
            drvpwmdrv[PWM6].pwm_tmr_cont = &T2CON;
            break;
        default:
            break;
    }

    switch (pol) {
        case PWM_ACT_H:
            *pwmdrvptr->pwm_ctrl_reg &= ~0x10;
            break;
        case PWM_ACT_L:
            *pwmdrvptr->pwm_ctrl_reg |= 0x10;
            break;
        default:
            break;
    }
    *drvpwmdrv->pwm_ctrl_reg &= ~0x80; //Disable PWM for sure
    *drvpwmdrv->pwm_tmr_cont &= ~0x80; //Timer 2 OFF
    *drvpwmdrv->pwm_tmr_clk |= 0x01; //Set Fosc/4
    *drvpwmdrv->pwm_tmr_cont |= 0x70; //Prescaler 1:128

}
/*--------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/

/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvPwmInit(void) {
    drvpwm_registered_drvcount = 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/
DRVPWM_DRV_HNDL DrvPWMRegisterChannel(PWM_CH ch, PWM_POL pol) {
    DrvPwmChannelInit(ch, pol);
    drvpwmchannel[drvpwm_registered_drvcount].ch = ch;
    drvpwm_registered_drvcount++;


    return (drvpwm_registered_drvcount - 1);
}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvPwmStart(DRVPWM_DRV_HNDL hndl, UNSIGNED_8 dc) {
    DRVPWM_DRV_PROPS* ch_ptr;
    PWM_DRV* drv_ptr;
    UNSIGNED_32 period;
    DOUBLE tfreq;
    ch_ptr = &drvpwmchannel[hndl];
    drv_ptr = &drvpwmdrv[ch_ptr->ch];

    //period = T_BASE / perc;
    *drv_ptr->pwm_tmr_cont &= ~0x80; //Disable timer
    tfreq = Get_SysClk() / 1000000;
    tfreq /= 128; //Prescaler 128:1
    tfreq /= 4; //Fosc/4

    PR2 = ((DOUBLE) (F_BASE) * tfreq) - 1;
    period = F_BASE / 400 * dc;
    period <<= 6;
    *drv_ptr->pwm_dc_high = period >> 8;
    *drv_ptr->pwm_dc_low = period & 0x00FF;

    *drv_ptr->pwm_ctrl_reg |= 0x80; //Enable PWM-CH
    *drv_ptr->pwm_tmr_cont |= 0x80; //Enable timer
}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvPwmStop(DRVPWM_DRV_HNDL hndl) {
    DRVPWM_DRV_PROPS* ch_ptr;
    PWM_DRV* drv_ptr;

    ch_ptr = &drvpwmchannel[hndl];
    drv_ptr = &drvpwmdrv[ch_ptr->ch];
    *drv_ptr->pwm_ctrl_reg &= ~0x80; //Enable PWM-CH

}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvPwmDutycycleUpdate(DRVPWM_DRV_HNDL hndl, UNSIGNED_16 dc) {
    DRVPWM_DRV_PROPS* ch_ptr = &drvpwmchannel[hndl];
    PWM_DRV* drv_ptr = &drvpwmdrv[ch_ptr->ch];

    UNSIGNED_32 period = F_BASE / 400 * dc;
    period <<= 6;
    drv_ptr = &drvpwmdrv[ch_ptr->ch];
    *drv_ptr->pwm_dc_high = period >> 8;
    *drv_ptr->pwm_dc_low = period & 0x00FF;
}
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/
