/* 
 * line.h
 * Isabelle Lai (ilai 01) and Andrea Foo (afoo01)
 * Comp40, HW1
 * January 31, 2020
 * Purpose: struct to represent a line of a file
 */

#include <stdio.h>
#include <stdlib.h>

struct Line {
    char *filename;
    int line_num;
};
