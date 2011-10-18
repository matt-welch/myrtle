/***************************************************************************************************************
 * FILE: file.c
 *
 * DESCRIPTION:
 * File I/O functions.
 *
 * AUTHORS: Kevin R. Burger (burgerk@asu.edu) [KRB]
 *
 * MODIFICATION HISTORY:
 * 20111010T1728 [JMW] added static function prototypes
 * ------------------------------------------------------------------------------------------------------------
 * 01 Oct 2011 [KRB] Initial revision.
 **************************************************************************************************************/

/* Write necessary #include directives here. Hint: there should be FIVE. HintHint: file.h is one of them. */
#include "file.h"
#include "globals.h"
#include "bool.h"

/* main.c for main_terminate_err()*/
#include "main.h"

/* stdio for fscanf() */
#include <stdio.h>

/* string for strcpy() */
#include <string.h>

/*--------------------------------------------------------------------------------------------------------------
 * TYPEDEFS
 *
 * This structure type defines the static global variables for this module:
 *
 * in_fname  -- A C-string which stores the input file name parsed from the -i command line option.
 * out_fname -- A C-string which stores the output file name parsed from the -o command line option.
 * fin       -- The input file stream. Will either be stdin or an input file.
 * out       -- The output file stream. Will either be stdout or an output file.
 *------------------------------------------------------------------------------------------------------------*/
typedef struct {
    char  in_fname[128];
    char  out_fname[128];
    FILE *fin;
    FILE *fout;
} global_t;

/*--------------------------------------------------------------------------------------------------------------
 * GLOBAL VARIABLE DEFINITIONS
 * 
 * 'globals' is a global variable structure which stores the global variables. This statement defines globals
 * and initializes each of the global variable. Note that the initialization of global variables is performed
 * once as the program is loaded into memory and before it begins executing.
 *------------------------------------------------------------------------------------------------------------*/
static global_t globals = {
    { '\0' },  /* Each element of in_fname[] is initialized to the null char.  */
    { '\0' },  /* Each element of out_fname[] is initialized to the null char. */
    NULL,      /* fin is initialized to NULL.                                  */
    NULL       /* fout is initialized to NULL.                                 */
};

/*--------------------------------------------------------------------------------------------------------------
 * STATIC FUNCTION DECLARATIONS (PROTOTYPES)
 *------------------------------------------------------------------------------------------------------------*/

/*
 * Write function declarations here for the static functions of this source code file. These function declara-
 * tions are necessary so that the functions can be properly called since they may be located in the source
 * code file below the point where they are called. That is the entire point of function declarations, i.e., a
 * function declaration gives the compiler three things: (1) the name of the function; (2) the data type of the
 * return value (or void); and (3) the number and data types of the parameters. In order to generate the code
 * to call a function, the compiler must know those three things. Hint: there should be four declarations.
 */

static void _file_close_in() ;
static void _file_close_out();
static void _file_open_in();
static void _file_open_out();

