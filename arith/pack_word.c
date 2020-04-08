/*
 *    Assignment: Project 4: Arith
 *
 *    Filename: pack_word.c
 *
 *    Authors: Isabelle Lai & Andrea Foo
 *
 *    Due Date: March 4, 2020
 *
 *    Summary: Packs each 2 by 2 pixel block into a 32 bit word and the
 *             corresponding decompression step that reverts it back to
 *             a scaled pixel image
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <a2methods.h>
#include <a2plain.h>
#include <a2blocked.h>
#include <assert.h>
#include <math.h>
#include <bitpack.h>
#include "pack_word.h"

/* CONSTANTS */
const unsigned A_WIDTH = 6;
const unsigned BCD_WIDTH = 6;
const unsigned CHROMA_WIDTH = 4;
const unsigned A_LSB = 26;
const unsigned B_LSB = 20;
const unsigned C_LSB = 14;
const unsigned D_LSB = 8;
const unsigned PB_LSB = 4;
const unsigned PR_LSB = 0;

/* FUNCTION PROTOTYPES */
static void scaled_to_packed_apply(int col, int row, 
                                   A2Methods_UArray2 scaled_image, 
                                   void *elem, void *cl);
static void packed_to_scaled_apply(int col, int row, 
                                   A2Methods_UArray2 scaled_image, 
                                   void *elem, void *cl);


/* Function: pack_words
 *
 * Input: Pnm_ppm original
 *
 * Output: Returns compressed 32 bit word format of the image. 
 *
 * Purpose: Reads in an image in scaled pixel format, and calls helper 
 *          functions to pack a,b,c,d,pb, and pr values into 32 bit words 
 *          using bitpack and store new valuesin new image of 32 bit words.
 * 
 * Effects: Checked runtime error if original image is NULL or new memory 
 *          could not be allocated. 
 */
Pnm_ppm pack_words(Pnm_ppm original)
{
    /* Allocate memory for the new compressed image */
    Pnm_ppm new_image = malloc(sizeof(*new_image));
    assert(new_image != NULL);

    /* Set values of each item in the new image */
    new_image -> denominator = original -> denominator;
    new_image -> width = original -> width;
    new_image -> height = original -> height;
    new_image -> methods = uarray2_methods_plain;
    
    /* Map over new image, taking corresponding pixel from original image, 
     * compressing, and storing in new image */
    new_image -> pixels = (A2Methods_UArray2) new_image -> methods -> 
                          new(new_image -> width, new_image -> height, 
                              sizeof(uint32_t));

    original -> methods -> map_default(original -> pixels, 
                                       scaled_to_packed_apply, new_image);
    
    return new_image;
}


/* Function: unpack_words
 *
 * Input: Pnm_ppm original (the image in scaled pixels format to decompress)
 *
 * Output: Returns decompressed scaled pixels format of the image. 
 *
 * Purpose: Reads in an image in 32 bit words format, and uses bitpack 
 *          functions to split up the 32 bit word back into a, b, c, d, pb, 
 *          and pr values in each portion of the 32 bit work and update the 
 *          scaled pixels from these pixels in the new image. 
 * 
 * Effects: Checked runtime error if original image is NULL or new memory 
 *          could not be allocated. 
 */
Pnm_ppm unpack_words(Pnm_ppm original)
{
    /* Allocate memory for the new compressed image */
    Pnm_ppm new_image = malloc(sizeof(*new_image));
    assert(new_image != NULL);

    /* Set values of each item in the new image */
    new_image -> denominator = original -> denominator;
    new_image -> width = original -> width;
    new_image -> height = original -> height;

    /* set image methods back to blocked since scaled pixels are in 2 by 
     * 2 blocks */
    new_image -> methods = uarray2_methods_blocked;

    /* Map over new image, taking corresponding pixel from original image, 
     * compressing, and storing in new image */
    new_image -> pixels = (A2Methods_UArray2) new_image -> methods -> 
                           new(new_image -> width, new_image -> height, 
                               get_scaled_size());

    new_image -> methods -> map_default(new_image -> pixels, 
                                        packed_to_scaled_apply, original);  
    return new_image;
}


