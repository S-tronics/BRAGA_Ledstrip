/*
 * File:   AppIrq.c
 * Author: stronics
 *
 * Created on 10 August 2020, 13:11
 */

/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include "..\Source\System\PIC16LF153x5\SysLibAll.h"
//Driver Include Section
#include "..\Source\Driver\PIC16LF153x5\DrvSci.h"
#include "..\Source\Driver\PIC16LF153x5\DrvPWM.h"
#include "..\Source\Driver\PIC16LF153x5\DrvTimer.h"
#include "..\Source\Driver\PIC16LF153x5\DrvGpio.h"
#include "..\Source\Driver\PIC16LF153x5\DrvNVM.h"
//Standard Include Section
#include "..\Source\Standard\I2C\StdVcnl4200.h"

//Application Include Section
#include "AppStrip.h"

#include <stdlib.h>
/**********************************************************************************************************************/

/***********************************************************************************************************************
; V E R I F Y    C O N F I G U R A T I O N
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
#define MUX_PUBLISH         0x01
#define MUX_PUBLISH_CONFIRM 0x02
#define MUX_PX_GET          0x03
#define MUX_PX_SET          0x04
#define MUX_PX_CONFIG       0x05
#define MUX_COLOR_SET       0x06
#define MUX_REFAB           0x07


#define INT_INTGIE  0x80        //Interrupt Global Enable

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/
typedef enum {
    SCI_IDLE,
    SCI_GUID,
    SCI_MUX,
    SCI_RGBW,
    SCI_END
}
CMD_STATE;

typedef enum {
    PX_IDLE,
    PX_STARTUP,
    PX_RUNNING,
    PX_COUNTING
}
PX_STATE;
/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static UNSIGNED_8 guid[18];
static UNSIGNED_8 mui[9];
static DRVPWM_DRV_HNDL pwm_red;
static DRVPWM_DRV_HNDL pwm_grn;
static DRVPWM_DRV_HNDL pwm_blu;
static DRVPWM_DRV_HNDL pwm_whi;
static UNSIGNED_8 stair_adr;
static UNSIGNED_8 DRIVER_PX_REQUEST_FLAG;

static counter_publish = 0;
static counter_sensor = 0;
static counter_px_wait = 0;

static UNSIGNED_8 r_data[3];
static UNSIGNED_8 s_data[2];
static SIGNED_16 current_px;
static SIGNED_16 previous_px = 0;

static BOOLEAN sensordata = FALSE;
static BOOLEAN px_wait_flag = FALSE;
static UNSIGNED_8 movement_detected = 0;

/**********************************************************************************************************************/

/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
DEVICE_STATE device_state = DEVICE_NO_ADR;
NO_ADR_STATE publish_confirm_state = NO_ADR_IDLE;
ADR_STATE receive_state = ADR_IDLE;
/**********************************************************************************************************************/

/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
UNSIGNED_8 map(UNSIGNED_8 x, UNSIGNED_8 in_min, UNSIGNED_8 in_max, UNSIGNED_8 out_min, UNSIGNED_8 out_max) 
{
    return (UNSIGNED_8) (((float) (x - in_min)*(out_max - out_min)) / ((float) (in_max - in_min) + out_min));
}
/**********************************************************************************************************************/

