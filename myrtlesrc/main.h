/***************************************************************************************************************
 * FILE: main.h
 *
 * DESCRIPTION:
 * See comments in main.c.
 *
 * AUTHORS: Kevin R. Burger (burgerk@asu.edu) [KRB]
 *
 * MODIFICATION HISTORY:
 * ------------------------------------------------------------------------------------------------------------
 * 01 Oct 2011 [KRB] Initial revision.
 **************************************************************************************************************/
#ifndef __MAIN_H__   /* Remember, this trick is used to prevent a header file from being included more than */
#define __MAIN_H__   /* once in a source code file. It's a completely sleazy hack, but that's the way C is. */

/*--------------------------------------------------------------------------------------------------------------
 * NONSTATIC FUNCTION DECLARATIONS (PROTOTYPES)
 *
 * See the comment concerning static functions in main.c. Note that in a function declaration the names of the
 * parameters can be omitted. Why? Because in order to generate the code for the function call, the compiler
 * only needs to know the data types of the parameters. The names of the formal parameters in the function
 * definition are irrelevant to the compiler when it is generated the code for the function call.
 *------------------------------------------------------------------------------------------------------------*/
extern void main_terminate_err(char *, int);

#endif  /* This #endif matches the #ifndef that begins on line 13. */
