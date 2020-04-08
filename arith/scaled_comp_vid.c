/*
 *    Assignment: Project 4: Arith
 *
 *    Filename: scaled_comp_vid.c
 *
 *    Authors: Isabelle Lai & Andrea Foo
 *
 *    Due Date: March 4, 2020
 *
 *    Summary: Implementation file for module of conversion of an image from 
 *             blocked component video format to scaled component video format 
 *             by quantizing values from blocked format pixels and saving new
 *             quantized values into new Pnm_ppm. Corresponding decompression
 *             step implemented here as well which reverts it back to
 *             floating point and de-quantized values. 
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <a2methods.h>
#include <assert.h>
#include <math.h>
#include <arith40.h>

#include "scaled_comp_vid.h"

/* CONSTANTS */
const float BCD_MULTIPLY_FACTOR = 100.0;
const float A_MULTIPLY_FACTOR = 63.0;
const float BCD_INTERVAL_RANGE = 30.0;
const float A_RANGE_MIN = 0.0;
const float A_RANGE_MAX = 1.0;

/* FUNCTION PROTOTYPES */
/* APPLY FUNCTIONS */
static void blocked_to_scaled_apply(int col, int row, 
                                    A2Methods_UArray2 scaled_image, 
                                    void *elem, void *cl);
static void scaled_to_blocked_apply(int col, int row, 
                                    A2Methods_UArray2 scaled_image, 
                                    void *elem, void *cl);

/* HELPER FUNCTIONS TO CONVERT FROM FLOATS TO INTS */
static int float_to_int(float num_float);
static float int_to_float(int num);
static int float_to_int_a(float num_float);
static float int_to_float_a(int num_float);


/* Function: scaled_cv_image_compress
 *
 * Input: Pnm_ppm original (the original blocked component video image)
 *
 * Output: Returns compressed scaled component video format of the image. 
 *
 * Purpose: Reads in an image in blocked component video format, and calls 
 *          helper functions to quantize float values and store quantized 
 *          values in scaled component video image.
 * 
 * Effects: Checked runtime error if original image is NULL or new memory 
 *          could not be allocated. 
 */
Pnm_ppm scaled_cv_image_compress(Pnm_ppm original)
{
    assert(original != NULL);

    /* Allocate memory for the new compressed image */
    Pnm_ppm new_image = malloc(sizeof(*new_image));
    assert(new_image != NULL);

    /* Set values of each item in the new image */
    new_image -> denominator = original -> denominator;
    new_image -> width = original -> width;
    new_image -> height = original -> height;
    new_image -> methods = original -> methods;
    new_image -> pixels = (A2Methods_UArray2) new_image -> methods -> 
                          new(new_image -> width, new_image -> height, 
                              get_scaled_size());

    /* Map over new image, taking corresponding pixel from original image, 
     * compressing, and storing in new image */
    original -> methods -> map_default(new_image -> pixels, 
                                       blocked_to_scaled_apply, original);        
    
    return new_image;
}

/* Function: scaled_cv_image_decompress
 *
 * Input: Pnm_ppm original (the image in scaled component video format to 
 *        decompress)
 *
 * Output: Returns decompressed blocked component video format of the image. 
 *
 * Purpose: Reads in an image in scaled component video format, and calls 
 *          helper functions to dequantize values in each pixel by converting 
 *          them into floats, storing new values in corresponding pixels in 
 *          new image. 
 * 
 * Effects: Checked runtime error if original image is NULL or new memory 
 *          could not be allocated. 
 */
Pnm_ppm scaled_cv_image_decompress(Pnm_ppm original)
{
    assert(original != NULL);

    /* Allocate memory for the new decompressed image */
    Pnm_ppm new_image = malloc(sizeof(*new_image));
    assert(new_image != NULL);

    /* Set values of each item in the new image */
    new_image -> denominator = original -> denominator;
    new_image -> width = original -> width;
    new_image -> height = original -> height;
    new_image -> methods = original -> methods;
    new_image -> pixels = (A2Methods_UArray2) new_image -> methods -> 
                          new(new_image -> width, new_image -> height, 
                              get_blocked_size());

    /* Map over new image, taking corresponding pixel from original image, 
     * decompressing, and storing in new image */
    new_image -> methods -> map_default(original -> pixels, 
                                        scaled_to_blocked_apply, new_image);  
    return new_image;
}

/* Function: blocked_to_scaled_apply
 *
 * Input: int col, int row, A2Methods_UArray2 scaled_image, void *elem, 
 *        void *cl (the image in blocked component video format that is 
 *        being compressed)
 *
 * Output: N/A
 *
 * Purpose: Apply function for transforming each blocked component video pixel 
 *          to scaled component video pixel format.
 *          Converts all float values stored in original pixel into 
 *          quantized integer values, updates pixel in new image to 
 *          store quantized values. 
 * 
 * Effects: Checked runtime error if blocked component video image is NULL. 
 *          Updates pixel in scaled image to store compressed values. 
 */
