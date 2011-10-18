/***************************************************************************************************************
 * FILE: main.c
 *
 * DESCRIPTION:
 * An interpreter for the Myrtle programming language.
 *
 * AUTHORS: Kevin R. Burger (burgerk@asu.edu) [KRB]
 *
 * MODIFICATION HISTORY:
 * 20111010T1558 [JMW] implemented functions: main(), _main_terminate_norm(), main_terminate_err()
 * ------------------------------------------------------------------------------------------------------------
 * 01 Oct 2011 [KRB] Initial revision.
 **************************************************************************************************************/
#include <stdio.h>    /* For fprintf() declaration.            */
#include <stdlib.h>   /* For exit() declaration.               */
#include <string.h>   /* For strcmp() declaration.             */
#include "bool.h"     /* For bool, false, true.                */
#include "file.h"     /* For declarations in file module.      */
#include "globals.h"  /* For global constant declarations.     */
#include "main.h"     /* For main_termiante_err() declaration. */
#include "myrtle.h"   /* For declarations in myrtle module.    */

/*--------------------------------------------------------------------------------------------------------------
 * STATICALLY GLOBAL PREPROCESSOR MACROS
 *
 * "Static" in this context means that these preprocessor macros are only accessible in this source code file.
 * If something is "non-static" then it can be accessed in other source code files.
 *------------------------------------------------------------------------------------------------------------*/
#define VERSION  "1.0.0"
#define COPY     "2011"
#define AUTHOR   "Kevin R. Burger"

/*--------------------------------------------------------------------------------------------------------------
 * STATIC FUNCTION DECLARATIONS
 *
 * A "static" is one that is defined or declared with the "static" reserved word. Static functions are only
 * visible in the source code file in which they are defined. That is, they cannot be called by functions in
 * other source code files. All functions should be static, unless you need to "export" the function so it can
 * be called from another source code file. For those types of functions, put a function declaration using the
 * reserved word "extern" in a header file with the same name as this source code file. For example, see main.h
 * for the extern function declaration for main_terminate_err().
 *
 * Hint: Think of the word "static" as meaning "private", and the word "extern" as meaning "public".
 *------------------------------------------------------------------------------------------------------------*/
static void _main_help();
static void _main_parse_cmd_line(int argc, char *argv[]);
static void _main_print_version();
static void _main_terminate_norm();

/*===================================== NONSTATIC FUNCTION DEFINITIONS =======================================*/

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: main()
 * DESCR:    Starting point of execution. Simply parses the command line, and if there is a source code file
 *           on the command line, passes the file name to run(). If the command line is invalid, then parse_
 *           command_line does not return. Note: In C, main() should NEVER be defined as a static function.
 *           Otherwise the C startup code will not be able to call it. You will get a linker error if you do
 *           define main() as static. So don't. There, I said it.
 * RETURNS:  Zero on success, non-zero on error.
 *------------------------------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])  {
    /* See what's on the command line. Call _main_parse_cmd_line() and pass argc and argv as parameters. */
	_main_parse_cmd_line(argc, argv);

    /* Call myrtle_interp() and return what it returns. */
    return myrtle_interp();
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: main_terminate_err()
 * DESCR:    Called to terminate the program abnormally with a non-zero return code. This function is declared
 *           nonstatically so it can be called from other source code files. See main.h for the declaration.
 * RETURNS:  Does not return, calls the C library exit() function to terminate the program with a negative
 *           exit code. The convention in Unix is that a program exits with zero on success and a non-zero
 *           value if terminating abnormally.
 *------------------------------------------------------------------------------------------------------------*/
void main_terminate_err(char *err_msg, int err_code) {
    /* 
     * Use fprintf() to print the err_msg string, followed by a period, followed by the string " Terminating."
     * followed by a newline, to stdout.
     */
    fprintf(stdout, "%s. Terminating.\n", err_msg);

    /* 
     * Call the C library exit() function and pass err_code as the parameter. Note that exit() is declared in
     * stdlib.h which is #included. exit() causes the program to terminate immediately. The error code is sent
     * back to the shell (e.g., Bash) and a Bash shell script can examine the return code to see if the program
     * terminated normally (the return code should be 0) or abnormally (the return code should be nonzero)
     */
    exit(err_code);
}

/*======================================= STATIC FUNCTION DEFINITIONS ========================================*/

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _main_help()
 * DESCR:    Displays the help message and then calls terminate_norm().
 * RETURNS:  Nothing
 *------------------------------------------------------------------------------------------------------------*/
static void _main_help() {
    fprintf(stdout, "A Myrtle programming language interpreter.\n");
    fprintf(stdout, "Usage: myrtle [options]\n\n");
    fprintf(stdout, "If there are no command line options, then Myrtle reads commands from\n");
    fprintf(stdout, "stdin and performs them and writes the output to stdout.\n\n");
    fprintf(stdout, "Options:\n");
    fprintf(stdout, "-i file    Reads commands from 'file'.\n");
    fprintf(stdout, "-o file    Sends output to 'file'.\n");
    fprintf(stdout, "-h         Displays this help message and terminates.\n");
    fprintf(stdout, "-V         Verbose mode. Displays commands as they are performed.\n");
    fprintf(stdout, "-v         Displays the version of the Myrtle interpreter and terminates.\n");
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _main_parse_cmd_line()
 * DESCR:    Examines the command line for the source code file and other command line options.
 * RETURNS:  Nothing
 *------------------------------------------------------------------------------------------------------------*/
static void _main_parse_cmd_line(int argc, char *argv[]) {
    int i;

    /* Call myrtle_verbose_set() to set the verbose flag to false. */
    myrtle_verbose_set(false);

    for (i = 1; i < argc; i++) {
        if (streq(argv[i], "-i")) {
            file_set_in_fname(argv[++i]);
        } else if (streq(argv[i], "-o")) {
            file_set_out_fname(argv[++i]);
        } else if (streq(argv[i], "-h")) {
            _main_print_version();
            _main_help();
            _main_terminate_norm();
        } else if (streq(argv[i], "-V")) {
            /* Call myrtle_verbose_set() to set the verbose flag to true. */
        	myrtle_verbose_set(true);
        } else if (streq(argv[i], "-v")) {
            _main_print_version();
            _main_terminate_norm();
        } else {
            _main_help();
            main_terminate_err("\nInvalid command line", TERM_ERR_CMD_LINE); 
        }
    }
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _main_print_version
 * DESCR:    Prints the version of the Myrtle interpreter.
 * RETURNS:  Nothing
 *------------------------------------------------------------------------------------------------------------*/
static void _main_print_version() {
    fprintf(stdout, "Myrtle (the Turtle) Ver %s -- (c) %s %s\n", VERSION, COPY, AUTHOR);
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _main_terminate_norm()
 * DESCR:    Called to terminate the program normally, i.e., with no error return code.
 * RETURNS:  Norhing
 * NOTES:    This function should defined as static because it is only called from functions in this source
 *           code file. The function should simply call exit() and pass TERM_NORM as the parameter.
 *------------------------------------------------------------------------------------------------------------*/
static void _main_terminate_norm() {
	exit(TERM_NORM);
}
