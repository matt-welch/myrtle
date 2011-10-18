#!/bin/bash
#---------------------------------------------------------------------------------------------------------------
# FILE:    Makefile
# DESCR:   Make file for the Myrtle programming language interprer.
# AUTHORS: Kevin R. Burger (burgerk@asu.edu)
#---------------------------------------------------------------------------------------------------------------

# -ansi   : Compile the code assuming that it conforms to Standard C (also known as ANSI C).
# -c      : Compile a .c file only to produce the .o file.
# -g      : Put debugging information in the .o file. Used by the GDB debugger.
# -O0     : Turn off all optimization. Necessary if you are going to debug using GDB.
# -Wall   : Turn on all warnings. Your code should compile with no errors or warnings.
CFLAGS  = -ansi -c -g -O0 -Wall

SOURCES = file.c     \
          globals.c  \
          main.c     \
          myrtle.c

OBJECTS = $(SOURCES:.c=.o)

TARGET  = myrtle

$(TARGET): $(OBJECTS) 
	gcc $(OBJECTS) -o $(TARGET)

%.o: %.c
	gcc $(CFLAGS) $< -o $@

%.d: %.c
	rm -f $@; gcc -MM $< > $@

include $(SOURCES:.c=.d)

.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f *.d
	rm -f $(TARGET)