/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void timer_tick(void) {
    counter_publish++;
    counter_sensor++;
    counter_px_wait++;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppStripInit(void) {
    UNSIGNED_16 address = 0x0000;
    address = DIA_MUI;

    for (UNSIGNED_8 i = 0; i < 9; i++) {
        mui[i] = DrvNVM_read_config(address) & 0xFF;
        address += 1;
    }

    stair_adr = DrvNVM_read_config(0x8000) & 0xFF;
    device_state = (stair_adr == 0xFF) ? DEVICE_NO_ADR : DEVICE_HAS_ADR;

    DrvTimerInit(100, AppStripSensorIsr);

    pwm_whi = DrvPWMRegisterChannel(PWM3, PWM_ACT_H);
    pwm_red = DrvPWMRegisterChannel(PWM4, PWM_ACT_H);
    pwm_grn = DrvPWMRegisterChannel(PWM5, PWM_ACT_H);
    pwm_blu = DrvPWMRegisterChannel(PWM6, PWM_ACT_H);

    DrvGpioInitPin(PORT_A, 4, PIN_IO_FUNC_OUTPUT_LOW);
    DrvGpioInitPin(PORT_A, 5, PIN_IO_FUNC_OUTPUT_LOW);
    DrvGpioInitPin(PORT_C, 3, PIN_IO_FUNC_OUTPUT_LOW);
    DrvGpioInitPin(PORT_C, 2, PIN_IO_FUNC_OUTPUT_LOW);

    DrvPwmStart(pwm_whi, 0);
    DrvPwmStart(pwm_red, 0);
    DrvPwmStart(pwm_grn, 0);
    DrvPwmStart(pwm_blu, 0);

    //Set ISL83483 Drive enable pin as output
    DrvGpioInitPin(PORT_A, 2, PIN_IO_FUNC_OUTPUT_LOW);
}
/*--------------------------------------------------------------------------------------------------------------------*/
DEVICE_STATE AppStripGetState(void) {
    return device_state;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppStripSetGreen(void) {
    DrvPwmStart(pwm_grn, 255);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppStripHandler(void) {
    static UNSIGNED_8 data = 0x00;
    static UNSIGNED_8 i = 0;

    //Serial Communication
    if (DrvSciReadData(&data)) 
    {
        switch (device_state) 
        {
            case DEVICE_NO_ADR:
                switch (publish_confirm_state) {
                    case NO_ADR_IDLE:
                        if (data == MUX_PUBLISH_CONFIRM)
                            publish_confirm_state = NO_ADR_RECEIVED_PUBLISH_CONFIRM;
                        break;
                    case NO_ADR_RECEIVED_PUBLISH_CONFIRM:
                        if (data == mui[i]) {
                            i++;
                            if (i == 9) {
                                publish_confirm_state = NO_ADR_RECEIVED_CORRECT_MUI;
                                i = 0;
                            }
                        } else {
                            i = 0;
                            publish_confirm_state = NO_ADR_IDLE;
                        }

                        break;
                    case NO_ADR_RECEIVED_CORRECT_MUI:
                        //UNSIGNED_8 config_ok = (data & 0x08) >> 7;
                        //UNSIGNED_8 stair_up = (data & 0x40) >> 6;
                        //UNSIGNED_8 stair_down = (data & 0x20) >> 5;
                        //UNSIGNED_8 stair_adr = (data & 0x1F); // 5 LBS are ADR

                        //Write stair number to NVM UserID location 0x8000
                        stair_adr = (data & 0x1F);
                        DrvNVM_write_config(0x8000, stair_adr);
                        device_state = DEVICE_HAS_ADR;

                        break;

                }
                break;
            case DEVICE_HAS_ADR:
                switch (receive_state) {
                    case ADR_IDLE:
                        if (data == MUX_COLOR_SET)
                            receive_state = ADR_RECEIVED_COLOR_SET;
                        else if (data == MUX_PX_CONFIG)
                            receive_state = ADR_RECEIVED_PX_CONFIG;
                        else if (data == MUX_PX_GET)
                            receive_state = ADR_RECEIVED_PX_GET;
                        else if (data == MUX_REFAB)
                            receive_state = ADR_RECEIVED_REFAB;
                        break;
                    case ADR_RECEIVED_PX_CONFIG:
                        receive_state = (data == stair_adr) ? ADR_CORRECT_PX_CONFIG : ADR_IDLE;
                        break;
                    case ADR_RECEIVED_PX_GET:
                        receive_state = (data == stair_adr) ? ADR_CORRECT_PX_GET : ADR_IDLE;
                        break;
                    case ADR_RECEIVED_COLOR_SET:
                        receive_state = (data == stair_adr) ? ADR_CORRECT_COLOR_SET : ADR_IDLE;
                        break;
                    case ADR_RECEIVED_REFAB:
                        receive_state = (data == stair_adr) ? ADR_CORRECT_REFAB : ADR_IDLE;
                        break;
                    case ADR_CORRECT_PX_CONFIG:
                        i++;
                        if (i == 1) {
                            //UNSIGNED_8 treshold_maximum = data;
                        }
                        if (i == 2) {
                            //UNSIGNED_8 treshold_minimum = data;
                            receive_state = ADR_IDLE;
                            i = 0;
                        }
                        receive_state = ADR_IDLE;
                        break;
                    case ADR_CORRECT_PX_GET:
                        if (data == 0x55) {
                            AppStripPXGet(&stair_adr, movement_detected);
                            //Only clear movement bit when driver asked
                            movement_detected = 0;
                        }
                        receive_state = ADR_IDLE;
                        break;
                    case ADR_CORRECT_COLOR_SET:
                        i++;
                        if (i == 1) {
                            DrvPwmDutycycleUpdate(pwm_whi, data);
                            //DrvPwmDutycycleUpdate(pwm_whi, map(data, 0, 255, 0, 100));
                        }
                        if (i == 2) {
                            DrvPwmDutycycleUpdate(pwm_red, data);
                            //DrvPwmDutycycleUpdate(pwm_red, map(data, 0, 255, 0, 100));
                        }
                        if (i == 3) {
                            DrvPwmDutycycleUpdate(pwm_grn, data);
                            //DrvPwmDutycycleUpdate(pwm_grn, map(data, 0, 255, 0, 100));
                        }
                        if (i == 4) {
                            DrvPwmDutycycleUpdate(pwm_blu, data);
                            //DrvPwmDutycycleUpdate(pwm_blu, map(data, 0, 255, 0, 100));
                            receive_state = ADR_CRC;
                            i = 0;
                        }
                        break;
                    case ADR_CORRECT_REFAB:         //Reset to factory settings
                        if(data == 0x55)
                        {
                            DrvNVM_erase_config(0x8000);
                            AppStripInit();
                        }
                        break;
                    case ADR_CRC:           //Check for CRC
                        receive_state = ADR_IDLE;
                        break;
                }
                break;
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppStripPXGet(UNSIGNED_8 *address, UNSIGNED_8 movement) {
    UNSIGNED_8 write_data[3];
    write_data[0] = MUX_PX_SET;
    write_data[1] = *address;
    write_data[2] = !movement << 7;
    write_data[2] |= movement << 3;
    //write_data[2] = ((!movement) << 7) | (movement << 3);

    DrvSciWriteData(write_data, 3);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppStripTimerTick() {
    SIGNED_16 slope = 0;

    // Na 20 timer interrupts (2s) stuur publish op bus
    if (counter_publish >= 20 && device_state == DEVICE_NO_ADR) {
        counter_publish = 0;
        AppStripPublish(mui, 0, 1);
    };

    if (counter_px_wait > 20) {
        counter_px_wait = 0;
        px_wait_flag = FALSE;
    }

    // Na 3 timer interrupts (300ms) lees px sensor
    if (counter_sensor >= 3 && device_state == DEVICE_HAS_ADR) {
        StdVcnl4200ReadPS(s_data);
        previous_px = current_px;
        current_px = (s_data[1] << 8) + s_data[0];

        /*check the slope (change) of px data to see if there is movement
         a = (y2-y1)/(x2-x1)
         because x2 = 1 & x1 = 0 the slope is current - previous
        slope = (current_px - previous_px) / (1 - 0);*/

        slope = current_px - previous_px;
        slope = abs(slope);

        //        UNSIGNED_8 write_data[3];
        //        write_data[0] = 0xFF;
        //        write_data[1] = (slope & 0xFF00) >> 8;
        //        write_data[2] = (slope & 0x00FF);
        //        DrvSciWriteData(write_data, 3);

        if (!px_wait_flag && (slope > 0x0100)) {
            px_wait_flag = TRUE;
            counter_px_wait = 0;

            movement_detected = 1;

        }
        counter_sensor = 0;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppStripPublish(UNSIGNED_8 *mui, BOOLEAN error, BOOLEAN config) {
    UNSIGNED_8 write_data[11]; //| Byte 0: 0x00 | Byte 1-9 | Byte 10: Error/config, see paper

    write_data[0] = MUX_PUBLISH;
    for (int i = 0; i <= 8; i++) {
        write_data[i + 1] = mui[i]; // Place mui in write_data array
    }
    write_data[10] = (error << 8) | config;

    DrvSciWriteData(write_data, 11);
}

/*--------------------------------------------------------------------------------------------------------------------*/
void AppStripSensorIsr(void) {
    sensordata = TRUE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppStripClearGPIO(void) {
    DrvGpioInitPin(PORT_A, 4, PIN_IO_FUNC_OUTPUT_LOW);
    DrvGpioInitPin(PORT_A, 5, PIN_IO_FUNC_OUTPUT_LOW);
    DrvGpioInitPin(PORT_C, 3, PIN_IO_FUNC_OUTPUT_LOW);
    DrvGpioInitPin(PORT_C, 2, PIN_IO_FUNC_OUTPUT_LOW);

    //Testing: Set uart TX pin output
    DrvGpioInitPin(PORT_C, 5, PIN_IO_FUNC_OUTPUT_LOW);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void AppDrvTxHandler(void) {
    DrvTxHandler();
}
/**********************************************************************************************************************/


