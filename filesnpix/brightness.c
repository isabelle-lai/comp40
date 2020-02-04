/* 
 * brightness.c
 * Isabelle Lai (ilai 01) and Andrea Foo (afoo01)
 * Comp40, HW1
 * January 31, 2020
 * Purpose: Find the average brightness of a pgm image file
 */

#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h>

/* function prototypes */
void read_pgm(FILE *fp);
float compute_brightness(Pnmrdr_T rdr);
float pix_brightness(Pnmrdr_T rdr, unsigned denominator); 

/*
 * main()
 * Purpose: open file and call functions to read image
 * Arguments: number of command line args and array of command line args
 * Returns: whether the program was successful or not
 */
int main(int argc, char *argv[]) {
    /* case when user provides image through stdin */
    if (argc == 1) {
        read_pgm(stdin);
    } 
    /* case when user provides pgm image */
    else if (argc == 2) {
        FILE *fp = fopen(argv[1], "r");
        /* check to make sure that file is not null */
        if (fp == NULL) {
            fprintf(stderr, "Error: Could not open file %s for reading.\n", 
                argv[1]);
            exit(EXIT_FAILURE);
        }
        /* call read_pgm to read the file */
        read_pgm(fp);
    }
    /* case when user provides too many files */
    else {
        fprintf(stderr, "Error: Too many files given.\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

/*
 * read_pgm()
 * Purpose: create PNMRDR image reader, call function to compute avg brightness,
 * and print brightness
 * Arguments: file stream that opened image file
 * Returns: none
 */
void read_pgm(FILE *fp) {
    Pnmrdr_T rdr;
    /* creates a reader for the pgm file */
    TRY 
        rdr = Pnmrdr_new(fp);
    EXCEPT(Pnmrdr_Badformat);
        fprintf(stderr, "Error: Incorrect file format.\n");
        exit(EXIT_FAILURE);
    END_TRY;
    /* calls computer brightness function to find the average brightness of the 
     * pgm file */
    float avg_brightness = compute_brightness(rdr);
    printf("%.3f\n", avg_brightness);
    /*close file and free memory to prevent memory leaks */
    fclose(fp);
    Pnmrdr_free(&rdr);
}

/*
 * compute_brightness()
 * Purpose: gets brightness of each pixel and computes avg brightness of 
 * total image
 * Arguments: PNMRDR image reader
 * Returns: the avg brightness of the image
 */
float compute_brightness(Pnmrdr_T rdr) {
    /* check to make sure that reader is not null */
    if (rdr == NULL) {
        fprintf(stderr, "Error reading file.\n");
        exit(EXIT_FAILURE);
    }
    /* reads pixel data from the reader */
    Pnmrdr_mapdata data = Pnmrdr_data(rdr);

    /* check to make sure the pgm data type is correct */
    if (data.type != Pnmrdr_gray) {
        printf("Error: Incorrect file format.\n");
        exit(EXIT_FAILURE);
    }

    unsigned pgm_size = data.width * data.height;
    /* check to make sure that the size of image is not zero */
    if (pgm_size == 0) {
        fprintf(stderr, "Error: Image pixel count is 0.\n");
        exit(EXIT_FAILURE);
    }

    float total_brightness = 0.0;
    /* loop through all pixels in the pgm file to and each individual
     * pixel brightness to the total brightness of the pgm file */
    for (unsigned i = 0; i < pgm_size; i++) {
        total_brightness += pix_brightness(rdr, data.denominator);
    }
    /* find average brightness by dividing total brightness with size */
    float avg_brightness = total_brightness / pgm_size;
    
    return avg_brightness;
}

/*
 * pix_brightness()
 * Purpose: finds the brightness of a single pixel
 * Arguments: PNMRDR image reader, the denominator of the scaled integer 
 * brightness of the graymap image
 * Returns: the brightness of the current pixel
 */
float pix_brightness(Pnmrdr_T rdr, unsigned denominator) {
    /* check to make sure reader is not null */
    if (rdr == NULL) {
        fprintf(stderr, "Error reading file.\n");
        exit(EXIT_FAILURE);
    }
    /* use numerator and denominator to calculate brightness of indiviudal 
     * pixel */
    unsigned numerator = Pnmrdr_get(rdr);
    float brightness = (float) numerator / (float) denominator;

    return brightness;
}
