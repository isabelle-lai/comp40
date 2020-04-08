/*
 *    Assignment: Project 4: Arith
 *
 *    Filename: processed_compressed_image.c
 *
 *    Authors: Isabelle Lai & Andrea Foo
 *
 *    Due Date: March 4, 2020
 *
 *    Summary: Implementation file for module of printing compressed image to
 *             binary text in standard output and reading a compressed image
 *             binary textfile and printing out the decompressed rgb pixel 
 *             image
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <a2methods.h>
#include <a2plain.h>
#include <a2blocked.h>
#include "bitpack.h"
#include <assert.h>
#include <math.h>
#include "process_compressed_image.h"

/* FUNCTION PROTOTYPES */
static void print_apply(int col, int row, 
                        A2Methods_UArray2 image, void *elem, void *cl);
static void read_apply(int col, int row, 
                       A2Methods_UArray2 image, void *elem, void *cl);

/* Function: print_compressed_image
 *
 * Input: Pnm_ppm image
 *
 * Output: Prints the newly compressed image to standard output
 *
 * Purpose: Prints out header for compressed image specified by spec 
 *          as well as the 32 bit words at each pixel in the image using a 
 *          print apply function
 * 
 * Effects: Checked runtime error if original image is NULL. 
 */
void print_compressed_image(Pnm_ppm image)
{
    assert(image != NULL);
    int width = image -> width;
    int height = image -> height;

    /* Printing image format header specified by spec */
    printf("COMP40 Compressed image format 2\n%u %u", width, height);
    printf("\n");   

    /* Mapping through each pixel and printing out 32 bit words at each pixel */
    image -> methods -> map_default(image -> pixels, print_apply, NULL);    

}


/* Function: read_compressed_image
 *
 * Input: FILE *fp
 *
 * Output: Returns a Pnm_ppm image that contains 32 bit words at each pixel
 *
 * Purpose: Reads the binary text file of a compressed image of 32 words and 
 *          returns a new Pnm_ppm image that consists of 32 bit words at each 
 *          pixel. 
 * 
 * Effects: Checked runtime error if original image is NULL or if malloc fails.
 */
Pnm_ppm read_compressed_image(FILE *fp)
{
    assert(fp != NULL);
    unsigned height, width;

    /* Read image header as specified by spec */
    int read = fscanf(fp, "COMP40 Compressed image format 2\n%u %u", &width, 
                      &height);
    assert(read == 2);
    int c = getc(fp);
    assert(c == '\n');

    /* Create new pnmppm image to store 32 bit words at each pixel */
    Pnm_ppm new_image = malloc(sizeof(*new_image));
    assert(new_image != NULL);

    /* Setting attributes of the pnm ppm image with methods chosen to be plain
     * because it will be going through row major and pixels to each be a 
     * size of a 32 bit word
     */
    new_image -> denominator = 1;
    new_image -> width = width;
    new_image -> height = height;
    new_image -> methods = uarray2_methods_plain;
    uint32_t a;
    new_image -> pixels = (A2Methods_UArray2) new_image -> methods -> 
                          new(new_image -> width, new_image -> height, 
                              sizeof(a));

    /* Maps through image to set each pixel in the image to a 32 bit word 
     * from the file */
    new_image -> methods -> map_default(new_image -> pixels, read_apply, fp);  
    return new_image;
}


/* Function: print_apply
 *
 * Input: int col, int row, A2Methods_UArray2 image, void *elem, void *cl
 *
 * Output: Prints each 32 bit word in image to standard output per byte 
 *
 * Purpose: Loops through each 32 bit word of current pixel in increments
 *          of 8 (1 byte) and printing each byte out to standard output
 *          using putchar. Since the computer stores data in Little 
 *          Endian, we reverse the order of getting each byte by starting 
 *          at index 24 to print out in Big Endian. 
 * 
 * Effects: Prints each 32 bit word in image to standard output per byte.
 *          Reverses order of how computer stores data in Little Endian
 *          and print out in Big Endian by starting at index 24 and 
 *          decrementing from that index. 
 */
static void print_apply(int col, int row, A2Methods_UArray2 image, void *elem, 
                        void *cl)
{
    (void) col;
    (void) row;
    (void) image;
    (void) cl;
    uint32_t *word = (uint32_t *) elem;

    /* Loop through 32 bit word in decrements of 8 starting at index 24
     * to print to file in Big Endian 
     */
    for (int i = 24; i >= 0; i -= 8) {
        putchar(Bitpack_getu(*word, 8, i));
    }

}


/* Function: read_apply
 *
 * Input: int col, int row, A2Methods_UArray2 image, void *elem, void *cl
 *
 * Output: Read each 32 bit word from given file to new image per byte 
 *
 * Purpose: Loops through each current 32 bit word from file in increments
 *          of 8 (1 byte) and putting each byte in the new image.
 *          Since the data was printed to the binary file in Big Endian,
 *          we reverse the order of reading each 32 bit word by starting
 *          at index 24 and decrementing by 8 each time (1 byte) to
 *          store data in Little Endian. 
 * 
 * Effects: Updates new image with 32 bit words at each pixel, making sure
 *          that the data is stored in Little Endian. 
 */
static void read_apply(int col, int row, A2Methods_UArray2 image, void *elem,
                       void *cl)
{
    (void) col;
    (void) row;
    (void) image;

    FILE *fp = (FILE *) cl;
    assert(fp != NULL);
    uint32_t *word = (uint32_t *) elem;

    /* Loop through 32 bit word in decrements of 8 starting at index 24
     * to read to file in Little Endian 
     */
    for (int i = 24; i >= 0; i -= 8) {
        uint32_t curr = getc(fp);
        *word = Bitpack_newu(*word, 8, i, curr);
    }
   
}