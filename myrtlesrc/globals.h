/***************************************************************************************************************
 * FILE: globals.h
 *
 * DESCRIPTION:
 * Global variable declarations. See comments in globals.c.
 *
 * AUTHORS: Kevin R. Burger (burgerk@asu.edu) [KRB]
 *
 * MODIFICATION HISTORY:
 * * 20111010T1716 [JMW] added ifndef, define, directives to prevent multiple inclusion
 * ------------------------------------------------------------------------------------------------------------
 * 01 Oct 2011 [KRB] Initial revision.
 **************************************************************************************************************/

/* What goes here at the beginning of a header file? */
#ifndef __GLOBALS_H__
#define __GLOBALS_H__

/*--------------------------------------------------------------------------------------------------------------
 * PREPROCESSOR MACRO DEFINITIONS
 *
 * The #define preprocessor directive defines a "macro". A macro is just a name for something. In these macros,
 * "TERM_NORM" is equivalent to "0", so whenever the preprocessor sees TERM_NORM in the source code file, it
 * simply replaces that string with the string "0". That is, macros are just textual-substitution of one string
 * for another by the preprocessor. One major use of macros in C is to define named constants, at least this
 * is the way it was historically done before the addition of the "const" reserved word to Standard C. These
 * days, these really should be defined as named constants by writing:
 *
 *     const int TERM_NORM = 0;
 *
 * But I am doing it this way because you need to learn how to do it both ways because you will see both ways
 * in C programs.
 *------------------------------------------------------------------------------------------------------------*/
#define TERM_NORM            0  /* A C program should return 0 from main on normal exit.   */
#define TERM_ERR_INPUT      -1  /* Any non-zero return value indicates an error condition. */
#define TERM_ERR_CMD_LINE   -2
#define TERM_ERR_OUTPUT     -3
#define TERM_ERR_UNK_CMD    -4

/*
 * I hate writing "if (!strcmp(s1, s2))" to compare two strings for equality because I think it is ugly. This
 * macro function allows me to compare two strings for equality by writing "if (streq(s1, s2))..." which I think
 * reads much better.
 */
#define streq(s1,s2) (!strcmp((s1),(s2)))

/*--------------------------------------------------------------------------------------------------------------
 * GLOBAL CONSTANT DECLARATIONS
 *------------------------------------------------------------------------------------------------------------*/
extern const int MAX_WORLD_ROWS;
extern const int MAX_WORLD_COLS;

/* What goes here at the end of a header file? */
#endif