static void blocked_to_scaled_apply(int col, int row, 
                                    A2Methods_UArray2 scaled_image, 
                                    void *elem, void *cl)
{
    (void) scaled_image;
    Scaled_Pixel scaled_pixel = (Scaled_Pixel) elem;
    Pnm_ppm blocked_image = (Pnm_ppm) cl;
    assert(blocked_image != NULL);

    /* Get original blocked pixel to compress */
    Blocked_Comp_Vid_Pixel blocked_pixel = blocked_image -> methods -> 
                                           at(blocked_image -> pixels, col, 
                                              row);

    /* Quantize a, b, c, d values from blocked pixel */
    int a = float_to_int_a(get_blocked_a(blocked_pixel));
    int b = float_to_int(get_blocked_b(blocked_pixel));
    int c = float_to_int(get_blocked_c(blocked_pixel));
    int d = float_to_int(get_blocked_d(blocked_pixel));
    
    /* Set quantized a, b, c, d values in scaled pixel */
    set_scaled_a(scaled_pixel, a);
    set_scaled_b(scaled_pixel, b);
    set_scaled_c(scaled_pixel, c);
    set_scaled_d(scaled_pixel, d);

    /* Quantize pb and pr values from blocked pixel and store new values in 
     * scaled pixel */
    unsigned avg_pb = 
        Arith40_index_of_chroma(get_blocked_avg_pb(blocked_pixel));
    unsigned avg_pr = 
        Arith40_index_of_chroma(get_blocked_avg_pr(blocked_pixel));
    set_scaled_pb(scaled_pixel, avg_pb);
    set_scaled_pr(scaled_pixel, avg_pr);

}

/* Function: scaled_to_blocked_apply
 *
 * Input: int col, int row, A2Methods_UArray2 blocked_image, void *elem, 
 *        void *cl (the image in blocked component video format that is
 *        being decompressed to)
 *
 * Output: N/A
 *
 * Purpose: Apply function for transforming each scaled component video pixel 
 *          to blocked component video pixel format.
 *          Converts all quantized int values stored in original pixel into 
 *          float values, updates pixel in new image to store float values. 
 * 
 * Effects: Checked runtime error if blocked component video image is NULL. 
 *          Updates pixel in blocked image to store decompressed values.
 */
static void scaled_to_blocked_apply(int col, int row, 
                                    A2Methods_UArray2 scaled_image, 
                                    void *elem, void *cl)
{
    (void) scaled_image;
    Scaled_Pixel scaled_pixel = (Scaled_Pixel) elem;
    Pnm_ppm blocked_image = (Pnm_ppm) cl;
    assert(blocked_image != NULL);

    /* Get blocked_pixel to store decompressed values */
    Blocked_Comp_Vid_Pixel blocked_pixel = blocked_image -> methods -> 
                                           at(blocked_image -> pixels, col, 
                                              row);
    
    /* De-quantize a, b, c, d values in scaled pixel and store the de-quantized
     * int values in blocked pixel */
    set_blocked_a(blocked_pixel, int_to_float_a(get_scaled_a(scaled_pixel)));
    set_blocked_b(blocked_pixel, int_to_float(get_scaled_b(scaled_pixel)));
    set_blocked_c(blocked_pixel, int_to_float(get_scaled_c(scaled_pixel)));
    set_blocked_d(blocked_pixel, int_to_float(get_scaled_d(scaled_pixel)));

    /* De-quantize pb, pr values in scaled pixel and store the de-quantized int
     * values in blocked pixel */
    set_blocked_avg_pb(blocked_pixel, 
                       Arith40_chroma_of_index(get_scaled_pb(scaled_pixel)));
    set_blocked_avg_pr(blocked_pixel, 
                       Arith40_chroma_of_index(get_scaled_pr(scaled_pixel)));
}

/* Function: float_to_int
 *
 * Input: float num_float
 * 
 * Output: argument converted to a quantized int
 *
 * Purpose: quantizes a float to a five-bit value, specifically for 
 *          quantizing b, c, d values from DCT calculations
 * 
 * Effects: N/A  
 */
static int float_to_int(float num_float)
{
    int num_int = 0;
    num_float = num_float * BCD_MULTIPLY_FACTOR;

    if (num_float > BCD_INTERVAL_RANGE){
        /* if float is larger than range of interval, set to 15 */
        num_int = BCD_INTERVAL_RANGE;
    } else if (num_float < ((-1.0) * BCD_INTERVAL_RANGE)) {
        /* if float is smaller than range of interval, set to 15 */
        num_int = ((-1.0) * BCD_INTERVAL_RANGE);
    /* checking if positive or negative */
    } else if (num_float > 0.0) {
        num_int = num_float;
    } else if (num_float < 0.0) {
        num_int = ceil(num_float);
    }

    return num_int;
}

/* Function: int_to_float
 *
 * Input: int num
 * 
 * Output: argument converted to a float
 *
 * Purpose: reverses quantization, turning an int into a float 
 *          by dividing by the multiply factor for values b, c, d from
 *          DCT compression
 * 
 * Effects: N/A  
 */
static float int_to_float(int num) 
{
    float num_float = 0.0;
    num_float = (float) num / BCD_MULTIPLY_FACTOR;
    return num_float;
}

/* Function: float_to_int_a
 *
 * Input: float num_float
 * 
 * Output: argument converted to a quantized int
 *
 * Purpose: quantizes a float into a nine-bit value, specifically for 
 *          quantizing a from DCT calculations
 * 
 * Effects: N/A  
 */
static int float_to_int_a(float num_float) 
{
    int new_a;
    num_float = num_float * A_MULTIPLY_FACTOR;

    if (num_float > A_RANGE_MAX) {
        /* if float is larger than range of interval, set to 1 */
        new_a = 1;
    } else if (num_float < A_RANGE_MIN) {
        /* if float is smaller than range of interval, set to 0 */
        new_a = 0;
    }
    
    new_a = num_float;
    return new_a;
} 

/* Function: int_to_float
 *
 * Input: int num
 * 
 * Output: argument converted to a float
 *
 * Purpose: reverses quantization, turning an int into a float 
 *          by dividing by the multiply factor for value a from
 *          DCT compression
 * 
 * Effects: N/A  
 */
static float int_to_float_a(int num_float) 
{
   float new_a = 0.0;
   new_a = (float) num_float / A_MULTIPLY_FACTOR;
   return new_a;
} 





