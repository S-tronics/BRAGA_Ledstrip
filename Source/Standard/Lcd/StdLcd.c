/**********************************************************************************************************************/
/**
 * @file        StdLcd.c
 *
 * @author      Stijn Vermeersch
 * @date        30.07.2020
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
#define STDLCD_C
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
#include "..\..\Driver\PIC18F2X\DrvGpio.h"
//STANDARD lib include section
#include "StdLcd.h"
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
#define LCD_X           
#define LCD_Y
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/
void StdLcdPort(char a);
void StdLcdCmd(char a);
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
static LCD_PORT lcdport;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void StdLcdPort(char a)
{
	if(a & 1)
        DrvGpioSetPin(lcdport.D4.port, lcdport.D4.pin);
	else
		DrvGpioClearPin(lcdport.D4.port, lcdport.D4.pin);
	if(a & 2)
		DrvGpioSetPin(lcdport.D5.port, lcdport.D5.pin);
	else
		DrvGpioClearPin(lcdport.D5.port, lcdport.D5.pin);
	if(a & 4)
		DrvGpioSetPin(lcdport.D6.port, lcdport.D6.pin);
	else
		DrvGpioClearPin(lcdport.D6.port, lcdport.D6.pin);
	if(a & 8)
		DrvGpioSetPin(lcdport.D7.port, lcdport.D7.pin);
	else
		DrvGpioClearPin(lcdport.D7.port, lcdport.D7.pin);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdLcdCmd(char a)
{
    DrvGpioClearPin(lcdport.RES.port, lcdport.RES.pin);
	StdLcdPort(a);
    DrvGpioSetPin(lcdport.EN.port, lcdport.EN.pin);
    __delay_ms(4);
    DrvGpioClearPin(lcdport.EN.port, lcdport.EN.pin);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void StdLcdInit(LCD_PORT port)
{
    lcdport = port;
    //D4
    DrvGpioInitPin(port.D4.port, port.D4.pin, PIN_IO_FUNC_OUTPUT_LOW);
    //D5
    DrvGpioInitPin(port.D5.port, port.D5.pin, PIN_IO_FUNC_OUTPUT_LOW);
    //D6
    DrvGpioInitPin(port.D6.port, port.D6.pin, PIN_IO_FUNC_OUTPUT_LOW);
    //D7
    DrvGpioInitPin(port.D7.port, port.D7.pin, PIN_IO_FUNC_OUTPUT_LOW);
    //EN
    DrvGpioInitPin(port.EN.port, port.EN.pin, PIN_IO_FUNC_OUTPUT_LOW);
    //RES
    DrvGpioInitPin(port.RES.port, port.RES.pin, PIN_IO_FUNC_OUTPUT_LOW);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdLcdConfig(void)
{
  StdLcdPort(0x00);
  __delay_ms(20);
  StdLcdCmd(0x03);
  __delay_ms(5);
  StdLcdCmd(0x03);
  __delay_ms(11);
  StdLcdCmd(0x03);
  StdLcdCmd(0x02);
  StdLcdCmd(0x02);
  StdLcdCmd(0x08);
  StdLcdCmd(0x00);
  StdLcdCmd(0x0C);
  StdLcdCmd(0x00);
  StdLcdCmd(0x06);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdLcdClear(void)
{
	StdLcdCmd(0);
	StdLcdCmd(1);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdLcdSetCursor(char a, char b)
{
	char temp,z,y;
	if(a == 1)
	{
	  temp = 0x80 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		StdLcdCmd(z);
		StdLcdCmd(y);
	}
	else if(a == 2)
	{
		temp = 0xC0 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		StdLcdCmd(z);
		StdLcdCmd(y);
	}
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdLcdWriteChar(char a)
{
   char temp,y;
   temp = a & 0x0F;
   y = a & 0xF0;
   DrvGpioSetPin(lcdport.RES.port, lcdport.RES.pin);
   StdLcdPort(y>>4);             //Data transfer
   DrvGpioSetPin(lcdport.EN.port, lcdport.EN.pin);
   __delay_us(40);
   DrvGpioClearPin(lcdport.EN.port, lcdport.EN.pin);
   StdLcdPort(temp);
   DrvGpioSetPin(lcdport.EN.port, lcdport.EN.pin);
   __delay_us(40);
   DrvGpioClearPin(lcdport.EN.port, lcdport.EN.pin);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdLcdWriteString(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   StdLcdWriteChar(a[i]);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdLcdShiftRight(void)
{
	StdLcdCmd(0x01);
	StdLcdCmd(0x0C);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void StdLcdShiftLeft(void)
{
	StdLcdCmd(0x01);
	StdLcdCmd(0x08);
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/


