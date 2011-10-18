/***************************************************************************************************************
 * FILE: globals.c
 *
 * DESCRIPTION:
 * Defines global constants. You may wonder why I am including globals.h in this source code file. The reason
 * is that by including globals.h, the compiler will check that the constant definition (in particular, the
 * data type of the constant) in the .c file matches that of the data type of the constant declaration in the
 * header file. This is not strictly necessary, but it is a good defensive programming mechanism.
 *
 * AUTHORS: Kevin R. Burger (burgerk@asu.edu) [KRB]
 *
 * MODIFICATION HISTORY:
 * ------------------------------------------------------------------------------------------------------------
 * 01 Oct 2011 [KRB] Initial revision.
 **************************************************************************************************************/
#include "globals.h"

/*--------------------------------------------------------------------------------------------------------------
 * GLOBAL CONSTANT DEFINITIONS
 * 
 * Global constants, unlike global variables, are not discouraged. The reason is that a global constant cannot
 * be changed, i.e., it is read-only. Therefore, it is not possible for the type of side effects that can arise
 * with global variables to occur. of course, a constant should only be global to more than one source code
 * file when that constant is actually used in all of those source code files. If a global constant is going
 * to only be used in one source code file, then it should be defined as a "static" global constant in that one
 * source code file.
 *
 * Note that global constants are "defined" and initialized in a .c file. The global constant should be "de-
 * clared" but not initialized in a .h file. See globals.h for the declarations of these two constants. The
 * purpose of declaring the constant in a header file is so that source code files that need to access that
 * constant can access it by simply including the header file. The compiler only needs to see the declaration
 * in order for that constant to be accessible in a source code files.
 *
 * Never define a global variable or constant in a header file. Remember: "definitions" go in .c files and
 * "declarations" go in .h files.
 *------------------------------------------------------------------------------------------------------------*/

/* Define an int constant named MAX_WORLD_ROWS and initialize it to 50. */
const int MAX_WORLD_ROWS = 50;

/* Define an int constant named MAX_WORLD_COLS and initialize it to 50. */
const int MAX_WORLD_COLS = 50;

