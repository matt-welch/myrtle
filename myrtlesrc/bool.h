/***************************************************************************************************************
 * FILE: bool.h
 *
 * DESCRIPTION:
 * Useful user-defined data types.
 *
 * AUTHORS: Kevin R. Burger (burgerk@asu.edu) [KRB]
 *
 * MODIFICATION HISTORY:
 * 20111010T1710 [JMW] added ifndef, define, directives to prevent multiple inclusion
 * ------------------------------------------------------------------------------------------------------------
 * 01 Oct 2011 [KRB] Initial revision.
 **************************************************************************************************************/

/* What goes here at the beginning of a header file? */
#ifndef __BOOL_H__
#define __BOOL_H__

/*--------------------------------------------------------------------------------------------------------------
 * MACRO DEFINITIONS
 *------------------------------------------------------------------------------------------------------------*/
#define false (0)         /* It's good to sometimes parenthesize the hell out of these things to avoid bugs.  */
#define true  (!(false))

/*--------------------------------------------------------------------------------------------------------------
 * TYPE DEFINITIONS
 *
 * The typedef reserved word is used to create what looks like a new data type. But really, all it does is
 * create a new name (i.e., an alias) for an existing data type. The syntax is: typedef existing-type new-type;
 * In this case, we are "creating" a new type named "bool" which can be used to represent boolean variables.
 * Since C does not have a boolean data type, integers are used to represent boolean variables. A signed char
 * is an integer data type (char variables just store an ASCII value). The reserved word tells the compiler
 * that the values placed in a "bool" variable will be either negative or positive. In C, 0 is used to repre-
 * sent false, and any non-zero value is considered to be true (e.g., -1, 1, 12344, and -2992929 are all true)
 *------------------------------------------------------------------------------------------------------------*/
typedef signed char bool;

/* What goes here at the end of a header file? */
#endif
