/**********************************************************************************************************************/
/**
 * @file        gpio\DrvGpio.c
 *
 * @author      Stijn Vermeersch
 * @date        02.01.2017
 *
 * @brief       Basic GPIO manipulation functionality
 *
 * This GPIO manipulating module is designed to be used for initialisation and modifying IO ports and pins.\n
 * The implementation is <em>NOT</em> designed to be FAST!\n
 *
 *
 * \n<hr>\n
 * Copyright (c) 2016, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#define GPIO__DRVGPIO_C
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
#include "DrvGpio.h"
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/

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

//HOOK_GPIO_INCOMING					incoming_gpio_hook;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
UNSIGNED_8 gpiohandler = 0x00;
/**********************************************************************************************************************/



/***********************************************************************************************************************
; L O C A L   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
void DrvGpioInit(void)
{
    
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioInitPin(GPIO_PORT io_port, UNSIGNED_8 io_pin_nr, GPIO_PIN_FUNC io_pin_func)
{
	UNSIGNED_8 io_pin = 0;
    
	io_pin |= (0x01 << io_pin_nr);
	
	DrvGpioInitPinMask(io_port, io_pin, io_pin_func);
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioSetPin(GPIO_PORT io_port, UNSIGNED_8 io_pin_nr)
{
UNSIGNED_8 addr_port = 0x00;

    DrvGpioSetPinMask(io_port, (0x01 << io_pin_nr));
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioClearPin(GPIO_PORT io_port, UNSIGNED_8 io_pin_nr)
{
UNSIGNED_8 addr_port = 0x00;

    DrvGpioClearPinMask(io_port, (0x01 << io_pin_nr));
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioTogglePin(GPIO_PORT io_port, UNSIGNED_8 io_pin_nr)
{
UNSIGNED_8* io_pin_ptr;
UNSIGNED_8 port = 0x00;

	switch(io_port)
		{
		case PORT_A:
			io_pin_ptr = (UNSIGNED_8*) &LATA;
			break;
		case PORT_C:
			io_pin_ptr = (UNSIGNED_8*) &LATC;
			break;
		default:
			break;
		}

	port = *io_pin_ptr;
	port &=	(0x01 << io_pin_nr);

	if(port > 0)
	{
		DrvGpioClearPinMask(io_port, (0x01 << io_pin_nr));
	}
	else
	{
		DrvGpioSetPinMask(io_port, (0x01 << io_pin_nr));
	}
}
/*--------------------------------------------------------------------------------------------------------------------*/
BOOLEAN DrvGpioIsPinHigh(GPIO_PORT io_port, UNSIGNED_8 io_pin_nr)
{
UNSIGNED_8* io_pin_ptr;
UNSIGNED_8 port = 0x00;

	switch(io_port)
		{
		case PORT_A:
			io_pin_ptr = (UNSIGNED_8*) &PORTA;
			break;
		case PORT_C:
			io_pin_ptr = (UNSIGNED_8*) &PORTC;
			break;
		default:
			break;
		}

	port = *io_pin_ptr;
	if(port & (0x01 << io_pin_nr))
	{
		return TRUE;
	}
    return FALSE;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioInitPinMask(GPIO_PORT io_port, UNSIGNED_8 io_pin_mask, GPIO_PIN_FUNC io_pin_func)
{
	UNSIGNED_8* io_dir_ptr;
    UNSIGNED_8* io_set_ptr;
    UNSIGNED_8* io_clr_ptr;
    UNSIGNED_8* io_ansel_ptr;

	switch(io_port)
	{
	case PORT_A:
		io_dir_ptr = (UNSIGNED_8*) &TRISA;
		io_set_ptr = (UNSIGNED_8*) &LATA;
		io_clr_ptr = (UNSIGNED_8*) &LATA;
        io_ansel_ptr = (UNSIGNED_8*) &ANSELA;
		break;
	case PORT_C:
		io_dir_ptr = (UNSIGNED_8*) &TRISC;
		io_set_ptr = (UNSIGNED_8*) &LATC;
		io_clr_ptr = (UNSIGNED_8*) &LATC;
        io_ansel_ptr = (UNSIGNED_8*) &ANSELC;
		break;
	
	default:
		break;
	}

	if(io_pin_func == PIN_IO_FUNC_INPUT)
	{
		*io_dir_ptr |= io_pin_mask;
        *io_ansel_ptr &= ~io_pin_mask;
	}
	else
	{
		*io_dir_ptr &= ~io_pin_mask;
        *io_ansel_ptr &= ~io_pin_mask;
		if(io_pin_func == PIN_IO_FUNC_OUTPUT_LOW)
		{
			*io_clr_ptr &= ~io_pin_mask;
		}
		else
		{
			if(io_pin_func == PIN_IO_FUNC_OUTPUT_HIGH)
			{
				*io_set_ptr |= io_pin_mask;
			}
		}
	}	
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioSetPinMask(GPIO_PORT io_port, UNSIGNED_8 io_pin_mask)
{
    UNSIGNED_8* io_set_ptr;

	switch(io_port)
	{
	case PORT_A:
		io_set_ptr = (UNSIGNED_8*) &LATA;
		break;
	case PORT_C:
		io_set_ptr = (UNSIGNED_8*) &LATC;
		break;
	default:
		break;
	}

	*io_set_ptr |= io_pin_mask;
}
/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioClearPinMask(GPIO_PORT io_port, UNSIGNED_8 io_pin_mask)
{
	UNSIGNED_8* io_clr_ptr;

	switch(io_port)
	{
	case PORT_A:
		io_clr_ptr = (UNSIGNED_8*) &LATA;
		break;
		break;
	case PORT_C:
		io_clr_ptr = (UNSIGNED_8*) &LATC;
		break;
	default:
		break;
	}
	
	*io_clr_ptr &= ~io_pin_mask;
}

/*--------------------------------------------------------------------------------------------------------------------*/
UNSIGNED_8 DrvGpioReadPortData(GPIO_PORT io_port)
{
UNSIGNED_8* io_pin_ptr;

	switch(io_port)
		{
		case PORT_A:
			io_pin_ptr = (UNSIGNED_8*) &PORTA;
			break;
		case PORT_C:
			io_pin_ptr = (UNSIGNED_8*) &PORTC;
			break;
		default:
			break;
		}
		
	return (UNSIGNED_8)(*io_pin_ptr);
}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioIntEnable(GPIO_PORT port, UNSIGNED_8 io_pin_nr)
{		
		switch(port)
		{
			case PORT_A:
				
			 	break;
			case PORT_C:
				
				break;
			default:
			 	break;
		}
}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioIntDisable(GPIO_PORT port, UNSIGNED_8 io_pin_nr)
{
	switch(port)
		{
			case PORT_A:
				
			 	break;
			case PORT_C:
				
				break;
			default:
			 	break;
		}
}

/*--------------------------------------------------------------------------------------------------------------------*/
void DrvGpioIncomingIsr(void)				   
{

   	

}
/*--------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/