/*======================================= NONSTATIC FUNCTION DEFINITIONS =====================================*/

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: file_close_files()
 * DESCR:    Closes the input and output files.    
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
void file_close_files() {
    /* Call the static functions to close the input and output files. */
	_file_close_in();
	_file_close_out();
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: file_next_token()
 * DESCR:    Returns the next string (i.e., in programming language terms, these "words" are called "tokens")
 *           from the input source code file.
 * RETURNS:  A pointer to a char buffer containing the token, or NULL on EOF.
 *------------------------------------------------------------------------------------------------------------*/
char *file_next_token() {
    /* A static local variable is one that is allocated statically, i.e., the variable comes into existence
     * when the program begins executing and the variable remains allocated until the program terminates.
     * However, unlike a global variable, a static local variable only has scope from the point where it is
     * defined to the end of the function. Therefore the scope of 'buffer' is basically this function. The
     * reason buffer is static is because we return a pointer to this buffer, and when we return from the
     * function, we do not want the buffer to be deallocated. I could have defined buffer as a static global
     * variable in this source code file, but then it would be global to every function in this source code
     * file, but this is the only function in which it is used. Therefore, it is better to define it as a
     * static local variable.
     */
    static char buffer[32];
    int read = fscanf(globals.fin, "%s", buffer);  /* & not needed because buffer is an array.     */
    return ((read == 1) ? buffer : NULL);            /* fscanf() returns the number of strings read. */
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: file_open_files()
 * DESCR:    Opens both the input and output files.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
void file_open_files() {
    /* Call the static functions to open the input and output files. */
	_file_open_in();
	_file_open_out();
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: file_set_in_fname()
 * DESCR:    Mutator function for the globals.in_fname variable.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
void file_set_in_fname(char *fname) {
    strcpy(globals.in_fname, fname);
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: file_set_out_fname()
 * DESCR:    Mutator function for the globals.out_fname variable.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
void file_set_out_fname(char *fname) {
    /* Copy the string 'fname' to globals.out_fname. */
    strcpy(globals.out_fname, fname);
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: file_write_char()
 * DESCR:    Writes one character to the output file.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
void file_write_char(char ch) {
    fprintf(globals.fout, "%c", ch);
}

/*========================================= STATIC FUNCTION DEFINITIONS ======================================*/

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _file_close_in()
 * DESCR:    Closes the input file stream.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
static void _file_close_in() {
    if (globals.fin != stdin) fclose(globals.fin);  /* Don't close stdin. */
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _file_close_out()
 * DESCR:    Closes the output file stream.
 * RETURNS:  Nothing
 *------------------------------------------------------------------------------------------------------------*/
static void _file_close_out() {
    if (globals.fout != stdout) fclose(globals.fout);  /* Don't close stdout. */
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _file_open_in()
 * DESCR:    If an input filename was specified on the command line with the -i option, then this function
 *           opens that file for reading. The mode "rt" means read/text and is used to open a text file for
 *           reading. If the -i command line option was not specified, then the globals.in_fname variable
 *           will be the empty string (i.e., the first char will be '\0'). In this case, we are reading from
 *           stdin, so globals.fin will be set to stdin.
 * RETURNS:  Nothing. If the filename specified with the -i command line option cannot be opened, then the
 *           program terminates with an error code of TERM_ERR_INPUT. Otherwise, globals.fin will be a valid
 *           file stream pointer.
 *------------------------------------------------------------------------------------------------------------*/
static void _file_open_in() {
    globals.fin = *globals.in_fname ? fopen(globals.in_fname, "rt") : stdin;
    if (!globals.fin) {
        char buffer[128];
        sprintf(buffer, "Cannot open input file '%s'", globals.in_fname);
        main_terminate_err(buffer, TERM_ERR_INPUT);
    }
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _file_open_out()
 * DESCR:    If an output filename was specified on the command line with the -o option, then this function
 *           opens that file for writing. The mode "wt" means write/text and is used to open a text file for
 *           writing. If the -o command line option was not specified, then the globals.out_fname variable
 *           will be the empty string (i.e., the first char will be '\0'). In this case, we are writing to
 *           stdout, so globals.fout will be set to stdout.
 * RETURNS:  Nothing. If the filename specified with the -o command line option cannot be opened, then the
 *           program terminates with an error code of TERM_ERR_OUTPUT. Otherwise, globals.fout will be a valid
 *           file stream pointer.
 *------------------------------------------------------------------------------------------------------------*/
static void _file_open_out() {
    globals.fout = *globals.out_fname ? fopen(globals.out_fname, "wt") : stdout;
    if (!globals.fout) {
        char buffer[128];
        sprintf(buffer, "Cannot open outut file '%s'", globals.out_fname);
        main_terminate_err(buffer, TERM_ERR_INPUT);
    }
}
