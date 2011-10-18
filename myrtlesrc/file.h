/***************************************************************************************************************
 * FILE: file.h
 *
 * DESCRIPTION:
 * 
 *
 * AUTHORS: Kevin R. Burger (burgerk@asu.edu) [KRB]
 *
 * MODIFICATION HISTORY:
 * 20111010T1728 [JMW] added ifndef, define, directives to prevent multiple inclusion
 * 20111010T1729 [JMW] added nonstatic fcn prototypes
 * ------------------------------------------------------------------------------------------------------------
 * 01 Oct 2011 [KRB] Initial revision.
 **************************************************************************************************************/

/* What goes here at the beginning of a header file? */
#ifndef __FILE_H__
#define __FILE_H__

/*--------------------------------------------------------------------------------------------------------------
 * NONSTATIC FUNCTION DECLARATIONS (PROTOTYPES)
 *
 * Note that in a function declaration, the data types of the parameters must be specified, but the parameter
 * name can be omitted, although it does not hurt to write one, especially if it helps the reader to understand
 * what the parameter is or does.
 *
 * Hint: think of the word "extern" as meaning "public".
 *------------------------------------------------------------------------------------------------------------*/

/* 
 * Write function declarations for the nonstatic functions defined in file.c. Note that the word "extern" at
 * the beginning of the line tells the compiler that these are function declarations for functions defined in
 * an "external" file, aka, another source code file. Hint: these should be six function declarations here.
 */
extern void file_close_files();
extern char *file_next_token();
extern void file_open_files();
extern void file_set_in_fname(char *fname);
extern void file_set_out_fname(char *fname);
extern void file_write_char(char ch);

/* What goes here at the end of a header file? */
#endif