/* Function: scaled_to_packed_apply
 *
 * Input: int col, int row, A2Methods_UArray2 blocked_image, void *elem, 
 *        void *cl (the image in 32 bit word format that is being 
 *        compressed to)
 *
 * Output: N/A 
 *
 * Purpose: Apply function for transforming each scaled pixel to 32 bit words 
 *          using bitpack module functions. Sets each portion of the 32 bit 
 *          word to the scaled pixel's values 
 *          
 * Effects: Checked runtime error if component video image is NULL.       
 */
static void scaled_to_packed_apply(int col, int row, 
                                   A2Methods_UArray2 scaled_image, 
                                   void *elem, void *cl)
{
    (void) scaled_image;
    Scaled_Pixel scaled_pixel = (Scaled_Pixel) elem;
    Pnm_ppm packed_image = (Pnm_ppm) cl;

    uint32_t *word_p = packed_image -> methods -> 
                       at(packed_image -> pixels, col, row);
    uint32_t word = 0;

    /* sets each portion of the 32 bit word with a,b,c,d,pb, and pr values 
     * using newu and news functions from Bitpack module
     */
    word = Bitpack_newu(word, A_WIDTH, A_LSB, get_scaled_a(scaled_pixel));
    word = Bitpack_news(word, BCD_WIDTH, B_LSB, get_scaled_b(scaled_pixel));
    word = Bitpack_news(word, BCD_WIDTH, C_LSB, get_scaled_c(scaled_pixel));
    word = Bitpack_news(word, BCD_WIDTH, D_LSB, get_scaled_d(scaled_pixel));

    word = Bitpack_newu(word, CHROMA_WIDTH, PB_LSB, 
                        get_scaled_pb(scaled_pixel));
    word = Bitpack_newu(word, CHROMA_WIDTH, PR_LSB, 
                        get_scaled_pr(scaled_pixel));

    /*sets value current word pointer to the word just created */
    *word_p = word;

}


/* Function: packed_to_scaled_apply
 *
 * Input: int col, int row, A2Methods_UArray2 blocked_image, void *elem, 
 *        void *cl (the image in scaled pixel format that is being 
 *        decompressed to)
 *
 * Output: N/A 
 *
 * Purpose: Apply function for transforming each 32 bit word into a scaled 
 *          pixel using bitpack module functions. Gets each portion of
 *          the 32 bit word for the scaled pixel's values 
 *          
 * Effects: Checked runtime error if component video image is NULL.       
 */
static void packed_to_scaled_apply(int col, int row, 
                                   A2Methods_UArray2 scaled_image, 
                                   void *elem, void *cl)
{
    (void) scaled_image;
    Scaled_Pixel scaled_pixel = (Scaled_Pixel) elem;
    Pnm_ppm packed_image = (Pnm_ppm) cl;

    uint32_t *word = packed_image -> methods -> 
                     at(packed_image -> pixels, col, row);
    
    /* sets the a ,b,c,d,pb, and pr values of a scaled pixel using the getu 
     * and gets functions from bitpack module by accessing sections of the 
     * word based on each value's lsb and width 
     */
    set_scaled_a(scaled_pixel, Bitpack_getu(*word, A_WIDTH, A_LSB));
    set_scaled_b(scaled_pixel, Bitpack_gets(*word, BCD_WIDTH, B_LSB));
    set_scaled_c(scaled_pixel, Bitpack_gets(*word, BCD_WIDTH, C_LSB));
    set_scaled_d(scaled_pixel, Bitpack_gets(*word, BCD_WIDTH, D_LSB));
    set_scaled_pb(scaled_pixel, Bitpack_getu(*word, CHROMA_WIDTH, PB_LSB));
    set_scaled_pr(scaled_pixel, Bitpack_getu(*word, CHROMA_WIDTH, PR_LSB));
}