/**********************************************************************************************************************/
/**
 * @file        SysTypes.h
 *
 * @author      Stijn Vermeersch
 * @date        02.01.2017
 *
 * @brief       Headerfile for the system types configuration
 *
 * Headerfile for the system types configuration\n
 *
 *
 * \n<hr>\n
 * Copyright (c) 2017, S-tronics\n
 * All rights reserved.
 * \n<hr>\n
 */
/**********************************************************************************************************************/
#ifndef SYSTYPES_H
#define SYSTYPES_H
/**********************************************************************************************************************/



/***********************************************************************************************************************
; I N C L U D E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S Y M B O L   D E F I N I T I O N S   A N D   M A C R O S
;---------------------------------------------------------------------------------------------------------------------*/
//#ifndef FALSE
//	#define FALSE 	0
//#ifndef TRUE
//	#define TRUE	!FALSE
//#endif
//#endif

#define VPTR	void*			//Define of void pointer

#define STRING  char*

//#define inline  __inline 

//Define of NULL value									 
//#define NULL ((void *)0)


#define SIZEOF(param) ((sizeof param))

//A set of 4 MSB-bits out of the 			   
#define NIBBLE3(nibble)	((nibble >> 12) & 0x000F)
#define	NIBBLE2(nibble)	((nibble >> 8) & 0x000F)
#define NIBBLE1(nibble)	((nibble >> 4) & 0x000F)
#define	NIBBLE0(nibble) ((nibble) & 0x000F)
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   T Y P E D E F S
;---------------------------------------------------------------------------------------------------------------------*/

//Type definition for an unsigned 8-bit wide character
typedef unsigned char 		UNSIGNED_8;	 

typedef unsigned char 		CHAR;

//Type definition for a signed 8-bit wide character
typedef signed char			SIGNED_8;

//Type definition for an unsigned 16-bit wide word
typedef unsigned int		UNSIGNED_16;

//Type definition for a signed 16-bit wide word
typedef signed int          SIGNED_16;

//Type definition for an unsigned 32-bit wide long word
typedef unsigned long		UNSIGNED_32;

//Type definition for a signed 32-bit wide character
typedef signed long			SIGNED_32;

//typedef unsigned long long 	UNSIGNED_64;

//typedef signed long long	SIGNED_64;

typedef float				FLOAT;

typedef double				DOUBLE;

//Type definition for a string declaration


			
//Type definition for boolean variable
typedef enum
{
	FALSE = 0,
	TRUE 	
}
BOOLEAN;


/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   V A R I A B L E S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   F U N C T I O N   P R O T O T Y P E S
;---------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************************************************************/



/***********************************************************************************************************************
; E X P O R T E D   S T A T I C   I N L I N E   F U N C T I O N S
;---------------------------------------------------------------------------------------------------------------------*/
/**********************************************************************************************************************/



#endif /* SYSTYPES_H */
