/**********************************************************************************************************************/
/**
 * @file        StdAdp5585.c
 *
 * @author      Stijn Vermeersch
 * @date        27.07.2020
 *
 * @brief       
 *
 *
 * \n<hr>\n
 * Copyright (c) 2020, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#define STDADP5585_C
/**********************************************************************************************************************/



/***********************************************************************************************************************
; V E R I F Y    C O N F I G U R A T I O N
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/
#include "..\..\System\PIC18F2X\SysLibAll.h"
//DRIVER lib include section
#include "..\..\Driver\PIC18F2X\DrvI2C.h"
//STANDARD lib include section
#include "StdAdp5585.h"
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
/* ADP5585 I2C ADDRESS*/
#define ADP5585P                    0x34
/* ADP5585 Common Registers */
#define ADP5585_ID                  0x00
#define ADP5585_INT_STATUS          0x01
#define ADP5585_STATUS              0x02
#define ADP5585_FIFO_1              0x03
#define ADP5585_FIFO_2              0x04
#define ADP5585_FIFO_3              0x05
#define ADP5585_FIFO_4              0x06
#define ADP5585_FIFO_5              0x07
#define ADP5585_FIFO_6              0x08
#define ADP5585_FIFO_7              0x09
#define ADP5585_FIFO_8              0x0A
#define ADP5585_FIFO_9              0x0B
#define ADP5585_FIFO_10             0x0C
#define ADP5585_FIFO_11             0x0D
#define ADP5585_FIFO_12             0x0E
#define ADP5585_FIFO_13             0x0F
#define ADP5585_FIFO_14             0x10
#define ADP5585_FIFO_15             0x11
#define ADP5585_FIFO_16             0x12
#define ADP5585_GPI_INT_STAT_A      0x13
#define ADP5585_GPI_INT_STAT_B      0x14
#define ADP5585_GPI_STATUS_A		0x15
#define ADP5585_GPI_STATUS_B		0x16
#define ADP5585_RPULL_CONFIG_A		0x17
#define ADP5585_RPULL_CONFIG_B		0x18
#define ADP5585_RPULL_CONFIG_C		0x19
#define ADP5585_RPULL_CONFIG_D		0x1A
#define ADP5585_GPI_INT_LEVEL_A		0x1B
#define ADP5585_GPI_INT_LEVEL_B		0x1C
#define ADP5585_GPI_EVENT_EN_A		0x1D
#define ADP5585_GPI_EVENT_EN_B		0x1E
#define ADP5585_GPI_INTERRUPT_EN_A	0x1F
#define ADP5585_GPI_INTERRUPT_EN_B	0x20
#define ADP5585_DEBOUNCE_DIS_A		0x21
#define ADP5585_DEBOUNCE_DIS_B		0x22
#define ADP5585_GPO_DATA_OUT_A		0x23
#define ADP5585_GPO_DATA_OUT_B		0x24
#define ADP5585_GPO_OUT_MODE_A		0x25
#define ADP5585_GPO_OUT_MODE_B		0x26
#define ADP5585_GPIO_DIRECTION_A	0x27
#define ADP5585_GPIO_DIRECTION_B	0x28
#define ADP5585_RESET1_EVENT_A		0x29
#define ADP5585_RESET1_EVENT_B		0x2A
#define ADP5585_RESET1_EVENT_C		0x2B
#define ADP5585_RESET2_EVENT_A		0x2C
#define ADP5585_RESET2_EVENT_B		0x2D
#define ADP5585_RESET_CFG           0x2E
#define ADP5585_PWM_OFFT_LOW		0x2F
#define ADP5585_PWM_OFFT_HIGH		0x30
#define ADP5585_PWM_ONT_LOW         0x31
#define ADP5585_PWM_ONT_HIGH		0x32
#define ADP5585_PWM_CFG             0x33
#define ADP5585_LOGIC_CFG           0x34
#define ADP5585_LOGIC_FF_CFG		0x35
#define ADP5585_LOGIC_INT_EVENT_EN	0x36
#define ADP5585_POLL_PTIME_CFG		0x37
#define ADP5585_PIN_CONFIG_A		0x38
#define ADP5585_PIN_CONFIG_B		0x39
#define ADP5585_PIN_CONFIG_D		0x3A
#define ADP5585_GENERAL_CFG         0x3B
#define ADP5585_INT_EN              0x3C

//==============================================
/*          ADP5585 specific Event defines.   */
//==============================================
//#define ADP5585_KEY_1       0x01
//#define ADP5585_KEY_2       0x02
//#define ADP5585_KEY_3       0x03
//
//#define ADP5585_KEY_4       0x06
//#define ADP5585_KEY_5       0x07
//#define ADP5585_KEY_6       0x08
//
//#define ADP5585_KEY_7       0x0B
//#define ADP5585_KEY_8       0x0C
//#define ADP5585_KEY_9       0x0D
//
//#define ADP5585_KEY_AST     0x10 // Asterisk: *
//#define ADP5585_KEY_0       0x11
//#define ADP5585_KEY_HASHTAG 0x12
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
static UNSIGNED_8 reg = 0x00;
static UNSIGNED_8 data = 0x00;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static DRVI2C_DEV_HNDL i2c0;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void StdAdp5585WriteCmd(UNSIGNED_8 reg, UNSIGNED_8 data)
{
    DrvI2CBeginTransmission(i2c0, I2C_W);
    DrvI2CWriteMstr(i2c0, &reg);
    DrvI2CWriteMstr(i2c0, &data);
    DrvI2CStop(i2c0);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdMtch112ReadCmd(UNSIGNED_8 reg, UNSIGNED_8* data)
{
    DrvI2CBeginTransmission(i2c0, I2C_W);				//Startbit + write address
	DrvI2CWriteMstr(i2c0, &reg);
	DrvI2CRepeatedStart(i2c0);
	DrvI2CWriteAddrMstr(i2c0, I2C_R);
	DrvI2CReadMstr(i2c0, data, TRUE);
    DrvI2CStop(i2c0);
}
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void StdAdp5585Init(void)
{
    i2c0 =  DrvI2CRegisterDevice(I2C_CHANNEL_A, ADP5585P, 400000, I2C_MASTER);
    reg = ADP5585_PIN_CONFIG_A;
    data = 0x0F;
    StdAdp5585WriteCmd(reg, data);
    reg = ADP5585_PIN_CONFIG_B;
    data = 0x07;
    StdAdp5585WriteCmd(reg, data);
    reg = ADP5585_GENERAL_CFG;
    data = 0xA2;
    StdAdp5585WriteCmd(reg, data);
    reg = ADP5585_INT_EN;
    data = 0x01;
    StdAdp5585WriteCmd(reg, data);
    reg = ADP5585_INT_STATUS;
    data = 0x17;
    StdAdp5585WriteCmd(reg, data);        
}
/*--------------------------------------------------------------------------------------------------------------------*/
UNSIGNED_8 StdAdp5585ReadFifo(void)
{
    static UNSIGNED_8 output = 0x00;
            
    StdMtch112ReadCmd(ADP5585_FIFO_1, &output);
    return output;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdAdp5585ClearInterrupt(void)
{
    reg = ADP5585_INT_STATUS;
    data = 0x17;
    StdAdp5585WriteCmd(reg, data); 
}
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/


