/***************************************************************************************************************
 * FILE: myrtle.h
 *
 * DESCRIPTION:
 * 
 *
 * AUTHORS: Kevin R. Burger (burgerk@asu.edu) [KRB]
 *
 * MODIFICATION HISTORY:
 * 20111010T1747 [JMW] added ifndef, define directives; added CMD_ macros
 * ------------------------------------------------------------------------------------------------------------
 * 01 Oct 2011 [KRB] Initial revision.
 **************************************************************************************************************/

/* What goes here at the beginning of a header file? */
#ifndef __MYRTLE_H__
#define __MYRTLE_H__

/* You need to #include one header file here. I wonder which one it is. */
#include "globals.h"

/*--------------------------------------------------------------------------------------------------------------
 * PREPROCESSOR MACRO DEFINITIONS
 *
 * Use #define to define CMD_FORWARD as 1, CMD_HYPER as 2, CMD_LEFT as 3, CMD_PENCHAR as 4, CMD_PENDOWN as 5,
 * CMD_PENUP as 6, and CMD_RIGHT as 7.
 *
 * Note: These constants could have been defined using "const int ... ".
 *------------------------------------------------------------------------------------------------------------*/
#define CMD_FORWARD 1
#define CMD_HYPER 2
#define CMD_LEFT 3
#define CMD_PENCHAR 4
#define CMD_PENDOWN 5
#define CMD_PENUP 6
#define CMD_RIGHT 7

/*--------------------------------------------------------------------------------------------------------------
 * NONSTATIC FUNCTION DECLARATIONS (PROTOTYPES)
 *
 * A function declaration just consists of the header of the function. A function must be defined or declared
 * before it can be called. A function declaration can be placed in the source code file above the function
 * call to the function. When the compiler sees the declaration, it will learn three things: (1) the name of
 * the function; (2) the data type of the return value (or void); and (3) the number and data types of the
 * funtions's parameters. It needs all of this information in order to be able to generate the code to call
 * the function.
 *
 * Function declaration are normally place in an header file, and then that header file is included in the
 * source code file that calls the function. This header file is included by main.c (look at it now). The
 * reserved word "extern" is short for "external" and tells the compiler that the function is declared extern-
 * al to the source code file in which the function is being called.
 *
 * Hint: Think of the word "extern" as meaning "public".
 *------------------------------------------------------------------------------------------------------------*/
extern int  myrtle_interp();
extern bool myrtle_verbose_get();
extern void myrtle_verbose_set(bool);

/* What goes here at the end of a header file? */
#endif
