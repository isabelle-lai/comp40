/*
 *    Assignment: Project 4: Arith
 *
 *    Filename: compress40.c
 *
 *    Authors: Isabelle Lai & Andrea Foo
 *
 *    Due Date: March 4, 2020
 *
 *    Summary: Calls all the compress and decompress functions created in other 
 *             modules to compress the RGB_pixel image to a compressed bitpacked
 *             32 bit word image and the corresponding decompress functions to 
 *             get back original RGB pixel image
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pnm.h>
#include <a2methods.h>

#include "compress40.h"
#include "a2plain.h" 
#include "a2blocked.h"
#include "rgb_to_comp_vid.h"
#include "blocked_comp_vid.h"
#include "scaled_comp_vid.h"
#include "pack_word.h"
#include "process_compressed_image.h"

/* Function: compress40
 *
 * Input: FILE *input
 *
 * Output: prints the compressed image in binary text to stdout by calling 
 *         functions from other modules 
 *
 * Purpose: Reads in input pnmppm file and compresses the image through a 
 *          series of compression steps
 * 
 * Effects: Checked run time error if anything is null or if memory failure 
 *          and frees all memory
 */
extern void compress40(FILE *input)
{
    /* sets the methods that will be used to be the blocked methods */
    A2Methods_T methods = uarray2_methods_blocked;
    assert(input != NULL);

    /* A series of compression steps */
    Pnm_ppm original = Pnm_ppmread(input, methods);
    Pnm_ppm compressed_cv_image = cv_image_compress(original);
    Pnm_ppm compressed_blocked_cv_image = 
        blocked_cv_image_compress(compressed_cv_image);
    Pnm_ppm compressed_scaled_image = 
        scaled_cv_image_compress(compressed_blocked_cv_image);
    Pnm_ppm compressed_final = pack_words(compressed_scaled_image);

    /* Prints compressed image that is a textfile of 32 bit words to 
       standard output */
    print_compressed_image(compressed_final);
 
    /* Frees all memory of pnm_ppm created at each compression step and */
    assert(original != NULL);
    Pnm_ppmfree(&original);

    assert(compressed_cv_image != NULL);
    Pnm_ppmfree(&compressed_cv_image);

    assert(compressed_blocked_cv_image != NULL);
    Pnm_ppmfree(&compressed_blocked_cv_image);

    assert(compressed_scaled_image != NULL);
    Pnm_ppmfree(&compressed_scaled_image);

    assert(compressed_final != NULL);
    Pnm_ppmfree(&compressed_final);

}


/* Function: decompress40
 *
 * Input: FILE *input
 *
 * Output: Writes decompressed rgb image to std out
 *
 * Purpose: Reads in binary text file of a compressed image and decompresses 
 *          it back to rgb pixel image
 * 
 * Effects: Checked run time error if anything is null and frees all memory
 */
extern void decompress40(FILE *input)
{
    assert(input != NULL);
    /* A series of decompression steps to convert 32 bit binary words in 
       pnmppm into rgb_pixel format in pnmppm*/
    Pnm_ppm words = read_compressed_image(input);
    Pnm_ppm decompressed_words = unpack_words(words);
    Pnm_ppm decompressed_blocked_cv_image = 
        scaled_cv_image_decompress(decompressed_words);
    Pnm_ppm decompressed_cv_image = 
        blocked_cv_image_decompress(decompressed_blocked_cv_image);
    Pnm_ppm decompressed_rgb_image = cv_image_decompress(decompressed_cv_image);
    
    /* Prints decompressed image of rgb pixels standard output */
    Pnm_ppmwrite(stdout, decompressed_rgb_image);

    /* Frees all memory of pnm_ppm created at each decompression step */
    assert(words != NULL);
    Pnm_ppmfree(&words);

    assert(decompressed_words != NULL);
    Pnm_ppmfree(&decompressed_words);

    assert(decompressed_blocked_cv_image != NULL);
    Pnm_ppmfree(&decompressed_blocked_cv_image);

    assert(decompressed_cv_image != NULL);
    Pnm_ppmfree(&decompressed_cv_image);

    assert(decompressed_rgb_image != NULL);
    Pnm_ppmfree(&decompressed_rgb_image);
}

