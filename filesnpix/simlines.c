/* 
 * simlines.c
 * Isabelle Lai (ilai 01) and Andrea Foo (afoo01)
 * Comp40, HW1
 * January 31, 2020
 * Purpose: Find similar lines within and between files, sort the lines into 
 * their corresponding matchgroups, and print out each matchgroup of similar
 * lines
 */

#include <stdio.h>
#include <stdlib.h>
#include "readaline.h"
#include "atom.h"
#include "table.h"
#include "seq.h"
#include "assert.h"
#include "line.h"

/* function prototypes */
int clean_array(char **array, size_t num_bytes);
void add_matchgrp(Table_T lines_table, char *textfile, const char *curr_line, 
    int line_count);
void process_line(size_t num_bytes, char **datapp, char *textfile, 
    Table_T lines_table, int line_count);
void print_matchgrp(Table_T lines_table);
void read_file(int argc, char *argv[], Table_T lines_table);

/*
 * main()
 * Purpose: open file and call functions to read in file and print matchgroups
 * Arguments: number of command line args and array of command line args
 * Returns: whether the program was successful or not
 */
int main(int argc, char *argv[]) {
    Table_T lines_table = Table_new(10000, NULL, NULL);

    /* only begin reading from file if there is a file given */
    if (argc > 1) {
        read_file(argc, argv, lines_table);
        print_matchgrp(lines_table);
    }

    Table_free(&lines_table);
    exit(EXIT_SUCCESS);
}

/*
 * clean_array()
 * Purpose: removes extra white space and punctation from a line of text
 * Arguments: double pointer to the array of chars, the number of bytes in the
 * line stored in the array
 * Returns: the number of true characters remaining in the array
 */
int clean_array(char **array, size_t num_bytes) {
    assert(array != NULL);
    assert(*array != NULL);

    /* allocate memory for new array on the heap */
    char *temp_array = malloc(num_bytes + 1);
    assert(temp_array != NULL);

    int index = 0;

    for (int i = 0; i < (int) num_bytes - 1; i++) {

        /* check for valid characters and remove characters that are invalid */
        if ((((*array)[i] >= 'A') && ((*array)[i] <= 'Z')) || 
            (((*array)[i] >= 'a') && ((*array)[i] <= 'z')) || 
            (((*array)[i] >= '0') && ((*array)[i] <= '9')) ||  
            ((*array)[i] == '_')) {
            temp_array[index] = (*array)[i];
            index++;
        }
        else if (((*array)[i] == ' ') && (index > 0) && 
            (temp_array[index - 1] != ' ')) {
            /* remove extra white space from the array */
            temp_array[index] = (*array)[i];
            index++;
        }
    }

    /* set final two characters in the array to newline and null terminating 
     * character */
    temp_array[index] = '\n';
    index++;
    temp_array[index] = '\0';
    free(*array);
    *array = temp_array;
    return index;
}

/*
 * add_matchgrp()
 * Purpose: creates a Line struct and stores the struct in a existing matchgroup
 * or creates a new matchgroup and adds it to the table
 * Arguments: pointer to a Hanson Table, filename to store in struct, line 
 * number to store in struct
 * Returns: none
 */
void add_matchgrp(Table_T lines_table, char *textfile, const char *curr_line, 
    int line_count) {

    /* initialize struct to store filename and line number information */
    struct Line *structp = malloc(sizeof(*structp));
    assert(structp != NULL);
    structp->filename = textfile;
    structp->line_num = line_count;

    /* Table_get() returns NULL if key does not exist in the table */
    Seq_T curr_value = (Seq_T) Table_get(lines_table, curr_line);

    if (curr_value == NULL) {
        /* add new matchgroup if matchgroup does not exist already */
        Seq_T temp_seq = Seq_seq(structp, NULL);
        Table_put(lines_table, curr_line, temp_seq);

    }
    else {
        /* if matchgroup already exists, add struct to existing matchgroup */
        Seq_addhi(curr_value, structp);
    }
}

/*
 * process_line()
 * Purpose: creates Atom to store the text of a line and calls function
 * to add that line to the Hanson Table
 * Arguments: number of bytes in the line, double pointer to the array storing
 * the line, name of the file, pointer to the Hanson Table, current line number
 * in the file
 * Returns: none
 */
void process_line(size_t num_bytes, char **datapp, char *textfile, 
    Table_T lines_table, int line_count) {
    int size = clean_array(datapp, num_bytes);

    /* only add line to matchgroup if it is not an empty line */
    if (size > 2){
        const char *curr_line = Atom_string(*datapp);
        add_matchgrp(lines_table, textfile, curr_line, line_count);
    } 

    free(*datapp);
}

/*
 * print_matchgrp()
 * Purpose: prints all the lines and corresponding matchgroups stored in the 
 * table
 * Arguments: pointer to the Hanson Table
 * Returns: none
 */
void print_matchgrp(Table_T lines_table) {    
    /* convert table to array with alternating indices storing key then value */
    void *value_array = Table_toArray(lines_table, NULL);
    assert(value_array != NULL);

    int index = 0;
    char *curr_key = ((char **)value_array)[index];
    assert(curr_key != NULL);
    Seq_T curr_value = ((Seq_T*)value_array)[index + 1];
    assert(curr_value != NULL);

    /* loop through each key value pair in the table array */
    while (curr_key != NULL) {
        int length = Seq_length(curr_value);
        struct Line *curr_line; 

        /* only print matchgroup if it has two or more lines in it */
        if (length >= 2) {
            if (index != 0){
                printf("\n");
            }
            printf("%s", curr_key);
            for (int i = 0; i < length; i++) {
                curr_line = Seq_remlo(curr_value);
                printf("%-20s %7d\n", curr_line->filename, curr_line->line_num);
                free(curr_line);
            }
        }
        else if (length == 1) {
            /* if matchgroup does not have at least two lines, remove from array
               and free memory */
            curr_line = Seq_remlo(curr_value);
            free(curr_line);
        }
        index += 2;

        /* advance key and and value to next pair in the array */
        curr_key = ((char **)value_array)[index];
        Seq_free(&curr_value);
        if (curr_key != NULL) {
            curr_value = ((Seq_T*)value_array)[index + 1];
        }
    }
    free(value_array);
}

/*
 * read_file()
 * Purpose: opens each file, reads in the lines of the file, and calls
 * functions to process those lines into the Hanson Table
 * Arguments: the number of arguments on the command line, an array storing
 * the arguments as strings, the Hanson Table
 * Returns: none
 */
void read_file(int argc, char *argv[], Table_T lines_table) {
    char *datap = NULL;
    char **datapp = &datap;

    /* read in each file given on the command line separately */
    for (int i = 0; i < argc - 1; i++) {
        char *textfile = argv[i + 1];
        FILE *inputfd = fopen(textfile, "r");
        assert(inputfd != NULL);
        int line_count = 1;
        size_t num_bytes = readaline(inputfd, datapp) + 1;
        process_line(num_bytes, datapp, textfile, lines_table, line_count);

        /* loop through each line in the file */
        while (num_bytes != 0) {
            line_count++;
            num_bytes = readaline(inputfd, datapp);

            /* if readaline returns NULL, indicates end of file and loop ends */
            if (datap == NULL) {
                break;
            }
            process_line(num_bytes, datapp, textfile, lines_table, line_count);

        }
        fclose(inputfd);
    }
}
