/* 
 * readaline.c
 * Isabelle Lai (ilai 01) and Andrea Foo (afoo01)
 * Comp40, HW1
 * January 31, 2020
 * Purpose: Read in a line from a file and store it in an array of characters
 */

#include <stdio.h>
#include <stdlib.h>
#include "readaline.h"
#include "assert.h"

/*
 * readaline()
 * Purpose: read in a line from an open file and store the line in an
 * array of chars
 * Arguments: the open file stream, a double pointer to the char array that
 * will store the line
 * Returns: the number of bytes in the line that was read in
 */
size_t readaline(FILE *inputfd, char **datapp) {
    /* check for NULL parameters */
    assert(inputfd != NULL);
    assert(datapp != NULL);
    size_t array_size = 200 * sizeof(char);

    /* allocate memory for array */
    char *arrayp = *datapp;
    arrayp = malloc(array_size);
    assert(arrayp != NULL);
    assert(datapp != NULL);

    int index = 0;
    char next_char = fgetc(inputfd);

    /* check if end of file has been reached*/
    if (next_char == EOF) {
            free(arrayp);
            *datapp = NULL;
            return 0;
    }
    
    while (next_char != '\n') {
        assert(next_char != EOF);

        /* reallocate memory if there is not enough space */
        if (index == (int) array_size - 1) {
            arrayp = (char *) realloc(arrayp, array_size * 2);
            assert(arrayp != NULL);
            array_size = array_size * 2;
        }
        arrayp[index] = next_char;
        index++;
        next_char = fgetc(inputfd);
    }
    arrayp[index] = '\n';
    *datapp = arrayp;

    assert(*datapp != NULL);
    assert(arrayp != NULL);
    return index + 1;
}
