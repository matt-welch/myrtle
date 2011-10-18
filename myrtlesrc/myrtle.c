/***************************************************************************************************************
 * FILE: myrtle.c
 *
 * DESCRIPTION:
 * Implements the Myrtle programming language interpreter.
 *
 * AUTHORS: Kevin R. Burger (burgerk@asu.edu) [KRB]
 *
 * MODIFICATION HISTORY:
 * 20111010T1748 [JMW] added static int MAX_CMDS
 * ------------------------------------------------------------------------------------------------------------
 * 01 Oct 2011 [KRB] Initial revision.
 **************************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bool.h"
#include "file.h"
#include "globals.h"
#include "main.h"
#include "myrtle.h"

/*--------------------------------------------------------------------------------------------------------------
 * GLOBAL PREPROCESSOR MACRO DEFINITIONS
 *
 * I have a good reason for defining these constants as preprocessor macros rather than using the const reserv-
 * ed word. The reason is that in the initialization of the 'globals' variable on line x, I want to initialize
 * 'dir' to DIR_EAST. If DIR_EAST were defined using "static const int DIR_EAST = 1", then I would not be able
 * to use the static constant in that initialization because... well, for some reason I don't quite understand.
 * All I know is that the gcc C compiler barfed on that line, so I switched these to be preprocessor macros.  
 *------------------------------------------------------------------------------------------------------------*/
#define DIR_NORTH 0
#define DIR_EAST  1
#define DIR_SOUTH 2
#define DIR_WEST  3

/*--------------------------------------------------------------------------------------------------------------
 * GLOBAL CONSTANT DEFINITIONS
 *------------------------------------------------------------------------------------------------------------*/

/* Define MAX_CMDS as a static int constant and initialize it to 7. */
static int MAX_CMDS = 9;

/*--------------------------------------------------------------------------------------------------------------
 * TYPEDEFS
 *
 * This structure type stores a tuple: a string for a command, and a pointer to a function which is to be
 * called in order to perform the command.
 *
 * This code illustrates what is called a "function pointer", or a "pointer to a function." Remember, that a
 * pointer variable contains the address in memory of something. Well, it is possible to define a pointer
 * variable which points to a function, i.e., the pointer variable will contain the address in memory where
 * the code for the function is located. Here is how to define and use a function pointer variable,
 *
 * -- Returns true if x < y, false otherwise.
 * int compare_int(int *x, int *y) {  
 *     return *x < *y;
 * }
 *
 * -- Returns true if person p1's name is lexically less than p2's name.
 * int compare_person(struct person *p1, struct person *p2) {
 *     return strcmp(p1->name, p2->name);
 * }
 *
 * -- Generic bubble sort rountine. Sorts an array containing 'size' elements. 'compare' is a pointer to
 * -- a comparison function. If we are sorting an array of ints then we pass compare_int to bubble_sort().
 * -- If we are sorting an array of struct person, we pass compare_person to bubble_sort().
 * void bubble_sort(void *array, int size, int (*compare)(void *, void *)) {
 *     -- Performs the bubble sort algorithm on 'array'.
 * }
 *
 * int main() {
 *     int A[10];
 *     struct person people[10];                 -- Assume this struct has been defined.
 *     ...                                       -- Assume code is here to fill up the A and people arrays.
 *     bubble_sort(A, 10, compare_int);          -- Sort the int array. Pass compare_int().
 *     bubble_sort(people, 10, compare_person);  -- Sort the struct person array. Pass compare_person().
 *     return 0;
 * }
 *
 * Note that when passing compare_int or compare_person to bubble_sort, we do not have to precede the function
 * name with an & like we would if we were passing an int parameter using "pass-by-pointer". Function names
 * are treated the same as array names, i.e., the name of an array is equivalent to the address in memory of
 * the first element of the array; the name of a function is equivalent to the address in memory of the first
 * instruction of the function. That said, it is syntactically legal to write "&compare_int", but don't do it
 * because the other C programmers will laugh at you.
 *
 * Note the syntax in bubble_sort() for specifying that the third parameter is a function pointer. If we have
 * a function declaration "int f(int, double *)" then the way to define the function pointer variable to f() is:
 *
 *    int (*function_pointer)(int, double *)
 *
 * Weird, I know. It is critial that parentheses surround 'function_pointer', i.e., if you simply write,
 *
 *    int *function_pointer(int, double *)
 *
 * Then this tells the compiler that there is a function named 'function_pointer' which returns a pointer to
 * an int. That's not the same as saying that 'function_pointer' is a pointer to a function which returns an
 * int.
 */
