/**********************************************************************************************************************/
/**
 * @file        StdVcnl4200.c
 *
 * @author      Stijn Vermeersch
 * @date        13.04.2020
 *
 * @brief       
 *
 * \n<hr>\n
 * Copyright (c) 2021, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#define STDVCNL4200_C
/**********************************************************************************************************************/



/***********************************************************************************************************************
; V E R I F Y    C O N F I G U R A T I O N
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include "..\..\System\PIC16LF153x5\SysLibAll.h"
//DRIVER lib include section
#include "..\..\Driver\PIC16LF153x5\DrvI2C.h"
#include "..\..\Driver\PIC16LF153x5\DrvSci.h"
//STANDARD lib include section
#include "StdVcnl4200.h"
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
/* ADP5585 I2C ADDRESS*/
#define VCNL4200                    0x51		//7-bit slave address
/* ADP5585 Common Registers */
#define VCNL4200_ALSCONF			0x00
#define VCNL4200_PS_CONF1_2         0x03		//PS duty ratio, integration time, persistence, and PS enable / disable
#define VCNL4200_PS_CONF3_MS        0x04		//PS multi pulse, active force mode, enable sunlight cancellation
#define VCNL4200_PS_CANC_L          0x05		//PS cancellation level setting, LSB
#define VCNL4200_PS_CANC_H          0x05		//PS cancellation level setting, MSB
#define VCNL4200_PS_THDL            0x06		//PS low interrupt threshold setting
#define VCNL4200_PS_THDH            0x07		//PS high interrupt threshold setting
#define VCNL4200_PS_Data            0x08		//PS output data
#define VCNL4200_ID          		0x0E		//Device ID
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
void StdVcnl4200WriteCmd(UNSIGNED_8* reg, UNSIGNED_8* data);
void StdVcnl4200ReadCmd(UNSIGNED_8* reg, UNSIGNED_8* data);
void StdVcnl4200ReadId(void);
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static UNSIGNED_8 reg = 0x00;
static UNSIGNED_8 sdata[2];
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static DRVI2C_DEV_HNDL i2c0;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void StdVcnl4200WriteCmd(UNSIGNED_8* reg, UNSIGNED_8* data)
{
    DrvI2CMasterTransmission(i2c0, reg, data, 2);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdVcnl4200ReadCmd(UNSIGNED_8* reg, UNSIGNED_8* data)
{
    DrvI2CMasterReception(i2c0, reg, data, 2);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdVcnl4200ReadId(void)
{
    reg = VCNL4200_ID;
    StdVcnl4200ReadCmd(&reg, sdata);
}
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void StdVcnl4200Init(void)
{
    i2c0 =  DrvI2CRegisterDevice(I2C_CHANNEL_A, VCNL4200, 400000, I2C_MASTER);
    //ALS Configuration
    // Low register first
    reg = VCNL4200_ALSCONF;                 
    sdata[0] = 0x01;                            //ALS Shutdown
    sdata[1] = 0x00;                            //Default 0x00
    StdVcnl4200WriteCmd(&reg, sdata);
    reg = VCNL4200_PS_CONF1_2;
    sdata[0] = 0x0A;                            //IRED duty 1/160; IT = 9T; PS = Power ON | 0b00001010
    sdata[1] = 0x00;                            //PS output is 12 bits; Interrupt disable
    StdVcnl4200WriteCmd(&reg, sdata);
    reg = VCNL4200_PS_CONF3_MS;
    sdata[0] = 0x20;                            //PS multipulse numbers = 4; 2x typical sunlight immunity; sunlight cancellation enable                                          //0b0010 0000
    sdata[1] = 0x21;                            //PS-detection logic output mode; 1.5x typical sunlight capability; LED_I = 75mA
                                                //Led I mag op 50mA ingesteld worden. | 0b00100001
    StdVcnl4200WriteCmd(&reg, sdata);
    reg = VCNL4200_PS_THDL;
    sdata[0] = 0x20;                            //PS low int. threshold setting (16-bit)
    sdata[1] = 0x00;
    StdVcnl4200WriteCmd(&reg, sdata);
    reg = VCNL4200_PS_THDH;
    sdata[0] = 0x20;                            //PS high int. threshold setting (16-bit))
    sdata[1] = 0xFF;
    StdVcnl4200WriteCmd(&reg, sdata);
    
    //StdVcnl4200ReadId();
    
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdVcnl4200ReadPS(UNSIGNED_8* data)
{
    reg = VCNL4200_PS_Data;
    StdVcnl4200ReadCmd(&reg, data);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/