typedef struct {
	char *cmd;          /* cmd is a pointer to a string. */
	void (*perform)();  /* perform is a pointer to a function that performs the command. */
} cmd_t;

/*--------------------------------------------------------------------------------------------------------------
 * This structure type defines the static global variables for this module:
 *------------------------------------------------------------------------------------------------------------*/

typedef struct {
	bool  pendown;      /* True if Myrtle's pen is down.                                                      */
	bool  verbose;      /* If true, the command being performed is sent to the terminal. False by default.    */
	char  penchar;      /* The char being drawn by the pen. Space char ' ' by default.                        */
	char  **world;      /* A dynamically-allocated 2D-array of chars which is Myrtle's world.                 */
	int   line;         /* The line number in the input source file being executed. Starts at 1.              */
	int   dir;          /* The direction Myrtle is facing. East by default.                                   */
	int   row;          /* The row in the world where Myrtle is at. Zero by default.                          */
	int   col;          /* The col in the world where Myrtle is at. Zero by default.                          */
	cmd_t cmd_table[];  /* The command table.                                                                 */
} global_t;

/*--------------------------------------------------------------------------------------------------------------
 * STATIC FUNCTION DECLARATIONS (PROTOTYPES)
 *------------------------------------------------------------------------------------------------------------*/
static void   _myrtle_cmd_backward();
static void   _myrtle_cmd_forward();
static void   _myrtle_cmd_hyper();
static void   _myrtle_cmd_left();
static cmd_t *_myrtle_cmd_lookup(char *cmd);
static void   _myrtle_cmd_penchar();
static void   _myrtle_cmd_pendown();
static void   _myrtle_cmd_penup();
static void   _myrtle_cmd_perform(char *cmd);
static void   _myrtle_cmd_right();
static void   _myrtle_cmd_stop();

static int    _myrtle_col_get();
static void   _myrtle_col_set(int);

static int    _myrtle_line_get();
static void   _myrtle_line_set(int n);
static void   _myrtle_line_inc();

static int    _myrtle_dir_get();
static void   _myrtle_dir_set(int dir);

static void   _myrtle_move(int squares);

static void   _myrtle_pen_down();
static char   _myrtle_pen_char_get();
static void   _myrtle_pen_char_set(char ch);
static bool   _myrtle_pen_is_down();
static void   _myrtle_pen_up();

static int    _myrtle_row_get();
static void   _myrtle_row_set(int);

static void   _myrtle_world_clear();
static void   _myrtle_world_draw_char();
static void   _myrtle_world_init();
static void   _myrtle_world_write();

/*--------------------------------------------------------------------------------------------------------------
 * GLOBAL VARIABLE DEFINITIONS
 *
 * Defines global variables. I like to stick a g_ prefix on all global variables so the name will not conflict
 * with any local variables.
 *
 * Any variable defined outside of the functions of the program is a global variable. Global variables have
 * scope from the point at which they are defined to the end of the source code file. Therefore, if you want
 * every function in a source code file to have access to a global variable, define the variable near the top
 * of the source code file. Global variables have life time which is the duration of the program. That is, the
 * global variable is allocated to memory by the operating system loader (the program that loads programs into
 * memory before executing them). The global variable exists in memory for as long as the program is executing.
 *
 * Use of global variables is STRONGLY discouraged. The only acceptable reasons to use a global variable are:
 * (1) if the variable has to be allocated for the entire duration of the program; and/or (2) if the variable
 * is going to be used in the vast majority of the functions in one or more source code files. In the latter
 * case, it is easier to define the variable as a global variable and share it among all of the functions
 * rather than having to pass it as a parameter among all of the functions. In general, avoid global variables
 * like the plague, because improper can lead to difficult-to-find bugs.
 *
 * The word "static" tells the compiler that this global variables scope is this, and only this, source code
 * file. It cannot be accessed from another source code file (even by writing an "extern bool G_verbose;"
 * declaration because the linker will not find it and will fail.
 *
 * I STRONGLY suggest if you use global variables, that you define them as static in a source code file and
 * provide accessor/mutator functions to read/write them.
 *------------------------------------------------------------------------------------------------------------*/
static global_t globals = {
		false,
		false,
		' ',
		NULL,
		1,
		DIR_EAST,
		0,
		0,
		{
				{ "backward", _myrtle_cmd_backward },
				{ "forward",  _myrtle_cmd_forward  },
				{ "hyper",    _myrtle_cmd_hyper    },
				{ "left",     _myrtle_cmd_left     },
				{ "penchar",  _myrtle_cmd_penchar  },
				{ "pendown",  _myrtle_cmd_pendown  },
				{ "penup",    _myrtle_cmd_penup    },
				{ "right",    _myrtle_cmd_right    },
				{ "stop",	  _myrtle_cmd_stop     }
		}
};

/*===================================== NONSTATIC FUNCTION DEFINITIONS =======================================*/

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: myrtle_interp()
 * DESCR:    Implements the interpreter for the Myrtle programming language.
 * RETURNS:  An int: zero on success, a negative integer on failure.
 * PSEUDOCODE:
 * 1. Call file_open_files() to open the input and output files. Note that by the time we reach this function
 *    the command line has been parsed and the name(s) of the input and output files are stored in the globals
 *    variable of the "file" module.
 * 2. Call the appropriate function in this source code file to initialize Myrtle's world.
 * 3. Write a for loop which initializes globals.line to 1 by calling the _myrtle_line_set() mutator function;
 *    the condition is "true" (we break out of this loop when file_next_token() returns NULL; the post-loop
 *    expression is to increment the line number by calling _myrtle_line_inc().
 *    a. Call file_next_token() to get a char pointer named 'cmd' to the next token in the source code file.
 *    b. If 'cmd' is NULL (i.e., we reached the end of the input file) then break out of this loop.
 *    c. If the verbose flag is set, then call fprintf(stdout, "Performing command: %s\n", cmd).
 *    d. Call _myrtle_cmd_perform() and pass 'cmd' as the parameter.
 * 4. Call the appropriate function in this source code file to write Myrtle's world to the output file.
 * 5. Call file_close_files() to close the input and output files.
 * 6. Return 0.
 *------------------------------------------------------------------------------------------------------------*/
int myrtle_interp() {

	char *cmd;
	/*
	 * 1. Call file_open_files() to open the input and output files. Note that by the time we reach this function
	 *    the command line has been parsed and the name(s) of the input and output files are stored in the globals
	 *    variable of the "file" module.
	 *    Currently, this is probably not correct*/
	file_open_files();

	/* 2. Call the appropriate function in this source code file to initialize Myrtle's world. */
	_myrtle_world_init();

	/*  3. Write a for loop which initializes globals.line to 1 by calling the _myrtle_line_set() mutator function;
	 *    the condition is "true" (we break out of this loop when file_next_token() returns NULL; the post-loop
	 *    expression is to increment the line number by calling _myrtle_line_inc().
	 *    a. Call file_next_token() to get a char pointer named 'cmd' to the next token in the source code file.
	 *    b. If 'cmd' is NULL (i.e., we reached the end of the input file) then break out of this loop.
	 *    c. If the verbose flag is set, then call fprintf(stdout, "Performing command: %s\n", cmd).
	 *    d. Call _myrtle_cmd_perform() and pass 'cmd' as the parameter.*/
	for((_myrtle_line_set(1)); (cmd=file_next_token()); _myrtle_line_inc() ){
		/*cmd = file_next_token();*/
		if(cmd==NULL) break;
		if(globals.verbose) fprintf(stdout, "Performing command: %s\n", cmd);
		_myrtle_cmd_perform(cmd);
	}

	/*  4. Call the appropriate function in this source code file to write Myrtle's world to the output file.*/
	_myrtle_world_write();

	/* 5. Call file_close_files() to close the input and output files.*/
	file_close_files();

	/* 6. Return 0.*/
	return 0;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: myrtle_verbose_get()
 * DESCR:    Accessor function for globals.verbose.
 * RETURNS:  The value of globals.verbose which is a bool.
 *------------------------------------------------------------------------------------------------------------*/
bool myrtle_verbose_get() {
	return globals.verbose;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: myrtle_verbose_set()
 * DESCR:    Mutator function for globals.verbose.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
void myrtle_verbose_set(bool flag) {
	globals.verbose = flag;
}

/*======================================= STATIC FUNCTION DEFINITIONS ========================================*/

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_cmd_backward()
 * DESCR:    Performs the 'backward' command. There should be an integer following the word 'backward' in the
 *           statement. This is the number of squares to move backward. Note: if Myrtle reaches one of the edges
 *           of her world, then she stops there and does not move, lest she fall off since everyone knows the
 *           world is flat. Function is analogous to _myrtle_cmd_forward().
 * RETURNS:  Nothing.
 * PSEUDOCODE:
 * 1. Define i and squares as int variables.
 * 2. Call file_next_token() to get 'n' from the input file. Pass this string to atoi() which will convert it
 *    into an equivalent integer. Assign the return value from atoi() to squares.
 * 3. Write a for loop (using i as the index variable) which iterates 'squares' times.
 *    a. Call _myrtle_move(1) to move Myrtle backward one square.
 *    b. If the pen is down, then call _myrtle_world_draw_char() to draw a character in the square that Myrtle
 *       just entered.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_cmd_backward() {
	/* 1. Define i and squares as int variables. */
	int i, squares;
	char *n;

	/* 2. Call file_next_token() to get 'n' from the input file. Pass this string to atoi() which will convert it
	 *    into an equivalent integer. Assign the return value from atoi() to squares. */
	n = file_next_token();
	squares = atoi(n);

	/* 3. Write a for loop (using i as the index variable) which iterates 'squares' times.
	 *    a. Call _myrtle_move(1) to move Myrtle backward one square.
	 *    b. If the pen is down, then call _myrtle_world_draw_char() to draw a character in the square that Myrtle
	 *       just entered.*/
	for(i = squares; i > 0; --i) {
		_myrtle_move(-1);
		if(_myrtle_pen_is_down()) _myrtle_world_draw_char();
	}
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_cmd_foward()
 * DESCR:    Performs the 'forward' command. There should be an integer following the word 'forward' in the
 *           statement. This is the number of squares to move forward. Note: if Myrtle reaches one of the edges
 *           of her world, then she stops there and does not move, lest she fall off since everyone knows the
 *           world is flat.
 * RETURNS:  Nothing.
 * PSEUDOCODE:
 * 1. Define i and squares as int variables.
 * 2. Call file_next_token() to get 'n' from the input file. Pass this string to atoi() which will convert it
 *    into an equivalent integer. Assign the return value from atoi() to squares.
 * 3. Write a for loop (using i as the index variable) which iterates 'squares' times.
 *    a. Call _myrtle_move(1) to move Myrtle forward one square.
 *    b. If the pen is down, then call _myrtle_world_draw_char() to draw a character in the square that Myrtle
 *       just entered.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_cmd_forward() {
	/* 1. Define i and squares as int variables. */
	int i, squares;
	char *n;

	/* 2. Call file_next_token() to get 'n' from the input file. Pass this string to atoi() which will convert it
	 *    into an equivalent integer. Assign the return value from atoi() to squares. */
	n = file_next_token();
	squares = atoi(n);

	/* 3. Write a for loop (using i as the index variable) which iterates 'squares' times.
	 *    a. Call _myrtle_move(1) to move Myrtle forward one square.
	 *    b. If the pen is down, then call _myrtle_world_draw_char() to draw a character in the square that Myrtle
	 *       just entered.*/
	for(i = squares; i > 0; --i) {
		_myrtle_move(1);
		if(_myrtle_pen_is_down()) _myrtle_world_draw_char();
	}
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_cmd_hyper()
 * DESCR:    Performs the 'hyper' command. There should be two integers following the word 'hyper' in the state-
 *           ment. These are read as strings and converted into equivalent int values using the C library func-
 *           tion atoi(). If they are not ints or they are missing, then something bad is likely to happen. I
 *           suggest wearing a flak jacket whenever using this interprer. Note that when Myrtle hyperspaces
 *           she lands facing the same direction she was originally. If the pen was down, then a char is drawn
 *           in the new square. If the pen is up, then no char is drawn.
 * RETURNS:  Nothing.
 * PSEUDOCODE:
 * 1. Call file_next_token() to get the row from the input file. Use atoi() to convert this into an integer
 *    and assign it to a defined int variable named 'row'.
 * 2. Do Step 1 to get 'col'.
 * 3. Call the _myrtle_row_set() and _myrtle_col_set() mutator functions to update Myrtle's row and col.
 * 4. If the pen is down, then draw a character in the square that Myrtle just landed in.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_cmd_hyper() {
	/* 1. Call file_next_token() to get the row from the input file. Use atoi() to convert this into an integer
	 *    and assign it to a defined int variable named 'row'.*/
	char *n;
	int row, col;

	n = file_next_token();
	row = atoi(n);

	/* 2. Do Step 1 to get 'col'. */
	n = file_next_token();
	col = atoi(n);

	/* 3. Call the _myrtle_row_set() and _myrtle_col_set() mutator functions to update Myrtle's row and col.*/
	_myrtle_row_set(row);
	_myrtle_col_set(col);

	/* 4. If the pen is down, then draw a character in the square that Myrtle just landed in.*/
	if( _myrtle_pen_is_down() ) _myrtle_world_draw_char();
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_cmd_left()
 * DESCR:    Performs the 'left' command.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_cmd_left() {
	if (_myrtle_dir_get() == DIR_NORTH) _myrtle_dir_set(DIR_WEST);
	else _myrtle_dir_set(_myrtle_dir_get() - 1);
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_cmd_lookup()
 * DESCR:    Looks up 'cmd_string' in the globals.cmd_table table.
 * RETURNS:  A pointer to the command in the globals.cmd_table array. NULL if the command is not found. This
 *           would be caused by a syntax error in the Myrtle source code file.
 * NOTE:     Suppose A is an array of ints, defined "int A[10];" and we want to return the address of A. We
 *           can simply write "A" and the compiler will interpret that to mean "the address of the first
 *           element of A". However, suppose we wish to return the address of the third element of A. If we
 *           simply write "A[2]" the compiler will interpret that to mean the "value" of the third element of
 *           A. If we want the "address" of the third element, we have to write "&A[2]". Note that "&A[0]"
 *           is equivalent to "&A" which is equivalent to "A".
 *------------------------------------------------------------------------------------------------------------*/
static cmd_t *_myrtle_cmd_lookup(char *cmd_string) {
	int i;
	for (i = 0; i < MAX_CMDS; i++) {
		if (streq(globals.cmd_table[i].cmd, cmd_string)) return &globals.cmd_table[i];
	}
	return NULL;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_cmd_penchar()
 * DESCR:    Performs the 'penchar' command. There should be a char following the word 'penchar' in the state-
 *           ment. If there's not, then they are out of luck.
 * RETURNS:  Nothing.
 * PSEUDOCODE:
 * 1. Call file_next_token() to get a char pointer to the next token. Call this char pointer variable 'token'.
 * 2. Pass the first char of 'token' to _myrtle_pen_char_set().
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_cmd_penchar() {
	char *next = file_next_token();
	_myrtle_pen_char_set(next[0]);
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_pendown()
 * DESCR:    Performs the 'pendown' command.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_cmd_pendown() {
	/* This one is trivial. */
	globals.pendown = true;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_cmd_penup()
 * DESCR:    Performs the 'penup' command.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_cmd_penup() {
	/* And so is this one. */
	globals.pendown = false;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_cmd_perform()
 * DESCR:    Performs the command 'cmd' by making Myrtle do whatever she needs to do.
 * RETURNS:  Nothing.
 * NOTE:     Printing a string to a string buffer using sprintf() is an old and extremely useful C trick.
 *           Learn it. You will see it in C code (well, at least in the code I write).
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_cmd_perform(char *cmd_string) {
	cmd_t *command = _myrtle_cmd_lookup(cmd_string);
	if (!command) {
		char buffer[128];
		sprintf(buffer, "Unknown command '%s' on line %d", cmd_string, _myrtle_line_get());
		main_terminate_err(buffer, TERM_ERR_UNK_CMD);
	}
	command->perform();
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_cmd_right()
 * DESCR:    Performs the 'right' command.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_cmd_right() {
	/* function is similar to _myrtle_cmd_left(), but rotating clockwise */
	if (_myrtle_dir_get() == DIR_WEST) _myrtle_dir_set(DIR_NORTH);
	else _myrtle_dir_set(_myrtle_dir_get() + 1);
}


/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_cmd_stop()
 * DESCR:	 Stops myrtle in her tracks, terminates program, draws output
 * RETURNS:	 Nothing.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_cmd_stop() {
	/* TODO: is there an easy to terminate the program from _stop()
	 * 1. program terminates immediately*/
	/* 2. send myrtle's world to the output file */
	_myrtle_world_write();
}


/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_col_get()
 * DESCR:    Accessor function for globals.col.
 * RETURNS:  The value of the int globals.col variable.
 *------------------------------------------------------------------------------------------------------------*/
static int _myrtle_col_get() {
	return globals.col;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_col_set()
 * DESCR:    Mutator function for globals.col. Note that as Myrtle is moving, if she reaches either the west
 *           or east border of her world, she stops moving and essentially just keeps banging her head against
 *           the wall.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_col_set(int col) {
	if (col < 0) globals.col = 0;
	else if (col >= MAX_WORLD_COLS) globals.col = MAX_WORLD_COLS - 1;
	else globals.col = col;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_dir_get()
 * DESCR:    Accessor function for the globals.dir variable.
 * RETURNS:  The value of int variable globals.dir.
 *------------------------------------------------------------------------------------------------------------*/
static int _myrtle_dir_get() {
	return globals.dir;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_dir_set()
 * DESCR:    Mutator function for the globals.dir variable.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_dir_set(int dir) {
	if(dir > -1 && dir < 4) globals.dir = dir;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_line_get()
 * DESCR:    Accessor function for globals.line.
 * RETURNS:  The value of int variable globals.line.
 *------------------------------------------------------------------------------------------------------------*/
static int _myrtle_line_get() {
	return globals.line;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_line_inc()
 * DESCR:    Increments globals.line.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_line_inc() {
	_myrtle_line_set(_myrtle_line_get() + 1);
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_line_set()
 * DESCR:    Mutator function for globals.line.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_line_set(int n) {
	/* check for valid values of line */
	if(n > -1) globals.line = n;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_move()
 * DESCR:    Moves Myrtle 'squares' squares in the direction she is facing. Note that if Myrtle reaches one of
 *           the edges of her world, she just stops.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_move(int squares) {
	if (_myrtle_dir_get() == DIR_NORTH) {
		_myrtle_row_set(_myrtle_row_get() - squares);
	} else if (_myrtle_dir_get() == DIR_EAST) {
		_myrtle_col_set(_myrtle_col_get() + squares);
	} else if (_myrtle_dir_get() == DIR_SOUTH) {
		_myrtle_row_set(_myrtle_row_get() + squares);
	} else {
		_myrtle_col_set(_myrtle_col_get() - squares);
	}
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_pen_char_get()
 * DESCR:    Accessor function for the globals.penchar variable.
 * RETURNS:  The value of the char globals.penchar variable.
 *------------------------------------------------------------------------------------------------------------*/
static char _myrtle_pen_char_get() {
	return globals.penchar;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_pen_char_set()
 * DESCR:    Mutator function for the globals.penchar variable.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_pen_char_set(char ch) {
	globals.penchar = ch;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_pen_down()
 * DESCR:    Sets the globals.pendown variable to indicate that Myrtle's pen is down.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_pen_down() {
	globals.pendown = true;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_pen_is_down()
 * DESCR:    Returns true if Myrtle's pen is down.
 * RETURNS:  See description.
 *------------------------------------------------------------------------------------------------------------*/
static bool _myrtle_pen_is_down() {
	return globals.pendown;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_pen_up()
 * DESCR:    Sets the globals.pendown variable to indicate that Myrtle's pen is up.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_pen_up() {
	globals.pendown = false;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_row_get()
 * DESCR:    Accessor function for globals.row.
 * RETURNS:  The value of the int globals.row variable.
 *------------------------------------------------------------------------------------------------------------*/
static int _myrtle_row_get() {
	return globals.row;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_row_set()
 * DESCR:    Mutator function for globals.row. Note that as Myrtle is moving, if she reaches either the north
 *           or south border of her world, she stops moving and essentially just keeps banging her head against
 *           the wall.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_row_set(int row) {
	/* Hint: see _myrtle_col_set(). This function is very similar. */
	if (row < 0) globals.row = 0;
	else if (row >= MAX_WORLD_ROWS) globals.row = MAX_WORLD_ROWS - 1;
	else globals.row = row;
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_world_clear()
 * DESCR:    Clears all of the squares in Myrtle's world to the space char.
 * RETURNS:  Nothing.
 * PSEUDOCODE:
 * 1. Write nested for row/for col loops which iterate over each row and col of the globals.world[][] array.
 *    a. Put a space in each array element.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_world_clear() {
	int r, c;
	for(r = 0; r< MAX_WORLD_ROWS; ++r){
		for(c = 0; c<MAX_WORLD_COLS; ++c){
			globals.world[r][c] = ' ';
		}
	}
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_world_draw_char()
 * DESCR:    Draws the current globals.penchar character in the square Myrtle is in.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_world_draw_char() {
	if (!_myrtle_pen_is_down()) return;
	globals.world[_myrtle_row_get()][_myrtle_col_get()] = _myrtle_pen_char_get();
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_world_init()
 * DESCR:    Initializes Myrtle's world by dynamically allocating a 2D-array of chars. Each square in the world
 *           is set to the space char.
 * RETURNS:  Nothing.
 * PSEUDOCODE:
 * 1. Dynamically allocate a 2D-array of chars with MAX_WORLD_ROWS rows and MAX_WORLD_COLS cols.
 * 2. Call _myrtle_world_clear() to initialize each square in Myrtle's world to the space char.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_world_init() {
	/*1. Dynamically allocate a 2D-array of chars with MAX_WORLD_ROWS rows and MAX_WORLD_COLS cols.*/
	int r;
	globals.world = (char**)malloc(MAX_WORLD_ROWS * sizeof(char*));
	for(r=0;r < MAX_WORLD_ROWS; r++){
		globals.world[r] = (char*)malloc(MAX_WORLD_COLS * sizeof(char));
	}

	/*2. Call _myrtle_world_clear() to initialize each square in Myrtle's world to the space char. */
	_myrtle_world_clear();
}

/*--------------------------------------------------------------------------------------------------------------
 * FUNCTION: _myrtle_world_write()
 * DESCR:    Writes the 2D-array of char representing Myrtle's world to the output file.
 * RETURNS:  Nothing.
 *------------------------------------------------------------------------------------------------------------*/
static void _myrtle_world_write() {
	int row, col;
	for (row = 0; row < MAX_WORLD_ROWS; row++) {
		for (col = 0; col < MAX_WORLD_COLS; col++) {
			file_write_char(globals.world[row][col]);
		}
		file_write_char('\n');
	}
}
